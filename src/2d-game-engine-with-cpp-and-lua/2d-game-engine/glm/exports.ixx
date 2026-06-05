module;

#include <glm/glm.hpp>

export module engine:glm.exports;

export namespace SDL
{
	using
		::glm::vec2,
		::glm::vec3,
		::glm::vec4,
		::glm::mat2,
		::glm::mat3,
		::glm::mat4,
		::glm::normalize
		;
}
