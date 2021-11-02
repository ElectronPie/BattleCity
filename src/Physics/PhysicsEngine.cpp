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
                const auto newPosition = currentObject->getCurrentPosition() + currentObject->getCurrentDirection() * static_cast<float>(currentObject->getCurrentVelocity() * delta);
                const auto& colliders = currentObject->getColliders();
                std::vector<std::shared_ptr<IGameObject>> objectsToCheck = m_pCurrentLevel->getObjectsInArea(newPosition, currentObject->getSize() + newPosition);

                bool hasCollision = false;

                for (const auto& currentObjectToCheck : objectsToCheck)
                {
                    const auto& collidersToCheck = currentObjectToCheck->getColliders();
                    if (!collidersToCheck.empty())
                    {
                        if (hasIntersection(colliders, newPosition, collidersToCheck, currentObjectToCheck->getCurrentPosition()))
                        {
                            hasCollision = true;
                        }
                    }
                }

                if (!hasCollision)
                {
                    currentObject->getCurrentPosition() = newPosition;
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
            for (const auto& currentCollider2 : colliders2)
            {
                if ((currentCollider1.bottomLeft.x + position1.x <= currentCollider2.bottomLeft.x + position2.x  &&
                     currentCollider2.bottomLeft.x + position2.x <= currentCollider1.topRight.x   + position1.x  ||
                     currentCollider1.bottomLeft.x + position1.x <= currentCollider2.topRight.x   + position2.x  &&
                     currentCollider2.topRight.x   + position2.x <= currentCollider1.topRight.x   + position1.x) &&
                    (currentCollider1.bottomLeft.y + position1.y <= currentCollider2.bottomLeft.y + position2.y  &&
                     currentCollider2.bottomLeft.y + position2.y <= currentCollider1.topRight.y   + position1.y  ||
                     currentCollider1.bottomLeft.y + position1.y <= currentCollider2.topRight.y   + position2.y  &&
                     currentCollider2.topRight.y   + position2.y <= currentCollider1.topRight.y   + position1.y))
                    return true;
            }
        }

        return false;
    }
}