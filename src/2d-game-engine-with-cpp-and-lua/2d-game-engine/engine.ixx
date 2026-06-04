export module engine;

export namespace Engine
{
	class Game
	{
	public:
		void Initialize()
		{}
		void Run()
		{
			while (true)
			{
				ProcessInput();
				Update();
				Render();
			}
		}
		void ProcessInput()
		{
		}
		void Update()
		{}
		void Render()
		{

		}

		void Destroy()
		{}
	private:
	};
}