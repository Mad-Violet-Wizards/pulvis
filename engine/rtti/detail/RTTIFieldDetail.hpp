#pragma once

namespace engine::rtti::detail
{
	class IRTTIField
	{
		public:
		virtual ~IRTTIField() = default;
	};

	template<class C, typename T>
	class RTTIField : public IRTTIField
	{
	public:

		RTTIField(T C::* _field)
			: m_Field(_field)
		{

		}

		T Get(C* _instance)
		{
			return _instance->*m_Field;
		}

		void Set(C* _instance, T _value)
		{
			_instance->*m_Field = _value;
		}

	private:

		T C::* m_Field;
	};
}
