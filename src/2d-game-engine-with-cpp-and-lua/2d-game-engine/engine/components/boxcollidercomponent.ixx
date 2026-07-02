export module engine:components.boxcollidercomponent;
import :glm;

export namespace Engine
{
	struct BoxColliderComponent
	{
		int Width = 0;
		int Height = 0;
		glm::vec2 Offset = { 0, 0 };
	};
}