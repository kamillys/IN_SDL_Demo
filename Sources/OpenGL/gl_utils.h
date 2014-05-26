#pragma once

#include <glm/glm.hpp>
#include <ostream>

std::ostream& operator<<(std::ostream& o, const glm::dvec2& m);
std::ostream& operator<<(std::ostream& o, const glm::dvec3& m);
std::ostream& operator<<(std::ostream& o, const glm::dvec4& m);

std::ostream& operator<<(std::ostream& o, const glm::vec2& m);
std::ostream& operator<<(std::ostream& o, const glm::vec3& m);
std::ostream& operator<<(std::ostream& o, const glm::vec4& m);

std::ostream& operator<<(std::ostream& o, const glm::mat3& m);
std::ostream& operator<<(std::ostream& o, const glm::mat4& m);
