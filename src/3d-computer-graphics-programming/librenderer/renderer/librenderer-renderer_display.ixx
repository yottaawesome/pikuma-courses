export module librenderer:renderer_display;
import :util;
import :shared;
import :renderer_mainapp;

export namespace display
{
    enum class colors
    {
        red = 0xffff0000,
        green = 0xff00ff00,
        blue = 0xff0000ff,
        white = 0xffffffff,
        black = 0xff000000
    };

    constexpr void draw_line_grid(const std::int32_t step, const std::uint32_t color, util::color_buffer& buffer)
    {
        for (uint32_t row = 0; row < buffer.height(); row++)
        {
            for (uint32_t column = 0; column < buffer.width(); column++)
            {
                if (row == 0)
                    buffer.set(row, column, color);
                else if (column == 0)
                    buffer.set(row, column, color);
                else if (std::div(row + 1ui32, step).rem == 0)
                    buffer.set(row, column, color);
                else if (std::div(column + 1ui32, step).rem == 0)
                    buffer.set(row, column, color);
            }
        }
    }

    constexpr void draw_dot_grid(const std::int32_t step, const std::uint32_t color, util::color_buffer& buffer)
    {
        for (uint32_t row = 0; row < buffer.height(); row += 10)
            for (uint32_t column = 0; column < buffer.width(); column += 10)
                buffer.set(row, column, color);
    }

    constexpr void draw_pixel(const std::uint32_t x, const std::uint32_t y, const std::uint32_t color, util::color_buffer& buffer)
    {
        buffer.set(x, y, color);
    }

    constexpr void draw_rect(
        const std::uint32_t x,
        const std::uint32_t y,
        const std::uint32_t width,
        const std::uint32_t height,
        const std::uint32_t color,
        util::color_buffer& buffer
    )
    {
        for (uint32_t row = y; row < y + width and row < buffer.height(); row++)
            for (uint32_t column = x; column < x + height and column < buffer.width(); column++)
                draw_pixel(row, column, color, buffer);
    }

    void render_color_buffer(
        sdl::SDL_Renderer* renderer,
        util::color_buffer& buffer,
        sdl::SDL_Texture* color_buffer_texture
    )
    {
        sdl::SDL_UpdateTexture(
            color_buffer_texture,
            nullptr,
            buffer.raw_buffer(),
            buffer.pitch()
        );
        sdl::SDL_RenderCopy(
            renderer,
            color_buffer_texture,
            nullptr,
            nullptr
        );
    }

    // DDA algorithm
    constexpr void draw_line(
        const int x0,
        const int y0,
        const int x1,
        const int y1,
        const std::uint32_t color,
        util::color_buffer& buffer
    )
    {
        int delta_x = x1 - x0;
        int delta_y = y1 - y0;

        int side_length = std::abs(delta_x) >= std::abs(delta_y) ? std::abs(delta_x) : std::abs(delta_y);

        float x_inc = delta_x / static_cast<float>(side_length);
        float y_inc = delta_y / static_cast<float>(side_length);

        float current_x = static_cast<float>(x0);
        float current_y = static_cast<float>(y0);

        for (int i = 0; i <= side_length; i++)
        {
            draw_pixel(
                static_cast<uint32_t>(std::round(current_y)),
                static_cast<uint32_t>(std::round(current_x)),
                color, 
                buffer
            );
            current_x += x_inc;
            current_y += y_inc;
        }
    }

    constexpr void draw_triangle(
        const util::triangle triangle,
        const std::uint32_t color,
        util::color_buffer& buffer
    )
    {
        draw_line(
            static_cast<int>(triangle.points[0].x),
            static_cast<int>(triangle.points[0].y),
            static_cast<int>(triangle.points[1].x),
            static_cast<int>(triangle.points[1].y),
            color,
            buffer
        ); // line from 0 -> 1
        draw_line(
            static_cast<int>(triangle.points[1].x),
            static_cast<int>(triangle.points[1].y),
            static_cast<int>(triangle.points[2].x),
            static_cast<int>(triangle.points[2].y),
            color,
            buffer
        ); // line from 1 -> 2
        draw_line(
            static_cast<int>(triangle.points[2].x),
            static_cast<int>(triangle.points[2].y),
            static_cast<int>(triangle.points[0].x),
            static_cast<int>(triangle.points[0].y),
            color,
            buffer
        ); // and back to 2 -> 0
    }

    void fill_flat_bottom_triangle(util::triangle tri, std::uint32_t color, util::color_buffer& buffer)
    {
        float inv_slope_1 = static_cast<float>(tri.points[1].x - tri.points[0].x) / (tri.points[1].y - tri.points[0].y);
        float inv_slope_2 = static_cast<float>(tri.points[2].x - tri.points[0].x) / (tri.points[2].y - tri.points[0].y);

        float x_start = tri.points[0].x;
        float x_end = tri.points[0].x;

        // While looping, you may end up with extreme slopes that can
        // cause huge lines to be drawn. Here, we correct for this.
        float max_width = std::abs(tri.points[2].x - tri.points[1].x); // prevents constexpr
        for (int y = static_cast<int>(tri.points[0].y); y <= tri.points[2].y; y++)
        {
            draw_line(static_cast<int>(x_start), static_cast<int>(y), static_cast<int>(x_end), static_cast<int>(y), color, buffer);
            x_start += inv_slope_1;
            x_end += inv_slope_2;

            if (std::abs(x_end - x_start) > max_width)
            {
                x_start = tri.points[1].x;
                x_end = tri.points[2].x;
            }
        }
    }

    constexpr void fill_flat_top_triangle(util::triangle tri, std::uint32_t color, util::color_buffer& buffer)
    {
        float inv_slope_1 = static_cast<float>(tri.points[2].x - tri.points[0].x) / (tri.points[2].y - tri.points[0].y);
        float inv_slope_2 = static_cast<float>(tri.points[2].x - tri.points[1].x) / (tri.points[2].y - tri.points[1].y);

        float x_start = tri.points[2].x;
        float x_end = tri.points[2].x;

        for (int y = static_cast<int>(tri.points[2].y); y >= tri.points[0].y; y--)
        {
            draw_line(static_cast<int>(x_start), static_cast<int>(y), static_cast<int>(x_end), static_cast<int>(y), color, buffer);
            x_start -= inv_slope_1;
            x_end -= inv_slope_2;
        }
    }

    constexpr void draw_filled_triangle(
        util::triangle triangle,
        std::uint32_t color,
        util::color_buffer& buffer
    )
    {
        // Sort by ascending y-coordinate
        std::ranges::sort(triangle.points, [](auto a, auto b) { return a.y < b.y; });
        
        if (triangle.points[1].y == triangle.points[2].y)
        {
            // Simply darw flat-bottom triangle
            fill_flat_bottom_triangle({ 
                    .points{
                        triangle.points[0].x, triangle.points[0].y, 
                        triangle.points[1].x, triangle.points[1].y, 
                        triangle.points[2].x, triangle.points[2].y
                    } 
                },
                color,
                buffer
            );
            return;
        }
        else if (triangle.points[0].y == triangle.points[1].y)
        {
            // Simply darw flat-top triangle
            fill_flat_top_triangle({
                    .points{
                        triangle.points[0].x, triangle.points[0].y, 
                        triangle.points[1].x, triangle.points[1].y, 
                        triangle.points[2].x, triangle.points[2].y
                    } 
                },
                color,
                buffer
            );
        }
        else
        {
            // Find midpoint coordinates
            int My = static_cast<int>(triangle.points[1].y);
            int Mx = static_cast<int>((((triangle.points[2].x - triangle.points[0].x) * (triangle.points[1].y - triangle.points[0].y)) / (triangle.points[2].y - triangle.points[0].y)) + triangle.points[0].x);
            fill_flat_bottom_triangle({ 
                    .points{ 
                        triangle.points[0].x, triangle.points[0].y, 
                        triangle.points[1].x, triangle.points[1].y, 
                        static_cast<float>(Mx), static_cast<float>(My)
                    } 
                },
                color,
                buffer
            );
            fill_flat_top_triangle({ 
                    .points{
                        triangle.points[1].x, triangle.points[1].y, 
                        static_cast<float>(Mx), static_cast<float>(My), 
                        triangle.points[2].x, triangle.points[2].y
                    } 
                },
                color,
                buffer
            );
        }
    }
}