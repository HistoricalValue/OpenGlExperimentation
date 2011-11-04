#include <stdio.h>
#include <list>
#include "uthreadlib.h"
#include "ufunctors.h"
#include "DDebug.h"

class ParallelisationManager {
public:
	static const size_t	NumberOfParallelJobs = 4u;

	///////////////////////////////////////////////////////

	struct Job {
		virtual void	operator () (void) const = 0;
		virtual Job*	Clone (void) const = 0;
		virtual void	Delete (void) = 0;
		virtual			~Job (void) {}
	};

	///////////////////////////////////////////////////////

	void	StartJob (Job const& job)
				{ StartJob(MakeJobWrapper(job.Clone())); }

	///////////////////////////////////////////////////////

	void	CleanUpDoneJobs (void) {
				LOCK_BLOCK(mutex)

				for (JobWrappersList::const_iterator i(runningJobs.begin()), next(i); i != runningJobs.end(); i = next) 
					if ((**i).HasFinished()) {
						DDELETE(*i);
						next = runningJobs.erase(i);
					}
					else
						next = ++i;
			}

	///////////////////////////////////////////////////////

	ParallelisationManager (void):
		mutex				(	),
		pendingJobs			(	),
		runningJobs			(	),
		numberOfRunningJobs (0u	)
		{}

	~ParallelisationManager (void) {
		while (HasPendingJob())
			GetNextPendingJob()->WaitToFinish();
		while (HasRunningJob()) {
			GetNextRunningJob()->WaitToFinish();
			CleanUpDoneJobs();
		}
	}

	///////////////////////////////////////////////////////
private:

	class JobWrapper {
	public:
		typedef void	(ParallelisationManager::* const NotifyJobCompleteFunc) (void);

		static void		RunJobWrapper (void* const job_voidp)
							{ (*reinterpret_cast<JobWrapper* const>(job_voidp))(); }

		JobWrapper*		Start (void) {
							thread->Start(this);
							hasStartedMutex.UnLock();
							return this;
						}

		void			operator () (void) {
							DASSERT(notifyJobComplete && !done);

							job();
							(parallelisationManager.*notifyJobComplete)();
							done = true;
						}

		void			WaitToFinish (void) {
							LOCK_BLOCK(hasStartedMutex)
							thread->WaitToFinish();

							DASSERT(done);
						}

		bool			HasFinished (void) const
							{ return done; }

		JobWrapper (ParallelisationManager* pm, NotifyJobCompleteFunc const njc, Job* const _job):
			parallelisationManager	(*DNULLCHECK(pm)									),
			notifyJobComplete		((DASSERT(njc), njc)								),
			job						(*DNULLCHECK(_job)									),
			thread					(DNEWCLASS(uthread, (&JobWrapper::RunJobWrapper))	),
			done					(false												),
			hasStartedMutex			(													)
			{ hasStartedMutex.WaitToLock(); }
		~JobWrapper (void) { DASSERT(done); WaitToFinish(); CleanUp(); }

	private:
		ParallelisationManager&			parallelisationManager;
		NotifyJobCompleteFunc const		notifyJobComplete;
		Job&							job;
		uthread* const					thread;
		bool							done;
		umutex							hasStartedMutex;

		void	CleanUp (void)
					{ DDELETE(thread); job.Delete(); }

				JobWrapper (JobWrapper const&);
		void	operator = (JobWrapper const*);
	};

	///////////////////////////////////////////////////////
	
	typedef std::list<JobWrapper*>	JobWrappersList;

	umutex				mutex;
	JobWrappersList		pendingJobs;
	JobWrappersList		runningJobs;
	size_t				numberOfRunningJobs;

	///////////////////////////////////////////////////////

	JobWrapper* MakeJobWrapper (Job* const job)
					{ return (DNEWCLASS(JobWrapper, (this, &ParallelisationManager::OnJobComplete, job))); }

	void		StartJob (JobWrapper* const wrapper) {
					LOCK_BLOCK(mutex)

					if (numberOfRunningJobs > NumberOfParallelJobs)
						pendingJobs.push_back(wrapper);
					else {
						++numberOfRunningJobs;
						runningJobs.push_back(wrapper->Start());
					}
				}

	void		DispatchNextPendingJob (void) {
					JobWrapper* const wrapper = PopNextPendingJob();
					if (wrapper)
						StartJob(wrapper);
				}

	void		OnJobComplete (void) {
					DecremenetNumberOfRunningJobs();
					DispatchNextPendingJob();
				}

	///////////////////////////////////////////////////////
	// synchronised job handling
	void		DecremenetNumberOfRunningJobs (void)
					{ LOCK_BLOCK(mutex) --numberOfRunningJobs; }
	bool		HasRunningJob (void)
					{ return LOCK_EXPR_VALUE(!runningJobs.empty(), mutex); }
	JobWrapper*	GetNextRunningJob (void)
					{ return LOCK_EXPR_VALUE(runningJobs.front(), mutex); }

	bool		HasPendingJob (void)
					{ return LOCK_EXPR_VALUE(!pendingJobs.empty(), mutex); }
	JobWrapper*	GetNextPendingJob (void)
					{ return LOCK_EXPR_VALUE(pendingJobs.front(), mutex); }
	JobWrapper*	PopNextPendingJob (void) {
					LOCK_BLOCK(mutex)

					JobWrapper* wrapper(NULL);
					if (!pendingJobs.empty()) {
						wrapper = pendingJobs.front();
						pendingJobs.pop_front();
					}

					return wrapper;
				}


	// no copying
	ParallelisationManager (ParallelisationManager const&);
	void operator = (ParallelisationManager const&);
};

template <typename I, typename Op>
class ParallelisedForeach: public ParallelisationManager::Job {
public:
	///////////////////////////////////////////////////////
	// ParallelisationManager::Job Interface
	virtual void					operator () (void) const {
										DASSERT(InvariantsHold());

										size_t const	firstIndex	((total / parts) * part),
														endIndex	(part == parts - 1u? total : (total / parts) * (part + 1));

										I i(begin), e(begin);
										std::advance(i, firstIndex);
										std::advance(e, endIndex);

										DASSERT(!(part == parts - 1u) || e == end);

										for (; i != e; ++i)
											op(*i);
									}

	virtual ParallelisedForeach*	Clone (void) const
										{ return DNEWCLASS(ParallelisedForeach, (*this)); }

	virtual void					Delete (void)
										{ DDELETE(this); }

	///////////////////////////////////////////////////////

	static ParallelisedForeach*		New (size_t const _part, size_t const _parts, size_t const _total, I const& _begin, I const& _end, Op const& _op)
										{ return DNEWCLASS(ParallelisedForeach, (_part, _parts, _total, _begin, _end, _op)); }

	///////////////////////////////////////////////////////

	ParallelisedForeach (size_t const _part, size_t const _parts, size_t const _total, I const& _begin, I const& _end, Op const& _op):
		part		(_part			),
		parts		(_parts			),
		total		(_total			),
		begin		(_begin			),
		end			(_end			),
		op			(_op			)
		{ DASSERT(InvariantsHold()); }
	ParallelisedForeach (ParallelisedForeach const& o):
		part		(o.part			),
		parts		(o.parts		),
		total		(o.total		),
		begin		(o.begin		),
		end			(o.end			),
		op			(o.op			)
		{ DASSERT(InvariantsHold()); }
	virtual ~ParallelisedForeach (void) {}

	///////////////////////////////////////////////////////

private:
	size_t const	part, parts, total;
	I const			begin, end;
	Op const		op;

	bool	InvariantsHold (void) const
				{ return parts > 0 && part < parts && begin != end && std::distance(begin, end) == total; }

	void operator = (ParallelisedForeach const&);
};

template <typename I, typename Op>
ParallelisedForeach<I, Op> const MakeParallelisedForeach (size_t const _part, size_t const _parts, size_t const _total, I const& _begin, I const& _end, Op const& _op)
	{ return ParallelisedForeach<I, Op>(_part, _parts, _total, _begin, _end, _op); }

template <typename C, typename Op>
void parallel_foreach (ParallelisationManager& m, C& c, size_t const parts, Op const& op) {
	for (size_t part(0u); part < parts; ++part)
		m.StartJob(MakeParallelisedForeach(part, parts, c.size(), c.begin(), c.end(), op));
}

///////////////////////////////////////////////////////////

struct vec3 {
	float x, y, z;

	vec3 const	operator * (vec3 const& v) const
					{ return vec3(x * v.x, y * v.y, z * v.z); }
	vec3 const	operator * (float const f) const
					{ return vec3(f * x, f * y, f * z); }

	vec3 const	mul_scalar (float const f) const
					{ return (*this) * f; }

	void		operator = (vec3 const& v)
					{ x = v.x; y = v.y; z = v.z; }

	vec3 (float _x, float _y, float _z): x(_x), y(_y), z(_z) {}
	vec3 (vec3 const& o): x(o.x), y(o.y), z(o.z) {}
	~vec3 (void) {}
};

///////////////////////////////////////////////////////

static inline void onerror (char const* const msg)
	{ puts(msg); }

static inline void modvec (vec3& v)
	{ v = v * 15.0f; }

int main6 (int argc, char** argv) {
	dinit(&onerror);

	{
		typedef std::list<vec3> vec3s;

		vec3s* const	vsp(reinterpret_cast<vec3s* const>(operator new(sizeof(vec3s))));
		new(vsp) vec3s;
		vec3s&			vs(*DNULLCHECK(vsp));

		puts("generating...");
		for (float i = 0.01f; i < 1.0f; i += 0.0000001f)
			vs.push_back(vec3(1.0f, 0.1f, 0.01f));

		{
			ParallelisationManager m;

			puts("modifying...");
			parallel_foreach(m, vs, 4, &modvec);
		}

		puts("don");
//		for (vec3s::const_iterator i = vs.begin(); i != vs.end(); ++i)
//			printf("<%f,%f,%f>\n", i->x, i->y, i->z);


		getchar();
	}

	dclose();
	return 0;
}
