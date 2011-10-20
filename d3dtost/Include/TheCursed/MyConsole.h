#ifndef __MY__CONSOLE__H__
#define __MY__CONSOLE__H__

#include <PWindows.h>
#pragma warning( push, 0 )
#	include "utypes.h"
#	include <tchar.h>
#	include <fstream>
#	include <ostream>
#	include <sstream>
#pragma warning( pop )
#include <PAssert.h>

namespace my {

	class Console {
	public:
													Console (void);
													~Console (void);

	void											WriteToOutputStream (TCHAR const*);
	void											WriteToOutputStreamA (char const*);
	void											WriteToErrorStream (TCHAR const*);

	template <typename T>
	Console&										Append (T const& o) {
														_T_OSSTREAM imp;
														imp << o;
														WriteToOutputStream(imp.str().c_str());
														return *this;
													}
	template <typename T>
	Console&										AppendA (T const& o) {
														std::ostringstream imp;
														imp << o;
														WriteToOutputStreamA(imp.str().c_str());
														return *this;
													}
	bool											IsValid (void) const;

	private:
		HANDLE										outProxy;
		HANDLE										errProxy;
		bool										consoleCreated;
		std::ofstream								log;

		Console (Console const&);
		void operator = (Console const&);
	}; // class Console

	template <typename T> static inline
	Console& operator << (Console& c, T const& o)
		{ return c.AppendA(o); }

	template <> static inline
	Console& operator <<<TCHAR const*> (Console& c, TCHAR const* const& str)
		{ PASSERT(false) (void)str; return c; }

	template <const size_t N> static inline
	Console& operator << (Console& c, const TCHAR (& str)[N])
		{ PASSERT(false) return c; }

} // namespace my

#endif // __MY__CONSOLE__H__
