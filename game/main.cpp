#include <iostream>
#include "engine/engine_pch.hpp"
#include "engine/core/Setup.hpp"
#include "engine/core/Application.hpp"

int main(int argc, const char* argv[])
{
    printf("[Main] :)\n");
    
    engine::core::ApplicationSetup app_setup;
    app_setup.m_ClientApp = engine::core::EClientApp::Game;
    app_setup.m_WindowWidth = 800;
    app_setup.m_WindowHeight = 600;
    engine::core::Application app(app_setup);
    app.Run();

    return 0;
}
