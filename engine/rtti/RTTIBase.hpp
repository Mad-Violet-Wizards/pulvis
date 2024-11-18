#pragma once

#include "RTTITypeTraits.hpp"
#include "RTTITypeInfo.hpp"

namespace engine::rtti
{
	class IRTTIBase
	{
		public:

			virtual ~IRTTIBase() = default;

			virtual type_id_t GetTypeId() const = 0;

			virtual bool IsTypeOfId(type_id_t _type) const = 0;

			template<typename T>
			bool IsTypeOf() const
			{
				return IsTypeOfId(CRTTITypeInfo<T>::GetTypeId());
			}

			template<typename T>
			T* Cast() noexcept
			{
				return reinterpret_cast<T*>(const_cast<void*>(internal_cast(CRTTITypeInfo<T>::GetTypeId())));
			}

			template<typename T>
			T const* Cast() const noexcept
			{
				return reinterpret_cast<T const*>(internal_cast(CRTTITypeInfo<T>::GetTypeId()));
			}

		protected:

			virtual void const* internal_cast(type_id_t _type_id) const noexcept = 0;
	};
}
