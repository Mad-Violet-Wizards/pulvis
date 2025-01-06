#pragma once

template<typename T, typename Allocator>
class Vector
{
	public:

		constexpr static size_t DEFAULT_CAPCITY = 8;

	public:

		Vector() = default;
		Vector(Allocator& _allocator) 
			: m_Allocator(_allocator) 
		{
			Reserve(DEFAULT_CAPCITY);
		}

		Vector(size_t _size, Allocator& _allocator) 
			: m_Allocator(_allocator)
		{
			Reserve(_size);
		}

		Vector(std::initializer_list<T> _list, Allocator& _allocator) 
			: m_Allocator(_allocator)
		{
			Reserve(_list.size());

			for (const T& element : _list)
			{
				PushBack(element);
			}
		}

		Vector(const Vector& _other) 
			: m_Allocator(_other.m_Allocator)
		{
			Reserve(_other.m_Size);

			for (size_t i = 0; i < _other.m_Size; ++i)
			{
				PushBack(_other.m_Data[i]);
			}
		}

		Vector(Vector&& _other) noexcept 
			: m_Allocator(_other.m_Allocator)
		{
			m_Data = _other.m_Data;
			m_Size = _other.m_Size;
			m_Capacity = _other.m_Capacity;

			_other.m_Data = nullptr;
			_other.m_Size = 0;
			_other.m_Capacity = 0;
		}

		Vector& operator=(const Vector& _other)
		{
			if (this != &_other)
			{
				Clear();
				Reserve(_other.m_Size);

				for (size_t i = 0; i < _other.m_Size; ++i)
				{
					PushBack(_other.m_Data[i]);
				}
			}

			return *this;
		}

		Vector& operator=(Vector&& _other) noexcept
		{
			if (this != &_other)
			{
				Clear();

				m_Data = _other.m_Data;
				m_Size = _other.m_Size;
				m_Capacity = _other.m_Capacity;

				_other.m_Data = nullptr;
				_other.m_Size = 0;
				_other.m_Capacity = 0;
			}

			return *this;
		}

		~Vector()
		{
			Clear();	
		}

		void PushBack(const T& _element)
		{
			if (m_Size == m_Capacity)
			{
				Reserve(m_Capacity * 2);
			}

			m_Allocator.construct(m_Data + m_Size, _element);
			++m_Size;
		}

		void EmplaceBack(T&& _element)
		{
			if (m_Size == m_Capacity)
			{
				Reserve(m_Capacity * 2);
			}

			m_Allocator.construct(m_Data + m_Size, std::move(_element));
			++m_Size;
		}

		void PopBack()
		{
			if (m_Size > 0)
			{
				m_Allocator.destroy(m_Data + m_Size - 1);
				--m_Size;
			}
		}

		void Clear()
		{
			for (size_t i = 0; i < m_Size; ++i)
			{
				m_Allocator.destroy(m_Data + i);
			}

			m_Allocator.Deallocate(m_Data, m_Capacity);
			m_Data = nullptr;
			m_Size = 0;
			m_Capacity = 0;
		}

		void Reserve(size_t _size)
		{
			if (_size > m_Capacity)
			{
				T* new_data = m_Allocator.Allocate<T>(_size);

				for (size_t i = 0; i < m_Size; ++i)
				{
					m_Allocator.construct(new_data + i, m_Data[i]);
					m_Allocator.destroy(m_Data + i);
				}

				if (m_Capacity > 0)
				{
					m_Allocator.Deallocate(m_Data, m_Capacity);
				}

				m_Data = new_data;
				m_Capacity = _size;
			}
		}

		T& operator[](size_t _index)
		{
			return m_Data[_index];
		}

		const T& operator[](size_t _index) const
		{
			return m_Data[_index];
		}

		size_t Size() const
		{
			return m_Size;
		}

		size_t Capacity() const
		{
			return m_Capacity;
		}

		bool Empty() const
		{
			return m_Size == 0;
		}

		T* Data()
		{
			return m_Data;
		}

		const T* Data() const
		{
			return m_Data;
		}

		T& Front()
		{
			return m_Data[0];
		}

		const T& Front() const
		{
			return m_Data[0];
		}

		T& Back()
		{
			return m_Data[m_Size - 1];
		}

		const T& Back() const
		{
			return m_Data[m_Size - 1];
		}

	private:

		Allocator& m_Allocator;
		T* m_Data{ nullptr };
		size_t m_Size{ 0 };
		size_t m_Capacity{ 0 };
};
