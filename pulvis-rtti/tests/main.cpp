#include <catch2/catch2.hpp>

#include "PulvisRttiTests_rtti_autogen.hpp"

#include "ScriptableService.hpp"
#include "EventDispatcher.hpp"
#include "assets/AssetRegistry.hpp"
#include "MountSystem.hpp"

int main(int argc, const char* argv[])
{
  RegisterRTTI_PulvisRttiTests();

  pulvis::events::CEventDispatcher eventDispatcher;
  pulvis::fs::CMountSystem            mountSystem;
  pulvis::fs::assets::CAssetRegistry  assetRegistry(mountSystem, eventDispatcher);
  pulvis::scriptable::CScriptableService scriptableService(assetRegistry);

  scriptableService.Initialize();
  RegisterLuaBindings_PulvisRttiTests(scriptableService);

  const int result = Catch::Session().run(argc, argv);

  scriptableService.Shutdown();
  return result;
}