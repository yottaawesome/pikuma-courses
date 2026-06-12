export module engine:components.spritecomponent;

export namespace Engine
{
	struct SpriteComponent
	{
		int width{ 0 };
		int height{ 0 };

		void Update(double deltaTime)
		{
		}
	};
}