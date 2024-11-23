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
		{
			m_Impl = nullptr;
		}

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

	private:

		class Impl;
		Impl* m_Impl;
	};
}
