#include <iostream>
#include "engine/engine_pch.hpp"
#include "engine/core/Setup.hpp"
#include "engine/core/Application.hpp"

int main(int argc, const char* argv[])
{
    printf("[Main] :)\n");
    
    engine::core::SApplicationSetup app_setup;
    const std::string app_name = "Game";
    app_setup.m_ApplicationName = const_cast<char*>(app_name.c_str());
    const std::string app_to_run = "Game";
    app_setup.m_AplicationToRun = const_cast<char*>(app_to_run.c_str());
    app_setup.m_ClientApp = engine::core::EClientApp::Game;
    app_setup.m_WindowWidth = 800;
    app_setup.m_WindowHeight = 600;
    engine::core::Application app(app_setup);
    app.Run();

    return 0;
}
