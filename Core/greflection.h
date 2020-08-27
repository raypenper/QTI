#ifndef GREGISTER_H
#define GREGISTER_H

#include <QString>
#include <QMap>
#include <QList>

// 注册类信息结构
template <class T>
struct GReflectionInfo
{
	typedef T * (*GCreateInstanceFunc)();

	QString mClassName;						// 类名
	QMap<QString, QString> mProperties;		// 附加属性表	
	GCreateInstanceFunc mCreateFunc;		// 创建函数

	GReflectionInfo(const QString &className="", GCreateInstanceFunc ptr=NULL)
	{
		mClassName = className;
		mCreateFunc = ptr;
	}

	GReflectionInfo(const GReflectionInfo &other)
	{
		*this = other;
	}

	GReflectionInfo & operator = (const GReflectionInfo &other)
	{
		mClassName = other.mClassName;
		mCreateFunc = other.mCreateFunc;
		mProperties = other.mProperties;
	}
};

// 注册辅助类
template <class T, class B>
class GReflectionRegister
{
public:
	GReflectionRegister(const QString &className, GReflectionInfo<B> *pInfo)
	{
		GReflection<B>::registerClass(className, pInfo);
		T::initGReflectionInfo<T>(pInfo);
	}
};

// 声明动态类宏
#define GREFLECTION_CLASS(klass, baseKlass) \
private: static GReflectionRegister<klass, baseKlass> *mpReflectionRegister; \
public: static baseKlass * createInstance() \
{ \
	return new klass(); \
} \
public: static QString getClassName() { return #klass; } \
public: QString className() { return klass::getClassName(); }

// 实现动态类宏
#define IMPL_GREFLECTION_CLASS(klass, baseKlass) \
	GReflectionRegister<klass, baseKlass> * klass::mpReflectionRegister = new GReflectionRegister<klass, baseKlass>(#klass, \
		new GReflectionInfo<baseKlass>(#klass, klass::createInstance));

// 反射类基类(通过继承该类使得子类具有注册和反射功能)
template <class T>
class GReflection
{
protected:
	// 反射注册信息映射表
	static QMap<QString, GReflectionInfo<T> *> *mpReflectionMap;

public:
	// 根据类名创建类对象
	static T * instance(const QString &className)
	{
		if(mpReflectionMap == NULL)
		{
			return NULL;
		}

		if(mpReflectionMap->find(className) != mpReflectionMap->end())
		{
			return (*mpReflectionMap)[className]->mCreateFunc();
		}

		return NULL;
	}

	// 根据类模板创建类对象
	template <class R>
	static R * instance()
	{
		return dynamic_cast<R *>(instance(R::getClassName()));
	}

	// 注册类
	static bool registerClass(const QString &className, GReflectionInfo<T> *pInfo)
	{
		// 在这里进行构造, 确保注册类构造函数时映射表一定存在
		if(mpReflectionMap == NULL)
		{
			mpReflectionMap = new QMap<QString, GReflectionInfo<T> *>();
		}

		if(mpReflectionMap->find(className) != mpReflectionMap->end())
		{
			Q_ASSERT(false);
			return false;
		}

		(*mpReflectionMap)[className] = pInfo;
		return true;
	}

	// 注销类
	static void unregisterClass(const QString &className)
	{
		if(mpReflectionMap != NULL)
		{
			mpReflectionMap->remove(className);
		}
	}

	// 得到注册的类型信息列表
	static QList<GReflectionInfo<T> *> getRegisterInfoList()
	{
		return mpReflectionMap->values();
	}

	// 得到反射类型信息
	static GReflectionInfo<T> * getReflectionInfo(const QString &className)
	{
		QMap<QString, GReflectionInfo<T> *>::iterator it = mpReflectionMap->find(className);
		if (it == mpReflectionMap->end()) return NULL;
		else return it.value();
	}

	// 初始反射类信息, 如果子类希望增加额外注册信息, 可以重新实现该函数
	// 通过info.mProperties设置额外属性
	template <class S>
	static void initGReflectionInfo(GReflectionInfo<T> *)
	{

	}
};

template<class T>
QMap<QString, GReflectionInfo<T> *> * GReflection<T>::mpReflectionMap = NULL;

#endif