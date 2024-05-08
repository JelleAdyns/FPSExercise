#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include <memory>
#include "Component.h"
#include "Subject.h"

namespace dae
{
    class PhysicsComponent;
    class CollisionComponent final : public Component
    {
    public:

        enum class CollidingSide
        {
            Top = 0b0001,
            Left = 0b0010,
            Bottom = 0b0100,
            Right = 0b1000
        };

        explicit CollisionComponent(GameObject* pOwner);
        explicit CollisionComponent(GameObject* pOwner, const glm::vec2& posOffset, const glm::vec2& size);
        virtual ~CollisionComponent();

        CollisionComponent(const CollisionComponent&) = delete;
        CollisionComponent(CollisionComponent&&) noexcept = delete;
        CollisionComponent& operator= (const CollisionComponent&) = delete;
        CollisionComponent& operator= (CollisionComponent&&) noexcept = delete;

        virtual void Update() override;
        virtual void PrepareImGuiRender() override;

        uint8_t GetCollisionFlags() const;

        const glm::vec2& GetOverlappedDistance();

        void SetOffset(const glm::vec2& newOffset);
        void SetSize(const glm::vec2& newSize);

        void CheckForCollision();

    private:
        //const glm::vec2& GetOffset();

        struct Box
        {
            float top;
            float left;
            float bottom;
            float right;
        };

        bool IsColliding(const Box& box, const Box& otherBox, CollidingSide sideToTest);

        bool m_HasPhysicsComponent;
        uint8_t m_CollisionFlags;

        glm::vec2 m_PosOffset;
        glm::vec2 m_Size;

        glm::vec2 m_GeneralOffset;
        glm::vec2 m_GeneralSize;

        glm::vec2 m_OverlappedDistance;

        static std::vector<CollisionComponent*> m_pVecAllCollisionComponents;
    };

}

#endif // !COLLISIONCOMPONENT_H