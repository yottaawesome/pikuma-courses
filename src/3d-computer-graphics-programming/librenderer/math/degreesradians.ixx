export module librenderer:degreesradians;
import std;

export namespace math
{
	struct degrees;
	struct radians final
	{
		explicit constexpr radians(float radianValue) noexcept : value(radianValue) {}
		explicit constexpr radians(degrees deg) noexcept;
		float value = 0;
		constexpr operator float() const noexcept { return value; }
		constexpr auto operator<=>(const radians&) const = default;
		constexpr auto operator==(const radians& rhs) const noexcept -> bool = default;
	};

	struct degrees final
	{
		explicit constexpr degrees(float degValue) noexcept : value(degValue) {}
		explicit constexpr degrees(radians deg) noexcept
			: value{ deg.value * 180.f / static_cast<float>(std::numbers::pi) }
		{}
		float value = 0;
		constexpr operator float() const noexcept { return value; }
		constexpr auto operator<=>(const degrees&) const = default;
		constexpr auto operator==(const degrees&) const noexcept -> bool = default;
	};

	bool operator==(const degrees&, const radians&) = delete;

	constexpr radians::radians(degrees v) noexcept
		: value{ v.value * static_cast<float>(std::numbers::pi) / 180.f }
	{ }
}
