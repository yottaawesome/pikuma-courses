export module engine:components.projectileemittercomponent;
import std;
import :glm;
import :sdl3;

export namespace Engine
{
	struct ProjectileEmitterComponent
	{
		glm::vec2 ProjectileVelocity{};
		int RepeatFrequency = 0; // in milliseconds
		int ProjectileDuration = 10000; // in milliseconds
		int HitPercentDamage = 10;
		bool IsFriendly = false;
		std::uint64_t LastEmissionTime = SDL::SDL_GetTicks(); // in milliseconds
	};
}
