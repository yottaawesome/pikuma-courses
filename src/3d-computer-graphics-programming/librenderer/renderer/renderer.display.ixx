export module librenderer:renderer.display;
import std;
import :math;
import :sdl;
import :renderer.primitives;
import :renderer.buffer_2d;

export namespace renderer
{
    enum class colors : std::uint32_t
    {
        red = 0xffff0000,
        green = 0xff00ff00,
        blue = 0xff0000ff,
        white = 0xffffffff,
        black = 0xff000000
    };

    constexpr void draw_line_grid(std::int32_t step, std::uint32_t color, renderer::color_buffer& buffer)
    {
        for (uint32_t row = 0; row < buffer.height(); row++)
        {
            for (uint32_t column = 0; column < buffer.width(); column++)
            {
                if (row == 0)
                    buffer.set(row, column, color);
                else if (column == 0)
                    buffer.set(row, column, color);
                else if (std::div(static_cast<int>(row + 1), step).rem == 0)
                    buffer.set(row, column, color);
                else if (std::div(static_cast<int>(column + 1), step).rem == 0)
                    buffer.set(row, column, color);
            }
        }
    }

    constexpr void draw_dot_grid(std::int32_t step, std::uint32_t color, renderer::frame_buffer& buffer)
    {
        for (uint32_t row = 0; row < buffer.color.height(); row += 10)
            for (uint32_t column = 0; column < buffer.color.width(); column += 10)
                buffer.color.set(row, column, color);
    }

	// This is in row-major form. This means that if you're
	// working with Cartesian coordinates, you need to swap 
    // x and y to get the correct pixel.
    constexpr void draw_pixel(std::uint32_t x, std::uint32_t y, std::uint32_t color, renderer::frame_buffer& buffer)
    {
        buffer.color.set(x, y, color);
    }

    constexpr void draw_rect(
        std::uint32_t x,
        std::uint32_t y,
        std::uint32_t width,
        std::uint32_t height,
        std::uint32_t color,
        renderer::frame_buffer& buffer
    )
    {
        for (uint32_t row = y; row < y + width and row < buffer.color.height(); row++)
            for (uint32_t column = x; column < x + height and column < buffer.color.width(); column++)
                draw_pixel(row, column, color, buffer);
    }

    void render_color_buffer(
        sdl::SDL_Renderer* renderer,
        renderer::color_buffer& buffer,
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
        renderer::frame_buffer& buffer
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
            auto px = static_cast<int>(std::round(current_x));
            auto py = static_cast<int>(std::round(current_y));
            if (px >= 0 and py >= 0)
                draw_pixel(
                    static_cast<uint32_t>(py),
                    static_cast<uint32_t>(px),
                    color,
                    buffer
                );
            current_x += x_inc;
            current_y += y_inc;
        }
    }

    constexpr void draw_triangle(
        const renderer::triangle& triangle,
        const std::uint32_t color,
        renderer::frame_buffer& buffer
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

    constexpr void draw_triangle_pixel(
        int x,
        int y,
        const std::array<textured_vertex, 3>& vertex,
        renderer::frame_buffer& buffer,
		std::uint32_t color
    )
    {
        if (x < 0 or y < 0
            or static_cast<std::uint32_t>(y) >= buffer.depth.height()
            or static_cast<std::uint32_t>(x) >= buffer.depth.width())
            return;

        auto weights = math::barycentric_weights(
            math::vector_2f{ .x = vertex[0].position.x, .y = vertex[0].position.y },
            math::vector_2f{ .x = vertex[1].position.x, .y = vertex[1].position.y },
            math::vector_2f{ .x = vertex[2].position.x, .y = vertex[2].position.y },
            math::vector_2f{ .x = static_cast<float>(x), .y = static_cast<float>(y) }
        );

        auto alpha = float{ weights.x };
        auto beta = float{ weights.y };
        auto gamma = float{ weights.z };

        // Perform the interpolation of all U/w and V/w values using barycentric 
        // weights and a factor of 1/w.
        auto interpolated_u = float{
            (vertex[0].texcoords.u / vertex[0].position.w) * alpha
            + (vertex[1].texcoords.u / vertex[1].position.w) * beta
            + (vertex[2].texcoords.u / vertex[2].position.w) * gamma
        };
        auto interpolated_v = float{
            (vertex[0].texcoords.v / vertex[0].position.w) * alpha
            + (vertex[1].texcoords.v / vertex[1].position.w) * beta
            + (vertex[2].texcoords.v / vertex[2].position.w) * gamma
        };
        // Also interpolate the 1/w value for the current pixel.
        auto interpolated_w_reciprocal = float{
            (1.f / vertex[0].position.w) * alpha
            + (1.f / vertex[1].position.w) * beta
            + (1.f / vertex[2].position.w) * gamma
        };

        // Now we can divide back both interpolated U and V by the interpolated 1/w.
        interpolated_u /= interpolated_w_reciprocal;
        interpolated_v /= interpolated_w_reciprocal;

        // Use 1/w directly for depth testing: larger 1/w means
        // closer to the camera. Avoids the precision loss that
        // a "1 - 1/w" transformation would introduce.
        if (interpolated_w_reciprocal > buffer.depth[static_cast<uint32_t>(y), static_cast<uint32_t>(x)])
        {
            draw_pixel(static_cast<uint32_t>(y), static_cast<uint32_t>(x), color, buffer);
            buffer.depth.set(static_cast<uint32_t>(y), static_cast<uint32_t>(x), interpolated_w_reciprocal);
        }
    }

    constexpr void draw_filled_triangle(
        const triangle& triangle,
        std::uint32_t color,
        renderer::frame_buffer& buffer
    )
    {
        // Sort by ascending y-coordinate
        auto vertices = std::array{
            textured_vertex{triangle.vertices[0], triangle.texcoords[0]},
            textured_vertex{triangle.vertices[1], triangle.texcoords[1]},
            textured_vertex{triangle.vertices[2], triangle.texcoords[2]}
        };

        std::ranges::sort(
            vertices,
            [](const auto& a, const auto& b) static noexcept
            {
                return a.position.y < b.position.y;
            });
        // Some systems have the v coordinate growing 
        // downwards, while others have it growing upwards.
        // In this case, it grows downwards, so we flip it to 
        // match the texture space.
        for (textured_vertex& vertex : vertices)
        {
            vertex.texcoords.v = 1.f - vertex.texcoords.v;
        }

        // Render upper part of triangle
        float inv_slope_1 = 0;
        float inv_slope_2 = 0;
        if (vertices[1].position.y - vertices[0].position.y != 0)
            inv_slope_1 = static_cast<float>((vertices[1].position.x - vertices[0].position.x) / math::abs(vertices[1].position.y - vertices[0].position.y));
        if (vertices[2].position.y - vertices[0].position.y != 0)
            inv_slope_2 = static_cast<float>((vertices[2].position.x - vertices[0].position.x) / math::abs(vertices[2].position.y - vertices[0].position.y));

        if (vertices[1].position.y - vertices[0].position.y != 0)
        {
            for (
                float y = vertices[0].position.y;
                y <= (vertices[1].position.y);
                y++
            )
            {
                float x_start = (vertices[1].position.x + (y - vertices[1].position.y) * inv_slope_1);
                float x_end = (vertices[0].position.x + (y - vertices[0].position.y) * inv_slope_2);

                if (x_start > x_end)
                    std::swap(x_start, x_end);

                for (float x = x_start; x < x_end; x++)
                {
                    draw_triangle_pixel(x, y, vertices, buffer, color);
                }
            }
        }

        // Render bottom part of triangle
        inv_slope_1 = 0;
        inv_slope_2 = 0;
        if (vertices[2].position.y - vertices[1].position.y != 0)
            inv_slope_1 = static_cast<float>((vertices[2].position.x - vertices[1].position.x) / math::abs(vertices[2].position.y - vertices[1].position.y));
        if (vertices[2].position.y - vertices[0].position.y != 0)
            inv_slope_2 = static_cast<float>((vertices[2].position.x - vertices[0].position.x) / math::abs(vertices[2].position.y - vertices[0].position.y));

        if (vertices[2].position.y - vertices[1].position.y != 0)
        {
            for (
                float y = (vertices[1].position.y);
                y <= (vertices[2].position.y);
                y++
            )
            {
                float x_start = (vertices[1].position.x + (y - vertices[1].position.y) * inv_slope_1);
                float x_end = (vertices[0].position.x + (y - vertices[0].position.y) * inv_slope_2);

                if (x_start > x_end)
                    std::swap(x_start, x_end);

                for (float x = x_start; x < x_end; x++)
                {
                    draw_triangle_pixel(x, y, vertices, buffer, color);
                }
            }
        }
    }

	// Expects x and y to be in Cartesian space.
    constexpr void draw_texel(
		int x,
		int y,
		const std::array<textured_vertex, 3>& vertex,
        const std::uint32_t* const texture,
        size_t texture_width,
        size_t texture_height,
		renderer::frame_buffer& buffer
    )
    {
        if (x < 0 or y < 0
            or static_cast<std::uint32_t>(y) >= buffer.depth.height()
            or static_cast<std::uint32_t>(x) >= buffer.depth.width())
            return;

        auto weights = math::barycentric_weights(
            math::vector_2f{ .x = vertex[0].position.x, .y = vertex[0].position.y },
            math::vector_2f{ .x = vertex[1].position.x, .y = vertex[1].position.y },
            math::vector_2f{ .x = vertex[2].position.x, .y = vertex[2].position.y },
            math::vector_2f{ .x = static_cast<float>(x), .y = static_cast<float>(y) }
        );

        auto alpha = float{weights.x};
        auto beta = float{weights.y};
        auto gamma = float{weights.z};

		// Perform the interpolation of all U/w and V/w values using barycentric 
        // weights and a factor of 1/w.
		auto interpolated_u = float{
            (vertex[0].texcoords.u / vertex[0].position.w) * alpha
            + (vertex[1].texcoords.u / vertex[1].position.w)* beta
            + (vertex[2].texcoords.u / vertex[2].position.w)* gamma
        };
        auto interpolated_v = float{
            (vertex[0].texcoords.v / vertex[0].position.w) * alpha
            + (vertex[1].texcoords.v / vertex[1].position.w) * beta
            + (vertex[2].texcoords.v / vertex[2].position.w) * gamma
        } ;
		// Also interpolate the 1/w value for the current pixel.
        auto interpolated_w_reciprocal = float{
            (1.f / vertex[0].position.w) * alpha
            + (1.f / vertex[1].position.w) * beta
            + (1.f / vertex[2].position.w) * gamma
        };

		// Now we can divide back both interpolated U and V by the interpolated 1/w.
        interpolated_u /= interpolated_w_reciprocal;
        interpolated_v /= interpolated_w_reciprocal;

		// Map the UV coordinate to the full texture width and height.
        auto tex_x = math::abs(static_cast<int>(interpolated_u * texture_width)) % static_cast<int>(texture_width);
        auto tex_y = math::abs(static_cast<int>(interpolated_v * texture_height)) % static_cast<int>(texture_height);

        auto colorIndex = (texture_width * tex_y) + tex_x;
        if (colorIndex >= texture_width * texture_height)
            colorIndex = texture_width * texture_height - 1;

        // Use 1/w directly for depth testing: larger 1/w means
        // closer to the camera. Avoids the precision loss that
        // a "1 - 1/w" transformation would introduce.
        if (interpolated_w_reciprocal > buffer.depth[static_cast<uint32_t>(y), static_cast<uint32_t>(x)])
        {
            draw_pixel(static_cast<uint32_t>(y), static_cast<uint32_t>(x), texture[colorIndex], buffer);
            buffer.depth.set(static_cast<uint32_t>(y), static_cast<uint32_t>(x), interpolated_w_reciprocal);
        }
	}

    // Draw a textured triangle with flat-top/flat-bottom method.
    constexpr void draw_textured_triangle(
        const triangle& triangle,
        const std::uint32_t* const texture,
		size_t texture_width,
		size_t texture_height,
        renderer::frame_buffer& buffer
    )
    {
        // Sort by ascending y-coordinate
        auto vertices = std::array{
            textured_vertex{triangle.vertices[0], triangle.texcoords[0]},
            textured_vertex{triangle.vertices[1], triangle.texcoords[1]},
            textured_vertex{triangle.vertices[2], triangle.texcoords[2]}
        };

        std::ranges::sort(
            vertices,
            [](const auto& a, const auto& b) static noexcept
            {
                return a.position.y < b.position.y;
            });
        // Some systems have the v coordinate growing 
        // downwards, while others have it growing upwards.
        // In this case, it grows downwards, so we flip it to 
        // match the texture space.
        for (textured_vertex& vertex : vertices)
        {
			vertex.texcoords.v = 1.f - vertex.texcoords.v; 
        }

		// Render upper part of triangle
        float inv_slope_1 = 0;
        float inv_slope_2 = 0;
        if (vertices[1].position.y - vertices[0].position.y != 0)
			inv_slope_1 = static_cast<float>((vertices[1].position.x - vertices[0].position.x) / math::abs(vertices[1].position.y - vertices[0].position.y));
		if (vertices[2].position.y - vertices[0].position.y != 0)
			inv_slope_2 = static_cast<float>((vertices[2].position.x - vertices[0].position.x) / math::abs(vertices[2].position.y - vertices[0].position.y));

        if (vertices[1].position.y - vertices[0].position.y != 0)
        {
            for (
                float y = vertices[0].position.y;
                y <= vertices[1].position.y;
                y++
            )
            {
                float x_start = vertices[1].position.x + (y - vertices[1].position.y) * inv_slope_1;
                float x_end = vertices[0].position.x + (y - vertices[0].position.y) * inv_slope_2;

                if (x_start > x_end)
                    std::swap(x_start, x_end);

                for (float x = x_start; x < x_end; x++)
                {
					draw_texel(x, y, vertices, texture, texture_width, texture_height, buffer);
                }
            }
        }

		// Render bottom part of triangle
        inv_slope_1 = 0;
        inv_slope_2 = 0;
        if (vertices[2].position.y - vertices[1].position.y != 0)
            inv_slope_1 = static_cast<float>((vertices[2].position.x - vertices[1].position.x) / math::abs(vertices[2].position.y - vertices[1].position.y));
        if (vertices[2].position.y - vertices[0].position.y != 0)
            inv_slope_2 = static_cast<float>((vertices[2].position.x - vertices[0].position.x) / math::abs(vertices[2].position.y - vertices[0].position.y));

        if (vertices[2].position.y - vertices[1].position.y != 0)
        {
            for (
                float y = vertices[1].position.y;
                y <= vertices[2].position.y;
                y++
            )
            {
                float x_start = vertices[1].position.x + (y - vertices[1].position.y) * inv_slope_1;
                float x_end = vertices[0].position.x + (y - vertices[0].position.y) * inv_slope_2;

                if (x_start > x_end)
                    std::swap(x_start, x_end);

                for (float x = x_start; x < x_end; x++)
                {
                    draw_texel(x, y, vertices, texture, texture_width, texture_height, buffer);
                }
            }
        }
    }
}
