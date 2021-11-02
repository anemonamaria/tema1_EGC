#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare1(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill = false);
    Mesh* CreatePlayer(const std::string name, glm::vec3 leftBottomCorner);
   // Mesh* CreateDog(std::string name, glm::vec3 color = glm::vec3(1, 0.5f, 0));
}
