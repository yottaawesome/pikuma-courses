export module librenderer:util.functions;
import std;
import :win32;

export namespace util
{
    constexpr bool is_debug =
#ifdef _DEBUG
        true;
#else
        false;
#endif // _DEBUG

    constexpr bool is_release = not is_debug;

    template<typename...TArgs>
    auto print_debug_string(std::format_string<TArgs...> fmt, TArgs&&...args) -> std::string
    {
        auto error = std::format("{}\n", std::format(fmt, std::forward<TArgs>(args)...));
        win32::OutputDebugStringA(error.c_str());
        return error;
    }
}
