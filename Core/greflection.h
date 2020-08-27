#ifndef GREGISTER_H
#define GREGISTER_H

#include <QString>
#include <QMap>
#include <QList>

// ע������Ϣ�ṹ
template <class T>
struct GReflectionInfo
{
	typedef T * (*GCreateInstanceFunc)();

	QString mClassName;						// ����
	QMap<QString, QString> mProperties;		// �������Ա�	
	GCreateInstanceFunc mCreateFunc;		// ��������

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

// ע�Ḩ����
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

// ������̬���
#define GREFLECTION_CLASS(klass, baseKlass) \
private: static GReflectionRegister<klass, baseKlass> *mpReflectionRegister; \
public: static baseKlass * createInstance() \
{ \
	return new klass(); \
} \
public: static QString getClassName() { return #klass; } \
public: QString className() { return klass::getClassName(); }

// ʵ�ֶ�̬���
#define IMPL_GREFLECTION_CLASS(klass, baseKlass) \
	GReflectionRegister<klass, baseKlass> * klass::mpReflectionRegister = new GReflectionRegister<klass, baseKlass>(#klass, \
		new GReflectionInfo<baseKlass>(#klass, klass::createInstance));

// ���������(ͨ���̳и���ʹ���������ע��ͷ��书��)
template <class T>
class GReflection
{
protected:
	// ����ע����Ϣӳ���
	static QMap<QString, GReflectionInfo<T> *> *mpReflectionMap;

public:
	// �����������������
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

	// ������ģ�崴�������
	template <class R>
	static R * instance()
	{
		return dynamic_cast<R *>(instance(R::getClassName()));
	}

	// ע����
	static bool registerClass(const QString &className, GReflectionInfo<T> *pInfo)
	{
		// ��������й���, ȷ��ע���๹�캯��ʱӳ���һ������
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

	// ע����
	static void unregisterClass(const QString &className)
	{
		if(mpReflectionMap != NULL)
		{
			mpReflectionMap->remove(className);
		}
	}

	// �õ�ע���������Ϣ�б�
	static QList<GReflectionInfo<T> *> getRegisterInfoList()
	{
		return mpReflectionMap->values();
	}

	// �õ�����������Ϣ
	static GReflectionInfo<T> * getReflectionInfo(const QString &className)
	{
		QMap<QString, GReflectionInfo<T> *>::iterator it = mpReflectionMap->find(className);
		if (it == mpReflectionMap->end()) return NULL;
		else return it.value();
	}

	// ��ʼ��������Ϣ, �������ϣ�����Ӷ���ע����Ϣ, ��������ʵ�ָú���
	// ͨ��info.mProperties���ö�������
	template <class S>
	static void initGReflectionInfo(GReflectionInfo<T> *)
	{

	}
};

template<class T>
QMap<QString, GReflectionInfo<T> *> * GReflection<T>::mpReflectionMap = NULL;

#endif