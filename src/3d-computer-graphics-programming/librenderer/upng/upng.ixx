module;

extern "C" {
#include "upng.h"
}

export module librenderer:upng;
import std;
import :raii;

export namespace upng
{
	using
		::upng_error,
		::upng_format,
		::upng_t,
		::upng_new_from_bytes,
		::upng_new_from_file,
		::upng_free,
		::upng_get_error,
		::upng_get_format,
		::upng_get_width,
		::upng_get_height,
		::upng_decode,
		::upng_get_buffer
		;
}

export namespace std
{
	template<>
	struct formatter<upng_error>
	{
		constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator { return ctx.begin(); }

		auto format(upng_error e, format_context& ctx) const -> format_context::iterator
		{
			switch (e)
			{
				case UPNG_EOK:
					return std::format_to(ctx.out(), "UPNG_EOK: No error");
				case UPNG_ENOMEM:
					return std::format_to(ctx.out(), "UPNG_ENOMEM: Memory allocation failed");
				case UPNG_ENOTFOUND:
					return std::format_to(ctx.out(), "UPNG_ENOTFOUND: Resource not found (file missing)");
				case UPNG_ENOTPNG:
					return std::format_to(ctx.out(), "UPNG_ENOTPNG: Image data does not have a PNG header");
				case UPNG_EMALFORMED:
					return std::format_to(ctx.out(), "UPNG_EMALFORMED: Image data is not a valid PNG image");
				case UPNG_EUNSUPPORTED:
					return std::format_to(ctx.out(), "UPNG_EUNSUPPORTED: Critical PNG chunk type is not supported");
				case UPNG_EUNINTERLACED:
					return std::format_to(ctx.out(), "UPNG_EUNINTERLACED: Image interlacing is not supported");
				case UPNG_EUNFORMAT:
					return std::format_to(ctx.out(), "UPNG_EUNFORMAT: Image color format is not supported");
				case UPNG_EPARAM:
					return std::format_to(ctx.out(), "UPNG_EPARAM: Invalid parameter to method call");
				default:
					return std::format_to(ctx.out(), "Unknown upng error code: {}", static_cast<int>(e));
			}
		}
	};
}

export namespace upng
{
	using upng_unique_ptr = raii::direct_unique_ptr<upng_t, upng_free>;

	class error final : public std::runtime_error
	{
	public:
		error(upng_error error_code, std::string_view message)
			: std::runtime_error(std::format("upng error: {}: {}", error_code, message)), code(error_code)
		{ }
		constexpr auto error_code(this const auto& self) noexcept -> upng_error { return self.code; }

	private:
		upng_error code = upng_error::UPNG_EOK;
	};

	struct upng_texture final
	{
		upng_texture(const std::filesystem::path& path)
		{
			auto x = upng_new_from_file(path.string().c_str());
			png = upng_unique_ptr(x);
			if (png == nullptr)
				throw std::runtime_error("Failed to load PNG from file");
			
			upng_decode(png.get());
			if (auto error_code = upng_get_error(png.get()); error_code != UPNG_EOK)
				throw error(error_code, "Failed to decode PNG");
		}
		
		const unsigned char* buffer() const noexcept { return upng_get_buffer(png.get()); }
		unsigned width() const noexcept { return upng_get_width(png.get()); }
		unsigned height() const noexcept { return upng_get_height(png.get()); }
		upng_format format() const noexcept { return upng_get_format(png.get()); }

	private:
		upng_unique_ptr png;
	};
}
