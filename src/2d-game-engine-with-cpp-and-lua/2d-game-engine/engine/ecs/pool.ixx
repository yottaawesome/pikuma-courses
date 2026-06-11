export module engine:ecs.pool;
import std;

export namespace Engine
{
	class IPool
	{
	public:
		virtual ~IPool() = default;
	};

	template<typename T>
	class Pool : public IPool
	{
	public:
		virtual ~Pool() = default;

		constexpr Pool(size_t size = 100) : data(size) {}

		constexpr auto IsEmpty() const -> bool
		{
			return data.empty();
		}

		constexpr auto GetSize() const -> size_t
		{
			return data.size();
		}

		constexpr void Resize(size_t newSize)
		{
			data.resize(newSize);
		}

		constexpr void Clear()
		{
			data.clear();
		}

		constexpr void Add(const T& component)
		{
			data.push_back(component);
		}

		constexpr void Set(size_t index, const T& component)
		{
			if (index >= data.size())
				throw std::out_of_range{ "Index out of range" };
			data[index] = component;
		}

		constexpr auto Get(size_t index) -> T&
		{
			if (index >= data.size())
				throw std::out_of_range{ "Index out of range" };
			return data[index];
		}

		constexpr auto operator[](size_t index) -> T&
		{
			if (index >= data.size())
				throw std::out_of_range{ "Index out of range" };
			return data[index];
		}

	private:
		std::vector<T> data{};
	};
}
