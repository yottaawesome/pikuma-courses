export module engine:ecs.registry;
import std;
import :log;
import :ecs.component;
import :ecs.system;
import :ecs.entity;
import :ecs.pool;

export namespace Engine
{
	class Registry
	{
	public:
		auto CreateEntity() -> Entity
		{
			auto entity = Entity{ numEntities++ };
			entitiesToBeAdded.insert(entity);
			auto entityId = entity.GetId();
			if (entityId >= entityComponentSignatures.size())
				entityComponentSignatures.resize(entityId + 1);
			Log::Info("Created entity with ID: {}", entity.GetId());
			return entity;
		}

		// Add or remove entities after the update loop to avoid modifying collections while iterating
		void Update()
		{
			for (auto entity : entitiesToBeAdded)
				AddEntityToSystems(entity);
			entitiesToBeAdded.clear();
		}

		template<typename TComponent, typename...TArgs>
		void AddComponent(Entity entity, TArgs&&... args)
		{
			auto componentId = Component<TComponent>::GetId();
			if (componentId >= componentPools.size())
				componentPools.resize(componentId + 1, nullptr);
			if (not componentPools[componentId])
				componentPools[componentId] = std::make_shared<Pool<TComponent>>();
				
			auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
			auto entityId = entity.GetId();
			if (entityId >= componentPool->size())
				componentPool->resize(numEntities);

			componentPool->Set(entityId, TComponent{ std::forward<TArgs>(args)... });
			entityComponentSignatures[entityId].set(componentId);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			auto componentId = Component<T>::GetId();
			auto entityId = entity.GetId();
			entityComponentSignatures[entityId].set(componentId, false); //.reset(componentId) also works;
		}

		template<typename T>
		auto HasComponent(Entity entity) -> bool
		{
			auto componentId = Component<T>::GetId();
			auto entityId = entity.GetId();
			return entityComponentSignatures[entityId].test(componentId);
		}

		template<typename T>
		auto GetComponent(Entity entity) -> T&
		{
			return {};
		}

		template<typename TSystem, typename...TArgs>
		void AddSystem(TArgs&&... args)
		{
			systems[typeid(TSystem)] = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
			// Course code uses an unnecessarily verbose way
			//systems.insert(std::make_pair(typeid(TSystem), newSystem));
		}
		
		template<typename TSystem>
		void RemoveSystem(TSystem&& system)
		{
			systems.erase(typeid(TSystem));
		}

		template<typename TSystem>
		auto HasSystem() -> bool
		{
			return systems.contains(typeid(TSystem));
		}

		template<typename TSystem>
		auto GetSystem() -> TSystem&
		{
			if (not HasSystem<TSystem>())
				throw std::runtime_error{ "System not found" };
			//return *static_cast<TSystem*>(systems.find(typeid(TSystem))->second);
			return *static_cast<TSystem*>(systems[typeid(TSystem)].get());
		}

		void AddEntityToSystems(Entity entity)
		{
			auto entitySignature = entityComponentSignatures[entity.GetId()];
			for (auto& [_, system] : systems)
			{
				auto systemSignature = system->GetSignature();
				auto isInterested = (entitySignature & systemSignature) == systemSignature;
				if (isInterested)
					system->AddEntity(entity);
			}
		}

	private:
		unsigned numEntities = 0;
		std::vector<std::shared_ptr<IPool>> componentPools{};
		std::vector<Signature> entityComponentSignatures{};
		std::unordered_map<std::type_index, std::shared_ptr<System>> systems{};

		std::set<Entity> entitiesToBeAdded{};
		std::set<Entity> entitiesToBeKilled{};
	};
}
