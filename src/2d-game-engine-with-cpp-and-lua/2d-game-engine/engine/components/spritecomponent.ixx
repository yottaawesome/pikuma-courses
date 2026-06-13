export module engine:components.spritecomponent;
import std;

export namespace Engine
{
	struct SpriteComponent
	{
		std::string assetId;
		int width{ 0 };
		int height{ 0 };

		void Update(double deltaTime)
		{
		}
	};
}