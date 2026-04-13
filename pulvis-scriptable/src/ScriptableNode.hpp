#pragma once

#include <sol3/sol.hpp>
#include <memory>

namespace pulvis::scriptable
{
    class CScriptableService;

    class IScriptableNode : public std::enable_shared_from_this<IScriptableNode>
    {
        public:

            IScriptableNode() = default;
            virtual ~IScriptableNode() = default;

            void RegisterSelf(CScriptableService& _scriptableService);

            virtual void RegisterLuaBindings(sol::state& _luaState) = 0;
    };
}
