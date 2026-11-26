export module Engine:Events.KeyPressedEvent;
import std;
import :EventBus.Event;
import :SDL3;

export namespace Engine
{
	struct KeyPressedEvent : public Event
	{
		KeyPressedEvent(SDL::SDL_KeyboardEvent event) : Event(event) {}
		SDL::SDL_KeyboardEvent Event;
	};
}
