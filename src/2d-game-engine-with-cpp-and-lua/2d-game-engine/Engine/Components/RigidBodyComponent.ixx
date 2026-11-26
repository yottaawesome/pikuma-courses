export module Engine:Components.RigidBodyComponent;
import :GLM;

export namespace Engine
{
	struct RigidBodyComponent
	{
		glm::vec2 velocity{ 0.0f, 0.0f };
		float mass{ 1.0f };
	};
}