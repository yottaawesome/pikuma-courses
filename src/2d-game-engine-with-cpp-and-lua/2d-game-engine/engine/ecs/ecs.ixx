export module engine:ecs;
import std;

export namespace Engine
{
	constexpr auto MaxComponents = 32u;
	using Signature = std::bitset<MaxComponents>;

	class IComponent
	{
	protected:
		static inline int nextId{};
	};

	template<typename T>
	class Component : public IComponent
	{
	public:
		static auto GetId() -> int
		{
			static auto id = nextId++;
			return id;
		}
	};

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

	class System
	{
	public:
		System() = default;

		constexpr void AddEntity(this System& self, Entity entity)
		{
			self.entities.push_back(entity);
		}
		constexpr void RemoveEntity(this System& self, Entity entity)
		{
			auto it = std::ranges::find_if(self.entities, [&](const auto& e) { return e == entity; });
			self.entities.erase(it);
		}
		constexpr auto GetEntities(this const System& self) -> std::vector<Entity>
		{
			return self.entities;
		}
		constexpr auto GetSignature(this const System& self) -> const Signature&
		{
			return self.componentSignature;
		}

		template<typename TComponent>
		constexpr void RequireComponent(this System& self)
		{
			const auto componentId = Component<TComponent>::GetId();
			self.componentSignature.set(componentId);
		}
	private:
		Signature componentSignature{};
		std::vector<Entity> entities{};
	};

	class Registry
	{
	public:
	};
}

namespace
{
	using namespace Engine;

	// Test adding, removing, and retrieving entities from a system.
	static_assert(
		[] -> bool
		{
			auto s = System{};
			s.AddEntity(Entity{ 1 });
			s.AddEntity(Entity{ 2 });
			auto e = s.GetEntities();
			if (e.size() != 2)
				throw std::runtime_error{ "Expected 2 entities" };
			s.RemoveEntity(Entity{ 1 });
			e = s.GetEntities();
			if (e.size() != 1 or e[0].GetId() != 2)
				throw std::runtime_error{ "Expected 1 entity of id 2" };
			return true;
		}()
	);
}
