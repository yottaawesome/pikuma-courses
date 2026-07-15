export module engine:events.keypressedevent;
import std;
import :eventbus.event;
import :sdl3;

export namespace Engine
{
	struct KeyPressedEvent : public Event
	{
		KeyPressedEvent(SDL::SDL_Scancode scancode) : Scancode(scancode) {}
		SDL::SDL_Scancode Scancode;
	};
}
