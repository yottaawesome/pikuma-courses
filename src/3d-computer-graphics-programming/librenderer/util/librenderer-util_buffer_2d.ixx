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
    struct buffer_2d final
    {
        using backing_type = T;

        constexpr buffer_2d() = default;

        constexpr buffer_2d(const T width, const T height)
            : m_width(width),
            m_height(height),
            m_buffer(std::vector<T>(width * height))
        {}

        constexpr T& operator[](const uint64_t index) noexcept
        {
            return m_buffer[index];
        }

        constexpr T width()            const noexcept { return m_width; }
        constexpr T height()           const noexcept { return m_height; }
        constexpr T total_elements()   const noexcept { return m_buffer.size(); }
        constexpr T* raw_buffer()      noexcept { return m_buffer.data(); }

        constexpr void set(
            uint64_t row,
            uint64_t column,
            const T value
        ) noexcept(util::is_release())
        {
            if constexpr (util::is_debug()) // for debugging only
            {
                if (row >= m_height)
                    //row = m_height - 1;
                    throw std::runtime_error(std::format("Invalid row {}", row));
                if (column >= m_width)
                    //column = m_width - 1;
                    throw std::runtime_error(std::format("Invalid width {}", column));
            }
            m_buffer[row * m_width + column] = value;
        }

        constexpr void fill(const T value = 0) noexcept
        {
            std::ranges::fill(m_buffer, value);
        }

        constexpr uint32_t pitch() const noexcept
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

static_assert(
    []{
        util::buffer_2d<std::uint32_t> buffer;
        return buffer.width() == 0 and buffer.height() == 0;
    }(), "Default constructed buffer is expected to be zero width and height.");
static_assert(
    [] {
        util::buffer_2d<uint32_t> buffer{ 200, 300 };
        return buffer.width() == 200 and buffer.height() == 300;
    }(), "A buffer constructed with arguments 200 and 300 should have width and height of 200 and 300 respectively.");
static_assert(
    [] {
        util::buffer_2d<uint32_t> buffer{ 200, 300 };
        buffer.set(0, 1, 45);
        return buffer.raw_buffer()[1] == 45;
    }(), "A set() operation did not modify the expected pixel to the correct value.");
static_assert(
    [] {
        util::buffer_2d<uint32_t> buffer{ 200, 300 };
        buffer.set(1, 1, 45);
        return buffer.raw_buffer()[200 + 1] == 45;
    }(), "A set() operation did not modify the expected pixel to the correct value.");