export module engine:eventbus.eventcallback;
import std;
import :eventbus.event;

export namespace Engine
{
	class IEventCallback
	{
	public:
		virtual ~IEventCallback() = default;
		void Execute(const Event& event)
		{
			Call(event);
		}
	private:
		virtual void Call(const Event& event) = 0;
	};

	template<typename TOwner, typename TEvent>
	class EventCallback : public IEventCallback
	{
	public:
		using CallbackFunction = auto(TOwner::*)(const TEvent&)->void;
		EventCallback(TOwner* owner, CallbackFunction callback)
			: owner(owner), callback(callback)
		{}
	private:
		TOwner* owner = nullptr;
		CallbackFunction callback = nullptr;
		void Call(const Event& event) override
		{
			std::invoke(callback, owner, static_cast<const TEvent&>(event));
		}
	};
}