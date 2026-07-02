export module engine:systems.collisionsystem;
import std;
import :ecs;
import :components;
import :log;

export namespace Engine
{
	class CollisionSystem : public System
	{
	public:
		CollisionSystem(Registry& registry)
			: registry(registry)
		{
			RequireComponent<TransformComponent>();
			RequireComponent<BoxColliderComponent>();
		}
		void Update()
		{
			// AABB collision detection
			for (auto entityA : GetEntities())
			{
				auto& transformA = registry.GetComponent<TransformComponent>(entityA);
				auto& colliderA = registry.GetComponent<BoxColliderComponent>(entityA);
				for (auto entityB : GetEntities())
				{
					if (entityA == entityB) 
						continue;
					auto& transformB = registry.GetComponent<TransformComponent>(entityB);
					auto& colliderB = registry.GetComponent<BoxColliderComponent>(entityB);
					if (CheckCollision(transformA, colliderA, transformB, colliderB))
					{
						Log::Info("Collision detected between entities {} and {}", entityA.GetId(), entityB.GetId());
					}
				}
			}
		}
	private:
		auto CheckCollision(
			const TransformComponent& transformA, const BoxColliderComponent& colliderA,
			const TransformComponent& transformB, const BoxColliderComponent& colliderB
		) -> bool
		{
			// AABB collision detection
			return (transformA.position.x < transformB.position.x + colliderB.Width and
					transformA.position.x + colliderA.Width > transformB.position.x and
					transformA.position.y < transformB.position.y + colliderB.Height and
					transformA.position.y + colliderA.Height > transformB.position.y);
		}

		Registry& registry;
	};
}