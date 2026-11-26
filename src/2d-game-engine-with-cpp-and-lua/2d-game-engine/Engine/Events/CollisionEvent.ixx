export module Engine:Events.CollisionEvent;
import :ECS;
import :EventBus.Event;

export namespace Engine
{
	class CollisionEvent : public Event
	{
	public:
		CollisionEvent(Entity entityA, Entity entityB)
			: entityA(entityA), entityB(entityB)
		{}
		Entity entityA;
		Entity entityB;
	};
}
