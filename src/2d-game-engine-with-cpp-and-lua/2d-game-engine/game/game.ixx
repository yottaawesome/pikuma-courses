export module engine:game;
import std;
import :sdl3;
import :raii;
import :glm;

namespace Engine
{
	auto playerPosition = glm::vec2{ 0.0f, 0.0f };
	auto playerVelocity = glm::vec2{ 0.0f, 0.0f };
}

export namespace Engine
{
	constexpr auto FPS = 30;
	constexpr auto MillisPerFrame = 1000 / FPS;

	class Game
	{
	private:
		bool isRunning = false;
		std::uint64_t millisecsPreviousFrame = 0;
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
		{
			playerPosition = glm::vec2{ 10.0f, 20.0f };
			playerVelocity = glm::vec2{ 1.0f, 0.0f };
		}

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

		void ProcessInput(this Game& self)
		{
			auto sdlEvent = SDL::SDL_Event{};
			while (SDL::SDL_PollEvent(&sdlEvent))
			{
				switch (sdlEvent.type)
				{
					case SDL::SDL_EventType::SDL_EVENT_QUIT:
					{
						self.isRunning = false;
						break;
					}
					case SDL::SDL_EventType::SDL_EVENT_KEY_DOWN:
					{
						if (sdlEvent.key.scancode == SDL::Scancode::Escape)
						{
							self.isRunning = false;
							return;
						}
						break;
					}
				}
			}
		}

		void Update(this Game& self)
		{
			// Naive time limiting and update loop.
			auto result = SDL::SDL_GetTicks() - self.millisecsPreviousFrame;
			if (result and result <= MillisPerFrame)
				SDL::SDL_Delay(static_cast<std::uint32_t>(MillisPerFrame - result));

			self.millisecsPreviousFrame = SDL::SDL_GetTicks(); // ms since SDL was initialized
			playerPosition += playerVelocity;
		}

		void Render(this Game& self)
		{
			constexpr auto darkSapphire = SDL::SDL_Color{ 31, 48, 94, 255 };
			constexpr auto royalBlue = SDL::SDL_Color{ 48, 92, 222, 255 };
			constexpr auto clearColor = darkSapphire;

			SDL::SDL_SetRenderDrawColor(self.renderer.get(), clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			SDL::SDL_RenderClear(self.renderer.get());

			auto surface = SDL::IMG_Load("./assets/images/tank-tiger-right.png");
			if (!surface)
				throw SDL::SdlError{ "Failed to load image" };
			auto texture = SDL::SDL_CreateTextureFromSurface(self.renderer.get(), surface);
			if (!texture)
				throw SDL::SdlError{ "Failed to create texture from surface" };
			SDL::SDL_DestroySurface(surface);
			auto dstRect = SDL::SDL_FRect{ playerPosition.x, playerPosition.y, 32, 32 };
			SDL::SDL_RenderTexture(self.renderer.get(), texture, nullptr, &dstRect);
			SDL::SDL_DestroyTexture(texture);

			SDL::SDL_RenderPresent(self.renderer.get());
		}

		void Destroy(this Game& self)
		{}
	private:
	};
}