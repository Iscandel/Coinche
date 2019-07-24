#ifndef H__NONCOPYABLE_311020132116__H
#define H__NONCOPYABLE_311020132116__H

///////////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// \brief Utility class thats prevents any derived class to be copyable
///////////////////////////////////////////////////////////////////////////////
class NonCopyable
{
protected:
	NonCopyable() {}
	~NonCopyable() {}
private:
	NonCopyable(const NonCopyable&);
	const NonCopyable& operator = (const NonCopyable&);
};


#endif