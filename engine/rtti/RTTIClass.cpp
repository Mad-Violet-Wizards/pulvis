#include "engine/engine_pch.hpp"
#include "RTTIClass.hpp"
#include "detail/RTTIClassDetail.hpp"

namespace engine::rtti
{
  //////////////////////////////////////////////////////////////////////////
	CRTTIClass::CRTTIClass(const char* _name)
	{
		m_Name = _name;
		m_TypeId = rtti::Hash(_name);
	}

  CRTTIClass::~CRTTIClass()
  {
  }

	const std::string& CRTTIClass::GetName() const
	{
    return m_Name;
	}

	type_id_t CRTTIClass::GetTypeId() const
	{
    return m_TypeId;
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

  CRTTIClass* CRTTIClass::FindInStorage(std::string_view _sv_name)
  {
    return detail::CRTTIClassStorage::FindClassByNameSv(_sv_name);
  }

  const CRTTIClass* CRTTIClass::FindConstInStorage(std::string_view _sv_name)
  {
    return detail::CRTTIClassStorage::FindConstClassByNameSv(_sv_name);
  }

  void CRTTIClass::AttachParent(CRTTIClass* _parent)
  {
		m_Parents.push_back(_parent);
  }

  void CRTTIClass::AddMethod(CRTTIMethod* _method)
    {
      m_Methods.push_back(_method);
    }

    CRTTIMethod* CRTTIClass::FindMethodByName(const char* _method_name)
    {
      for (CRTTIMethod* m : m_Methods)
			{
				if (m->GetName() == _method_name)
				{
					return m;
				}
			}

      for (CRTTIClass* p : m_Parents)
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
			for (const CRTTIMethod* m : m_Methods)
			{
				if (m->GetName() == _method_name)
				{
					return m;
				}
			}

			for (const CRTTIClass* p : m_Parents)
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
      m_Fields.push_back(_field);
    }

    CRTTIField* CRTTIClass::FindFieldByName(const char* _field_name)
    {
      for (CRTTIField* f : m_Fields)
      {
        if (f->GetName() == _field_name)
        {
          return f;
        }
      }

      for (CRTTIClass* p : m_Parents)
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
      for (const CRTTIField* f : m_Fields)
			{
				if (f->GetName() == _field_name)
				{
					return f;
				}
			}

      for (const CRTTIClass* p : m_Parents)
      {
        if (const CRTTIField* f = p->FindConstFieldByName(_field_name))
        {
          return f;
        }
      }

      return nullptr;
    }
    const std::vector<CRTTIField*>& CRTTIClass::GetFields() const
    {
      return m_Fields;
    }
}