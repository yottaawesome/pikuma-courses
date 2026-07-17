export module engine:systems.cameramovementsystem;
import std;
import :ecs;
import :sdl3;
import :components.camerafollowcomponent;
import :components.transformcomponent;

export namespace Engine
{
	class CameraMovementSystem : public System
	{
	public:
		CameraMovementSystem(Registry& registry)
			: registry(registry)
		{
			RequireComponent<CameraFollowComponent>();
			RequireComponent<TransformComponent>();
		}

		void Update(SDL::SDL_Rect& camera, std::uint32_t windowWidth, std::uint32_t windowHeight, std::uint32_t mapWidth, std::uint32_t mapHeight)
		{
			for (auto entity : GetEntities())
			{
				auto& transform = registry.GetComponent<TransformComponent>(entity);

				if (transform.position.x + (camera.w / 2) < mapWidth)
					camera.x = static_cast<int>(transform.position.x - (windowWidth / 2));
				if (transform.position.y + (camera.h / 2) < mapHeight)
					camera.y = static_cast<int>(transform.position.y - (windowHeight / 2));

				camera.x = camera.x < 0 ? 0 : camera.x;
				camera.y = camera.y < 0 ? 0 : camera.y;
				camera.x = camera.x > camera.w ? camera.w : camera.x;
				camera.y = camera.y > camera.h ? camera.h : camera.y;
			}
		}
	private:
		Registry& registry;
	};
}
