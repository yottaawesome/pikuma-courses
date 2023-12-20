export module renderer;

import shared;
import util;

export 
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
            T width()            const noexcept  { return m_width; }
            T height()           const noexcept  { return m_height; }
            T total_elements()   const noexcept  { return m_buffer.size(); }
            T* raw_buffer()      noexcept        { return m_buffer.data(); }

            void set(
                const uint64_t row, 
                const uint64_t column, 
                const T value
            ) noexcept
            {
                m_buffer[row * m_height + column] = value;
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

    struct main_app
    {
        main_app() = default;
        main_app(const main_app&) = delete;
        main_app& operator=(const main_app&) = delete;
        main_app(main_app&&) = default;
        main_app& operator=(main_app&&) = default;

        operator bool() const noexcept { return is_running; }

        bool is_running = false;

        // The SDL context for our application
        std::unique_ptr<util::sdl_context> context;
        // The main window
        util::sdl_window_unique_ptr window;
        // The main renderer that works with our window
        util::sdl_renderer_unique_ptr renderer;
        // Our color buffer, we copy this to the color_buffer_texture
        // texture and then pass the texture to the renderer
        color_buffer main_buffer{};
        // We copy the color buffer to this
        util::sdl_texture_unique_ptr color_buffer_texture;
    };
}
