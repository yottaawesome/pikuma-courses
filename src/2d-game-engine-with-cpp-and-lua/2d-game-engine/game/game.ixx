export module engine:game;
import std;
import :sdl3;

export namespace Engine
{
	class Game
	{
	private:
		bool isRunning = false;
		SDL::SDL_Window* window = nullptr;
		SDL::SDL_Renderer* renderer = nullptr;

	public:
		~Game()
		{
			if (renderer)
			{
				SDL::SDL_DestroyRenderer(renderer);
				SDL::SDL_DestroyWindow(window);
				SDL::SDL_Quit();
			}
		}

		void Initialize(this Game& self)
		{
			if (not SDL::SDL_Init(SDL::InitFlags::Video))
				throw SDL::SdlError{"Failed to initialize SDL"};

			auto success = SDL::SDL_CreateWindowAndRenderer(
				nullptr,
				800,
				600,
				SDL::Window::Borderless | SDL::Window::Fullscreen,
				&self.window,
				&self.renderer
			);
			if (not success)
				throw SDL::SdlError{ "Failed to create SDL window and renderer" };
			success = SDL::SDL_SetWindowPosition(self.window, SDL::Windowpos::Centered, SDL::Windowpos::Centered);
			if (not success)
				throw SDL::SdlError{ "Failed to set SDL window position" };
			self.isRunning = true;
		}

		void Setup(this Game& self)
		{}

		void Run(this Game& self)
		{
			self.Setup();
			while (self.isRunning)
			{
				self.ProcessInput();
				self.Update();
				self.Render();
			}
		}

		void ProcessInput()
		{
			auto sdlEvent = SDL::SDL_Event{};
			while (SDL::SDL_PollEvent(&sdlEvent))
			{
				switch (sdlEvent.type)
				{
					case SDL::SDL_EventType::SDL_EVENT_QUIT:
					{
						isRunning = false;
						break;
					}
					case SDL::SDL_EventType::SDL_EVENT_KEY_DOWN:
					{
						if (sdlEvent.key.scancode == SDL::Scancode::Escape)
						{
							isRunning = false;
							return;
						}
						break;
					}
				}
			}
		}

		void Update()
		{}

		void Render()
		{
			constexpr auto darkSapphire = SDL::SDL_Color{ 31, 48, 94, 255 };
			constexpr auto royalBlue = SDL::SDL_Color{ 48, 92, 222, 255 };
			constexpr auto clearColor = darkSapphire;

			SDL::SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			SDL::SDL_RenderClear(renderer);

			auto player = SDL::SDL_FRect{ 10, 10, 50, 50 };
			SDL::SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL::SDL_RenderFillRect(renderer, &player);

			SDL::SDL_RenderPresent(renderer);
		}

		void Destroy()
		{}
	private:
	};
}