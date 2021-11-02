#include "object2D.h"

#include <vector>
#include <math.h>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare1(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
	float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateEnemy(const std::string& name, glm::vec3 leftBottomCorner,
	glm::vec3 color, glm::vec3 color_eye) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, 0.5, 0), color),
		VertexFormat(corner + glm::vec3(0.05, 0.5, 0), color),
		VertexFormat(corner + glm::vec3(0.05, 0.6, 0), color),
		VertexFormat(corner + glm::vec3(0.15, 0.5, 0), color),
		VertexFormat(corner + glm::vec3(0.15, 0.6, 0), color),
		VertexFormat(corner + glm::vec3(0.35, 0.5, 0), color),
		VertexFormat(corner + glm::vec3(0.35, 0.6, 0), color),
		VertexFormat(corner + glm::vec3(0.45, 0.5, 0), color),
		VertexFormat(corner + glm::vec3(0.45, 0.6, 0), color),
		VertexFormat(corner + glm::vec3(0.5, 0, 0), color),
		VertexFormat(corner + glm::vec3(0.5, 0.5, 0), color),
		/*VertexFormat(corner + glm::vec3(0.08, 0.53, 0), color_eye),
		VertexFormat(corner + glm::vec3(0.11, 0.53, 0), color_eye),
		VertexFormat(corner + glm::vec3(0.11, 0.56, 0), color_eye),
		VertexFormat(corner + glm::vec3(0.08, 0.56, 0), color_eye),
		VertexFormat(corner + glm::vec3(0.38, 0.53, 0), color_eye),
		VertexFormat(corner + glm::vec3(0.41, 0.53, 0), color_eye),
		VertexFormat(corner + glm::vec3(0.41, 0.56, 0), color_eye),
		VertexFormat(corner + glm::vec3(0.38, 0.56, 0), color_eye),
		*/
	};

	Mesh* enemy = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 2, 1,
		0, 4, 2,
		0, 6, 4,
		0, 8, 6,
		0, 11, 8,
		0, 10, 11,  // TODO vezi astea aici ca nu deseneaza bine
		2, 5, 3,
		2, 4, 5,
		6, 9, 7,
		6, 8, 9,
		/*12, 14, 15,
		12, 13, 14,
		16, 18, 19,
		16, 17, 18*/
	};


	enemy->InitFromData(vertices, indices);
	return enemy;

}

Mesh* object2D::CreatePlayer(const std::string name, glm::vec3 leftBottomCorner) {
	glm::vec3 corner = leftBottomCorner;


	Mesh* player = new Mesh(name);

	return player;
}