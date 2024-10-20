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
	};
}
