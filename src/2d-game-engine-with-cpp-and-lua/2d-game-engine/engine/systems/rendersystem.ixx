export module engine:systems.rendersystem;
import :ecs;
import :components;
import :sdl3;
import :ecs;
import :assetstore;

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

		void Update(SDL::SDL_Renderer* renderer, AssetStore& assetStore)
		{
			constexpr auto darkSapphire = SDL::SDL_Color{ 31, 48, 94, 255 };
			constexpr auto royalBlue = SDL::SDL_Color{ 48, 92, 222, 255 };
			constexpr auto clearColor = SDL::SDL_Color{ 0, 0, 0, 255 };

			SDL::SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			SDL::SDL_RenderClear(renderer);

			// Sort by z-index before rendering to ensure correct layering.
			// This is not efficient, as sorting happens every frame, there
			// may be better ways of doing things, such as sorting at the
			// time of entity creation.
			auto basicEntities = GetEntities();
			struct RenderableEntity
			{
				TransformComponent transformComponent;
				SpriteComponent spriteComponent;
			};
			auto renderableEntities = basicEntities
				| std::views::transform(
					[this](auto entity) -> RenderableEntity
					{
						return { registry.GetComponent<TransformComponent>(entity), registry.GetComponent<SpriteComponent>(entity) };
					})
				| std::ranges::to<std::vector<RenderableEntity>>();
			std::ranges::sort(renderableEntities, [](auto& lhs, auto& rhs) -> bool { return lhs.spriteComponent.zIndex < rhs.spriteComponent.zIndex; });

			for (const auto& renderableEntity : renderableEntities)
			{
				auto& transform = renderableEntity.transformComponent;
				auto& sprite = renderableEntity.spriteComponent;

				auto texture = assetStore.GetTexture(sprite.assetId);
				auto rect = SDL::SDL_FRect{
					transform.position.x,
					transform.position.y,
					static_cast<float>(sprite.width * transform.scale.x),
					static_cast<float>(sprite.height * transform.scale.y)
				};
				auto srcRect = sprite.srcRect;
				//SDL::SDL_RenderTexture(renderer, texture, &srcRect, &rect);

				SDL::SDL_RenderTextureRotated(renderer, texture, &srcRect, &rect, transform.rotation, nullptr, SDL::SDL_FlipMode::SDL_FLIP_NONE);
			}
			SDL::SDL_RenderPresent(renderer);
		}
	private:
		Registry& registry;
	};
}
