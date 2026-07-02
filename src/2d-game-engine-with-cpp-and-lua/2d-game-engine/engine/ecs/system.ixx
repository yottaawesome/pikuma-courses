export module engine:ecs.system;
import std;
import :ecs.component;
import :ecs.entity;

export namespace Engine
{
	class System
	{
	public:
		System() = default;

		constexpr auto AddEntity(this auto& self, Entity entity) -> decltype(auto)
		{
			self.entities.push_back(entity);
			return std::forward_like<decltype(self)>(self);
		}
		constexpr void RemoveEntity(this System& self, Entity entity)
		{
			auto it = std::ranges::find_if(self.entities, [&](const auto& e) { return e == entity; });
			if (it != self.entities.end())
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
}

namespace
{
	using namespace Engine;

	// Test adding, removing, and retrieving entities from a system.
	static_assert(
		[] -> bool
		{
			auto s = System{};
			s.AddEntity(Entity{ 1 }).AddEntity(Entity{ 2 });
			auto e = s.GetEntities();
			if (e.size() != 2)
				throw "Expected 2 entities";
			s.RemoveEntity(Entity{ 1 });
			e = s.GetEntities();
			if (e.size() != 1 or e[0].GetId() != 2)
				throw "Expected 1 entity of id 2";
			return true;
		}()
	);
}
