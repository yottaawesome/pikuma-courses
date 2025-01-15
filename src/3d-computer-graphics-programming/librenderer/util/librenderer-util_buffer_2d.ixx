export module librenderer:util_buffer_2d;
import std;
import std.compat;
import :util_functions;

export namespace util
{
    template<typename T>
    concept is_arithmetic = std::is_arithmetic_v<T>;

    // represents a 2D buffer of an underlying numeric type.
    template<is_arithmetic T>
    class buffer_2d final
    {
        public:
            using backing_type = T;

        public:
            buffer_2d() = default;

            buffer_2d(const T width, const T height)
                : m_width(width),
                m_height(height),
                m_buffer(std::vector<T>(width * height))
            {}

        public:
            T& operator[](const uint64_t index) noexcept
            {
                return m_buffer[index];
            }

        public:
            T width()            const noexcept { return m_width; }
            T height()           const noexcept { return m_height; }
            T total_elements()   const noexcept { return m_buffer.size(); }
            T* raw_buffer()      noexcept { return m_buffer.data(); }

            void set(
                uint64_t row,
                uint64_t column,
                const T value
            ) noexcept(util::is_release())
            {
                if constexpr (util::is_debug()) // for debugging only
                {
                    if (row >= m_height)
                        row = m_height - 1;
                        //throw std::runtime_error(std::format("Invalid row {}", row));
                    if (column >= m_width)
                        column = m_width - 1;

                        //throw std::runtime_error(std::format("Invalid width {}", column));
                }
                m_buffer[row * m_width + column] = value;
            }

            void fill(const T value = 0) noexcept
            {
                std::ranges::fill(m_buffer, value);
            }

            uint32_t pitch() const noexcept
            {
                // this is the number of bytes in each row
                return m_width * sizeof(uint32_t);
            }

        private:
            uint32_t m_width = 0;
            uint32_t m_height = 0;
            std::vector<T> m_buffer = std::vector<T>(m_width * m_height);
    };

    // Each byte in uint32_t is one element of the pixel colour
    // 0x{alpha}{red}{green}{blue}
    // 0xFFFFFFFF
    using color_buffer = buffer_2d<uint32_t>;
}