export module Engine:Systems.ProjectileLifecycleSystem;
import :ECS;
import :Components;
import :SDL3;

export namespace  Engine
{
	class ProjectileLifecycleSystem : public System
	{
	public:
		ProjectileLifecycleSystem(Registry& registry) : registry(registry)
		{
			RequireComponent<ProjectileComponent>();
		}
		void Update()
		{
			for (auto entity : GetEntities())
			{
				auto& projectile = registry.GetComponent<ProjectileComponent>(entity);
				auto currentTime = static_cast<int>(SDL::SDL_GetTicks());
				if (currentTime - projectile.StartTime >= projectile.Duration)
				{
					registry.KillEntity(entity);
				}
			}
		}
	private:
		Registry& registry;
	};
}