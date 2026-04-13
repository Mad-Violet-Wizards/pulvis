#pragma once

#include "ScriptableNode.hpp"

#include <vector>
#include <memory>

#include <sol3/sol.hpp>

namespace pulvis::scriptable
{
    class CScriptableService
    {
        public:

            CScriptableService();
            ~CScriptableService();

            CScriptableService(const CScriptableService&) = delete;
            CScriptableService& operator=(const CScriptableService&) = delete;

            void Initialize();
            void Shutdown();

            void RegisterScriptableNode(std::shared_ptr<IScriptableNode> _scriptableNode);
            
						sol::state& GetLuaState() { return m_LuaState; }
						const sol::state& GetLuaState() const { return m_LuaState; }

        private:

            std::vector<std::shared_ptr<IScriptableNode>> m_ScriptableNodes;
            sol::state m_LuaState;

    };
}