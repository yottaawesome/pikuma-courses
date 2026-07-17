export module engine:systems.debugrendersystem;
import std;
import :sdl3;
import :ecs;
import :components;

export namespace Engine
{
	class DebugRenderSystem : public System
	{
	public:
		DebugRenderSystem(Registry& registry)
			: registry(registry)
		{
			RequireComponent<TransformComponent>();
			RequireComponent<BoxColliderComponent>();
		}
		void Update(SDL::SDL_Renderer* renderer, SDL::SDL_Rect& camera)
		{
			if (not debugRenderingEnabled)
				return;
			for (auto entity : GetEntities())
			{
				auto& transform = registry.GetComponent<TransformComponent>(entity);
				auto& collider = registry.GetComponent<BoxColliderComponent>(entity);
				auto rect = SDL::SDL_FRect{
					.x = transform.position.x + collider.Offset.x - camera.x,
					.y = transform.position.y + collider.Offset.y - camera.y,
					.w = static_cast<float>(collider.Width * transform.scale.x),
					.h = static_cast<float>(collider.Height * transform.scale.y),
				};
				SDL::SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for debug boxes
				SDL::SDL_RenderRect(renderer, &rect);
			}
		}
		constexpr auto ToggleDebugRendering() -> bool
		{
			return std::exchange(debugRenderingEnabled, not debugRenderingEnabled);
		}
	private:
		Registry& registry;
		bool debugRenderingEnabled = false;
	};
}