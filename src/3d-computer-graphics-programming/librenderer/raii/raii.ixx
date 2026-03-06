export module librenderer:raii;
import std;

export namespace raii
{
    template<auto VDeleteFn>
    struct deleter
    {
        static constexpr void operator()(auto resource) noexcept
        {
            VDeleteFn(resource);
        }
    };

    template<typename T, auto VDeleteFn>
	using direct_unique_ptr = std::unique_ptr<T, deleter<VDeleteFn>>;
    template<typename T, auto VDeleteFn>
    using indirect_unique_ptr = std::unique_ptr<std::remove_pointer_t<T>, deleter<VDeleteFn>>;
}
