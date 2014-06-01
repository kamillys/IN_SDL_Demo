#include "mini_camera.h"
#include <glm/ext.hpp>

using namespace mini;
using namespace glm;

Camera::Camera(glm::vec3 position)
    : m_position(position.x, position.y, position.z, 1.0f), m_angleX(0), m_angleY(0)
{
}

void Camera::GetViewMatrix(glm::mat4& viewMtx)
{
    viewMtx =
            glm::rotate(mat4(), degrees(-m_angleX), glm::vec3(1,0,0))
            * glm::rotate(mat4(), degrees(-m_angleY), glm::vec3(0,1,0))
            * glm::translate(mat4(), -vec3(m_position))
            ;
}

glm::mat4 Camera::GetViewMatrix()
{
    glm::mat4 viewMtx;
    GetViewMatrix(viewMtx);
    return viewMtx;
}

glm::vec4 Camera::getForwardDir()
{
    return rotateY(vec4(0, 0, 1, 0), degrees(m_angleY));
}

glm::vec4 Camera::getRightDir()
{
    return rotateY(vec4(1, 0, 0, 0), degrees(m_angleY));
}

void Camera::Move(glm::vec3 v)
{
    m_position += glm::vec4(v, 0);
}

void Camera::Rotate(float dx, float dy)
{
    m_angleX = m_angleX + dx;
    m_angleY = m_angleY + dy;
}
