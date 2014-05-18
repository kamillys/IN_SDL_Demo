#pragma once

#include <glm/glm.hpp>


namespace mini
{
        class Camera
        {
        public:
                Camera(glm::vec3 position);

                glm::mat4 GetViewMatrix();
                void GetViewMatrix(glm::mat4& m);

                glm::vec4 getPosition()
                { return m_position; }
                glm::vec4 getForwardDir();
                glm::vec4 getRightDir();

                void Move(glm::vec3 v);
                void Rotate(float dx, float dy);

        private:
                float m_angleX, m_angleY;
                glm::vec4 m_position;
        };
}
