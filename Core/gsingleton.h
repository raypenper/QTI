#ifndef GSINGLETON_H
#define GSINGLETON_H

#define G_SINGLETON(Class) \
public: static Class * instance() \
{ \
	static Class *singleton = NULL; \
	if (!singleton) singleton = new Class(); \
	return singleton; \
}

template <class T>
class GSingleton
{
public:
	static T * instance()
	{
		static T *singleton = NULL;
		if (!singleton) singleton = new T();
		return singleton;
	}
};

#endif