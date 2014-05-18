#include "mini_collision.h"
#include <glm/ext.hpp>
#include <cfloat>


using namespace std;
using namespace mini;
using namespace glm;

OrientedBoundingRectangle::OrientedBoundingRectangle(glm::vec2 corner,
                                                     float width, float height, float rotation)
{
    glm::vec2 c = corner;
    glm::mat4 m =
            glm::scale(mat4(), vec3(width, height, 0))*
            glm::rotate(mat4(), degrees(rotation), vec3(0,0,1));

    glm::vec2 dx(1, 0);
    glm::vec2 dy(0, 1);
    dx = vec2(m*vec4(dx, 0, 0));
    dx = vec2(m*vec4(dy, 0, 0));
    if (width < 0 && height > 0)
    {
        c += dx;
        dx = -dx;
    }
    else if (width > 0 && height < 0)
    {
        c += dy;
        dy = -dy;
    }
    m_dx = vec2(dx);
    m_dy = vec2(dy);
    m_corner = c;
}

OrientedBoundingRectangle::OrientedBoundingRectangle(const OrientedBoundingRectangle& right)
    : m_corner(right.m_corner), m_dx(right.m_dx), m_dy(right.m_dy)
{ }

float OrientedBoundingRectangle::Distance(const BoundingCircle& circle)
{
    glm::vec2 center = circle.getCenter();
    glm::vec2 translate;
    float min_dist;
    bool inside = _Collision(center, translate, min_dist);
    return inside ? 0.0f : min_dist;
}

bool OrientedBoundingRectangle::_Collision(glm::vec2 center, glm::vec2& translate, float& min_dist)
{
    glm::vec2 p[4];
    glm::vec2 v[4];
    float len[4];
    p[0] = m_corner;
    v[0] = m_dx;
    v[1] = m_dy;
    len[0] = len[2] = glm::length(v[0]);
    len[1] = len[3] = glm::length(v[1]);
    v[0] = glm::normalize(v[0]);
    v[1] = glm::normalize(v[1]);
    v[2] = -v[0];
    v[3] = -v[1];
    for (int i = 1; i < 4; ++i)
        p[i] = p[i - 1] + len[i - 1] * v[i - 1];

    bool inside = true;
    min_dist = FLT_MAX;
    translate = vec2();
    for (int i = 0; i < 4; ++i)
    {
        glm::vec2 q = center - p[i];
        float t = glm::dot(q, v[i]);
        float clampt = clamp(t, 0.0f, len[i]);
        if (t != clampt)
            inside = false;
        q = p[i] + clampt*v[i];
        glm::vec2 trDir = center - q;
        float dist = glm::length(trDir);
        if (dist < min_dist)
        {
            min_dist = dist;
            translate = dist == 0 ? vec2(-v[i].y,v[i].x)
                                  : glm::normalize(trDir);
        }
    }
    return inside;
}

glm::vec2 OrientedBoundingRectangle::Collision(const BoundingCircle& circle)
{
    glm::vec2 cntr = circle.getCenter();
    glm::vec2 center = cntr;
    glm::vec2 translate;
    float min_dist;
    bool inside = _Collision(center, translate, min_dist);
    float radius = circle.getRadius();
    if (inside)
        translate = -translate*(min_dist + radius);
    else if (min_dist < radius)
        translate *= radius - min_dist;
    else translate = vec2();
    return translate;
}

CollisionEngine::CollisionEngine(glm::vec2 characterPosition, float characterRadius)
    : m_character(characterPosition, characterRadius)
{ }

glm::vec2 CollisionEngine::SetObstacles(vector<OrientedBoundingRectangle>&& obstacles)
{
    m_obstacles = move(obstacles);
    return ResolveCollisions();
}

glm::vec2 CollisionEngine::MoveObstacle(unsigned int obstacleIndex, OrientedBoundingRectangle obstacle)
{
    if (obstacleIndex >= m_obstacles.size())
        return glm::vec2();
    m_obstacles[obstacleIndex] = obstacle;
    return ResolveCollisions();
}

glm::vec2 CollisionEngine::MoveCharacter(glm::vec2& v)
{
    glm::vec2 oldPos = m_character.getCenter();
    glm::vec2 newPos = oldPos + v;
    m_character.setCenter(newPos);
    ResolveCollisions();
    newPos = m_character.getCenter();
    v = newPos - oldPos;
    return newPos;
}

glm::vec2 CollisionEngine::ResolveCollisions()
{
    glm::vec2 oldPos = m_character.getCenter();
    glm::vec2 newPos = oldPos;
    m_character.setCenter(newPos);
    for ( OrientedBoundingRectangle& rect : m_obstacles )
    {
        glm::vec2 tr = rect.Collision(m_character);
        newPos += tr;
        m_character.setCenter(newPos);
    }
    glm::vec2 v;
    v = newPos - oldPos;
    return v;
}

OrientedBoundingRectangle CollisionEngine::getObstacle(unsigned int obstacleIndex)
{
    if (obstacleIndex >= m_obstacles.size())
        return OrientedBoundingRectangle();
    return m_obstacles[obstacleIndex];
}

float CollisionEngine::DistanceToObstacle(unsigned int obstacleIndex)
{
    if (obstacleIndex >= m_obstacles.size())
        return FLT_MAX;
    return m_obstacles[obstacleIndex].Distance(m_character);
}
