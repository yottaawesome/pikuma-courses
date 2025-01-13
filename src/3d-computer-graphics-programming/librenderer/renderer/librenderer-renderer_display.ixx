export module librenderer:renderer_display;
import :util;
import :shared;
import :renderer_mainapp;

export namespace display
{
    bool initialize_window()
    {
        // Nothing to do -- setup logic moved to main_app.
        return true;
    }

    void draw_line_grid(const int32_t step, const uint32_t color, util::color_buffer& buffer)
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

    void draw_dot_grid(const int32_t step, const uint32_t color, util::color_buffer& buffer)
    {
        for (uint32_t row = 0; row < buffer.height(); row += 10)
            for (uint32_t column = 0; column < buffer.width(); column += 10)
                buffer.set(row, column, color);
    }

    void draw_pixel(const uint32_t x, const uint32_t y, const uint32_t color, util::color_buffer& buffer)
    {
        buffer.set(x, y, color);
    }

    void draw_rect(
        const uint32_t x,
        const uint32_t y,
        const uint32_t width,
        const uint32_t height,
        const uint32_t color,
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
    void draw_line(
        const int x0,
        const int y0,
        const int x1,
        const int y1,
        const uint32_t color,
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

    void draw_triangle(
        const util::triangle triangle,
        const uint32_t color,
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

    // TODO
    void fill_flat_bottom_triangle(util::triangle tri, uint32_t color)
    {

    }

    void fill_flat_top_triangle(util::triangle tri, uint32_t color)
    {

    }

    void draw_filled_triangle(
        util::triangle triangle,
        uint32_t color,
        util::color_buffer& buffer
    )
    {
        // Sort by ascending y-coordinate
        std::ranges::sort(triangle.points, [](auto a, auto b) { return a.y < b.y; });
        // Find midpoint coordinates
        int My = triangle.points[1].y;
        int Mx =
            (((triangle.points[2].x - triangle.points[0].x) * (triangle.points[1].y - triangle.points[0].y)) / (triangle.points[2].y - triangle.points[0].y)) + triangle.points[0].x;
        fill_flat_bottom_triangle(
            { {triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, static_cast<float>(Mx), static_cast<float>(My)} },
            color
        );
        fill_flat_top_triangle(
            { {triangle.points[1].x, triangle.points[1].y, static_cast<float>(Mx), static_cast<float>(My), triangle.points[2].x, triangle.points[2].y} },
            color
        );
    }
}