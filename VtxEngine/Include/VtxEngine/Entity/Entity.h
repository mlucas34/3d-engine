#pragma once
#include <VtxEngine/Prerequisites.h>
#include <VtxEngine/Entity/Component.h>
#include <map>

namespace Vtx {
	class Entity
	{
	public:
		Entity();
		virtual ~Entity();
		void ReleaseEntity();

		World *GetWorld();
		TransformComponent *GetTransform();
		InputSystem *GetInputSystem();

		template <typename T>
		T* CreateComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "Templated class must derive from Component");
			auto c = GetComponent<T>();
			if (!c)
			{
				size_t id = typeid(T).hash_code();
				auto e = new T();
				CreateComponentInternal(e, id);
				return e;
			}

			return nullptr;
		}

		template <typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "Templated class must derive from Component");
			size_t id = typeid(T).hash_code();
			return (T*)GetComponentInternal(id);
		}

	protected:
		size_t m_id = 0;
		World *m_world = nullptr;

		virtual void OnCreate() {}
		virtual void OnUpdate(float deltaTime) {}

		void CreateComponentInternal(Component *component, size_t id);
		Component *GetComponentInternal(size_t id);
		void RemoveComponent(size_t id);


		TransformComponent *m_transform = nullptr;
		std::map<size_t, std::unique_ptr<Component>> m_components;

		friend class World;
		friend class Component;
	};
}

