export module engine:systems.animationsystem;
import :sdl3;
import :ecs;
import :components;

export namespace Engine
{
	class AnimationSystem : public System
	{
	public:
		AnimationSystem(Registry& registry)
			: registry(registry)
		{
			RequireComponent<SpriteComponent>();
			RequireComponent<AnimationComponent>();
		}

		void Update()
		{
			for (auto entity : GetEntities())
			{
				auto& animation = registry.GetComponent<AnimationComponent>(entity);
				auto& sprite = registry.GetComponent<SpriteComponent>(entity);
				animation.CurrentFrame = 
					(((SDL::SDL_GetTicks() - animation.StartTime) 
						* animation.FrameSpeedRate) / 1000) % animation.NumFrames;
				sprite.srcRect.x = animation.CurrentFrame*sprite.width;
			}
		}
	private:
		Registry& registry;
	};
}