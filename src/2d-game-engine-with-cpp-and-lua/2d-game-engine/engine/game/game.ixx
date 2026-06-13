export module engine:game;
import std;
import :sdl3;
import :raii;
import :glm;
import :log;
import :ecs;
import :components;
import :systems;
import :assetstore;

namespace Engine
{
	auto playerPosition = glm::vec2{ 0.0f, 0.0f };
	auto playerVelocity = glm::vec2{ 0.0f, 0.0f };
}

export namespace Engine
{
	constexpr auto FPS = 60;
	constexpr auto MillisPerFrame = 1000 / FPS;

	class Game
	{
	public:
		~Game() = default;

		consteval auto IsFrameRateCapped() const noexcept -> bool
		{
			return true;
		}

		void Initialize(this Game& self)
		{
			Log::Info("Initializing game...");
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

			self.window = SDL::WindowUniquePtr{ wnd };
			self.renderer = SDL::RendererUniquePtr{ rnd };
			success = SDL::SDL_SetWindowPosition(self.window.get(), SDL::Windowpos::Centered, SDL::Windowpos::Centered);
			if (not success)
				throw SDL::SdlError{ "Failed to set SDL window position" };

			self.isRunning = true;
		}

		void Setup(this Game& self)
		{
			playerPosition = glm::vec2{ 10.0f, 20.0f };
			playerVelocity = glm::vec2{ 100.0f, 0.0f };

			self.registry.AddSystem<MovementSystem>(self.registry);
			self.registry.AddSystem<RenderSystem>(self.registry);

			self.assetStore.AddTexture(self.renderer.get(), "tank-image", "./assets/images/tank-panther-right.png");
			self.assetStore.AddTexture(self.renderer.get(), "truck-image", "./assets/images/truck-ford-right.png");

			auto tank = Entity{self.registry.CreateEntity()};
			self.registry.AddComponent<TransformComponent>(tank, glm::vec2{ 10.0f, 20.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0);
			self.registry.AddComponent<RigidbodyComponent>(tank, glm::vec2{ 100.0f, 0.0f }, 1.0f);
			self.registry.AddComponent<SpriteComponent>(tank, "tank-image", 32, 32);

			auto truck = Entity{ self.registry.CreateEntity() };
			self.registry.AddComponent<TransformComponent>(truck, glm::vec2{ 50.0f, 50.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0);
			self.registry.AddComponent<RigidbodyComponent>(truck, glm::vec2{ 100.0f, 0.0f }, 1.0f);
			self.registry.AddComponent<SpriteComponent>(truck, "truck-image", 32, 32);
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
			auto elapsedTicks = SDL::SDL_GetTicks() - self.millisecsPreviousFrame;
			if constexpr (self.IsFrameRateCapped())
				if (elapsedTicks and elapsedTicks <= MillisPerFrame)
					SDL::SDL_Delay(static_cast<std::uint32_t>(MillisPerFrame - elapsedTicks));
			auto deltaTime = double{ static_cast<double>(elapsedTicks) } / 1000.0;

			self.millisecsPreviousFrame = SDL::SDL_GetTicks(); // ms since SDL was initialized

			self.registry.GetSystem<MovementSystem>().Update(deltaTime);

			self.registry.Update(); // Add or remove entities from systems after the update loop
		}

		void Render(this Game& self)
		{
			constexpr auto darkSapphire = SDL::SDL_Color{ 31, 48, 94, 255 };
			constexpr auto royalBlue = SDL::SDL_Color{ 48, 92, 222, 255 };
			constexpr auto clearColor = darkSapphire;

			self.registry.GetSystem<RenderSystem>().Update(self.renderer.get(), self.assetStore);


			//SDL::SDL_SetRenderDrawColor(self.renderer.get(), clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			//SDL::SDL_RenderClear(self.renderer.get());

			/*auto surface = SDL::IMG_Load("./assets/images/tank-tiger-right.png");
			if (not surface)
				throw SDL::SdlError{ "Failed to load image" };
			auto texture = SDL::SDL_CreateTextureFromSurface(self.renderer.get(), surface);
			if (not texture)
				throw SDL::SdlError{ "Failed to create texture from surface" };
			SDL::SDL_DestroySurface(surface);
			auto dstRect = SDL::SDL_FRect{ playerPosition.x, playerPosition.y, 32, 32 };
			SDL::SDL_RenderTexture(self.renderer.get(), texture, nullptr, &dstRect);
			SDL::SDL_DestroyTexture(texture);*/

			//SDL::SDL_RenderPresent(self.renderer.get());
		}

		void Destroy(this Game& self)
		{}

	private:
		bool isRunning = false;
		std::uint64_t millisecsPreviousFrame = 0;
		SDL::SdlScope sdlScope{ SDL::InitFlags::Video };
		SDL::WindowUniquePtr window = nullptr;
		SDL::RendererUniquePtr renderer = nullptr;
		// Course code allocates this on the heap, but there's no actual reason to do that.
		Registry registry;
		AssetStore assetStore;
	};
}