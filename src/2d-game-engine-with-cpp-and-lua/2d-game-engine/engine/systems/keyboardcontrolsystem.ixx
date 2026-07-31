export module engine:systems.keyboardcontrolsystem;
import :ecs;
import :eventbus;
import :events;
import :log;
import :components;

export namespace Engine
{
	class KeyboardControlSystem : public System
	{
	public:
		KeyboardControlSystem(Registry& registry) : registry(registry) 
		{
			RequireComponent<KeyboardControlledComponent>();
			RequireComponent<SpriteComponent>();
			RequireComponent<RigidBodyComponent>();
		}
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
			for (auto entity : GetEntities())
			{
				auto keyboardControl = registry.GetComponent<KeyboardControlledComponent>(entity);
				auto& sprite = registry.GetComponent<SpriteComponent>(entity);
				auto& rigidBody = registry.GetComponent<RigidBodyComponent>(entity);

				switch (event.Event.scancode)
				{
				case SDL::Scancode::Up:
					// Move up
					rigidBody.velocity = keyboardControl.UpVelocity;
					sprite.srcRect.y = sprite.height * 0;
					break;
				case SDL::Scancode::Right:
					// Move right
					rigidBody.velocity = keyboardControl.RightVelocity;
					sprite.srcRect.y = sprite.height * 1;
					break;
				case SDL::Scancode::Down:
					// Move down
					rigidBody.velocity = keyboardControl.DownVelocity;
					sprite.srcRect.y = sprite.height * 2;
					break;
				case SDL::Scancode::Left:
					// Move left
					rigidBody.velocity = keyboardControl.LeftVelocity;
					sprite.srcRect.y = sprite.height * 3;
					break;
				default:
					break;
				}
			}

		}

		Registry& registry;
	};
}