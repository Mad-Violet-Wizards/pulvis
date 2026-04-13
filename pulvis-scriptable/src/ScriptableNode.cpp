#include "ScriptableNode.hpp"
#include "ScriptableService.hpp"

namespace pulvis::scriptable
{
    void IScriptableNode::RegisterSelf(CScriptableService& _scriptableService)
    {
        _scriptableService.RegisterScriptableNode(shared_from_this());
    }
}