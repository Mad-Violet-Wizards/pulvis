#include "ScriptableService.hpp"

namespace pulvis::scriptable
{
    CScriptableService::CScriptableService()
    {
    }

    CScriptableService::~CScriptableService() = default;

    void CScriptableService::Initialize()
    {
        m_LuaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string, sol::lib::table);

        for (const std::shared_ptr<IScriptableNode>& scriptableNode : m_ScriptableNodes)
        {
            scriptableNode->RegisterLuaBindings(m_LuaState);
        }
    }

    void CScriptableService::Shutdown()
    {
        m_ScriptableNodes.clear();
    }

    void CScriptableService::RegisterScriptableNode(std::shared_ptr<IScriptableNode> _scriptableNode)
    {
        m_ScriptableNodes.push_back(std::move(_scriptableNode));
    }
}