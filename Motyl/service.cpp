#pragma once
#include "service.h"

using namespace gk2;

void* Service::operator new(size_t size)
{
	return Utils::New16Aligned(size);
}

void Service::operator delete(void* ptr)
{
	Utils::Delete16Aligned(ptr);
}


Service::Service()
{
}

Service::~Service()
{
}
