#include "engine/engine_pch.hpp"
#include "RTTIClass.hpp"
#include "detail/RTTIClassDetail.hpp"

namespace engine::rtti
{
  class CRTTIClass::Impl
  {
    public:

		  std::string m_Name;
	  	type_id_t m_TypeId;
	  	std::vector<CRTTIMethod*> m_Methods;
	  	std::vector<CRTTIField*> m_Fields;
	  	std::vector<CRTTIClass*> m_Parents;
  };

  //////////////////////////////////////////////////////////////////////////
	CRTTIClass::CRTTIClass(const char* _name)
	{
    m_Impl = new Impl();
		m_Impl->m_Name = _name;
		m_Impl->m_TypeId = rtti::Hash(_name);
	}

  CRTTIClass::~CRTTIClass()
  {
    delete m_Impl;
  }

	const std::string& CRTTIClass::GetName() const
	{
    return m_Impl->m_Name;
	}

	type_id_t CRTTIClass::GetTypeId() const
	{
    return m_Impl->m_TypeId;
	}

  CRTTIClass* CRTTIClass::FindInStorage(type_id_t _type_id)
  {
    return detail::CRTTIClassStorage::FindClassById(_type_id);
  }

  const CRTTIClass* CRTTIClass::FindConstInStorage(type_id_t _type_id)
  {
    return detail::CRTTIClassStorage::FindConstClassById(_type_id);
  }

  CRTTIClass* CRTTIClass::FindInStorage(const char* _name)
  {
    return detail::CRTTIClassStorage::FindClassByName(_name);
  }

  const CRTTIClass* CRTTIClass::FindConstInStorage(const char* _name)
  {
    return detail::CRTTIClassStorage::FindConstClassByName(_name);
  }

  void CRTTIClass::AttachParent(CRTTIClass* _parent)
  {
		m_Impl->m_Parents.push_back(_parent);
  }

  void CRTTIClass::AddMethod(CRTTIMethod* _method)
    {
      m_Impl->m_Methods.push_back(_method);
    }

    CRTTIMethod* CRTTIClass::FindMethodByName(const char* _method_name)
    {
      for (CRTTIMethod* m : m_Impl->m_Methods)
			{
				if (m->GetName() == _method_name)
				{
					return m;
				}
			}

      for (CRTTIClass* p : m_Impl->m_Parents)
      {
        if (CRTTIMethod* m = p->FindMethodByName(_method_name))
        {
          return m;
        }
      }

      return nullptr;
    }

    const CRTTIMethod* CRTTIClass::FindConstMethodByName(const char* _method_name) const
    {
			for (const CRTTIMethod* m : m_Impl->m_Methods)
			{
				if (m->GetName() == _method_name)
				{
					return m;
				}
			}

			for (const CRTTIClass* p : m_Impl->m_Parents)
			{
				if (const CRTTIMethod* m = p->FindConstMethodByName(_method_name))
				{
					return m;
				}
			}

			return nullptr;
    }

    void CRTTIClass::AddField(CRTTIField* _field)
    {
      m_Impl->m_Fields.push_back(_field);
    }

    CRTTIField* CRTTIClass::FindFieldByName(const char* _field_name)
    {
      for (CRTTIField* f : m_Impl->m_Fields)
      {
        if (f->GetName() == _field_name)
        {
          return f;
        }
      }

      for (CRTTIClass* p : m_Impl->m_Parents)
			{
				if (CRTTIField* f = p->FindFieldByName(_field_name))
				{
					return f;
				}
			}

      return nullptr;
    }

    const CRTTIField* CRTTIClass::FindConstFieldByName(const char* _field_name) const
    {
      for (const CRTTIField* f : m_Impl->m_Fields)
			{
				if (f->GetName() == _field_name)
				{
					return f;
				}
			}

      for (const CRTTIClass* p : m_Impl->m_Parents)
      {
        if (const CRTTIField* f = p->FindConstFieldByName(_field_name))
        {
          return f;
        }
      }

      return nullptr;
    }
}