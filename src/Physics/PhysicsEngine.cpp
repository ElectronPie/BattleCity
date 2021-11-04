#include "PhysicsEngine.h"

#include "../Game/GameObjects/IGameObject.h"
#include "../Game/Level.h"

namespace Physics
{
    std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::m_dynamicObjects;
    std::shared_ptr<Level> PhysicsEngine::m_pCurrentLevel;

    void PhysicsEngine::init()
    {}

    void PhysicsEngine::terminate()
    {
        m_dynamicObjects.clear();
        m_pCurrentLevel.reset();
    }

    void PhysicsEngine::setCurrentLevel(std::shared_ptr<Level> pLevel)
    {
        m_pCurrentLevel.swap(pLevel);
    }

    void PhysicsEngine::update(const double delta)
    {
        for (auto& currentObject : m_dynamicObjects)
        {
            if (currentObject->getCurrentVelocity() > 0)
            {
                if (currentObject->getCurrentDirection().x != 0.f)
                {
                    currentObject->getCurrentPosition().y = static_cast<unsigned int>(currentObject->getCurrentPosition().y / 4.f + 0.5f) * 4.f;
                }
                if (currentObject->getCurrentDirection().y != 0.f)
                {
                    currentObject->getCurrentPosition().x = static_cast<unsigned int>(currentObject->getCurrentPosition().x / 4.f + 0.5f) * 4.f;
                }
                const auto newPosition = currentObject->getCurrentPosition() + currentObject->getCurrentDirection() * static_cast<float>(currentObject->getCurrentVelocity() * delta);
                const auto& colliders = currentObject->getColliders();
                std::vector<std::shared_ptr<IGameObject>> objectsToCheck = m_pCurrentLevel->getObjectsInArea(newPosition, currentObject->getSize() + newPosition);

                bool hasCollision = false;

                for (const auto& currentObjectToCheck : objectsToCheck)
                {
                    const auto& collidersToCheck = currentObjectToCheck->getColliders();
                    if (currentObjectToCheck->collides(currentObject->getObjectType()) && !collidersToCheck.empty())
                    {
                        if (hasIntersection(colliders, newPosition, collidersToCheck, currentObjectToCheck->getCurrentPosition()))
                        {
                            hasCollision = true;
                            currentObjectToCheck->onCollision();
                        }
                    }
                }

                if (!hasCollision)
                {
                    currentObject->getCurrentPosition() = newPosition;
                }
                else
                {
                    if (currentObject->getCurrentDirection().x != 0.f)
                    {
                        currentObject->getCurrentPosition().y = static_cast<unsigned int>(currentObject->getCurrentPosition().y / 8.f + 0.5f) * 8.f;
                    }
                    if (currentObject->getCurrentDirection().y != 0.f)
                    {
                        currentObject->getCurrentPosition().x = static_cast<unsigned int>(currentObject->getCurrentPosition().x / 8.f + 0.5f) * 8.f;
                    }
                    currentObject->onCollision();
                }
            }
        }
    }

    void PhysicsEngine::addDynamicObject(std::shared_ptr<IGameObject> pGameObject)
    {
        m_dynamicObjects.insert(std::move(pGameObject));
    }

    bool PhysicsEngine::hasIntersection(const std::vector<AABB>& colliders1, const glm::vec2& position1,
                                        const std::vector<AABB>& colliders2, const glm::vec2& position2)
    {
        for (const auto& currentCollider1 : colliders1)
        {
            const auto worldCollider1 = AABB(currentCollider1.bottomLeft + position1, currentCollider1.topRight + position1);
            for (const auto& currentCollider2 : colliders2)
            {
                const auto worldCollider2 = AABB(currentCollider2.bottomLeft + position2, currentCollider2.topRight + position2);

                if (worldCollider1.bottomLeft.x >= worldCollider2.topRight.x)
                {
                    return false;
                }
                if (worldCollider1.topRight.x <= worldCollider2.bottomLeft.x)
                {
                    return false;
                }
                if (worldCollider1.bottomLeft.y >= worldCollider2.topRight.y)
                {
                    return false;
                }
                if (worldCollider1.topRight.y <= worldCollider2.bottomLeft.y)
                {
                    return false;
                }

                return true;
            }
        }
        return false;
    }
}