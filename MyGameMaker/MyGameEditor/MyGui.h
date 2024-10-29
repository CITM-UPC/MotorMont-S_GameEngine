#pragma once

#include "MyGameEngine/MyWindow.h"




class MyGUI : public IEventProcessor
{
	MyGUI(SDL_Window* window, void* context);
	MyGUI(MyGUI&&) noexcept = delete;
	MyGUI(const MyGUI&) = delete;
	MyGUI& operator=(const MyGUI&) = delete;
	~MyGUI();


 
	void processEvent(const SDL_Event& event) override;
	void handleEvent(const SDL_Event& event);

};

