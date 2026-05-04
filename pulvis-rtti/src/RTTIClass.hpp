#pragma once

#include "RTTIBase.hpp"
#include "RTTIMethod.hpp"
#include "RTTIField.hpp"
#include "RTTIAttributes.hpp"

#include <vector>

namespace pulvis::rtti
{
	constexpr static const char* s_RttiInvalidClass = "INVALID_CLASS";

	class CRTTIClass
	{
	public:

		CRTTIClass() = default;
		CRTTIClass(const char* _name, ERTTIClassAttribute _attributes = ERTTIClassAttribute::None);
		~CRTTIClass();

		const std::string& GetName() const;
		type_id_t GetTypeId() const;

		void AttachParent(CRTTIClass* _parent);

		[[nodiscard]] bool HasAttribute(ERTTIClassAttribute _attribute) const
		{
			return (m_Attributes & _attribute) == _attribute;
		}

		[[nodiscard]] ERTTIClassAttribute GetAttributes() const
		{
			return m_Attributes;
		}

		void AddMethod(CRTTIMethod* _method);
		CRTTIMethod* FindMethodByName(const char* _method_name);
		const CRTTIMethod* FindConstMethodByName(const char* _method_name) const;

		void AddField(CRTTIField* _field);
		CRTTIField* FindFieldByName(const char* _field_name);
		const CRTTIField* FindConstFieldByName(const char* _field_name) const;
		const std::vector<CRTTIField*>& GetFields() const;

	public:

		static CRTTIClass* FindInStorage(type_id_t _type_id);
		static const CRTTIClass* FindConstInStorage(type_id_t _type_id);
		static CRTTIClass* FindInStorage(const char* _name);
		static const CRTTIClass* FindConstInStorage(const char* _name);

	private:

		std::string m_Name;
		type_id_t m_TypeId;
		ERTTIClassAttribute m_Attributes = ERTTIClassAttribute::None;
		std::vector<CRTTIMethod*> m_Methods;
		std::vector<CRTTIField*> m_Fields;
		std::vector<CRTTIClass*> m_Parents;
	};
}
