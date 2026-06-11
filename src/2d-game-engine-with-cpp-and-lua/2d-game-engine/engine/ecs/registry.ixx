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
		~Registry()
		{
			for (auto& pool : componentPools)
			{
				delete pool;
			}
			for (auto& [_, system] : systems)
			{
				delete system;
			}
		}

		auto CreateEntity() -> Entity
		{
			auto entity = Entity{ numEntities++ };
			entitiesToBeAdded.insert(entity);
			Log::Info("Created entity with ID: {}", entity.GetId());
			return entity;
		}

		// Add or remove entities after the update loop to avoid modifying collections while iterating
		void Update()
		{

		}

		void AddEntityToSystem(Entity entity)
		{
		}

		template<typename TComponent, typename...TArgs>
		void AddComponent(Entity entity, TArgs&&... args)
		{
			auto componentId = Component<TComponent>::GetId();

			if (componentId >= componentPools.size())
				componentPools.resize(componentId + 1, nullptr);
			if (not componentPools[componentId])
				componentPools[componentId] = new Pool<TComponent>();
				
			auto componentPool = static_cast<Pool<TComponent>*>(componentPools[componentId]);
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
			auto newSystem = new TSystem{ std::forward<TArgs>(args)... };
			systems[typeid(TSystem)] = newSystem;
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
			//return systems.find(typeid(TSystem)) != systems.end();
		}

		template<typename TSystem>
		auto GetSystem() -> TSystem&
		{
			if (not HasSystem<TSystem>())
				throw std::runtime_error{ "System not found" };
			//return *static_cast<TSystem*>(systems.find(typeid(TSystem))->second);
			return *static_cast<TSystem*>(systems[typeid(TSystem)]);
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
		std::vector<IPool*> componentPools{};
		std::vector<Signature> entityComponentSignatures{};
		std::unordered_map<std::type_index, System*> systems{};

		std::set<Entity> entitiesToBeAdded{};
		std::set<Entity> entitiesToBeKilled{};
	};
}
