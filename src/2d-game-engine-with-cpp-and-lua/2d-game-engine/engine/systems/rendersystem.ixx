export module engine:systems.rendersystem;
import :ecs;
import :components;
import :sdl3;
import :ecs;

export namespace Engine
{
	class RenderSystem : public System
	{
	public:
		RenderSystem(Registry& registry) : registry(registry)
		{
			RequireComponent<TransformComponent>();
			RequireComponent<SpriteComponent>();
		}

		void Update(SDL::SDL_Renderer* renderer)
		{
			constexpr auto darkSapphire = SDL::SDL_Color{ 31, 48, 94, 255 };
			constexpr auto royalBlue = SDL::SDL_Color{ 48, 92, 222, 255 };
			constexpr auto clearColor = SDL::SDL_Color{ 0, 0, 0, 255 };

			SDL::SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			SDL::SDL_RenderClear(renderer);

			SDL::SDL_SetRenderDrawColor(renderer, darkSapphire.r, darkSapphire.g, darkSapphire.b, darkSapphire.a);

			for (auto entity : GetEntities())
			{
				auto& transform = registry.GetComponent<TransformComponent>(entity);
				auto& sprite = registry.GetComponent<SpriteComponent>(entity);

				auto rect = SDL::SDL_FRect{
					transform.position.x,
					transform.position.y,
					static_cast<float>(sprite.width),
					static_cast<float>(sprite.height)
				};
				SDL::SDL_RenderFillRect(renderer, &rect);
			}
			SDL::SDL_RenderPresent(renderer);
		}
	private:
		Registry& registry;
	};
}
