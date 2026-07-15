export module engine:events.keypressedevent;
import std;
import :eventbus.event;

export namespace Engine
{
	struct KeyPressedEvent : public Event
	{
		int KeyCode = 0;
	};
}
