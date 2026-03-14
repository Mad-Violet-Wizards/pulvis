#pragma once

#include "FileTypes.hpp"

#include <vector>
#include <string>
#include <string_view>

namespace pulvis::fs
{
	class CFileBuffer
	{
		public:

			CFileBuffer() = default;

			explicit CFileBuffer(file_size_t _size)
				: m_Buffer(static_cast<size_t>(_size))
			{
			}

			CFileBuffer(const void* _data, file_size_t _size)
				: m_Buffer(static_cast<const uint8_t*>(_data), static_cast<const uint8_t*>(_data) + static_cast<size_t>(_size))
			{
			}

			CFileBuffer(const CFileBuffer&) = default;
			CFileBuffer& operator=(const CFileBuffer&) = default;

			CFileBuffer(CFileBuffer&&) noexcept = default;
			CFileBuffer& operator=(CFileBuffer&&) noexcept = default;

			const uint8_t* Data() const { return m_Buffer.data(); }
			uint8_t* Data() { return m_Buffer.data(); }

			file_size_t Size() const { return static_cast<file_size_t>(m_Buffer.size()); }
			bool Empty() const { return m_Buffer.empty(); }

			void Resize(file_size_t _size) { m_Buffer.resize(static_cast<size_t>(_size)); }

			void Clear()
			{
				m_Buffer.clear();
				m_Buffer.shrink_to_fit();
			}

			void Append(const void* _data, file_size_t _size)
			{
				const uint8_t* byte_data = static_cast<const uint8_t*>(_data);
				m_Buffer.insert(m_Buffer.end(), byte_data, byte_data + _size);
			}

			std::string_view ToStringView() const
			{
				return std::string_view(reinterpret_cast<const char*>(m_Buffer.data()), m_Buffer.size());
			}

			std::string ToString() const
			{
				return std::string(reinterpret_cast<const char*>(m_Buffer.data()), m_Buffer.size());
			}

		private:

			std::vector<uint8_t> m_Buffer;
	};
}