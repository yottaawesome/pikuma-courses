// We don't need to explicitly link to the sdl2 libs as vcpkg takes care of this for us.
// If using subsystem:console, define this to avoid undefined symbol linker errors.
//#define SDL_MAIN_HANDLED
// See https://stackoverflow.com/a/50087608/7448661.
// With subsystem:windows, you need to use WinMain() and not regular main(), but 
// the define is not required.

import std;
import librenderer;
import mainapp;

void process_input()
{
	constexpr float increment = 0.02f;

	constexpr auto HandleKeyDown = 
		[](sdl::SDL_Keycode key) static noexcept
		{
			switch (key)
			{
			case sdl::SDL_KeyCode::SDLK_ESCAPE:
				app_state::is_running = false;
				break;
			case sdl::SDL_KeyCode::SDLK_RIGHT:
				app_state::all_meshes.get_current_mesh().mesh.rotation.y -= increment;
				break;
			case sdl::SDL_KeyCode::SDLK_LEFT:
				app_state::all_meshes.get_current_mesh().mesh.rotation.y += increment;
				break;
			case sdl::SDL_KeyCode::SDLK_UP:
				app_state::all_meshes.get_current_mesh().mesh.rotation.x += increment;
				break;
			case sdl::SDL_KeyCode::SDLK_DOWN:
				app_state::all_meshes.get_current_mesh().mesh.rotation.x -= increment;
				break;
			case sdl::SDL_KeyCode::SDLK_w:
				app_state::all_meshes.get_current_mesh().mesh.translation.y -= increment;
				break;
			case sdl::SDL_KeyCode::SDLK_s:
				app_state::all_meshes.get_current_mesh().mesh.translation.y += increment;
				break;
			case sdl::SDL_KeyCode::SDLK_a:
				app_state::all_meshes.get_current_mesh().mesh.translation.x -= increment;
				break;
			case sdl::SDL_KeyCode::SDLK_d:
				app_state::all_meshes.get_current_mesh().mesh.translation.x += increment;
				break;
			case sdl::SDL_KeyCode::SDLK_e:
				app_state::all_meshes.get_current_mesh().mesh.translation.z += increment;
				break;
			case sdl::SDL_KeyCode::SDLK_f:
				app_state::all_meshes.get_current_mesh().mesh.translation.z -= increment;
				break;
			default:
				break; // do nothing for other keys
			}
		};

	constexpr auto HandleKeyUp = 
		[](sdl::SDL_Keycode key) static noexcept
		{
			switch (key)
			{
			case sdl::SDL_KeyCode::SDLK_1:
				app_state::render_settings.rendering_mode = renderer::render_mode::wireframe_with_dot;
				break;
			case sdl::SDL_KeyCode::SDLK_2:
				app_state::render_settings.rendering_mode = renderer::render_mode::wireframe;
				break;
			case sdl::SDL_KeyCode::SDLK_3:
				app_state::render_settings.rendering_mode = renderer::render_mode::filled;
				break;
			case sdl::SDL_KeyCode::SDLK_4:
				app_state::render_settings.rendering_mode = renderer::render_mode::filled_wireframe;
				break;
			case sdl::SDL_KeyCode::SDLK_5:
				app_state::render_settings.rendering_mode = renderer::render_mode::textured;
				break;
			case sdl::SDL_KeyCode::SDLK_6:
				app_state::render_settings.rendering_mode = renderer::render_mode::textured_wireframe;
				break;
			case sdl::SDL_KeyCode::SDLK_c:
				app_state::render_settings.culling_mode = renderer::cull_mode::enabled;
				break;
			case sdl::SDL_KeyCode::SDLK_d:
				app_state::render_settings.culling_mode = renderer::cull_mode::disabled;
				break;
			case sdl::SDL_KeyCode::SDLK_LEFTBRACKET:
				++app_state::all_meshes;
				break;
			case sdl::SDL_KeyCode::SDLK_RIGHTBRACKET:
				--app_state::all_meshes;
				break;
			}
		};

	sdl::SDL_Event eventInfo;
	sdl::SDL_PollEvent(&eventInfo);
	switch (eventInfo.type)
	{
		case sdl::SDL_EventType::SDL_QUIT:
		{
			app_state::is_running = false;
			break;
		}

		case sdl::SDL_EventType::SDL_KEYDOWN:
		{
			HandleKeyDown(eventInfo.key.keysym.sym);
			break;
		}

		case sdl::SDL_EventType::SDL_KEYUP:
		{
			HandleKeyUp(eventInfo.key.keysym.sym);
			break;
		}
	}
}

void update(const std::chrono::milliseconds elapsed_time)
{
	// Delay to meet target frame rate. Note: we can 
	// do this ourselves by keeping track of the 
	// elapsed milliseconds.
	auto time_to_wait = std::chrono::milliseconds{ app_state::frame_target_time - elapsed_time };
	if (time_to_wait.count() > 0 and time_to_wait <= app_state::frame_target_time)
		sdl::SDL_Delay(static_cast<std::uint32_t>(time_to_wait.count()));
	app_state::previous_frame_time = std::chrono::milliseconds{ sdl::SDL_GetTicks() };
	app_state::elapsed += elapsed_time;

	// Change to > 0 to make the cube progressively bigger.
	app_state::all_meshes.get_current_mesh().mesh.additively_scale_by(0);
	auto scaleMatrix = math::scale_matrix{ app_state::all_meshes.get_current_mesh().mesh.scale };
	//app_state::all_meshes.get_current_mesh().mesh.translation.z = 4;
	auto translation = math::translate_matrix{ app_state::all_meshes.get_current_mesh().mesh.translation };

	//app_state::all_meshes.get_current_mesh().mesh.rotation.x += 0.01f;
	//app_state::all_meshes.get_current_mesh().mesh.rotation.y += 0.01f;
	//app_state::all_meshes.get_current_mesh().mesh.rotation.z += 0.01f;
	auto rotationMatrix = math::rotation_matrix{ app_state::all_meshes.get_current_mesh().mesh.rotation };

	constexpr auto global_light = renderer::light{ { .x = 0, .y = 0, .z = 1 }, 0xffffffff };

	for (int i = 0; i < app_state::all_meshes.get_current_mesh().mesh.faces.size(); i++)
	{
		auto mesh_face = renderer::face{ app_state::all_meshes.get_current_mesh().mesh.faces[i] };
		auto face_vertices = std::array{
			app_state::all_meshes.get_current_mesh().mesh.vertices[mesh_face.a],
			app_state::all_meshes.get_current_mesh().mesh.vertices[mesh_face.b],
			app_state::all_meshes.get_current_mesh().mesh.vertices[mesh_face.c]
		};

		auto transformed_vertices = std::array<math::vector_4f, 3>{};
		for (int j = 0; j < 3; j++)
		{
			// These need to be applied in the correct order: 
			// scale, rotate, translate.
			// Scale our original vertex, then rotate, then 
			// the vertex away from the camera. The matrix 
			// translate*rotate*scale is called the world 
			// matrix and is responsible for placing the
			// mesh in its correct position in the 3D world.
			transformed_vertices[j] = translation 
				* rotationMatrix 
				* scaleMatrix 
				* face_vertices[j];
		}

		auto transformed_triangle = renderer::triangle{
			.vertices {
				transformed_vertices[0],
				transformed_vertices[1],
				transformed_vertices[2]
			}
		};
		auto normal = math::vector_4f{ transformed_triangle.compute_normal() };

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
		if (app_state::render_settings.culling_mode == renderer::cull_mode::enabled)
		{
			const auto& [vector_a, vector_b, _] = transformed_vertices;
			auto camera_ray = math::vector_4f{ app_state::camera_position - vector_a };
			if (math::dot_product(camera_ray, normal) <= 0) // cull the face
				continue;
		}

		// Loop all three vertices
		auto projected_triangle = renderer::triangle{
			.texcoords = { mesh_face.a_uv, mesh_face.b_uv, mesh_face.c_uv },
			.color =
				//global_light.compute_intensity_from_normal(mesh_face.color, normal)
				global_light.compute_intensity_from_normal(normal),
		};

		for (int j = 0; j < 3; j++)
		{
			// Project the current vertex
			auto projected_point = math::vector_4f{ app_state::proj_matrix * transformed_vertices[j] };

			// Scale into the view.
			projected_point.x *= app_state::window_dimensions.width() / 2;
			projected_point.y *= app_state::window_dimensions.height() / 2;

			// Invert the y coordinate to account for flipped y-axis in screen space.
			projected_point.y *= -1;

			// Translate the projected points to the middle of the screen.
			projected_point.x += app_state::window_dimensions.width() / 2;
			projected_point.y += app_state::window_dimensions.height() / 2;
			projected_triangle.vertices[j] = projected_point;
		}

		// Save the projected triangle in the array of triangles to render
		app_state::triangles_to_render.push_back(projected_triangle);
	}
}

void render(
	sdl::SDL_Renderer* renderer,
	sdl::SDL_Texture* color_buffer_texture,
	renderer::frame_buffer& frame_buffer,
	upng::upng_texture& texture
)
{

	//sdl::SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//sdl::SDL_RenderClear(renderer);
	//draw_line_grid(10, 0xffc0c0c0, app.main_buffer);
	//draw_rect(r.origin.x, r.origin.y, r.width, r.height, 0xffc0c0c0, buffer);
	//draw_rect(50, 50, 60, 50, 0xffc0c0c0, buffer);
	//draw_pixel(0, 50, 0xffff0000, buffer);
	renderer::draw_dot_grid(10, 0xff464646, frame_buffer);

	for (const renderer::triangle& triangle : app_state::triangles_to_render)
	{
		if (app_state::render_settings.should_draw_filled_triangles())
		{
			renderer::draw_filled_triangle(
				triangle,
				triangle.color, 
				frame_buffer
			);
		}

		if (app_state::render_settings.should_draw_textured_triangles())
		{
			//const auto [texture, width, height] = renderer::texture::get_selected();
			renderer::draw_textured_triangle(triangle, texture.uint32_buffer(), texture.width(), texture.height(), frame_buffer);
		}

		if (app_state::render_settings.should_draw_triangles())
			renderer::draw_triangle(triangle, 0xffffffff, frame_buffer);
		if (app_state::render_settings.should_draw_points())
		{
			[](auto&& frame_buffer, auto&&...point) static
			{
				(renderer::draw_pixel(
					static_cast<std::uint32_t>(point.y),
					static_cast<std::uint32_t>(point.x),
					0xffff0000,
					frame_buffer
				), ...);
			}(frame_buffer, triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]);
		}
	}

	renderer::render_color_buffer(renderer, frame_buffer.color, color_buffer_texture);
	frame_buffer.clear_color_buffer(0xff000000).clear_z_buffer();
	app_state::triangles_to_render.clear();

	sdl::SDL_RenderPresent(renderer);
}

//int main(int argc, char* argv[]) // use this on subsystem:console
auto WinMain(int argc, char* argv[]) -> int // use this on subsystem:windows
{
	using
		std::chrono::milliseconds, 
		std::chrono::high_resolution_clock, 
		std::chrono::duration_cast;

	// Translate all meshes away from the camera so that they're better visible
	app_state::all_meshes.for_each(
		[](app_state::mesh_and_texture& mat) static noexcept
		{
			mat.mesh.translation.z = 4;
		}
	);

	//display::initialize_window(app);

	auto elapsed = milliseconds{ 0 };
	while (app_state::is_running)
	{
		auto begin = high_resolution_clock::now();
		process_input();
		update(elapsed);
		render(
			app_state::sdl_renderer.get(),
			app_state::color_buffer_texture.get(),
			app_state::frame_buffer,
			app_state::all_meshes.get_current_mesh().texture
		);
		elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - begin);
	}

	return 0;
}
