export module engine:systems.movementsystem;
import :glm;
import :ecs;
import :components;

export namespace Engine
{
	class MovementSystem : public System
	{
	public:
		MovementSystem(Registry& registry) : registry{ registry }
		{
			RequireComponent<TransformComponent>();
			RequireComponent<RigidBodyComponent>();
		}

		void Update(double deltaTime)
		{
			for (auto entity : GetEntities())
			{
				auto& transform = registry.GetComponent<TransformComponent>(entity);
				auto& rigidbody = registry.GetComponent<RigidBodyComponent>(entity);

				transform.position += glm::vec2{
					rigidbody.velocity.x * static_cast<float>(deltaTime),
					rigidbody.velocity.y * static_cast<float>(deltaTime)
				};
			}
		}

	private:
		Registry& registry;
	};
}
