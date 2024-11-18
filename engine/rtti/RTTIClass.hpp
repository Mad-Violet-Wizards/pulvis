#pragma once

#include "RTTIBase.hpp"
#include "RTTIMethod.hpp"
#include "RTTIField.hpp"
#include "RTTIHash.hpp"
#include "engine/core/Export.hpp"

namespace engine::rtti
{
	constexpr static const char* s_RttiInvalidClass = "INVALID_CLASS";

	class PULVIS_API CRTTIClass
	{
	public:

		static CRTTIClass* FindInStorage(type_id_t _type_id);
		static const CRTTIClass* FindConstInStorage(type_id_t _type_id);
		static CRTTIClass* FindInStorage(const char* _name);
		static const CRTTIClass* FindConstInStorage(const char* _name);

		CRTTIClass()
			: m_Name(s_RttiInvalidClass)
			, m_TypeId(INVALID_TYPE_ID)
		{

		}

		CRTTIClass(const char* _name)
			: m_Name(_name)
			, m_TypeId(Hash(_name))
		{
		}

		const std::string& GetName() const
		{
			return m_Name;
		}

		type_id_t GetTypeId() const
		{
			return m_TypeId;
		}

		void AttachParent(CRTTIClass* _parent)
		{
			m_Parents.push_back(_parent);
		}


		void AddMethod(CRTTIMethod* _method);
		CRTTIMethod* FindMethodByName(const char* _method_name);
		const CRTTIMethod* FindConstMethodByName(const char* _method_name) const;

		void AddField(CRTTIField* _field);
		CRTTIField* FindFieldByName(const char* _field_name);
		const CRTTIField* FindConstFieldByName(const char* _field_name) const;

	private:

		std::string m_Name;
		type_id_t m_TypeId;
		std::vector<CRTTIMethod*> m_Methods;
		std::vector<CRTTIField*> m_Fields;
		std::vector<CRTTIClass*> m_Parents;
	};
}
