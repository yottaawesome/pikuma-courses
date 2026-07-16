export module engine:systems.keyboardmovementsystem;
import :ecs;
import :eventbus;
import :events;
import :log;

export namespace Engine
{
	class KeyboardMovementSystem : public System
	{
	public:
		KeyboardMovementSystem(Registry& registry) : registry(registry) {}
		void Update(float deltaTime)
		{

		}
		void SubscribeToEvents(EventBus& eventBus)
		{
			// Subscribe to keyboard events if needed
			eventBus.SubscribeToEvent<KeyPressedEvent>(this, &KeyboardMovementSystem::OnKeyPressedEvent);
		}
	private:
		void OnKeyPressedEvent(const KeyPressedEvent& event)
		{
			Log::Info("KeyboardMovementSystem received key pressed event: {}", SDL::SDL_GetKeyName(event.Event.key));
		}

		Registry& registry;
	};
}