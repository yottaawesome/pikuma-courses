export module engine:systems.projectileemitsystem;
import :ecs;
import :components;
import :sdl3;
import :eventbus;
import :events;

export namespace Engine
{
	class ProjectileEmitSystem : public System
	{
	public:
		ProjectileEmitSystem(Registry& registry)
			: registry(registry)
		{
			RequireComponent<ProjectileEmitterComponent>();
			RequireComponent<TransformComponent>();
		}

		void SubscribeToEvents(EventBus& eventBus)
		{
			// No events to subscribe to for this system
			eventBus.SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::OnKeyPressedEvent);
		}

		void Update(float deltaTime, Registry& registry)
		{
			for(auto entity : GetEntities())
			{
				// This is how we identify the player entity. We only want the player to emit projectiles on spacebar, not enemies.
				if (not registry.HasComponent<CameraFollowComponent>(entity))
					continue;

				auto& projectileEmitter = registry.GetComponent<ProjectileEmitterComponent>(entity);
				if (projectileEmitter.RepeatFrequency <= 0)
					continue;
				if (SDL::SDL_GetTicks() - projectileEmitter.LastEmissionTime < projectileEmitter.RepeatFrequency)
					continue;

				auto& transform = registry.GetComponent<TransformComponent>(entity);
				auto projectile = registry.CreateEntity();
				auto projectilePosition = transform.position;
				if (registry.HasComponent<RigidBodyComponent>(entity))
				{
					auto sprite = registry.GetComponent<SpriteComponent>(entity);
					projectilePosition.x += (transform.scale.x * sprite.width) / 2.0f;
					projectilePosition.y += (transform.scale.y * sprite.height) / 2.0f;
				}

				registry
					.AddComponent(projectile, TransformComponent{projectilePosition, glm::vec2{ 1.0f, 1.0f }, 0.0f})
					.AddComponent(projectile, RigidBodyComponent{projectileEmitter.ProjectileVelocity, 1.0f})
					.AddComponent(projectile, SpriteComponent{"bullet-image", 4, 4, 4})
					.AddComponent(projectile, BoxColliderComponent{4, 4})
					.AddComponent(projectile, ProjectileComponent{
						.IsFriendly = projectileEmitter.IsFriendly, 
						.HitPercentDamage = projectileEmitter.HitPercentDamage, 
						.Duration = projectileEmitter.ProjectileDuration
					});
				projectileEmitter.LastEmissionTime = SDL::SDL_GetTicks();
			}
		}

	private:
		void OnKeyPressedEvent(const KeyPressedEvent& event)
		{
			// Handle spacebar key press to emit a projectile
			if (event.Event.scancode != SDL::Scancode::Space)
				return;

			for (auto entity : GetEntities())
			{
				auto& projectileEmitter = registry.GetComponent<ProjectileEmitterComponent>(entity);
				if (SDL::SDL_GetTicks() - projectileEmitter.LastEmissionTime < projectileEmitter.RepeatFrequency)
					continue;	
				auto& transform = registry.GetComponent<TransformComponent>(entity);

				auto& rigidBody = registry.GetComponent<RigidBodyComponent>(entity);
				auto projectileVelocity = projectileEmitter.ProjectileVelocity;

				// if the entity has a rigid body, we can use its velocity to determine the direction of the projectile
				auto directionX = 0;
				auto directionY = 0;
				if (rigidBody.velocity.x > 0)
					directionX = 1;
				else if (rigidBody.velocity.x < 0)
					directionX = -1;
				if (rigidBody.velocity.y > 0)
					directionY = 1;
				else if (rigidBody.velocity.y < 0)
					directionY = -1;
				projectileVelocity = glm::vec2{ projectileVelocity.x * directionX, projectileVelocity.y * directionY };

				auto projectile = registry.CreateEntity();
				auto projectilePosition = transform.position;
				auto sprite = registry.GetComponent<SpriteComponent>(entity);
				projectilePosition.x += (transform.scale.x * sprite.width) / 2.0f;
				projectilePosition.y += (transform.scale.y * sprite.height) / 2.0f;

				registry
					.AddComponent(projectile, TransformComponent{ projectilePosition, glm::vec2{ 1.0f, 1.0f }, 0.0f })
					.AddComponent(projectile, RigidBodyComponent{ projectileVelocity, 1.0f })
					.AddComponent(projectile, SpriteComponent{ "bullet-image", 4, 4, 4 })
					.AddComponent(projectile, BoxColliderComponent{ 4, 4 })
					.AddComponent(projectile, ProjectileComponent{
						.IsFriendly = projectileEmitter.IsFriendly,
						.HitPercentDamage = projectileEmitter.HitPercentDamage,
						.Duration = projectileEmitter.ProjectileDuration
					});
				projectileEmitter.LastEmissionTime = SDL::SDL_GetTicks();
			}
		}

		Registry& registry;
	};
}