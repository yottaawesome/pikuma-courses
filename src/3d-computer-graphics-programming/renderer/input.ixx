export module mainapp:input;
import std;
import librenderer;
import :appstate;

namespace input
{
	constexpr float increment = 0.02f;

	auto HandleKeyDown(sdl::SDL_Keycode key) noexcept
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

	auto HandleKeyUp(sdl::SDL_Keycode key) noexcept
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
}

export namespace input
{
	void process_input()
	{
		auto eventInfo = sdl::SDL_Event{};
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
}