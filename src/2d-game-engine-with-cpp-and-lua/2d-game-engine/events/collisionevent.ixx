export module engine:events.collisionevent;
import :ecs;
import :eventbus.event;

export namespace Engine
{
	class CollisionEvent : public Event
	{
	public:
		Entity entityA;
		Entity entityB;
	};
}
