export module librenderer:buffer_2d;
import std;
import :util;
import :concepts;

export namespace math
{
    // represents a 2D buffer of an underlying numeric type.
    template<concepts::is_arithmetic T>
    struct buffer_2d final
    {
        using backing_type = T;

        constexpr buffer_2d() = default;

        constexpr buffer_2d(std::uint32_t width, std::uint32_t height)
            : m_width(width),
            m_height(height),
            m_buffer(std::vector<T>(width * height))
        {}

        constexpr auto operator[](this auto&& self, const std::uint64_t index) noexcept -> decltype(auto)
        {
            return std::forward_like<decltype(self)>(m_buffer[index]);
        }

        constexpr auto width()          const noexcept  -> std::uint32_t { return m_width; }
        constexpr auto height()         const noexcept  -> std::uint32_t { return m_height; }
        constexpr auto total_elements() const noexcept  -> size_t    { return m_buffer.size(); }
        constexpr auto raw_buffer()     noexcept        -> T*   { return m_buffer.data(); }

        constexpr void set(std::uint64_t row, std::uint64_t column, T value) noexcept(util::is_release)
        {
            if (check_bounds(row, column, std::nothrow))
                m_buffer[row * m_width + column] = value;
        }

		constexpr auto operator[](this auto&& self, std::uint64_t row, std::uint64_t column) noexcept -> decltype(auto)
        {
            self.check_bounds(row, column);
            return std::forward_like<decltype(self)>(self.m_buffer[row * self.m_width + column]);
		}

        constexpr auto check_bounds(
            this auto&& self, 
            std::uint64_t row, 
            std::uint64_t column,
            const std::nothrow_t&
        ) noexcept -> bool
        {
            return row < self.m_height and column < self.m_width;
        }

        constexpr void check_bounds(this auto&& self, std::uint64_t row, std::uint64_t column)
        {
            if constexpr (util::is_debug) // for debugging only
				if (not self.check_bounds(row, column, std::nothrow))
				    throw std::runtime_error(std::format("Index out of bounds {}:{} against {}:{}", row, column, self.m_width, self.m_height));
		}

        constexpr void fill(const T value = 0) noexcept
        {
            std::ranges::fill(m_buffer, value);
        }

		constexpr auto pitch() const noexcept -> std::uint32_t
        {
            // this is the number of bytes in each row
            return m_width * sizeof(std::uint32_t);
        }

    private:
        std::uint32_t m_width = 0;
        std::uint32_t m_height = 0;
        std::vector<T> m_buffer = std::vector<T>(m_width * m_height);
    };

    // Each byte in uint32_t is one element of the pixel colour
    // 0x{alpha}{red}{green}{blue}
    // 0xFFFFFFFF
    using color_buffer = buffer_2d<uint32_t>;
}

static_assert(
    []{
        math::buffer_2d<std::uint32_t> buffer;
        return buffer.width() == 0 and buffer.height() == 0;
    }(), "Default constructed buffer is expected to be zero width and height.");
static_assert(
    [] {
        math::buffer_2d<std::uint32_t> buffer{ 200, 300 };
        return buffer.width() == 200 and buffer.height() == 300;
    }(), "A buffer constructed with arguments 200 and 300 should have width and height of 200 and 300 respectively.");
static_assert(
    [] {
        math::buffer_2d<std::uint32_t> buffer{ 200, 300 };
        buffer.set(0, 1, 45);
        return buffer.raw_buffer()[1] == 45;
    }(), "A set() operation did not modify the expected pixel to the correct value.");
static_assert(
    [] {
        math::buffer_2d<std::uint32_t> buffer{ 200, 300 };
        buffer.set(1, 1, 45);
        return buffer.raw_buffer()[200 + 1] == 45;
    }(), "A set() operation did not modify the expected pixel to the correct value.");
static_assert(
    [] {
        math::buffer_2d<std::uint32_t> buffer{ 200, 300 };
        buffer[1,1] = 45;
        return buffer.raw_buffer()[200 + 1] == 45;
    }(), "A set() operation did not modify the expected pixel to the correct value.");
