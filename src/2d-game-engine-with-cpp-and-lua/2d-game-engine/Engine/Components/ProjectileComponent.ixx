export module Engine:Components.ProjectileComponent;
import :SDL3;

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