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


/*Mesh* object2D::CreatePlayer(const std::string name, glm::vec3 leftBottomCorner, glm::vec3 color) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		
	};

	Mesh* player = new Mesh(name);
	std::vector<unsigned int> indices = {
	
	};


	player->InitFromData(vertices, indices);
	return player;
}*/

Mesh* object2D::CreateBalloon(const std::string& name, glm::vec3 centre, float lengthX, glm::vec3 color) {
	float a = lengthX;

	int range = 90;

	std::vector<VertexFormat> vertices = {
		VertexFormat(centre + glm::vec3(0, 0, 0), color) // 0
	};

	for (int i = 0; i < range + 1; ++i) { // 1-91
		float angle = -M_PI / 2 + M_PI / 45 * i;
		vertices.push_back(VertexFormat(centre + glm::vec3(a * cos(angle), a * sin(angle), 0), color));
	}
	//vertices.push_back(VertexFormat(centre + glm::vec3(0, -b - b / 5, 0), color - glm::vec3(0.3f, 0.3f, 0.3f))); // 92
	//vertices.push_back(VertexFormat(centre + glm::vec3(a / 4, -b - 2 * b / 5, 0), color)); // 93
	//vertices.push_back(VertexFormat(centre + glm::vec3(-a / 4, -b - 2 * b / 5, 0), color)); // 94

	Mesh* balloon = new Mesh(name);
	
	std::vector<unsigned int> indices;
	for (int i = 1; i <= range; ++i) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(1 + i % range);
	}

	/*indices.push_back(82);
	indices.push_back(92);
	indices.push_back(10);

	indices.push_back(93);
	indices.push_back(94);
	indices.push_back(92);
	*/

	balloon->InitFromData(vertices, indices);
	return balloon;
}


Mesh* object2D::CreateDog(const std::string& name, glm::vec3 color) {
	glm::vec3 black = glm::vec3(0, 0, 0);

	std::vector<VertexFormat> vertices = {
		// the body
		VertexFormat(glm::vec3(-5,-4,0), color),
		VertexFormat(glm::vec3(3,-4,0), color),
		VertexFormat(glm::vec3(3,4,0), color - glm::vec3(0.2f, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(-1,4,0), color - glm::vec3(0.2f, 0.2f, 0.2f)),

		// the head
		VertexFormat(glm::vec3(-1,4,0), color),
		VertexFormat(glm::vec3(6,4,0), color),
		VertexFormat(glm::vec3(8,6,0), color),
		VertexFormat(glm::vec3(8,8,0), color),
		VertexFormat(glm::vec3(7,9,0), color),
		VertexFormat(glm::vec3(1.5f,8.5f,0), color),
		VertexFormat(glm::vec3(1.5f,11.5f,0), color),
		VertexFormat(glm::vec3(-1,9,0), color),

		// the nose
		VertexFormat(glm::vec3(8,8,0), black),
		VertexFormat(glm::vec3(8,9,0), black),
		VertexFormat(glm::vec3(7,9,0), black),

		// the tail
		VertexFormat(glm::vec3(-5,-4,0), color - glm::vec3(0.2f, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(-4,-2,0), color - glm::vec3(0.2f, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(-5,1,0), color),

		// the right leg
		VertexFormat(glm::vec3(-2,-4,0), color - glm::vec3(0.2f, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(1,-4,0), color - glm::vec3(0.1f, 0.1f, 0.1f)),
		VertexFormat(glm::vec3(-2,-1,0), color - glm::vec3(0.2f, 0.2f, 0.2f)),

		// the left leg
		VertexFormat(glm::vec3(3,-4,0), color - glm::vec3(0.2f, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(6,-4,0), color - glm::vec3(0.1f, 0.1f, 0.1f)),
		VertexFormat(glm::vec3(3,-1,0), color - glm::vec3(0.2f, 0.2f, 0.2f)),

		// the eye
		VertexFormat(glm::vec3(2.5f,7.5f,0), black) // index = 24
	};

	for (float i = 0; i < 2 * M_PI; i += M_PI / 6) // 25 - 36
		vertices.push_back(VertexFormat(glm::vec3(2.5f + 0.5f * cos(i), 7.5f + 0.5f * sin(i), 0), black));

	std::vector<unsigned int> indices = {
		// the eye
		24, 25, 26,
		24, 26, 27,
		24, 27, 28,
		24, 28, 29,
		24, 29, 30,
		24, 30, 31,
		24, 31, 32,
		24, 32, 33,
		24, 33, 34,
		24, 34, 35,
		24, 35, 36,
		24, 36, 25,


		21, 22, 23,
		18, 19, 20,
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		4, 7, 8,
		4, 8, 11,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
	};

	Mesh* dog = new Mesh(name);
	dog->InitFromData(vertices, indices);
	return dog;
}