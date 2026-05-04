#pragma once

#include "RTTIBase.hpp"
#include "RTTIMacros.hpp"

#include <string>

namespace pulvis::rtti::tests
{
	class [[Reflection::Class]] FieldsAttributes : public pulvis::rtti::IRTTIBase
	{
		RTTI_CLASS_API(FieldsAttributes);

	public:

		// INT FIELDS
		[[Reflection::Field]]
		int NoAttributeIntField;

		[[Reflection::Field("Serializable")]]
		int SerializableIntField = 0;

		[[Reflection::Field("Scriptable")]]
		int ScriptableIntField = 0;

		[[Reflection::Field("Editable")]]
		int EditableIntField = 0;

		[[Reflection::Field("EditorReadOnly")]]
		int EditorReadOnlyIntField = 0;

		[[Reflection::Field("Replicated")]]
		int ReplicatedIntField = 0;

		// MIXED FIELDS
		[[Reflection::Field]]
		float NoAttributeMixedField;

		[[Reflection::Field("Serializable")]]
		double SerializableMixedField = 0;

		[[Reflection::Field("Scriptable")]]
		unsigned int ScriptableMixedField = 0;

		[[Reflection::Field("Editable")]]
		char EditableMixedField = 0;

		[[Reflection::Field("EditorReadOnly")]]
		std::string EditorReadOnlyMixedField;

		[[Reflection::Field("Replicated")]]
		long long ReplicatedMixedField = 0;

		// MIXED FIELDS & MIXED ATTRIBUTES
		[[Reflection::Field("Serializable", "Scriptable")]]
		int SerializableScriptableIntField = 0;
		[[Reflection::Field("Editable", "EditorReadOnly")]]
		int EditableEditorReadOnlyIntField = 0;

		[[Reflection::Field("EditorReadOnly", "Replicated")]]
		float EditorReadOnlyReplicatedMixedField = 0;

		[[Reflection::Field("Serializable", "Editable", "Replicated")]]
		double SerializableEditableReplicatedMixedField = 0;

		[[Reflection::Field("Scriptable", "EditorReadOnly", "Replicated")]]
		unsigned int ScriptableEditorReadOnlyReplicatedMixedField = 0;

		[[Reflection::Field("Serializable", "Scriptable", "Editable", "Replicated")]]
		char SerializableScriptableEditableReplicatedMixedField = 0;
	};

	class [[Reflection::Class]] MethodsAttributes : public pulvis::rtti::IRTTIBase
	{
		RTTI_CLASS_API(MethodsAttributes);

	public:

		[[Reflection::Method]]
		void NoAttributeMethod() {}

		[[Reflection::Method("Scriptable")]]
		void ScriptableMethod() {}

		[[Reflection::Method("EditorCallable")]]
		void EditorCallableMethod() {}

		[[Reflection::Method("Command")]]
		void CommandMethod() {}

		[[Reflection::Method("Pure")]]
		virtual void PureVirtualMethod() = 0;

		[[Reflection::Method("Scriptable", "EditorCallable")]]
		void ScriptableEditorCallableMethod() {}

		[[Reflection::Method("EditorCallable")]]
		void EditorCallableEventMethod() {}

		[[Reflection::Method("Command", "Pure")]]
		virtual void CommandPureVirtualMethod() = 0;
		};
}