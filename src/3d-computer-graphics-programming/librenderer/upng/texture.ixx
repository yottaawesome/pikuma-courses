export module librenderer:upng.texture;
import std;
import :raii;
import :upng.exports;
import :upng.error;

export namespace upng
{
	using upng_unique_ptr = raii::direct_unique_ptr<upng_t, upng_free>;

	class upng_texture final
	{
	public:
		upng_texture(const std::filesystem::path& path)
		{
			png = upng_unique_ptr{ upng_new_from_file(path.string().c_str()) };
			if (not png)
				throw std::runtime_error("Failed to load PNG from file");
			if (auto result = upng_decode(png.get()); result != UPNG_EOK)
				throw error(result, "Failed to decode PNG");
		}

		auto buffer() const noexcept -> const unsigned char* { return upng_get_buffer(png.get()); }
		auto uint32_buffer() const noexcept -> const std::uint32_t* { return reinterpret_cast<const std::uint32_t*>(upng_get_buffer(png.get())); }
		auto width() const noexcept -> std::uint32_t { return upng_get_width(png.get()); }
		auto height() const noexcept -> std::uint32_t { return upng_get_height(png.get()); }
		auto format() const noexcept -> upng_format { return upng_get_format(png.get()); }

	private:
		upng_unique_ptr png;
	};
}
