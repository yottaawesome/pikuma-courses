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

		constexpr auto operator==(const Entity& other) noexcept -> bool
		{
			return id == other.id;
		}
	private:
		std::uint64_t id = 0;
	};

}