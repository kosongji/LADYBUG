#pragma once
#include "common.h"

template <class T>

class Singleton
{
protected:
	Singleton() {}
	~Singleton() {}

public:
	Singleton(const Singleton& t);
	Singleton& operator = (const Singleton&);

	static T& getInstance()
	{
		static T instance;
		return instance;
	}
};

