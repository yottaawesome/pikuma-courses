export module engine:systems.keyboardcontrolsystem;
import :ecs;
import :eventbus;
import :events;
import :log;

export namespace Engine
{
	class KeyboardControlSystem : public System
	{
	public:
		KeyboardControlSystem(Registry& registry) : registry(registry) {}
		void Update(float deltaTime)
		{

		}
		void SubscribeToEvents(EventBus& eventBus)
		{
			// Subscribe to keyboard events if needed
			eventBus.SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressedEvent);
		}
	private:
		void OnKeyPressedEvent(const KeyPressedEvent& event)
		{
			Log::Info("KeyboardControlSystem received key pressed event: {}", SDL::SDL_GetKeyName(event.Event.key));
		}

		Registry& registry;
	};
}