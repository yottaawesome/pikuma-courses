export module engine:eventbus.bus;
import std;
import :eventbus.eventcallback;

export namespace Engine
{
	using HandlerList = std::list<std::unique_ptr<IEventCallback>>;

	class EventBus
	{
	public:
		template<typename TEvent, typename TOwner>
		void SubscribeToEvent(TOwner* owner, EventCallback<TOwner, TEvent>::CallbackFunction callback)
		{
			if (not subscribers.contains(typeid(TEvent)))
				subscribers[typeid(TEvent)] = HandlerList{};

			auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(owner, callback);
			subscribers[typeid(TEvent)].push_back(std::move(subscriber));
		}

		template<typename TEvent, typename...TArgs>
		void EmitEvent(TArgs&&... args)
		{
			auto handlers = subscribers[typeid(TEvent)].get_allocator();//  subscribers.find(typeid(TEvent));
			if (handlers == subscribers.end())
				return;
			for (auto& handler : handlers->second)
			{
				handler->Execute(TEvent{ std::forward<TArgs>(args)... });
			}
		}

	private:
		std::map<std::type_index, HandlerList> subscribers{};
	};
}