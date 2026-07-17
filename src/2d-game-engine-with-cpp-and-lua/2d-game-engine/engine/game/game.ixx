export module engine:game;
import std;
import :sdl3;
import :raii;
import :glm;
import :log;
import :ecs;
import :eventbus;
import :components;
import :systems;
import :assetstore;
import :events;

export namespace Engine
{
	constexpr auto FPS = 60;
	constexpr auto MillisPerFrame = 1000 / FPS;
	constexpr auto WindowWidth = 800;
	constexpr auto WindowHeight = 600;
	auto MapWidth = 800;
	auto MapHeight = 600;

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
				WindowWidth,
				WindowHeight,
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

		void LoadLevel(this Game& self, int level)
		{
			self.registry
				.AddSystem<MovementSystem>(self.registry)
				.AddSystem<RenderSystem>(self.registry)
				.AddSystem<AnimationSystem>(self.registry)
				.AddSystem<CollisionSystem>(self.registry)
				.AddSystem<DamageSystem>(self.registry)
				.AddSystem<DebugRenderSystem>(self.registry)
				.AddSystem<KeyboardControlSystem>(self.registry)
				.AddSystem<CameraMovementSystem>(self.registry);

			self.assetStore.AddTexture(self.renderer.get(), "chopper-image", "./assets/images/chopper-spritesheet.png");
			self.assetStore.AddTexture(self.renderer.get(), "tank-image", "./assets/images/tank-panther-right.png");
			self.assetStore.AddTexture(self.renderer.get(), "truck-image", "./assets/images/truck-ford-right.png");
			self.assetStore.AddTexture(self.renderer.get(), "radar-image", "./assets/images/radar.png");
			// parse tileset, there are 30 tiles in 3 rows of 10 columns, each tile is 32x32 pixels, index is 0-29
			self.assetStore.AddTexture(self.renderer.get(), "tilemap-image", "./assets/tilemaps/jungle.png");
			auto tileSize = 32;
			auto tileScale = 3.0;
			auto mapNumCols = 25;
			auto mapNumRows = 20;
			auto mapFile = std::fstream{ "./assets/tilemaps/jungle.map" };
			for (int y = 0; y < mapNumRows; y++)
			{
				for (int x = 0; x < mapNumCols; x++)
				{
					auto ch = char{};
					mapFile.get(ch);
					auto srcRectY = std::atoi(&ch) * tileSize;
					mapFile.get(ch);
					auto srcRectX = std::atoi(&ch) * tileSize;
					mapFile.ignore();

					auto tile = Entity{ self.registry.CreateEntity() };
					self.registry
						.AddComponent<TransformComponent>(tile, glm::vec2{ x * (tileScale * tileSize), y * (tileScale * tileSize) }, glm::vec2{ tileScale, tileScale }, 0.0)
						.AddComponent<SpriteComponent>(tile, "tilemap-image", tileSize, tileSize, 0, srcRectX, srcRectY);
				}
			}
			MapWidth = mapNumCols * tileSize * tileScale;
			MapHeight = mapNumRows * tileSize * tileScale;

			auto chopper = Entity{ self.registry.CreateEntity() };
			constexpr auto Speed = 1500.f;
			self.registry
				.AddComponent<TransformComponent>(chopper, glm::vec2{ 10.0f, 10.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0)
				.AddComponent<RigidBodyComponent>(chopper, glm::vec2{ 100.0f, 0.0f }, 1.0f)
				// Since the initial velocity is to the right, the initial srcRect.y should be 32 * 1 (the second row of the spritesheet)
				.AddComponent<SpriteComponent>(chopper, "chopper-image", 32, 32, 1, 0, 32*1)
				.AddComponent<AnimationComponent>(chopper, 2, 15, true)
				.AddComponent<KeyboardControlledComponent>(chopper, glm::vec2{0, -Speed}, glm::vec2{Speed, 0}, glm::vec2{0, Speed}, glm::vec2{-Speed, 0})
				.AddComponent<CameraFollowComponent>(chopper);

			auto radar = Entity{ self.registry.CreateEntity() };
			self.registry
				.AddComponent<TransformComponent>(radar, glm::vec2{ WindowWidth* tileScale - 74, 10 }, glm::vec2{ 1.0f, 1.0f }, 0.0)
				.AddComponent<RigidBodyComponent>(radar, glm::vec2{ 0, 0.0f }, 1.0f)
				.AddComponent<SpriteComponent>(radar, "radar-image", 64, 64, 2)
				.AddComponent<AnimationComponent>(radar, 8, 5, true);

			auto tank = Entity{ self.registry.CreateEntity() };
			self.registry
				.AddComponent<TransformComponent>(tank, glm::vec2{ 500.0f, 10.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0)
				.AddComponent<RigidBodyComponent>(tank, glm::vec2{ -500, 0.0f }, 1.0f)
				.AddComponent<SpriteComponent>(tank, "tank-image", 32, 32, 1)
				.AddComponent<BoxColliderComponent>(tank, 32, 32)
				;

			auto truck = Entity{ self.registry.CreateEntity() };
			self.registry
				.AddComponent<TransformComponent>(truck, glm::vec2{ 10.0f, 10.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0)
				.AddComponent<RigidBodyComponent>(truck, glm::vec2{ 200.0f, 0.0f }, 1.0f)
				.AddComponent<SpriteComponent>(truck, "truck-image", 32, 32, 1)
				.AddComponent<BoxColliderComponent>(truck, 32, 32)
				;
		}

		void Setup(this Game& self)
		{
			self.LoadLevel(1);
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
						else if (sdlEvent.key.scancode == SDL::Scancode::D)
						{
							auto& debugRenderSystem = self.registry.GetSystem<DebugRenderSystem>();
							self.debugMode = debugRenderSystem.ToggleDebugRendering();
						}
						self.eventBus.EmitEvent<KeyPressedEvent>(sdlEvent.key);
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
			
			// Reset all events for the current frame
			self.eventBus.Reset();

			// Perform the subscription of the events for all systems.
			self.registry.GetSystem<DamageSystem>().SubscribeToEvents(self.eventBus);
			self.registry.GetSystem<KeyboardControlSystem>().SubscribeToEvents(self.eventBus);

			// Add or remove entities from systems after the update loop
			self.registry.Update(); 

			// Should this be done before Update()? Course doesn't do that.
			self.registry.GetSystem<MovementSystem>().Update(deltaTime);
			self.registry.GetSystem<AnimationSystem>().Update();
			self.registry.GetSystem<CollisionSystem>().Update(self.eventBus);
			self.registry.GetSystem<DamageSystem>().Update(self.eventBus);
			self.registry.GetSystem<KeyboardControlSystem>().Update(deltaTime);
			self.registry.GetSystem<CameraMovementSystem>().Update(self.camera, WindowWidth, WindowHeight, MapWidth, MapHeight);
		}

		void Render(this Game& self)
		{
			constexpr auto darkSapphire = SDL::SDL_Color{ 31, 48, 94, 255 };
			constexpr auto royalBlue = SDL::SDL_Color{ 48, 92, 222, 255 };
			constexpr auto clearColor = darkSapphire;

			SDL::SDL_SetRenderDrawColor(self.renderer.get(), clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			SDL::SDL_RenderClear(self.renderer.get());
			self.registry.GetSystem<RenderSystem>().Update(self.renderer.get(), self.assetStore, self.camera);
			self.registry.GetSystem<DebugRenderSystem>().Update(self.renderer.get());
			SDL::SDL_RenderPresent(self.renderer.get());


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
		bool debugMode = false;
		std::uint64_t millisecsPreviousFrame = 0;
		SDL::SdlScope sdlScope{ SDL::InitFlags::Video };
		SDL::WindowUniquePtr window = nullptr;
		SDL::RendererUniquePtr renderer = nullptr;
		// Course code allocates this on the heap, but there's no actual reason to do that.
		Registry registry;
		AssetStore assetStore;
		EventBus eventBus;
		SDL::SDL_Rect camera{ .x = 0, .y = 0, .w = WindowWidth, .h = WindowHeight };
	};
}