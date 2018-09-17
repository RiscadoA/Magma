#include <Magma/Framework/Memory/Handle.hpp>
#include <Magma/Framework/Memory/Exception.hpp>

#include <cstdlib>

Magma::Framework::Memory::Handle::Handle()
{
	m_obj = NULL;
}

Magma::Framework::Memory::Handle::Handle(mfmObject & obj)
{
	m_obj = &obj;
	mfError err = mfmAcquireObject(m_obj);
	if (err != MF_ERROR_OKAY)
		throw ObjectError("Failed to increase object reference count");
}

Magma::Framework::Memory::Handle::Handle(void * obj)
{
	m_obj = static_cast<mfmObject*>(obj);
	if (m_obj != NULL)
	{
		mfError err = mfmAcquireObject(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to increase object reference count");
	}
}

Magma::Framework::Memory::Handle::Handle(const Handle & rhs)
{
	m_obj = rhs.m_obj;
	if(m_obj != nullptr)
	{
		mfError err = mfmAcquireObject(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to increase object reference count");
	}
}

Magma::Framework::Memory::Handle::Handle(Handle && rhs)
{
	m_obj = rhs.m_obj;
	rhs.m_obj = nullptr;
}

Magma::Framework::Memory::Handle::~Handle()
{
	if (m_obj != nullptr)
	{
		mfError err = mfmReleaseObject(m_obj);
		if (err != MF_ERROR_OKAY)
			abort();
	}
}

void Magma::Framework::Memory::Handle::Set(mfmObject& obj)
{
	if (m_obj != nullptr)
	{
		mfError err = mfmReleaseObject(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to decrease object reference count");
	}
	m_obj = &obj;
	if (m_obj != nullptr)
	{
		mfError err = mfmAcquireObject(m_obj);
		if (err != MF_ERROR_OKAY)
			throw ObjectError("Failed to increase object reference count");
	}
}

bool Magma::Framework::Memory::Handle::Release()
{
	if (m_obj == nullptr)
		return false;
	mfError err = mfmReleaseObject(m_obj);
	if (err != MF_ERROR_OKAY)
		throw ObjectError("Failed to decrease object reference count");
	m_obj = nullptr;
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
