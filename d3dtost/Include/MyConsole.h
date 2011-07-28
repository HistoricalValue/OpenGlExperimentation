#ifndef __MY__CONSOLE__H__
#define __MY__CONSOLE__H__

#include "MyWindows.h"

#include <ostream>

namespace my {

	class Console {
	public:
													Console (void);
													~Console (void);

	void											WriteToOutputStream (LPCTSTR);
	void											WriteToOutputStreamA (char const*);
	void											WriteToErrorStream (LPCTSTR);

	bool											IsValid (void) const;

	private:
		HANDLE										outProxy;
		HANDLE										errProxy;
		bool										consoleCreated;

	}; // class Console

	template <typename T>
	Console& operator << (Console& c, T const& o) {
		_T_OSSTREAM imp;
		imp << o;
		c.WriteToOutputStream(imp.str().c_str());
		return c;
	}

} // namespace my

#endif // __MY__CONSOLE__H__
