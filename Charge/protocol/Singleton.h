//#pragma once
//#include "common.h"
//
////template <class T>
////
////class Singleton
////{
////protected:
////	Singleton() {}
////	~Singleton() {}
////
////public:
////	Singleton(const Singleton& t);
////	Singleton& operator = (const Singleton&);
////
////	static T& getInstance()
////	{
////		static T instance;
////		return instance;
////	}
////};
////
//
//template < typename T >
//class TemplateSingleton
//{
//protected:
//	TemplateSingleton()
//	{
//
//	}
//	virtual ~TemplateSingleton()
//	{
//
//	}
//
//public:
//	static T * GetInstance()
//	{
//		if (m_pInstance == NULL)
//			m_pInstance = new T;
//		return m_pInstance;
//	};
//
//	static void DestroyInstance()
//	{
//		if (m_pInstance)
//		{
//			delete m_pInstance;
//			m_pInstance = NULL;
//		}
//	};
//
//private:
//	static T * m_pInstance;
//};
//
//template <typename T> T * TemplateSingleton<T>::m_pInstance = 0;
//
//

//// C++ 11 
//template <typename T>
//inline T& Singleton<T>::getInstance() {
//	static T inst;
//	return inst;
//}