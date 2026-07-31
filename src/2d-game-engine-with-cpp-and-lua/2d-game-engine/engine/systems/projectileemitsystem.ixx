export module engine:systems.projectileemitsystem;
import :ecs;
import :components;
import :sdl3;

export namespace Engine
{
	class ProjectileEmitSystem : public System
	{
	public:
		ProjectileEmitSystem()
		{
			RequireComponent<ProjectileEmitterComponent>();
			RequireComponent<TransformComponent>();
		}

		void Update(float deltaTime, Registry& registry)
		{
			for(auto entity : GetEntities())
			{
				auto& transform = registry.GetComponent<TransformComponent>(entity);
				auto& projectileEmitter = registry.GetComponent<ProjectileEmitterComponent>(entity);

				if (SDL::SDL_GetTicks() - projectileEmitter.LastEmissionTime >= projectileEmitter.RepeatFrequency)
				{
					auto projectile = registry.CreateEntity();
					auto projectilePosition = transform.position;
					if (registry.HasComponent<RigidBodyComponent>(entity))
					{
						auto sprite = registry.GetComponent<SpriteComponent>(entity);
						projectilePosition.x += (transform.scale.x * sprite.width) / 2.0f;
						projectilePosition.y += (transform.scale.y * sprite.height) / 2.0f;
					}

					registry.AddComponent<TransformComponent>(projectile, projectilePosition, glm::vec2{ 1.0f, 1.0f }, 0.0f)
						.AddComponent<RigidBodyComponent>(projectile, projectileEmitter.ProjectileVelocity, 1.0f)
						.AddComponent<SpriteComponent>(projectile, "bullet-image", 4, 4, 4)
						.AddComponent<BoxColliderComponent>(projectile, 4, 4);
					projectileEmitter.LastEmissionTime = SDL::SDL_GetTicks();
				}
			}
		}
	};
}