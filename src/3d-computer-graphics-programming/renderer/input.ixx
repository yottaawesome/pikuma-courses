export module mainapp:input;
import std;
import renderer;
import :appstate;

namespace input
{
	constexpr float increment = 0.02f;
	constexpr float camera_increment = 3.f;

	auto HandleKeyDown(SDL_Keycode key, std::chrono::milliseconds elapsed_time) noexcept
	{
		auto state = SDL_GetKeyboardState(nullptr);
		auto applyTransform = state[SDL_Scancode::SDL_SCANCODE_LSHIFT];
		switch (key)
		{
			case SDL_KeyCode::SDLK_ESCAPE:
			{
				app_state::is_running = false;
				break;
			}
			case SDL_KeyCode::SDLK_RIGHT:
			{
				if (applyTransform)
					app_state::all_meshes.get_current_mesh().mesh.rotation.y -= increment;
				break;
			}
			case SDL_KeyCode::SDLK_LEFT:
			{
				if (applyTransform)
					app_state::all_meshes.get_current_mesh().mesh.rotation.y += increment;
				break;
			}
			case SDL_KeyCode::SDLK_UP:
			{
				if (applyTransform)
					app_state::all_meshes.get_current_mesh().mesh.rotation.x += increment;
				else
					app_state::camera.position.y += camera_increment * static_cast<float>(elapsed_time.count()) / 1000.f;
				break;
			}
			case SDL_KeyCode::SDLK_DOWN:
			{
				if (applyTransform)
					app_state::all_meshes.get_current_mesh().mesh.rotation.x -= increment;
				else
					app_state::camera.position.y -= camera_increment * static_cast<float>(elapsed_time.count()) / 1000.f;
				break;
			}
			case SDL_KeyCode::SDLK_w:
			{
				if (applyTransform)
				{
					app_state::all_meshes.get_current_mesh().mesh.translation.y -= increment;
				}
				else
				{
					app_state::camera.forward_velocity = renderer::scale(app_state::camera.direction, (5 * static_cast<float>(elapsed_time.count()) / 1000.f));
					app_state::camera.position += app_state::camera.forward_velocity;
				}
				break;
			}
			case SDL_KeyCode::SDLK_s:
			{
				if (applyTransform)
				{
					app_state::all_meshes.get_current_mesh().mesh.translation.y += increment;
				}
				else
				{
					app_state::camera.forward_velocity = renderer::scale(app_state::camera.direction, (-5 * static_cast<float>(elapsed_time.count()) / 1000.f));
					app_state::camera.position += app_state::camera.forward_velocity;
				}
				break;
			}
			case SDL_KeyCode::SDLK_a:
			{
				if (applyTransform)
					app_state::all_meshes.get_current_mesh().mesh.translation.x -= increment;
				else
					app_state::camera.yaw += 1 * static_cast<float>(elapsed_time.count()) / 1000.f;
				break;
			}
			case SDL_KeyCode::SDLK_d:
			{
				if (applyTransform)
					app_state::all_meshes.get_current_mesh().mesh.translation.x += increment;
				else
					app_state::camera.yaw -= 1 * static_cast<float>(elapsed_time.count()) / 1000.f;
				break;
			}
			case SDL_KeyCode::SDLK_e:
			{
				if (applyTransform)
					app_state::all_meshes.get_current_mesh().mesh.translation.z += increment;
				break;
			}
			case SDL_KeyCode::SDLK_f:
			{
				if (applyTransform)
					app_state::all_meshes.get_current_mesh().mesh.translation.z -= increment;
				break;
			}
			default:
				break; // do nothing for other keys
		}
	};

	auto HandleKeyUp(SDL_Keycode key, std::chrono::milliseconds elapsed_time) noexcept
	{
		switch (key)
		{
			case SDL_KeyCode::SDLK_1:
				app_state::render_settings.rendering_mode = renderer::render_mode::wireframe_with_dot;
				break;
			case SDL_KeyCode::SDLK_2:
				app_state::render_settings.rendering_mode = renderer::render_mode::wireframe;
				break;
			case SDL_KeyCode::SDLK_3:
				app_state::render_settings.rendering_mode = renderer::render_mode::filled;
				break;
			case SDL_KeyCode::SDLK_4:
				app_state::render_settings.rendering_mode = renderer::render_mode::filled_wireframe;
				break;
			case SDL_KeyCode::SDLK_5:
				app_state::render_settings.rendering_mode = renderer::render_mode::textured;
				break;
			case SDL_KeyCode::SDLK_6:
				app_state::render_settings.rendering_mode = renderer::render_mode::textured_wireframe;
				break;
			case SDL_KeyCode::SDLK_c:
				app_state::render_settings.culling_mode = renderer::cull_mode::enabled;
				break;
			case SDL_KeyCode::SDLK_x:
				app_state::render_settings.culling_mode = renderer::cull_mode::disabled;
				break;
			case SDL_KeyCode::SDLK_LEFTBRACKET:
				++app_state::all_meshes;
				break;
			case SDL_KeyCode::SDLK_RIGHTBRACKET:
				--app_state::all_meshes;
				break;
		}
	};
}

export namespace input
{
	void process_input(std::chrono::milliseconds elapsed_time)
	{
		auto eventInfo = SDL_Event{};
		SDL_PollEvent(&eventInfo);
		switch (eventInfo.type)
		{
			case SDL_EventType::SDL_QUIT:
			{
				app_state::is_running = false;
				break;
			}

			case SDL_EventType::SDL_KEYDOWN:
			{
				HandleKeyDown(eventInfo.key.keysym.sym, elapsed_time);
				break;
			}

			case SDL_EventType::SDL_KEYUP:
			{
				HandleKeyUp(eventInfo.key.keysym.sym, elapsed_time);
				break;
			}
		}
	}
}