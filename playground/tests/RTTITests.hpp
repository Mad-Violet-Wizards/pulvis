#pragma once

#include "engine/rtti/RTTITypeName.hpp"
#include "engine/rtti/RTTITypeTraits.hpp"
#include "engine/rtti/RTTIHash.hpp"
#include "engine/rtti/RTTITypeInfo.hpp"
#include "engine/rtti/RTTIBase.hpp"
#include "engine/rtti/RTTIMacros.hpp"
#include "engine/rtti/RTTIEnum.hpp"
#include "engine/rtti/detail/RTTIEnumDetail.hpp"

namespace engine::rtti
{
//////////////////////////////////////////////////////////////////////////
	namespace tests
	{
//////////////////////////////////////////////////////////////////////////
		enum class EMocRttiEnumExplicit
		{
			A = 0,
			B = 1,
			C = 2,
			D = 3,
			E = 4,
			Count = 5
		};		
		RTTI_ENUM_API(EMocRttiEnumExplicit);
//////////////////////////////////////////////////////////////////////////
		enum class EMocRttiEnumImplicit
		{
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H
		};
		RTTI_ENUM_API(EMocRttiEnumImplicit);
//////////////////////////////////////////////////////////////////////////
		enum class EMocRttiLongValuesNames
		{
			ThisIsFirstValueOfThisEnum,
			ChcialbymZjescPaczkaZMusemCzekoladowym,
			CzasamiCzlowiekMusiSieZastanowic,
			ToJestCzwartyElement,
			ToJestPiataWartosc,
			NoTakWlasnieBywaZeWZyciuNiewielePrzybywaAleZaToWieleOdplywa
		};
		RTTI_ENUM_API(EMocRttiLongValuesNames);
//////////////////////////////////////////////////////////////////////////
		struct SSimpleStruct {};
//////////////////////////////////////////////////////////////////////////
		class CSimpleClass {};
//////////////////////////////////////////////////////////////////////////
		class IInterface 
		{
			public: 
				virtual ~IInterface() = default;
				virtual int GetInt() const = 0;
		};
		class CImplementation : public IInterface
		{
			public:
				virtual int GetInt() const override { return 105; }
		};
//////////////////////////////////////////////////////////////////////////
		class IBase : public engine::rtti::IRTTIBase
		{
			RTTI_CLASS_API(IBase);
		};
//////////////////////////////////////////////////////////////////////////
		class CDerived : public IBase
		{
			RTTI_CLASS_API(CDerived, IBase);
		};
//////////////////////////////////////////////////////////////////////////
		class CDerived2 : public IBase
		{
			RTTI_CLASS_API(CDerived2, IBase);
		};
//////////////////////////////////////////////////////////////////////////
	}
//////////////////////////////////////////////////////////////////////////
}