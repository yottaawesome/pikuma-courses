export module librenderer:degreesradians;
import std;

export namespace math
{
	struct radians final
	{
		float value = 0;
		operator float() const noexcept { return value; }
		auto operator<=>(const radians&) const = default;
		auto operator==(float rhs) const noexcept -> bool { return value == rhs; }
	};

	struct degrees final
	{
		float value = 0;
		operator float() const noexcept { return value; }
		auto operator<=>(const degrees&) const = default;
		auto operator==(float rhs) const noexcept -> bool { return value == rhs; }
	};

	bool operator==(const degrees&, const radians&) = delete;

	auto convert(degrees v) noexcept -> radians
	{
		return { v.value * static_cast<float>(std::numbers::pi) / 180.f };
	}

	auto convert(radians v) noexcept -> degrees
	{
		return { v.value * 180.f / static_cast<float>(std::numbers::pi) };
	}
}
