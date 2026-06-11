export module engine:ecs.entity;
import std;

export namespace Engine
{
	class Entity
	{
	public:
		constexpr Entity(std::uint64_t id) : id{ id } {}

		constexpr auto GetId(this auto&& self) noexcept -> std::uint64_t
		{
			return self.id;
		}

		// Required for use in std::set
		constexpr auto operator<=>(const Entity& other) const noexcept = default;

	private:
		std::uint64_t id = 0;
	};
}
