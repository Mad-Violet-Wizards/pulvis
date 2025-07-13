#pragma once

namespace engine::experimental::rtti_autogen
{
//////////////////////////////////////////////////////////////////////////
        [[rtti::enum]]
		enum class EMocRttiEnumExplicit
		{
			A = 0,
			B = 1,
			C = 2,
			D = 3,
			E = 4,
			Count = 5
		};		
//////////////////////////////////////////////////////////////////////////
        [[rtti::enum]]
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
//////////////////////////////////////////////////////////////////////////
        [[rtti::enum]]
		enum class EMocRttiLongValuesNames
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
        [[rtti::class]]
		class IBase : public engine::rtti::IRTTIBase
		{
			public:
			
				virtual ~IBase() = default;

                [[rtti::method]]
				int GetVirtualBase() { return 0; }

                [[rtti::method]]
				int Add(int _a, int _b)
				{
					return _a + _b;
				}

                [[rtti::method]]
				int Subtract(int _a, int _b)
				{
					return _a - _b;
				}

                [[rtti::method]]
				int AddConst(int _a, int _b) const
				{
					return _a + _b;
				}

                [[rtti::method]]
				int SubtractConst(int _a, int _b) const
				{
					return _a - _b;
				}
		};
//////////////////////////////////////////////////////////////////////////
        [[rtti::class]]
		class IBase2 : public engine::rtti::IRTTIBase
		{
			public:
			
				virtual ~IBase2() = default;

                [[rtti::method]]
				int Multiply(int _a, int _b)
				{
					return _a * _b;
				}

                [[rtti::method]]
				int Divide(int _a, int _b)
				{
					return _a / _b;
				}

                [[rtti::method]]
				int MultiplyConst(int _a, int _b) const
				{
					return _a * _b;
				}

                [[rtti::method]]
				int DivideConst(int _a, int _b) const
				{
					return _a / _b;
				}
		};

        [[rtti::class]]
		class CDerived : public IBase
		{		
			public:

				int GetPrivateInt() const
				{
					return m_PrivateInt;
				}

                [[rtti::method]]
                int GetVirtualBase() { return 1; }

			private:

                [[rtti::field]]
				int m_PrivateInt = 0;
		};

        [[rtti::class]]
		class CDerived2 : public IBase
		{
		public:

			int GetPrivateInt() const
			{
				return m_PrivateInt;
			}

		private:
            [[rtti::field]]
			int m_PrivateInt = 0;
		};

        [[rtti::class]]
		class CDerivedBoth : public IBase, public IBase2
		{
		public:

			int GetPrivateInt() const
			{
				return m_PrivateInt;
			}

			private:

                [[rtti::field]]
				int m_PrivateInt = 0;
		};
//////////////////////////////////////////////////////////////////////////
}
}