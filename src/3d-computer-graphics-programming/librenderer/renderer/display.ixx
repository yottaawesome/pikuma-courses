export module librenderer:display;
import std;
import :math;

export namespace display
{
    enum class colors : std::uint32_t
    {
        red = 0xffff0000,
        green = 0xff00ff00,
        blue = 0xff0000ff,
        white = 0xffffffff,
        black = 0xff000000
    };

    constexpr void draw_line_grid(std::int32_t step, std::uint32_t color, math::color_buffer& buffer)
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

    constexpr void draw_dot_grid(std::int32_t step, std::uint32_t color, math::color_buffer& buffer)
    {
        for (uint32_t row = 0; row < buffer.height(); row += 10)
            for (uint32_t column = 0; column < buffer.width(); column += 10)
                buffer.set(row, column, color);
    }

    constexpr void draw_pixel(std::uint32_t x, std::uint32_t y, std::uint32_t color, math::color_buffer& buffer)
    {
        buffer.set(x, y, color);
    }

    constexpr void draw_rect(
        std::uint32_t x,
        std::uint32_t y,
        std::uint32_t width,
        std::uint32_t height,
        std::uint32_t color,
        math::color_buffer& buffer
    )
    {
        for (uint32_t row = y; row < y + width and row < buffer.height(); row++)
            for (uint32_t column = x; column < x + height and column < buffer.width(); column++)
                draw_pixel(row, column, color, buffer);
    }

    void render_color_buffer(
        sdl::SDL_Renderer* renderer,
        math::color_buffer& buffer,
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
        math::color_buffer& buffer
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
        const math::triangle& triangle,
        const std::uint32_t color,
        math::color_buffer& buffer
    )
    {
        draw_line(
            static_cast<int>(triangle.vertices[0].x),
            static_cast<int>(triangle.vertices[0].y),
            static_cast<int>(triangle.vertices[1].x),
            static_cast<int>(triangle.vertices[1].y),
            color,
            buffer
        ); // line from 0 -> 1
        draw_line(
            static_cast<int>(triangle.vertices[1].x),
            static_cast<int>(triangle.vertices[1].y),
            static_cast<int>(triangle.vertices[2].x),
            static_cast<int>(triangle.vertices[2].y),
            color,
            buffer
        ); // line from 1 -> 2
        draw_line(
            static_cast<int>(triangle.vertices[2].x),
            static_cast<int>(triangle.vertices[2].y),
            static_cast<int>(triangle.vertices[0].x),
            static_cast<int>(triangle.vertices[0].y),
            color,
            buffer
        ); // and back to 2 -> 0
    }

    void fill_flat_bottom_triangle(const math::triangle& tri, std::uint32_t color, math::color_buffer& buffer)
    {
        float inv_slope_1 = static_cast<float>(tri.vertices[1].x - tri.vertices[0].x) / (tri.vertices[1].y - tri.vertices[0].y);
        float inv_slope_2 = static_cast<float>(tri.vertices[2].x - tri.vertices[0].x) / (tri.vertices[2].y - tri.vertices[0].y);

        float x_start = tri.vertices[0].x;
        float x_end = tri.vertices[0].x;

        // While looping, you may end up with extreme slopes that can
        // cause huge lines to be drawn. Here, we correct for this.
        float max_width = std::abs(tri.vertices[2].x - tri.vertices[1].x); // prevents constexpr
        for (int y = static_cast<int>(tri.vertices[0].y); y <= tri.vertices[2].y; y++)
        {
            draw_line(static_cast<int>(x_start), static_cast<int>(y), static_cast<int>(x_end), static_cast<int>(y), color, buffer);
            x_start += inv_slope_1;
            x_end += inv_slope_2;

            if (std::abs(x_end - x_start) > max_width)
            {
                x_start = tri.vertices[1].x;
                x_end = tri.vertices[2].x;
            }
        }
    }

    constexpr void fill_flat_top_triangle(const math::triangle& tri, std::uint32_t color, math::color_buffer& buffer)
    {
        float inv_slope_1 = static_cast<float>(tri.vertices[2].x - tri.vertices[0].x) / (tri.vertices[2].y - tri.vertices[0].y);
        float inv_slope_2 = static_cast<float>(tri.vertices[2].x - tri.vertices[1].x) / (tri.vertices[2].y - tri.vertices[1].y);

        float x_start = tri.vertices[2].x;
        float x_end = tri.vertices[2].x;

        for (int y = static_cast<int>(tri.vertices[2].y); y >= tri.vertices[0].y; y--)
        {
            draw_line(static_cast<int>(x_start), static_cast<int>(y), static_cast<int>(x_end), static_cast<int>(y), color, buffer);
            x_start -= inv_slope_1;
            x_end -= inv_slope_2;
        }
    }

    constexpr void draw_filled_triangle(
        const math::triangle& triangle,
        std::uint32_t color,
        math::color_buffer& buffer
    )
    {
        // Sort by ascending y-coordinate
        math::vector_4f vertices[3]{ triangle.vertices[0], triangle.vertices[1], triangle.vertices[2] };
        std::ranges::sort(vertices, [](const auto& a, const auto& b) { return a.y < b.y; });
        
        if (vertices[1].y == vertices[2].y)
        {
            // Simply darw flat-bottom triangle
            fill_flat_bottom_triangle(
                { 
                    .vertices{
                        { vertices[0].x, vertices[0].y },
                        { vertices[1].x, vertices[1].y },
                        { vertices[2].x, vertices[2].y }
                    } 
                },
                color,
                buffer
            );
        }
        else if (vertices[0].y == vertices[1].y)
        {
            // Simply darw flat-top triangle
            fill_flat_top_triangle(
                {
                    .vertices{
                       { vertices[0].x, vertices[0].y },
                       { vertices[1].x, vertices[1].y },
                       { vertices[2].x, vertices[2].y }
                    } 
                },
                color,
                buffer
            );
        }
        else
        {
            // Find midpoint coordinates
            int My = static_cast<int>(vertices[1].y);
            int Mx = static_cast<int>(
                (
                    (
                        (vertices[2].x - vertices[0].x) 
                        * 
                        (vertices[1].y - vertices[0].y)
                    ) 
                    / (vertices[2].y - vertices[0].y)
                ) 
                + vertices[0].x
            );
            fill_flat_bottom_triangle({ 
                    .vertices{
                        { vertices[0].x, vertices[0].y},
                        { vertices[1].x, vertices[1].y},
                        {static_cast<float>(Mx), static_cast<float>(My)}
                    } 
                },
                color,
                buffer
            );
            fill_flat_top_triangle(
                { 
                    .vertices{
                        {vertices[1].x, vertices[1].y},
                        {static_cast<float>(Mx), static_cast<float>(My)},
                        {vertices[2].x, vertices[2].y}
                    } 
                },
                color,
                buffer
            );
        }
    }

    // Draw a textured triangle with flat-top/flat-bottom method.
    constexpr void draw_textured_triangle(
        const math::triangle& triangle, 
        std::uint32_t color, 
        math::color_buffer& buffer,
        const std::uint8_t* const texture
    )
    {
        // Sort by ascending y-coordinate
        math::vector_4f vertices[3]{ triangle.vertices[0], triangle.vertices[1], triangle.vertices[2] };
        std::ranges::sort(vertices, [](const auto& a, const auto& b) { return a.y < b.y; });
    }
}
