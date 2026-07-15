export module engine:systems.keyboardmovementsystem;
import :ecs;

export namespace Engine
{
	class KeyboardMovementSystem : public System
	{
	public:
		KeyboardMovementSystem(Registry& registry) : registry(registry) {}
		void Update(float deltaTime)
		{

		}
	private:
		Registry& registry;
	};
}