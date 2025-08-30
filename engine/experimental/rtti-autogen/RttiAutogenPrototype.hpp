#pragma once
#pragma warning(disable: 5030)

#include "engine/rtti/RTTIBase.hpp"
#include "engine/rtti/RTTIMacros.hpp"

namespace engine::experimental::rtti_autogen
{
//////////////////////////////////////////////////////////////////////////
		enum class [[Reflection::Enum]] EMocRttiEnumExplicit
		{
			A = 0,
			B = 1,
			C = 2,
			D = 3,
			E = 4,
			Count = 5
		};		
//////////////////////////////////////////////////////////////////////////
		enum class [[Reflection::Enum]] EMocRttiEnumImplicit
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
//////////////////////////////////////////////////////////////////////////
		enum class [[Reflection::Enum]] EMocRttiLongValuesNames
		{
			ThisIsFirstValueOfThisEnum,
			ChcialbymZjescPaczkaZMusemCzekoladowym,
			CzasamiCzlowiekMusiSieZastanowic,
			ToJestCzwartyElement,
			ToJestPiataWartosc,
			NoTakWlasnieBywaZeWZyciuNiewielePrzybywaAleZaToWieleOdplywa
		};
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
		class [[Reflection::Class]] IBase : public engine::rtti::IRTTIBase
		{
			public:
			
				virtual ~IBase() = default;

				[[Reflection::Method]]
				int GetVirtualBase() { return 0; }

				[[Reflection::Method]]
				int Add(int _a, int _b)
				{
					return _a + _b;
				}

				[[Reflection::Method]]
				int Subtract(int _a, int _b)
				{
					return _a - _b;
				}

				[[Reflection::Method]]
				int AddConst(int _a, int _b) const
				{
					return _a + _b;
				}

				[[Reflection::Method]]
				int SubtractConst(int _a, int _b) const
				{
					return _a - _b;
				}
		};
//////////////////////////////////////////////////////////////////////////
		class [[Reflection::Class]] IBase2 : public engine::rtti::IRTTIBase
		{
			public:
			
				virtual ~IBase2() = default;

				[[Reflection::Method]]
				int Multiply(int _a, int _b)
				{
					return _a * _b;
				}

				[[Reflection::Method]]
				int Divide(int _a, int _b)
				{
					return _a / _b;
				}

				[[Reflection::Method]]
				int MultiplyConst(int _a, int _b) const
				{
					return _a * _b;
				}

				[[Reflection::Method]]
				int DivideConst(int _a, int _b) const
				{
					return _a / _b;
				}
		};

		class [[Reflection::Class]] CDerived : public engine::experimental::rtti_autogen::IBase
		{		
			public:

				int GetPrivateInt() const
				{
					return m_PrivateInt;
				}

				[[Reflection::Method]]
				int GetVirtualBase() { return 1; }

			public:

				[[Reflection::Field]]
				int m_PrivateInt = 0;
		};

		class [[Reflection::Class]] CDerived2 : public engine::experimental::rtti_autogen::IBase
		{
		public:

			int GetPrivateInt() const
			{
				return m_PrivateInt;
			}

		public:

			[[Reflection::Field]]
			int m_PrivateInt = 0;
		};

		class [[Reflection::Class]] CDerivedBoth : public engine::experimental::rtti_autogen::IBase, public engine::experimental::rtti_autogen::IBase2
		{
		public:

			int GetPrivateInt() const
			{
				return m_PrivateInt;
			}

		public:

				[[Reflection::Field]]
				int m_PrivateInt = 0;
		};
//////////////////////////////////////////////////////////////////////////
}