#ifndef __MY_LOGGER_H__
#define __MY_LOGGER_H__

#include "MyUtil.h"
#include "MyString.h"
#include "MyList.h"

#include "PProtectedClass.h"

#define LOGGER_WITH_PROTECTED_LIST

#ifdef LOGGER_WITH_PROTECTED_LIST
#	define LOGGER__MESSAGES								PMessages
#else
#	define LOGGER__MESSAGES								UnPMessages
#endif

namespace my {

#pragma warning( push )
#pragma warning( disable: 4251 ) // "messages"(ProtectedClass<>), "loggerName"(String) needs dll-interface for clients of LoggerBase
	class MYUTIL_API LoggerBase {
	protected:
														LoggerBase (void);
														~LoggerBase (void);

		void											LogImpl (String const&);
	private:
		
		typedef codeshare::DefaultAllocator<String>		StringAllocator;
		typedef ProtectiveList<String,StringAllocator>	UnPMessages;
		typedef P_PROTECTED_CLASS(UnPMessages)			PMessages;
		typedef LOGGER__MESSAGES						Messages;
		Messages										messages;
		bool											isFull;
	};

	class MYUTIL_API SpecificLogger: public LoggerBase {
	public:
		explicit										SpecificLogger (String const& loggerName);
														~SpecificLogger (void);

		void											Error (String const&);
		void											Warning (String const&);
		void											Info (String const&);
	private:
		String											loggerName;
	};
#pragma warning( pop )

	class MYUTIL_API GenericLogger: public SpecificLogger {
	public:
														GenericLogger (void);
														~GenericLogger (void);
	};

} // namespace my

#endif // __MY_LOGGER_H__
