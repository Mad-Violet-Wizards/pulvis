#pragma once

class Game
{
public:

	~Game() = default;
	static Game& GetInstance();

	void Initialize();
	void Run();
	void Shutdown();

private:

	Game() = default;

	void BeginFrame();
	void Frame();
	void EndFrame();
};