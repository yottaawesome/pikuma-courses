export module librenderer:upng.error;
import std;
import :upng.exports;
import :upng.formatters;

export namespace upng
{
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
}