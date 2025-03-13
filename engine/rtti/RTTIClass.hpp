#pragma once

#include "RTTIBase.hpp"
#include "RTTIMethod.hpp"
#include "RTTIField.hpp"
#include "RTTIHash.hpp"

#include "engine/vendor/sol3/sol.hpp"

namespace engine::rtti
{
	constexpr static const char* s_RttiInvalidClass = "INVALID_CLASS";

	class CRTTIClass
	{
	public:

		static CRTTIClass* FindInStorage(type_id_t _type_id);
		static const CRTTIClass* FindConstInStorage(type_id_t _type_id);
		static CRTTIClass* FindInStorage(const char* _name);
		static const CRTTIClass* FindConstInStorage(const char* _name);
		static CRTTIClass* FindInStorage(std::string_view _sv_name);
		static const CRTTIClass* FindConstInStorage(std::string_view _sv_name);

		CRTTIClass() = default;
		CRTTIClass(const char* _name);
		~CRTTIClass();

		const std::string& GetName() const;
		type_id_t GetTypeId() const;

		void AttachParent(CRTTIClass* _parent);

		void AddMethod(CRTTIMethod* _method);
		CRTTIMethod* FindMethodByName(const char* _method_name);
		const CRTTIMethod* FindConstMethodByName(const char* _method_name) const;

		void AddField(CRTTIField* _field);
		CRTTIField* FindFieldByName(const char* _field_name);
		const CRTTIField* FindConstFieldByName(const char* _field_name) const;
		const std::vector<CRTTIField*>& GetFields() const;

	private:

		std::string m_Name;
		type_id_t m_TypeId;
		std::vector<CRTTIMethod*> m_Methods;
		std::vector<CRTTIField*> m_Fields;
		std::vector<CRTTIClass*> m_Parents;
	};
}
