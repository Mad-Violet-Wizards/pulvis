#include <catch2/catch2.hpp>

#include "PulvisRttiTests_rtti_autogen.hpp"

#include "ScriptableService.hpp"
#include "EventDispatcher.hpp"
#include "assets/AssetRegistry.hpp"
#include "FileSystem.hpp"

int main(int argc, const char* argv[])
{
  RegisterRTTI_PulvisRttiTests();

  pulvis::events::CEventDispatcher event_dispatcher;
  pulvis::fs::CMountSystem mount_system;
	pulvis::fs::CFileSystem file_system("pulvis-rtti-tests");
  pulvis::fs::assets::CAssetRegistry  asset_registry(file_system, event_dispatcher);
  pulvis::scriptable::CScriptableService scriptable_service(asset_registry);

  scriptable_service.Initialize();
  RegisterLuaBindings_PulvisRttiTests(scriptable_service);

  const int result = Catch::Session().run(argc, argv);

  scriptable_service.Shutdown();
  return result;
}