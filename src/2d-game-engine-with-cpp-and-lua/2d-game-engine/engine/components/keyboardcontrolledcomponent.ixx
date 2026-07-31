export module engine:components.keyboardcontrolledcomponent;
import :glm;

export namespace Engine
{
	struct KeyboardControlledComponent
	{
		glm::vec2 UpVelocity = { 0.0f, 0.0f };
		glm::vec2 RightVelocity = { 0.0f, 0.0f };
		glm::vec2 DownVelocity = { 0.0f, 0.0f };
		glm::vec2 LeftVelocity = { 0.0f, 0.0f };
	};
}