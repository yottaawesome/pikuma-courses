export module engine:ecs.component;
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
}
