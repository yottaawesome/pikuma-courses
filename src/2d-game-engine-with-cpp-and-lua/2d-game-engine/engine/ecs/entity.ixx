export module engine:ecs.entity;
import std;

export namespace Engine
{
	// The course code passes in the Registry and duplicates the various 
	// Component methods of the Registry into Entity. This creates a 
	// cyclic design and I really don't like this approach. I have 
	// elected to avoid it. It's possible to template this and avoid the
	// cyclic design, but then the System will also need to be templated.
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
