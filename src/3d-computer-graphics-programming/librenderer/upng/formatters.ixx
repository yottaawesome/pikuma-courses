export module librenderer:upng.formatters;
import std;
import :upng.exports;

export namespace std
{
	template<>
	struct formatter<upng::upng_error>
	{
		constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator { return ctx.begin(); }

		auto format(upng::upng_error e, format_context& ctx) const -> format_context::iterator
		{
			switch (e)
			{
			case upng::upng_error::UPNG_EOK:
				return std::format_to(ctx.out(), "UPNG_EOK: No error");
			case upng::upng_error::UPNG_ENOMEM:
				return std::format_to(ctx.out(), "UPNG_ENOMEM: Memory allocation failed");
			case upng::upng_error::UPNG_ENOTFOUND:
				return std::format_to(ctx.out(), "UPNG_ENOTFOUND: Resource not found (file missing)");
			case upng::upng_error::UPNG_ENOTPNG:
				return std::format_to(ctx.out(), "UPNG_ENOTPNG: Image data does not have a PNG header");
			case upng::upng_error::UPNG_EMALFORMED:
				return std::format_to(ctx.out(), "UPNG_EMALFORMED: Image data is not a valid PNG image");
			case upng::upng_error::UPNG_EUNSUPPORTED:
				return std::format_to(ctx.out(), "UPNG_EUNSUPPORTED: Critical PNG chunk type is not supported");
			case upng::upng_error::UPNG_EUNINTERLACED:
				return std::format_to(ctx.out(), "UPNG_EUNINTERLACED: Image interlacing is not supported");
			case upng::upng_error::UPNG_EUNFORMAT:
				return std::format_to(ctx.out(), "UPNG_EUNFORMAT: Image color format is not supported");
			case upng::upng_error::UPNG_EPARAM:
				return std::format_to(ctx.out(), "UPNG_EPARAM: Invalid parameter to method call");
			default:
				return std::format_to(ctx.out(), "Unknown upng error code: {}", static_cast<int>(e));
			}
		}
	};
}
