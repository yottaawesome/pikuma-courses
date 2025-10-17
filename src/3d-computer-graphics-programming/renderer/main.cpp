// We don't need to explicitly link to the sdl2 libs as vcpkg takes care of this for us.
// If using subsystem:console, define this to avoid undefined symbol linker errors.
//#define SDL_MAIN_HANDLED
// See https://stackoverflow.com/a/50087608/7448661.
// With subsystem:windows, you need to use WinMain() and not regular main(), but 
// the define is not required.

import std;
import librenderer;

void process_input()
{
    sdl::SDL_Event eventInfo;
    sdl::SDL_PollEvent(&eventInfo);

    switch (eventInfo.type)
    {
        case sdl::SDL_EventType::SDL_QUIT:
        {
            main_app::is_running = false;
            break;
        }

        case sdl::SDL_EventType::SDL_KEYDOWN:
        {
            if (eventInfo.key.keysym.sym == sdl::SDL_KeyCode::SDLK_ESCAPE)
                main_app::is_running = false;
            break;
        }

        case sdl::SDL_EventType::SDL_KEYUP:
        {
            switch (eventInfo.key.keysym.sym)
            {
            case sdl::SDL_KeyCode::SDLK_1:
                main_app::render_settings.rendering_mode = main_app::render_mode::wireframe_with_dot;
                break;
            case sdl::SDL_KeyCode::SDLK_2:
                main_app::render_settings.rendering_mode = main_app::render_mode::wireframe;
                break;
            case sdl::SDL_KeyCode::SDLK_3:
                main_app::render_settings.rendering_mode = main_app::render_mode::filled;
                break;
            case sdl::SDL_KeyCode::SDLK_4:
                main_app::render_settings.rendering_mode = main_app::render_mode::filled_wireframe;
                break;
            case sdl::SDL_KeyCode::SDLK_c:
                main_app::render_settings.culling_mode = main_app::cull_mode::enabled;
                break;
            case sdl::SDL_KeyCode::SDLK_d:
                main_app::render_settings.culling_mode = main_app::cull_mode::disabled;
                break;
            }
        }
    }
}

math::vector_2f project(math::vector_3f vec)
{
    /*
    * Perspective divide (perspective projection lecture):
    *   P'x = Px/Pz
    *   P'y = Py/Pz
    * where P'x and P'y are the projected points on the screen
    * and Px, Py, and Pz are the original 3-space components.
    * The idea is based on similar trianges, whose ratios
    * between sides must remain constant.
    *
    * Coordinate system handedness:
    *   Left-handed: Z-axis is positive away from the viewer.
    *   Right-handed: Z-axis is positive toward the viewer
    * 
    * We scale by the fov_factor and perform the perspective
    * divide.
    */
    return { 
        .x = main_app::fov_factor * vec.x / vec.z,
        .y = main_app::fov_factor * vec.y / vec.z
    };
}


void update(const std::chrono::milliseconds elapsed_time)
{
    // Delay to meet target frame rate. Note: we can 
    // do this ourselves by keeping track of the 
    // elapsed milliseconds.
    std::chrono::milliseconds time_to_wait = main_app::frame_target_time - elapsed_time;
    if (time_to_wait.count() > 0 and time_to_wait <= main_app::frame_target_time)
        sdl::SDL_Delay(static_cast<std::uint32_t>(time_to_wait.count()));
    main_app::previous_frame_time = std::chrono::milliseconds{ sdl::SDL_GetTicks() };
    main_app::elapsed += elapsed_time;

	// Change to > 0 to make the cube progressively bigger.
	main_app::cube_mesh.additively_scale_by(0);
    math::matrix4x4_f m = math::scale(
        main_app::cube_mesh.scale.x,
        main_app::cube_mesh.scale.y,
        main_app::cube_mesh.scale.z
    );

    main_app::cube_mesh.rotation.x += 0.01f;
    main_app::cube_mesh.rotation.y += 0.01f;
    main_app::cube_mesh.rotation.z += 0.01f;

    for (int i = 0; i < main_app::cube_mesh.faces.size(); i++)
    {
        math::face mesh_face = main_app::cube_mesh.faces[i];
        math::vector_3f face_vertices[3];
        face_vertices[0] = main_app::cube_mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = main_app::cube_mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = main_app::cube_mesh.vertices[mesh_face.c - 1];

        math::vector_3f transformed_vertices[3];
        for (int j = 0; j < 3; j++)
        {
            math::vector_3f transformed = face_vertices[j];

            // Use a matrix to scale our original vertex.
            math::vector_4f vec4 = transformed;
            auto scaleMatrix = math::scale(
                main_app::cube_mesh.scale.x,
                main_app::cube_mesh.scale.y,
                main_app::cube_mesh.scale.z
            );
            vec4 = scaleMatrix * vec4;
            transformed = vec4;

            transformed = transformed.rotate_x(main_app::cube_mesh.rotation.x);
            transformed = transformed.rotate_y(main_app::cube_mesh.rotation.y);
            transformed = transformed.rotate_z(main_app::cube_mesh.rotation.z);

            //Translate the vertex away from the camera
            //transformed.z -= main_app::camera_position.z;
            transformed.z += 5;
            transformed_vertices[j] = transformed;
        }

        /* Backface culling -- bypass rendering triangles that 
        * are not facing the camera.
        * Note:
        * This is a naive implementation and modern graphics APIs 
        * and 3D hardware approach back-face culling differently. 
        * For example, OpenGL does not compare the normal of the 
        * faces with the camera; instead, it does back-face culling 
        * after projection and uses the clockwise/counterclockwise 
        * order of the vertices to determine what is visible and 
        * what's not.
        * 
        * Note that backface culling is not the same as frustum
        * culling.
        */
        bool cullBackface = 
            main_app::render_settings.culling_mode == main_app::cull_mode::enabled 
            and [transformed_vertices] -> bool
                {
                    auto [vector_a, vector_b, vector_c] = transformed_vertices;
                    // B-A is A -> B
                    math::vector_3f vector_ab = vector_b - vector_a;
                    // C-A is A -> C
                    math::vector_3f vector_ac = vector_c - vector_a;
                    math::vector_3f ab_cross_ac =
                        math::cross_product(vector_ab, vector_ac);
                    ab_cross_ac.normalise();
                    // CamPosition - A is A -> CamPosition
                    math::vector_3f camera_ray = main_app::camera_position - vector_a;
                    camera_ray.normalise();
                    return math::dot_product(camera_ray, ab_cross_ac) < 0;
                }();
        if (cullBackface)
            continue;

        // Loop all three vertices
        math::triangle projected_triangle{ .color = mesh_face.color };
        for(int j = 0; j < 3; j++)
        {
            // Project the current vertex
            math::vector_2f projected_point = project(transformed_vertices[j]);
            
            // Scale and translate the projected points to the middle of the screen
            projected_point.x += main_app::window_dimensions.width() / 2;
            projected_point.y += main_app::window_dimensions.height() / 2;

            projected_triangle.points[j] = projected_point;
        }
        projected_triangle.average_depth =
            (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.f;

        // Save the projected triangle in the array of triangles to render
        main_app::triangles_to_render.push_back(projected_triangle);
    }
	// Painter's algorithm -- sort triangles by average depth from back to front.
    std::sort(
        main_app::triangles_to_render.begin(), 
        main_app::triangles_to_render.end(),
        [](const math::triangle& t1, const math::triangle& t2) static
        {
            return t1.average_depth > t2.average_depth;
		});
}

void render(
    sdl::SDL_Renderer* renderer,
    sdl::SDL_Texture* color_buffer_texture,
    math::color_buffer& buffer
)
{
    //sdl::SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //sdl::SDL_RenderClear(renderer);
    //draw_line_grid(10, 0xffc0c0c0, app.main_buffer);
    //draw_rect(r.origin.x, r.origin.y, r.width, r.height, 0xffc0c0c0, buffer);
    //draw_rect(50, 50, 60, 50, 0xffc0c0c0, buffer);
    //draw_pixel(0, 50, 0xffff0000, buffer);
    display::draw_dot_grid(10, 0xff464646, buffer);

    for (math::triangle triangle : main_app::triangles_to_render)
    {
        if (main_app::render_settings.should_draw_filled_triangles()) 
            display::draw_filled_triangle(triangle, triangle.color, buffer);

        if (main_app::render_settings.should_draw_triangles()) 
            display::draw_triangle(triangle, 0xffffffff, buffer);

        if (main_app::render_settings.should_draw_points())
        {
            [](auto&& buffer, auto&&...point) static
            {
                (display::draw_pixel(
                    static_cast<std::uint32_t>(point.y),
                    static_cast<std::uint32_t>(point.x),
                    0xffff0000,
                    buffer
                ), ...);
            }(buffer, triangle.points[0], triangle.points[1], triangle.points[2]);
        }
    }

    display::render_color_buffer(renderer, buffer, color_buffer_texture);

    buffer.fill(0xff000000);
    main_app::triangles_to_render.clear();

    sdl::SDL_RenderPresent(renderer);
}

//int main(int argc, char* argv[]) // use this on subsystem:console
int WinMain(int argc, char* argv[])
{
    using
        std::chrono::milliseconds, 
        std::chrono::high_resolution_clock, 
        std::chrono::duration_cast;

    //display::initialize_window(app);

    milliseconds elapsed{ 0 };
    while (main_app::is_running)
    {
        high_resolution_clock::time_point begin = high_resolution_clock::now();
        process_input();
        update(elapsed);
        render(
            main_app::sdl_renderer.get(),
            main_app::color_buffer_texture.get(),
            main_app::main_buffer
        );
        elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - begin);
    }

    return 0;
}
