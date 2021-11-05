#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare1(const std::string& name, glm::vec3 leftBottomCorner, float length,
	float width, glm::vec3 color, bool fill)
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
	};


	enemy->InitFromData(vertices, indices);
	return enemy;

}

Mesh* object2D::CreateCircle(const std::string& name, glm::vec3 centre, float lengthX, glm::vec3 color) {
	float a = lengthX;

	int range = 90;

	std::vector<VertexFormat> vertices = {
		VertexFormat(centre + glm::vec3(0, 0, 0), color) // 0
	};

	for (int i = 0; i < range + 1; ++i) { // 1-91
		float angle = -M_PI / 2 + M_PI / 45 * i;
		vertices.push_back(VertexFormat(centre + glm::vec3(a * cos(angle), a * sin(angle), 0), color));
	}
	Mesh* balloon = new Mesh(name);
	
	std::vector<unsigned int> indices;
	for (int i = 1; i <= range; ++i) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(1 + i % range);
	}

	balloon->InitFromData(vertices, indices);
	return balloon;
}

Mesh* object2D::CreatePlayer(const std::string& name) {
	glm::vec3 black = glm::vec3(0, 0, 0);

	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0.0f, 0.0f,0), black), 
	};

	for (float i = 0; i < 2 * M_PI; i += M_PI / 6) // 0-13
		vertices.push_back(VertexFormat(glm::vec3(0.0f + 1.0f * cos(i), 0.0f + 1.0f * sin(i), 0), black));

	// the left ear
	vertices.push_back(VertexFormat(glm::vec3(-1.0f, 1.0f, 0), black));
	for (float i = 0; i < 2 * M_PI; i += M_PI / 6) 
		vertices.push_back(VertexFormat(glm::vec3(-1.0f + 0.5f * cos(i), 1.0f + 0.5f * sin(i), 0), black));
	
	//the right ear
	vertices.push_back(VertexFormat(glm::vec3(1.0f, 1.0f, 0), black));
	for (float i = 0; i < 2 * M_PI; i += M_PI / 6) 
		vertices.push_back(VertexFormat(glm::vec3(1.0f + 0.5f * cos(i), 1.0f + 0.5f * sin(i), 0), black));


	std::vector<unsigned int> indices = {
		// the body 
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 9,
		0, 9, 10,
		0, 10, 11,
		0, 11, 12,
		0, 12, 13,

		// left ear
		14, 15, 16,
		14, 16, 17,
		14, 17, 18,
		14, 18, 19,
		14, 19, 20,
		14, 20, 21,
		14, 21, 22,
		14, 22, 23,
		14, 23, 24,
		14, 24, 25,
		14, 25, 26,
		14, 26, 27,

		// right ear
		28, 29, 30,
		28, 30, 31,
		28, 31, 32,
		28, 32, 33,
		28, 33, 34,
		28, 34, 35,
		28, 35, 36,
		28, 36, 37,
		28, 37, 38,
		28, 38, 39,
		28, 39, 40,
		28, 40, 41,

	};

	Mesh* player = new Mesh(name);
	player->InitFromData(vertices, indices);
	return player;
} 
