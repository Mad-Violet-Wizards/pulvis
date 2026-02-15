#pragma once

#include "RTTIBase.hpp"
#include "RTTIMacros.hpp"

struct SEmptyStruct {};
class CEmptyClass {};

namespace pulvis::rtti::tests
{
	class [[Reflection::Class]] IBase : public pulvis::rtti::IRTTIBase
	{
		RTTI_CLASS_API(IBase);

	public:

		virtual ~IBase() = default;

		[[Reflection::Method]]
		virtual int GetVirtualBase() { return 0; }

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
	class [[Reflection::Class]] IBase2 : public pulvis::rtti::IRTTIBase
	{
		RTTI_CLASS_API(IBase2);

	public:

		virtual ~IBase2() = default;

		[[Reflection::Method]]
		virtual int GetVirtualBase() { return 1; }

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

	class [[Reflection::Class]] CDerived : public pulvis::rtti::tests::IBase
	{
		RTTI_CLASS_API(CDerived, IBase);

	public:

		[[Reflection::Method]]
		virtual int GetVirtualBase() override { return 2; }

	public:

		[[Reflection::Field]]
		int IntField = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	class [[Reflection::Class]] CDerived2 : public pulvis::rtti::tests::IBase2
	{
		RTTI_CLASS_API(CDerived2, IBase2);

		[[Reflection::Method]]
		virtual int GetVirtualBase() override { return 3; }

	public:

		[[Reflection::Field]]
		int IntField = 0;
	};

	class [[Reflection::Class]] CDerivedBoth : public pulvis::rtti::tests::IBase, public pulvis::rtti::tests::IBase2
	{
		RTTI_CLASS_API(CDerivedBoth, IBase, IBase2);

		[[Reflection::Method]]
		virtual int GetVirtualBase() override { return 4; }

	public:

		[[Reflection::Field]]
		int IntField = 0;
	};
}

