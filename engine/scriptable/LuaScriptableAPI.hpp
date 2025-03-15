#pragma once
#include "engine/vendor/sol3/sol.hpp"
#include "engine/rtti/RTTIClass.hpp"

namespace engine::scriptable
{
	class LuaScriptableAPI
	{
		public:

			template<class C>
			static void ExportScriptableClassFields(sol::table& table, const rtti::CRTTIClass* rtti_class, C* _instance)
			{
				const std::vector<rtti::CRTTIField*>& fields = rtti_class->GetFields();
				for (rtti::CRTTIField* field : fields)
				{
					const std::string& field_name = field->GetName();
					const rtti::ERTTIFieldType field_type = field->GetFieldType();
					const rtti::ERTTIFieldAccess field_access = field->GetFieldAccess();

					switch (field_type)
					{
					case rtti::ERTTIFieldType::Int:
					{
						table.set_function(field_name, [field, rtti_class, _instance](int value)
							{
								field->Set<C, int>(_instance, value);
							});
						break;
					}
					case rtti::ERTTIFieldType::Float:
					{
						table.set_function(field_name, [field, rtti_class, _instance](float value)
							{
								field->Set<C, float>(_instance, value);
							});
						break;
					}
					case rtti::ERTTIFieldType::String:
					{
						table.set_function(field_name, [field, rtti_class, _instance](const std::string& value)
							{
								field->Set<C, std::string>(_instance, value);
							});
						break;
					}
					case rtti::ERTTIFieldType::Bool:
					{
						table.set_function(field_name, [field, rtti_class, _instance](bool value)
							{
								field->Set<C, bool>(_instance, value);
							});
						break;
					}
					case rtti::ERTTIFieldType::Uint:
					{
						table.set_function(field_name, [field, rtti_class, _instance](unsigned int value)
							{
								field->Set<C, unsigned int>(_instance, value);
							});
						break;
					}
					default:
					{
						PULVIS_WARNING_LOG("{}", "Unsupported field type.");
						break;
					}
					}
				}
			}
	};
}
