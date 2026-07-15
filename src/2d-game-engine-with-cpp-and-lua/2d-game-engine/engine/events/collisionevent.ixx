export module engine:events.collisionevent;
import :ecs;
import :eventbus.event;

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
