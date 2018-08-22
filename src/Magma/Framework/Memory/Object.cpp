#include "Object.hpp"
#include "Exception.hpp"

#include <cstdlib>

Magma::Framework::Memory::Object::Object()
{
	m_obj = NULL;
}

Magma::Framework::Memory::Object::Object(mfmObject & obj)
{
	m_obj = &obj;
	++m_obj->referenceCount;
}

Magma::Framework::Memory::Object::Object(void * obj)
{
	m_obj = static_cast<mfmObject*>(obj);
	if (m_obj != NULL)
		++m_obj->referenceCount;
}

Magma::Framework::Memory::Object::Object(const Object & rhs)
{
	m_obj = rhs.m_obj;
	if(m_obj != nullptr)
		++m_obj->referenceCount;
}

Magma::Framework::Memory::Object::Object(Object && rhs)
{
	m_obj = rhs.m_obj;
	rhs.m_obj = NULL;
}

Magma::Framework::Memory::Object::~Object()
{
	if (m_obj != nullptr)
	{
		--m_obj->referenceCount;
		if (m_obj->referenceCount == 0 && m_obj->destructorFunc != nullptr)
			m_obj->destructorFunc(m_obj);
	}
}

void Magma::Framework::Memory::Object::Set(mfmObject& obj)
{
	if (m_obj != nullptr)
	{
		--m_obj->referenceCount;
		if (m_obj->referenceCount == 0 && m_obj->destructorFunc != nullptr)
			m_obj->destructorFunc(m_obj);
	}
	m_obj = &obj;
	if (m_obj != nullptr)
		++m_obj->referenceCount;
}

bool Magma::Framework::Memory::Object::Release()
{
	if (m_obj == nullptr)
		return false;
	--m_obj->referenceCount;
	if (m_obj->referenceCount == 0 && m_obj->destructorFunc != nullptr)
		m_obj->destructorFunc(m_obj);
	return true;
}

mfmObject & Magma::Framework::Memory::Object::Get()
{
	if (m_obj == nullptr)
		throw NullPointerError("Failed to get object, object is NULL (wasn't set or was already released)");
	return *m_obj;
}

Magma::Framework::Memory::Object & Magma::Framework::Memory::Object::operator=(mfmObject & obj)
{
	this->Set(obj);
	return *this;
}

mfmObject * Magma::Framework::Memory::Object::operator->()
{
	return &this->Get();
}
