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
	mfError err = mfmIncObjectRef(m_obj);
	if (err != MF_ERROR_OKAY)
		throw ObjectError("Failed to increase object reference count");
}

Magma::Framework::Memory::Object::Object(void * obj)
{
	m_obj = static_cast<mfmObject*>(obj);
	if (m_obj != NULL)
	{
		mfError err = mfmIncObjectRef(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to increase object reference count");
	}
}

Magma::Framework::Memory::Object::Object(const Object & rhs)
{
	m_obj = rhs.m_obj;
	if(m_obj != nullptr)
	{
		mfError err = mfmIncObjectRef(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to increase object reference count");
	}
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
		mfError err = mfmDecObjectRef(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to decrease object reference count");
		mfmI32 refCount = 0;
		err = mfmGetObjectRefCount(m_obj, &refCount);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to get object reference count");
		if (refCount == 0 && m_obj->destructorFunc != nullptr)
			m_obj->destructorFunc(m_obj);
	}
}

void Magma::Framework::Memory::Object::Set(mfmObject& obj)
{
	if (m_obj != nullptr)
	{
		mfError err = mfmDecObjectRef(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to decrease object reference count");
		mfmI32 refCount = 0;
		err = mfmGetObjectRefCount(m_obj, &refCount);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to get object reference count");
		if (refCount == 0 && m_obj->destructorFunc != nullptr)
			m_obj->destructorFunc(m_obj);
	}
	m_obj = &obj;
	if (m_obj != nullptr)
	{
		mfError err = mfmIncObjectRef(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to increase object reference count");
	}
}

bool Magma::Framework::Memory::Object::Release()
{
	if (m_obj == nullptr)
		return false;
	mfError err = mfmDecObjectRef(m_obj);
	if (err != MF_ERROR_OKAY)
		throw ObjectError("Failed to decrease object reference count");
	mfmI32 refCount = 0;
	err = mfmGetObjectRefCount(m_obj, &refCount);
	if (err != MF_ERROR_OKAY)
		throw ObjectError("Failed to get object reference count");
	if (refCount == 0 && m_obj->destructorFunc != nullptr)
		m_obj->destructorFunc(m_obj);
	m_obj = NULL;
	return true;
}

mfmObject & Magma::Framework::Memory::Object::Get() const
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

Magma::Framework::Memory::Object & Magma::Framework::Memory::Object::operator=(Object obj)
{
	if (obj.GetNoChecks() == nullptr)
		this->Release();
	else
		this->Set(obj.Get());
	return *this;
}

mfmObject * Magma::Framework::Memory::Object::operator->() const
{
	return &this->Get();
}
