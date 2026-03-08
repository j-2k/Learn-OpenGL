#pragma once
#include "pch.h"

class Time {
public:
	static Time& getInstance() {
		static Time instance;
		return instance;
	}

	// Call this at the start of each frame to update deltaTime and lastFrame
	void update() {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	float getDeltaTime() const { return deltaTime; }
	float getTotalTime() const { return totalTime; }
	float getFPS() const { return 1.0f / deltaTime; }

	// Delete copy/move so nobody can accidentally duplicate the instance
	Time(const Time&) = delete;
	Time& operator=(const Time&) = delete;

private:
	Time() = default; // private constructor for singleton pattern
	
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	float totalTime = 0.0f; // Total time since the application started
};