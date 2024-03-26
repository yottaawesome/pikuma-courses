// We don't need to explicitly link to the sdl2 libs as vcpkg takes care of this for us.
// If using subsystem:console, define this to avoid undefined symbol linker errors.
//#define SDL_MAIN_HANDLED
// See https://stackoverflow.com/a/50087608/7448661.
// With subsystem:windows, you need to use WinMain() and not regular main(), but 
// the define is not required.

import shared;
import util;
import renderer;

main_app app{};

void setup() 
{
    // Nothing to do -- setup logic moved to main_app.
}

void teardown()
{
    // currently, nothing to do here as everything is managed
    // via smart pointers
}

void process_input()
{
    sdl::SDL_Event eventInfo;
    sdl::SDL_PollEvent(&eventInfo);

    switch (eventInfo.type)
    {
        case sdl::SDL_EventType::SDL_QUIT:
        {
            app.is_running = false;
        }
        break;

        case sdl::SDL_EventType::SDL_KEYDOWN:
        {
            if (eventInfo.key.keysym.sym == sdl::SDL_KeyCode::SDLK_ESCAPE)
                app.is_running = false;
        }
        break;
    }
}

util::vector_2f project(util::vector_3f vec)
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
        fov_factor * vec.x / vec.z, 
        fov_factor * vec.y / vec.z
    };
}

renderer::triangle triangles_to_render[renderer::mesh_faces.size()];

void update(const std::chrono::milliseconds elapsed)
{
    // Delay to meet target frame rate. Note: we can 
    // do this ourselves by keeping track of the 
    // elapsed milliseconds.
    std::chrono::milliseconds time_to_wait = frame_target_time - elapsed;
    if (time_to_wait.count() > 0 and time_to_wait <= frame_target_time)
        sdl::SDL_Delay(static_cast<uint32_t>(time_to_wait.count()));
    app.previous_frame_time = std::chrono::milliseconds{ sdl::SDL_GetTicks() };
    app.elapsed += elapsed;

    app.cube_rotation.x += 0.01f;
    app.cube_rotation.y += 0.01f;
    app.cube_rotation.z += 0.01f;

    for (int i = 0; i < renderer::mesh_faces.size(); i++)
    {
        renderer::face mesh_face = renderer::mesh_faces[i];
        util::vector_3f face_vertices[3];
        face_vertices[0] = renderer::mesh_vertices[mesh_face.a - 1];
        face_vertices[1] = renderer::mesh_vertices[mesh_face.b - 1];
        face_vertices[2] = renderer::mesh_vertices[mesh_face.c - 1];

        renderer::triangle projected_triangle;

        for (int j = 0; j < 3; j++)
        {
            util::vector_3f transformed_vertex = face_vertices[j];
            transformed_vertex = rotate_x(transformed_vertex, app.cube_rotation.x);
            transformed_vertex = rotate_y(transformed_vertex, app.cube_rotation.y);
            transformed_vertex = rotate_z(transformed_vertex, app.cube_rotation.z);

            //Translate the vertex away from the camera
            transformed_vertex.z -= app.camera_position.z;

            // Project the current vertex
            util::vector_2f projected_point = project(transformed_vertex);
            
            // Scale and translate the projected points to the middle of the screen
            projected_point.x += app.screen_dimensions.width / 2;
            projected_point.y += app.screen_dimensions.height / 2;

            projected_triangle.points[j] = projected_point;
        }

        // Save the projected triangle in the array of triangles to render
        triangles_to_render[i] = projected_triangle;
    }
}

void render(
    sdl::SDL_Renderer* renderer, 
    sdl::SDL_Texture* color_buffer_texture, 
    util::color_buffer& buffer
)
{
    //sdl::SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //sdl::SDL_RenderClear(renderer);
    //draw_line_grid(10, 0xffc0c0c0, app.main_buffer);
    //draw_rect(r.origin.x, r.origin.y, r.width, r.height, 0xffc0c0c0, buffer);
    //draw_rect(50, 50, 60, 50, 0xffc0c0c0, buffer);
    //draw_pixel(0, 50, 0xffff0000, buffer);
    display::draw_dot_grid(10, 0xff464646, buffer);

    for (int i = 0; i < renderer::mesh_faces.size(); i++)
    {
        renderer::triangle triangle = triangles_to_render[i];
        for (int i = 0; auto point : triangle.points)
        {
            display::draw_rect(
                static_cast<uint32_t>(point.x),
                static_cast<uint32_t>(point.y),
                3,
                3,
                0xffffff00, 
                buffer
            );
            if (i > 0)
            {
                display::draw_line(
                    triangle.points[i - 1].x,
                    triangle.points[i - 1].y,
                    triangle.points[i].x,
                    triangle.points[i].y,
                    0xffffff00,
                    buffer
                );
            }
            else
            {
                display::draw_line(
                    triangle.points[2].x,
                    triangle.points[2].y,
                    triangle.points[i].x,
                    triangle.points[i].y,
                    0xffffff00,
                    buffer
                );
            }
            i++;
        }
    }

    display::render_color_buffer(renderer, buffer, color_buffer_texture);

    buffer.fill(0xff000000);

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

    setup();

    milliseconds elapsed{ 0 };
    while (app.is_running)
    {
        high_resolution_clock::time_point begin = high_resolution_clock::now();
        process_input();
        update(elapsed);
        render(
            app.renderer.get(), 
            app.color_buffer_texture.get(), 
            app.main_buffer
        );
        elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - begin);
    }

    teardown();

    return 0;
}
