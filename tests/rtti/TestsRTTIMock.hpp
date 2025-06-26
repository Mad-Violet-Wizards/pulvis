#pragma once

#include "engine/rtti/RTTITypeName.hpp"
#include "engine/rtti/RTTITypeTraits.hpp"
#include "engine/rtti/RTTIHash.hpp"
#include "engine/rtti/RTTITypeInfo.hpp"
#include "engine/rtti/RTTIBase.hpp"
#include "engine/rtti/RTTIMacros.hpp"
#include "engine/rtti/RTTIEnum.hpp"
#include "engine/rtti/detail/RTTIEnumDetail.hpp"

namespace tests::rtti
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

			public:
			
				virtual ~IBase() = default;
				int GetVirtualBase() { return 0; }
				RTTI_METHOD_API(IBase, GetVirtualBase);

				int Add(int _a, int _b)
				{
					return _a + _b;
				}
				RTTI_METHOD_API(IBase, Add);

				int Subtract(int _a, int _b)
				{
					return _a - _b;
				}
				RTTI_METHOD_API(IBase, Subtract);

				int AddConst(int _a, int _b) const
				{
					return _a + _b;
				}
				RTTI_METHOD_API(IBase, AddConst);

				int SubtractConst(int _a, int _b) const
				{
					return _a - _b;
				}
				RTTI_METHOD_API(IBase, SubtractConst);
		};
//////////////////////////////////////////////////////////////////////////
		class IBase2 : public engine::rtti::IRTTIBase
		{
			RTTI_CLASS_API(IBase2);

			public:
			
				virtual ~IBase2() = default;

				int Multiply(int _a, int _b)
				{
					return _a * _b;
				}
				RTTI_METHOD_API(IBase2, Multiply);

				int Divide(int _a, int _b)
				{
					return _a / _b;
				}
				RTTI_METHOD_API(IBase2, Divide);

				int MultiplyConst(int _a, int _b) const
				{
					return _a * _b;
				}
				RTTI_METHOD_API(IBase2, MultiplyConst);

				int DivideConst(int _a, int _b) const
				{
					return _a / _b;
				}
				RTTI_METHOD_API(IBase2, DivideConst);
		};

		class CDerived : public IBase
		{
			RTTI_CLASS_API(CDerived, IBase);
		
			public:

				int GetPrivateInt() const
				{
					return m_PrivateInt;
				}

				int GetVirtualBase() { return 1; }
				RTTI_METHOD_API(CDerived, GetVirtualBase);

			private:

				int m_PrivateInt = 0;
				RTTI_FIELD_API(CDerived, m_PrivateInt);
		};
//////////////////////////////////////////////////////////////////////////
		class CDerived2 : public IBase
		{
			RTTI_CLASS_API(CDerived2, IBase);

		public:

			int GetPrivateInt() const
			{
				return m_PrivateInt;
			}

		private:
			int m_PrivateInt = 0;
			RTTI_FIELD_API(CDerived2, m_PrivateInt);
		};

		class CDerivedBoth : public IBase, public IBase2
		{
			RTTI_CLASS_API(CDerivedBoth, IBase, IBase2);

		public:

			int GetPrivateInt() const
			{
				return m_PrivateInt;
			}

			private:

				int m_PrivateInt = 0;
				RTTI_FIELD_API(CDerivedBoth, m_PrivateInt);
		};
//////////////////////////////////////////////////////////////////////////
}