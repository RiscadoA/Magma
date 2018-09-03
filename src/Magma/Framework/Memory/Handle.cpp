#include "Handle.hpp"
#include "Exception.hpp"

#include <cstdlib>

Magma::Framework::Memory::Handle::Handle()
{
	m_obj = NULL;
}

Magma::Framework::Memory::Handle::Handle(mfmObject & obj)
{
	m_obj = &obj;
	mfError err = mfmIncObjectRef(m_obj);
	if (err != MF_ERROR_OKAY)
		throw ObjectError("Failed to increase object reference count");
}

Magma::Framework::Memory::Handle::Handle(void * obj)
{
	m_obj = static_cast<mfmObject*>(obj);
	if (m_obj != NULL)
	{
		mfError err = mfmIncObjectRef(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to increase object reference count");
	}
}

Magma::Framework::Memory::Handle::Handle(const Handle & rhs)
{
	m_obj = rhs.m_obj;
	if(m_obj != nullptr)
	{
		mfError err = mfmIncObjectRef(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to increase object reference count");
	}
}

Magma::Framework::Memory::Handle::Handle(Handle && rhs)
{
	m_obj = rhs.m_obj;
	rhs.m_obj = NULL;
}

Magma::Framework::Memory::Handle::~Handle()
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

void Magma::Framework::Memory::Handle::Set(mfmObject& obj)
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

bool Magma::Framework::Memory::Handle::Release()
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

mfmObject & Magma::Framework::Memory::Handle::Get() const
{
	if (m_obj == nullptr)
		throw NullPointerError("Failed to get object, object is NULL (wasn't set or was already released)");
	return *m_obj;
}

Magma::Framework::Memory::Handle & Magma::Framework::Memory::Handle::operator=(mfmObject & obj)
{
	this->Set(obj);
	return *this;
}

Magma::Framework::Memory::Handle & Magma::Framework::Memory::Handle::operator=(Handle obj)
{
	if (obj.GetNoChecks() == nullptr)
		this->Release();
	else
		this->Set(obj.Get());
	return *this;
}

mfmObject * Magma::Framework::Memory::Handle::operator->() const
{
	return &this->Get();
}
