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
			auto entities = GetEntities();

			// Axis-Aligned Bounding Box (AABB) collision detection
			// We use iterators to avoid checking the same pair of entities twice and 
			// to avoid checking an entity against itself. This works because the data 
			// structure is linear, so each entity is only checked against the entities 
			// that come after it in the list. This is a slightly modified version of
			// the implementation provided in the course.
			//
			// Some comments from Copilot when I asked it about this approach:
			// 
			// That loop pattern is usually called pairwise iteration over unique 
			// unordered pairs. It avoids duplicate checks like(A, B) and (B, A)  by 
			// starting the inner loop at std::next(i), and it avoids self-checks 
			// like(A, A). It does not reduce the algorithmic complexity below quadratic, 
			// though: it’s still O(n²), just roughly half as many comparisons: n*(n-1)/2.
			// For collision detection, this is still considered a brute - force broad 
			// phase using unique pair generation.To truly avoid O(n²), you’d use spatial 
			// partitioning techniques like a uniform grid, quadtree, spatial hash, sweep 
			// and prune, or a BVH.
			for (auto i = entities.begin(); i != entities.end(); ++i)
			{
				auto entityA = *i;
				auto& transformA = registry.GetComponent<TransformComponent>(entityA);
				auto& colliderA = registry.GetComponent<BoxColliderComponent>(entityA);
				for (auto j = std::next(i); j != entities.end(); ++j)
				{
					auto entityB = *j;
					auto& transformB = registry.GetComponent<TransformComponent>(entityB);
					auto& colliderB = registry.GetComponent<BoxColliderComponent>(entityB);
					if (CheckAABBCollision(transformA, colliderA, transformB, colliderB))
					{
						Log::Info("Collision detected between entities {} and {}", entityA.GetId(), entityB.GetId());
						registry.KillEntity(entityA);
						registry.KillEntity(entityB);
					}
				}
			}
		}
	private:
		auto CheckAABBCollision(
			const TransformComponent& transformA, const BoxColliderComponent& colliderA,
			const TransformComponent& transformB, const BoxColliderComponent& colliderB
		) -> bool
		{
			// AABB collision detection
			return ((transformA.position.x + colliderA.Offset.x) < transformB.position.x + colliderB.Width and
					((transformA.position.x + colliderA.Offset.x) + colliderA.Width) > transformB.position.x and
					(transformA.position.y + colliderA.Offset.y) < transformB.position.y + colliderB.Height and
					((transformA.position.y + colliderA.Offset.y) + colliderA.Height) > transformB.position.y);
		}

		Registry& registry;
	};
}