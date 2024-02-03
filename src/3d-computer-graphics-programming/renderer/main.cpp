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

void update(const std::chrono::milliseconds elapsed)
{
    // convert the 3D cube points to 2D
    for (int i = 0; i < number_of_points; i++)
    {
        auto point = app.cube_points[i];
        // Move the points away from the camera
        point.z -= app.camera_position.z;
        app.projected_cube_points[i] = project(point);
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

    for (int i = 0; i < number_of_points; i++)
    {
        display::draw_rect(
            // translate to center of the screen.
            static_cast<uint32_t>(app.projected_cube_points[i].x + app.screen_dimensions.width / 2),
            static_cast<uint32_t>(app.projected_cube_points[i].y + app.screen_dimensions.height / 2),
            4,
            4,
            0xffffff00,
            buffer
        );
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
        elapsed = duration_cast<milliseconds>(begin - high_resolution_clock::now());
    }

    teardown();

    return 0;
}
