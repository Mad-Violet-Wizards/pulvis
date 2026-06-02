#include "animation/serializers/AnimationSerializeTrait.hpp"

#include "serialization/SerializationArchive.hpp"
#include "serialization/SerializeTraitRTTI.hpp"

#include "RTTIClass.hpp"
#include "RTTITypeInfo.hpp"

namespace pulvis::fs::serialization
{
	using namespace pulvis::systems::animation;

	namespace
	{
		template<typename T>
		void SerializeScalarFields(CSerializationArchive& _ar, const T& _v)
		{
			using namespace pulvis::rtti;

			if (const CRTTIClass* cls = CRTTIClass::FindConstInStorage(CRTTITypeInfo<T>::GetTypeId()))
			{
				T* mutable_ptr = const_cast<T*>(&_v);
				for (const CRTTIField* field : cls->GetFields())
				{
					SerializeRTTIField(_ar, field, mutable_ptr);
				}
			}
		}

		template<typename T>
		void DeserializeScalarFields(CSerializationArchive& _ar, T& _o)
		{
			using namespace pulvis::rtti;
			if (const CRTTIClass* cls = CRTTIClass::FindConstInStorage(CRTTITypeInfo<T>::GetTypeId()))
			{
				for (const CRTTIField* field : cls->GetFields())
				{
					DeserializeRTTIField(_ar, field, &_o);
				}
			}
		}
	}

	void TSerializeTrait<CAnimationFrame>::Serialize(CSerializationArchive& _ar, const char* _n, const CAnimationFrame& _v)
	{
		_ar.BeginObject(_n);
		SerializeScalarFields(_ar, _v);
		_ar.EndObject();
	}
	void TSerializeTrait<CAnimationFrame>::Deserialize(CSerializationArchive& _ar, const char* _n, CAnimationFrame& _o)
	{
		_ar.BeginObject(_n);
		DeserializeScalarFields(_ar, _o);
		_ar.EndObject();
	}

	void TSerializeTrait<SAnimationEvent>::Serialize(CSerializationArchive& _ar, const char* _n, const SAnimationEvent& _v)
	{
		_ar.BeginObject(_n);
		SerializeScalarFields(_ar, _v);
		_ar.EndObject();
	}
	void TSerializeTrait<SAnimationEvent>::Deserialize(CSerializationArchive& _ar, const char* _n, SAnimationEvent& _o)
	{
		_ar.BeginObject(_n);
		DeserializeScalarFields(_ar, _o);
		_ar.EndObject();
	}

	void TSerializeTrait<CAnimationSequenceEventDesc>::Serialize(CSerializationArchive& _ar, const char* _n, const CAnimationSequenceEventDesc& _v)
	{
		_ar.BeginObject(_n);
		SerializeScalarFields(_ar, _v);
		_ar.EndObject();
	}
	void TSerializeTrait<CAnimationSequenceEventDesc>::Deserialize(CSerializationArchive& _ar, const char* _n, CAnimationSequenceEventDesc& _o)
	{
		_ar.BeginObject(_n);
		DeserializeScalarFields(_ar, _o);
		_ar.EndObject();
	}

	void TSerializeTrait<CAnimationSequenceDesc>::Serialize(CSerializationArchive& _ar, const char* _n, const CAnimationSequenceDesc& _v)
	{
		_ar.BeginObject(_n);
		SerializeScalarFields(_ar, _v);
		_ar.Process("Frames", _v.Frames);
		_ar.Process("Events", _v.Events);
		_ar.EndObject();
	}
	void TSerializeTrait<CAnimationSequenceDesc>::Deserialize(CSerializationArchive& _ar, const char* _n, CAnimationSequenceDesc& _o)
	{
		_ar.BeginObject(_n);
		DeserializeScalarFields(_ar, _o);
		_ar.Process("Frames", _o.Frames);
		_ar.Process("Events", _o.Events);
		_ar.EndObject();
	}

	void TSerializeTrait<CAnimationDesc>::Serialize(CSerializationArchive& _ar, const char* _n, const CAnimationDesc& _v)
	{
		_ar.BeginObject(_n);
		SerializeScalarFields(_ar, _v);
		_ar.Process("Sequences", _v.Sequences);
		_ar.EndObject();
	}
	void TSerializeTrait<CAnimationDesc>::Deserialize(CSerializationArchive& _ar, const char* _n, CAnimationDesc& _o)
	{
		_ar.BeginObject(_n);
		DeserializeScalarFields(_ar, _o);
		_ar.Process("Sequences", _o.Sequences);
		_ar.EndObject();
	}
} // namespace pulvis::fs::serialization