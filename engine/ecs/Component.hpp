#pragma once

#include "engine/core/UUID.hpp"

namespace engine::ecs
{
	class IComponent
	{
		public:

			IComponent() = default;
			virtual ~IComponent() = default;

			core::UUID GetID() const { return m_ID; }


		private:

			core::UUID m_ID;


	};
}
