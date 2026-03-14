#include "RttiAttributesTestsMoc.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::rtti;
using namespace pulvis::rtti::tests;

TEST_CASE("RTTI Attributes -- Field", "[RTTI][Attributes][Fields]")
{
	const pulvis::rtti::CRTTIClass* cls = pulvis::rtti::CRTTIClass::FindConstInStorage(pulvis::rtti::CRTTITypeInfo<pulvis::rtti::tests::FieldsAttributes>::GetTypeId());

	SECTION("Int fields are ok?")
	{
		const CRTTIField* no_attr_field = cls->FindConstFieldByName("NoAttributeIntField");

		REQUIRE(no_attr_field != nullptr);
		CHECK(no_attr_field->HasAttribute(ERTTIFieldAttribute::None));

		const CRTTIField* serializable_field = cls->FindConstFieldByName("SerializableIntField");
		REQUIRE(serializable_field != nullptr);
		CHECK(serializable_field->HasAttribute(ERTTIFieldAttribute::Serializable));

		const CRTTIField* scriptable_field = cls->FindConstFieldByName("ScriptableIntField");
		REQUIRE(scriptable_field != nullptr);
		CHECK(scriptable_field->HasAttribute(ERTTIFieldAttribute::Scriptable));

		const CRTTIField* editable_field = cls->FindConstFieldByName("EditableIntField");
		REQUIRE(editable_field != nullptr);
		CHECK(editable_field->HasAttribute(ERTTIFieldAttribute::Editable));

		const CRTTIField* editor_readonly_field = cls->FindConstFieldByName("EditorReadOnlyIntField");
		REQUIRE(editor_readonly_field != nullptr);
		CHECK(editor_readonly_field->HasAttribute(ERTTIFieldAttribute::EditorReadOnly));

		const CRTTIField* replicated_field = cls->FindConstFieldByName("ReplicatedIntField");
		REQUIRE(replicated_field != nullptr);
		CHECK(replicated_field->HasAttribute(ERTTIFieldAttribute::Replicated));
	}

	SECTION("Mixed fields are ok?")
	{
		const CRTTIField* no_attr_field = cls->FindConstFieldByName("NoAttributeMixedField");
		REQUIRE(no_attr_field != nullptr);
		CHECK(no_attr_field->HasAttribute(ERTTIFieldAttribute::None));
		const CRTTIField* serializable_field = cls->FindConstFieldByName("SerializableMixedField");
		REQUIRE(serializable_field != nullptr);
		CHECK(serializable_field->HasAttribute(ERTTIFieldAttribute::Serializable));
		const CRTTIField* scriptable_field = cls->FindConstFieldByName("ScriptableMixedField");
		REQUIRE(scriptable_field != nullptr);
		CHECK(scriptable_field->HasAttribute(ERTTIFieldAttribute::Scriptable));
		const CRTTIField* editable_field = cls->FindConstFieldByName("EditableMixedField");
		REQUIRE(editable_field != nullptr);
		CHECK(editable_field->HasAttribute(ERTTIFieldAttribute::Editable));
		const CRTTIField* editor_readonly_field = cls->FindConstFieldByName("EditorReadOnlyMixedField");
		REQUIRE(editor_readonly_field != nullptr);
		CHECK(editor_readonly_field->HasAttribute(ERTTIFieldAttribute::EditorReadOnly));
		const CRTTIField* replicated_field = cls->FindConstFieldByName("ReplicatedMixedField");
		REQUIRE(replicated_field != nullptr);
		CHECK(replicated_field->HasAttribute(ERTTIFieldAttribute::Replicated));
	}

	SECTION("Mixed fields & mixed attributes")
	{
		const CRTTIField* serializable_scriptable_field = cls->FindConstFieldByName("SerializableScriptableIntField");
		REQUIRE(serializable_scriptable_field != nullptr);
		CHECK(serializable_scriptable_field->HasAttribute(ERTTIFieldAttribute::Serializable));
		CHECK(serializable_scriptable_field->HasAttribute(ERTTIFieldAttribute::Scriptable));
		const CRTTIField* editable_editor_readonly_field = cls->FindConstFieldByName("EditableEditorReadOnlyIntField");
		REQUIRE(editable_editor_readonly_field != nullptr);
		CHECK(editable_editor_readonly_field->HasAttribute(ERTTIFieldAttribute::Editable));
		CHECK(editable_editor_readonly_field->HasAttribute(ERTTIFieldAttribute::EditorReadOnly));
		const CRTTIField* editor_readonly_replicated_field = cls->FindConstFieldByName("EditorReadOnlyReplicatedMixedField");
		REQUIRE(editor_readonly_replicated_field != nullptr);
		CHECK(editor_readonly_replicated_field->HasAttribute(ERTTIFieldAttribute::EditorReadOnly));
		CHECK(editor_readonly_replicated_field->HasAttribute(ERTTIFieldAttribute::Replicated));
		const CRTTIField* serializable_editable_replicated_field = cls->FindConstFieldByName("SerializableEditableReplicatedMixedField");
		REQUIRE(serializable_editable_replicated_field != nullptr);
		CHECK(serializable_editable_replicated_field->HasAttribute(ERTTIFieldAttribute::Serializable));
		CHECK(serializable_editable_replicated_field->HasAttribute(ERTTIFieldAttribute::Editable));
		CHECK(serializable_editable_replicated_field->HasAttribute(ERTTIFieldAttribute::Replicated));
		const CRTTIField* scriptable_editor_readonly_replicated_field = cls->FindConstFieldByName("ScriptableEditorReadOnlyReplicatedMixedField");
		REQUIRE(scriptable_editor_readonly_replicated_field != nullptr);
		CHECK(scriptable_editor_readonly_replicated_field->HasAttribute(ERTTIFieldAttribute::Scriptable));
		CHECK(scriptable_editor_readonly_replicated_field->HasAttribute(ERTTIFieldAttribute::EditorReadOnly));
		CHECK(scriptable_editor_readonly_replicated_field->HasAttribute(ERTTIFieldAttribute::Replicated));
		const CRTTIField* serializable_scriptable_editable_replicated_field = cls->FindConstFieldByName("SerializableScriptableEditableReplicatedMixedField");
		REQUIRE(serializable_scriptable_editable_replicated_field != nullptr);
		CHECK(serializable_scriptable_editable_replicated_field->HasAttribute(ERTTIFieldAttribute::Serializable));
		CHECK(serializable_scriptable_editable_replicated_field->HasAttribute(ERTTIFieldAttribute::Scriptable));
		CHECK(serializable_scriptable_editable_replicated_field->HasAttribute(ERTTIFieldAttribute::Editable));
		CHECK(serializable_scriptable_editable_replicated_field->HasAttribute(ERTTIFieldAttribute::Replicated));
	}
}

TEST_CASE("RTTI Attributes -- Methods", "[RTTI][Attributes][Fields]")
{
	const pulvis::rtti::CRTTIClass* cls = pulvis::rtti::CRTTIClass::FindConstInStorage(pulvis::rtti::CRTTITypeInfo<pulvis::rtti::tests::MethodsAttributes>::GetTypeId());

	SECTION("Single flag methods are ok?")
	{
		const CRTTIMethod* no_attr_method = cls->FindConstMethodByName("NoAttributeMethod");
		REQUIRE(no_attr_method != nullptr);
		CHECK(no_attr_method->HasAttribute(ERTTIMethodAttribute::None));

		const CRTTIMethod* scriptable_method = cls->FindConstMethodByName("ScriptableMethod");
		REQUIRE(scriptable_method != nullptr);
		CHECK(scriptable_method->HasAttribute(ERTTIMethodAttribute::Scriptable));

		const CRTTIMethod* editor_callable_method = cls->FindConstMethodByName("EditorCallableMethod");
		REQUIRE(editor_callable_method != nullptr);
		CHECK(editor_callable_method->HasAttribute(ERTTIMethodAttribute::EditorCallable));

		const CRTTIMethod* event_method = cls->FindConstMethodByName("EventMethod");
		REQUIRE(event_method != nullptr);
		CHECK(event_method->HasAttribute(ERTTIMethodAttribute::Event));

		const CRTTIMethod* signal_method = cls->FindConstMethodByName("SignalMethod");
		REQUIRE(signal_method != nullptr);
		CHECK(signal_method->HasAttribute(ERTTIMethodAttribute::Signal));

		const CRTTIMethod* slot_method = cls->FindConstMethodByName("SlotMethod");
		REQUIRE(slot_method != nullptr);
		CHECK(slot_method->HasAttribute(ERTTIMethodAttribute::Slot));

		const CRTTIMethod* command_method = cls->FindConstMethodByName("CommandMethod");
		REQUIRE(command_method != nullptr);
		CHECK(command_method->HasAttribute(ERTTIMethodAttribute::Command));

		const CRTTIMethod* pure_virtual_method = cls->FindConstMethodByName("PureVirtualMethod");
		REQUIRE(pure_virtual_method != nullptr);
		CHECK(pure_virtual_method->HasAttribute(ERTTIMethodAttribute::Pure));

		const CRTTIMethod* scriptable_editor_callable_method = cls->FindConstMethodByName("ScriptableEditorCallableMethod");
		REQUIRE(scriptable_editor_callable_method != nullptr);
		CHECK(scriptable_editor_callable_method->HasAttribute(ERTTIMethodAttribute::Scriptable));
		CHECK(scriptable_editor_callable_method->HasAttribute(ERTTIMethodAttribute::EditorCallable));

		const CRTTIMethod* editor_callable_event_method = cls->FindConstMethodByName("EditorCallableEventMethod");
		REQUIRE(editor_callable_event_method != nullptr);
		CHECK(editor_callable_event_method->HasAttribute(ERTTIMethodAttribute::EditorCallable));
		CHECK(editor_callable_event_method->HasAttribute(ERTTIMethodAttribute::Event));

		const CRTTIMethod* event_signal_method = cls->FindConstMethodByName("EventSignalMethod");
		REQUIRE(event_signal_method != nullptr);
		CHECK(event_signal_method->HasAttribute(ERTTIMethodAttribute::Event));
		CHECK(event_signal_method->HasAttribute(ERTTIMethodAttribute::Signal));

		const CRTTIMethod* signal_slot_method = cls->FindConstMethodByName("SignalSlotMethod");
		REQUIRE(signal_slot_method != nullptr);
		CHECK(signal_slot_method->HasAttribute(ERTTIMethodAttribute::Signal));
		CHECK(signal_slot_method->HasAttribute(ERTTIMethodAttribute::Slot));

		const CRTTIMethod* command_pure_method = cls->FindConstMethodByName("CommandPureVirtualMethod");
		REQUIRE(command_pure_method != nullptr);
		CHECK(command_pure_method->HasAttribute(ERTTIMethodAttribute::Command));
		CHECK(command_pure_method->HasAttribute(ERTTIMethodAttribute::Pure));

		const CRTTIMethod* all_attributes_method = cls->FindConstMethodByName("AllAttributesMethod");
		REQUIRE(all_attributes_method != nullptr);
		CHECK(all_attributes_method->HasAttribute(ERTTIMethodAttribute::Scriptable));
		CHECK(all_attributes_method->HasAttribute(ERTTIMethodAttribute::EditorCallable));
		CHECK(all_attributes_method->HasAttribute(ERTTIMethodAttribute::Event));
		CHECK(all_attributes_method->HasAttribute(ERTTIMethodAttribute::Signal));
		CHECK(all_attributes_method->HasAttribute(ERTTIMethodAttribute::Slot));
		CHECK(all_attributes_method->HasAttribute(ERTTIMethodAttribute::Command));
		CHECK(all_attributes_method->HasAttribute(ERTTIMethodAttribute::Pure));
	}
}