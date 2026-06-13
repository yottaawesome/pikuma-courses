export module engine:components.spritecomponent;
import std;
import :sdl3;

export namespace Engine
{
	struct SpriteComponent
	{
		std::string assetId;
		int width{ 0 };
		int height{ 0 };
		int zIndex{ 0 };
		int srcRectX{ 0 };
		int srcRectY{ 0 };
		SDL::SDL_FRect srcRect{ static_cast<float>(srcRectX), static_cast<float>(srcRectY), static_cast<float>(width), static_cast<float>(height) };
	};
}