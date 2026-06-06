export module engine:game;
import std;
import :sdl3;
import :raii;

export namespace Engine
{
	

	class Game
	{
	private:
		bool isRunning = false;
		WindowUniquePtr window = nullptr;
		RendererUniquePtr renderer = nullptr;

	public:
		~Game()
		{
			renderer.reset();
			window.reset();
			SDL::SDL_Quit();
		}

		void Initialize(this Game& self)
		{
			if (not SDL::SDL_Init(SDL::InitFlags::Video))
				throw SDL::SdlError{"Failed to initialize SDL"};

			auto wnd = static_cast<SDL::SDL_Window*>(nullptr);
			auto rnd = static_cast<SDL::SDL_Renderer*>(nullptr);
			auto success = SDL::SDL_CreateWindowAndRenderer(
				nullptr,
				800,
				600,
				SDL::Window::Borderless | SDL::Window::Fullscreen,
				&wnd,
				&rnd
			);
			if (not success)
				throw SDL::SdlError{ "Failed to create SDL window and renderer" };

			self.window = WindowUniquePtr{ wnd };
			self.renderer = RendererUniquePtr{ rnd };
			success = SDL::SDL_SetWindowPosition(self.window.get(), SDL::Windowpos::Centered, SDL::Windowpos::Centered);
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

			SDL::SDL_SetRenderDrawColor(renderer.get(), clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			SDL::SDL_RenderClear(renderer.get());

			auto surface = SDL::IMG_Load("./assets/images/tank-tiger-right.png");
			if (!surface)
				throw SDL::SdlError{ "Failed to load image" };
			auto texture = SDL::SDL_CreateTextureFromSurface(renderer.get(), surface);
			if (!texture)
				throw SDL::SdlError{ "Failed to create texture from surface" };
			SDL::SDL_DestroySurface(surface);
			auto dstRect = SDL::SDL_FRect{ 10, 10, 32, 32 };
			SDL::SDL_RenderTexture(renderer.get(), texture, nullptr, &dstRect);
			SDL::SDL_DestroyTexture(texture);

			SDL::SDL_RenderPresent(renderer.get());
		}

		void Destroy()
		{}
	private:
	};
}