export module renderer:display;
import util;
import shared;
import :main_app;

export namespace display
{
    bool initialize_window(main_app& app)
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
}