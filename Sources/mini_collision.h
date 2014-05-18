#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace mini
{
        class OrientedBoundingRectangle;

        class BoundingCircle
        {
        public:
                /**********************************************************************
                Creates a bounding circle with given center and radius.
                **********************************************************************/
                BoundingCircle(glm::vec2 center, float radius)
                        : m_radius(radius), m_center(center) { }
                BoundingCircle(float cx, float cy, float radius)
                        : m_radius(radius), m_center(cx, cy) { }
                BoundingCircle(const BoundingCircle& right)
                        : m_radius(right.m_radius), m_center(right.m_center) { }

                inline float getRadius() const { return m_radius; }
                inline void setRadius(float radius) { m_radius = radius; }

                inline glm::vec2 getCenter() const { return m_center; }
                inline void setCenter(glm::vec2 center) { m_center = center; }
                inline void setCenter(float cx, float cy)
                { m_center = glm::vec2(cx, cy); }

        private:
                glm::vec2 m_center;
                float m_radius;
        };

        class OrientedBoundingRectangle //oriented boudning rectangle
        {
        public:
                /**********************************************************************
                 Creates an oriented bounding rectangle  with given lower left corner,
                 width, height, and a rotation angle in radians.
                **********************************************************************/
                OrientedBoundingRectangle(glm::vec2 corner = glm::vec2(), float width = 0,
                        float height = 0, float rotation = 0);
                OrientedBoundingRectangle(const OrientedBoundingRectangle& right);

                /**********************************************************************
                 Tests for  collision between this  bounding rectangle and  a bounding
                 circle. If  a collision is found  returns a translation  vector which
                 should be applied to the bounding circle to resolve the collision.
                **********************************************************************/
                glm::vec2 Collision(const BoundingCircle& c);
                float Distance(const BoundingCircle& c);
                glm::vec2 getP1() { return m_corner; }
                glm::vec2 getP2()
                { return glm::vec2(m_corner.x + m_dx.x, m_corner.y + m_dx.y); }
                glm::vec2 getP3()
                { return glm::vec2(m_corner.x + m_dx.x + m_dy.x, m_corner.y + m_dx.y + m_dy.y); }
                glm::vec2 getP4()
                { return glm::vec2(m_corner.x + m_dy.x, m_corner.y + m_dy.y); }

        private:
                glm::vec2 m_corner;
                glm::vec2 m_dx;
                glm::vec2 m_dy;
                bool _Collision(glm::vec2 center, glm::vec2& trDir, float& minDist);
        };

        class CollisionEngine
        {
        public:
                CollisionEngine(glm::vec2 characterPos = glm::vec2(0, 0), float characterRadius = 0.3);
                glm::vec2 SetObstacles(std::vector<mini::OrientedBoundingRectangle>&& obstacles);
                /**********************************************************************
                 Changes one of  the obstacle s and checks  for collistion.  If one is
                 found returns  a translation  vector which  should be applied  to the
                 character to resolve the collision.
                **********************************************************************/
                glm::vec2 MoveObstacle(unsigned int obstacleIndex, mini::OrientedBoundingRectangle obstacle);
                /**********************************************************************
                 Applies translation  vector to the character  position and checks for
                 collisions.  If one is found,  the parameter is  changed to represent
                 the acctual  translation that  should be applied  to the character to
                 avoid collisions, and returns the new position of the character.
                **********************************************************************/
                glm::vec2 MoveCharacter(glm::vec2& v);

                mini::OrientedBoundingRectangle getObstacle(unsigned int obstacleIndex);
                float DistanceToObstacle(unsigned int obstacleIdx);

        private:
                glm::vec2 ResolveCollisions();
                std::vector<mini::OrientedBoundingRectangle> m_obstacles;
                mini::BoundingCircle m_character;
        };
}
