#include "rzxpch.h"
#include "RZEntity.h"

#include "Razix/Scene/RZScene.h"
#include "Razix/Scene/RZComponents.h"

namespace Razix {

    RZEntity::RZEntity(entt::entity handle, RZScene* scene)
        : m_Entity(handle), m_Scene(scene) { }

    bool RZEntity::IsActive()
    {
        {
            if (HasComponent<ActiveComponent>())
                return m_Scene->m_Registry.get<ActiveComponent>(m_Entity).Active;

            return true;
        }
    }

    void RZEntity::SetActive(bool isActive)
    {
        GetOrAddComponent<ActiveComponent>().Active = isActive;
    }

    void RZEntity::SetParent(RZEntity entity)
    {
        {
            bool acceptable = false;
            auto hierarchyComponent = TryGetComponent<HierarchyComponent>();
            if (hierarchyComponent != nullptr) {
                acceptable = entity.m_Entity != m_Entity && (!entity.IsParent(*this)) && (hierarchyComponent->Parent != m_Entity);
            }
            else
                acceptable = entity.m_Entity != m_Entity;

            if (!acceptable)
                return;

            if (hierarchyComponent)
                HierarchyComponent::Reparent(m_Entity, entity.m_Entity, m_Scene->getRegistry(), *hierarchyComponent);
            else {
                m_Scene->getRegistry().emplace<HierarchyComponent>(m_Entity, entity.m_Entity);
            }
        }
    }

    RZEntity RZEntity::GetParent()
    {
        {
            auto hierarchyComp = TryGetComponent<HierarchyComponent>();
            if (hierarchyComp)
                return RZEntity(hierarchyComp->Parent, m_Scene);
            else
                return RZEntity(entt::null, nullptr);
        }
    }

    std::vector<RZEntity> RZEntity::GetChildren()
    {
        {
            std::vector<RZEntity> children;
            auto hierarchyComponent = TryGetComponent<HierarchyComponent>();
            if (hierarchyComponent) {
                entt::entity child = hierarchyComponent->First;
                while (child != entt::null && m_Scene->getRegistry().valid(child)) {
                    children.emplace_back(child, m_Scene);
                    hierarchyComponent = m_Scene->getRegistry().try_get<HierarchyComponent>(child);
                    if (hierarchyComponent)
                        child = hierarchyComponent->Next;
                }
            }

            return children;
        }
    }

    bool RZEntity::IsParent(RZEntity potentialParent)
    {
        {
            auto nodeHierarchyComponent = m_Scene->getRegistry().try_get<HierarchyComponent>(m_Entity);
            if (nodeHierarchyComponent) {
                auto parent = nodeHierarchyComponent->Parent;
                while (parent != entt::null) {
                    if (parent == potentialParent.m_Entity) {
                        return true;
                    }
                    else {
                        nodeHierarchyComponent = m_Scene->getRegistry().try_get<HierarchyComponent>(parent);
                        parent = nodeHierarchyComponent ? nodeHierarchyComponent->Parent : entt::null;
                    }
                }
            }

            return false;
        }
    }

}