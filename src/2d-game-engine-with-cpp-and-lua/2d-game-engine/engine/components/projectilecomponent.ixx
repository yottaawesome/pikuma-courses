export module engine:components.projectilecomponent;
import :sdl3;

export namespace Engine
{
	struct ProjectileComponent
	{
		bool IsFriendly = false;
		int HitPercentDamage = 0;
		int Duration = 0; // in milliseconds
		int StartTime = static_cast<int>(SDL::SDL_GetTicks()); // in milliseconds
	};
}