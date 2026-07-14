export module engine:systems.damagesystem;
import std;
import :ecs;
import :components;
import :eventbus;
import :events;
import :log;

export namespace Engine
{
	class DamageSystem : public System
	{
	public:
		DamageSystem(Registry& registry)
			: registry(registry)
		{
			RequireComponent<BoxColliderComponent>();
		}
		void SubscribeToEvents(EventBus& eventBus)
		{
			eventBus.SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
		}
		void Update(EventBus& eventBus)
		{
			// The DamageSystem doesn't need to do anything in the Update loop for now.
			// It reacts to collision events instead.
		}
	private:
		void OnCollision(const CollisionEvent& event)
		{
			Log::Info("DamageSystem received collision event between entities {} and {}", event.entityA.GetId(), event.entityB.GetId());
			registry.KillEntity(event.entityA);
			registry.KillEntity(event.entityB);
		}
		Registry& registry;
	};
}
