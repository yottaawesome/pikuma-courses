export module Engine:Components.AnimationComponent;
import std;
import :SDL3;

export namespace Engine
{
	struct AnimationComponent
	{
		int NumFrames = 1;
		int FrameSpeedRate = 1;
		bool IsLoop = false;
		std::uint64_t StartTime = SDL::SDL_GetTicks();
		int CurrentFrame = 0;
	};
}
