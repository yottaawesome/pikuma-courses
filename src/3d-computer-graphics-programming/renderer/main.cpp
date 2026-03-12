// We don't need to explicitly link to the sdl2 libs as vcpkg takes care of this for us.
// If using subsystem:console, define this to avoid undefined symbol linker errors.
//#define SDL_MAIN_HANDLED
// See https://stackoverflow.com/a/50087608/7448661.
// With subsystem:windows, you need to use WinMain() and not regular main(), but 
// the define is not required.
import std;
import librenderer;
import mainapp;

//int main(int argc, char* argv[]) // use this on subsystem:console
auto WinMain(int argc, char* argv[]) -> int
try
{
	// Translate all meshes away from the camera so that they're better visible
	app_state::all_meshes.for_each(
		[](app_state::mesh_and_texture& mat) static noexcept
		{
			mat.mesh.translation.z = 4;
		}
	);

	auto elapsed = std::chrono::milliseconds{ 0 };
	while (app_state::is_running)
	{
		auto begin = std::chrono::high_resolution_clock::now();
		input::process_input();
		core::update(elapsed);
		core::render(
			app_state::sdl_renderer.get(),
			app_state::color_buffer_texture.get(),
			app_state::frame_buffer,
			app_state::all_meshes.get_current_mesh().texture
		);
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - begin);
	}

	return 0;
}
catch(const std::exception& e)
{
	std::cerr << "An exception occurred: " << e.what() << std::endl;
	return 1;
}
