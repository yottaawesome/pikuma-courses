export module engine:components.transformcomponent;
import :glm;

export namespace Engine
{
	struct TransformComponent
	{
		glm::vec2 position{ 0.0f, 0.0f };
		glm::vec2 scale{ 1.0f, 1.0f };
		double rotation{ 0.0 };
	};
}
