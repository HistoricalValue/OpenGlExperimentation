#include <stdafx.h>


#ifdef LOGGER_WITH_PROTECTED_LIST
#	define LOGGER__GET_MESSAGES_VAL_INTO(INTO)			{ P_PROTECTED_CLASS_VAL(messages, INTO) }
#	define LOGGER__MESSAGES_PUSH_BACK(MSG)				{ P_PROTECTED_CLASS_OP(messages, push_back((MSG))) }
#	define LOGGER__IS_MESSAGE_LIST_FULL					P_PROTECTED_CLASS_CONSTOP(messages, IsFull())
#else
#	define LOGGER__GET_MESSAGES_VAL_INTO(INTO)			INTO = &messages;
#	define LOGGER__MESSAGES_PUSH_BACK(MSG)				messages.push_back((MSG));
#	define LOGGER__IS_MESSAGE_LIST_FULL					messages.IsFull()
#endif

namespace my {
	LoggerBase::LoggerBase (void):
		messages(UnPMessages()),
		isFull(false) {
		{ std::wostringstream fout; }
	}

	LoggerBase::~LoggerBase (void) {
		UnPMessages* messages_p(NULL);
		LOGGER__GET_MESSAGES_VAL_INTO(messages_p)
		UnPMessages::const_iterator			ite(messages_p->begin());
		UnPMessages::const_iterator const	end(messages_p->end());

		_T_OFSTREAM fout(_T("log.txt"), std::ios::out | std::ios::trunc);
		for (; ite != end; ++ite)
			fout << *ite << std::endl; // somethind in the future

		if (isFull)
			fout << _T("--- LOGGER WARNING ---: Logger got filled up and could not hold any more messages!!!") << std::endl;
	}

	void LoggerBase::LogImpl (String const& msg) {
		{ std::wostringstream fout; }
		PASSERT(LOGGER__IS_MESSAGE_LIST_FULL || !isFull)
		if (isFull || LOGGER__IS_MESSAGE_LIST_FULL)
			isFull = true;
		else
			LOGGER__MESSAGES_PUSH_BACK(msg)
		{ std::wostringstream fout; }
	}

	SpecificLogger::SpecificLogger (String const& _loggerName):
		loggerName(_loggerName)
	{ { std::wostringstream fout; } }

	SpecificLogger::~SpecificLogger (void) {
		{ std::wostringstream fout; }
	}

	void SpecificLogger::Error (String const& msg) {
		{ std::wostringstream fout; }
		LogImpl(loggerName + _T("/ERROR: ") + msg);
		{ std::wostringstream fout; }
	}

	void SpecificLogger::Warning (String const& msg) {
		{ std::wostringstream fout; }
		LogImpl(loggerName + _T("/WARNING: ") + msg);
		{ std::wostringstream fout; }
	}

	void SpecificLogger::Info (String const& msg) {
		{ std::wostringstream fout; }
		LogImpl(loggerName + _T("/INFO: ") + msg);
		{ std::wostringstream fout; }
	}


	GenericLogger::GenericLogger (void):
		SpecificLogger(_T("general"))
	{ { std::wostringstream fout; } }

	GenericLogger::~GenericLogger (void) {
		{ std::wostringstream fout; }
	}
} // namespace my