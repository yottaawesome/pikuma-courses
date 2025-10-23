export module librenderer:shading;
import std;

export namespace renderer
{
	auto apply_light_intensity(std::uint32_t original_color, float percentage_factor) noexcept -> std::uint32_t
	{
		auto a = (original_color & 0xFF000000);
		auto r = static_cast<std::uint32_t>((original_color & 0x00FF0000) * percentage_factor);
		auto g = static_cast<std::uint32_t>((original_color & 0x0000FF00) * percentage_factor);
		auto b = static_cast<std::uint32_t>((original_color & 0x000000FF) * percentage_factor);

		return a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
	}
}
