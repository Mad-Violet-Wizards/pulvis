#pragma once

#include "engine/rtti/RTTIMacros.hpp"

namespace engine::scriptable
{
	class IScriptable : public rtti::IRTTIBase
	{
		RTTI_CLASS_API(IScriptable)

public:

		virtual ~IScriptable() = default;
		virtual void Validate() = 0;
		virtual bool IsValid() const = 0;
	};
}