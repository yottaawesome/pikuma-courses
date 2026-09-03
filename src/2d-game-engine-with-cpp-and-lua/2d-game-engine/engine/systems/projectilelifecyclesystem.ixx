export module engine:systems.projectilelifecyclesystem;
import :ecs;
import :components;
import :sdl3;

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