export module engine:components.rigidbodycomponent;
import :glm;

export namespace Engine
{
	struct RigidbodyComponent
	{
		glm::vec2 velocity{ 0.0f, 0.0f };
		float mass{ 1.0f };
	};
}