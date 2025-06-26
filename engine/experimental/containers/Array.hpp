#pragma once

namespace engine::experimental::containers
{
	template<typename T, size_t N>
	class Array
	{
		public:
			
			Array()
			{

			}
		
			Array(std::initializer_list<T> _list)
			{

			}

			Array(const Array& _other)
			{

			}

			Array(Array&& _other) noexcept
			{

			}

			Array& operator=(const Array& _other)
			{
				return *this;
			}

			Array& operator=(Array&& _other) noexcept
			{
				return *this;
			}

			~Array()
			{

			}

			T& operator[](size_t _index)
			{
				return m_Data[_index];
			}

			const T& operator[](size_t _index) const
			{
				return m_Data[_index];
			}

			T* Front()
			{
				return m_Data;
			}

			const T* Front() const
			{
				return m_Data;
			}

			T* Back()
			{
				return m_Data + m_Size - 1;
			}

			const T* Back() const
			{
				return m_Data + m_Size - 1;
			}

			size_t Size() const
			{
				return m_Size;
			}

			size_t Capacity() const
			{
				return N;
			}

			bool Empty() const
			{
				return m_Size == 0;
			}

		private:

			T m_Data[N];
			size_t m_Size = 0;
	};
}