#pragma once

#include <memory>
#include "./Renderer/Renderer.h"

class Application
{
public:
	Application();

	void initialize();
	void run();
	void setup();
	void input();
	void update();
	void render();
	void destroy();

private:
	std::shared_ptr<Renderer> renderer;

	float target_seconds_per_frame = 0.0f;
	bool running = false;
};

