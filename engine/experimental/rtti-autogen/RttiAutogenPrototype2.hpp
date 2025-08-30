#pragma once
#pragma warning(disable: 5030)

#include "engine/rtti/RTTIBase.hpp"
#include "engine/rtti/RTTIMacros.hpp"

namespace engine::experimental::rtti_autogen
{
    enum class [[Reflection::Enum]] EMocRttiPlaceholder
    {
        Placeholder1,
        Placeholder2,
        Placeholder3,
        Placeholder4,
        Placeholder5
    };

    class [[Reflection::Class]] CPlaceholderClass : public engine::rtti::IRTTIBase
    {
    public:

        [[Reflection::Method]]
        void DoSomethingCool()
        {
            
        }
    };
}
