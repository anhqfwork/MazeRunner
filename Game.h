#pragma once

#include "libs.h"
#include "Camera.h"

//ZOOOOOOOOOOOM IN BEFORE RECORDING!

//ENUMERATIONS
enum shader_enum { SHADER_CORE_PROGRAM = 0 };
enum texture_enum { TEX_PINK = 0, TEX_PINK_SPECULAR, 
					TEX_CONTAINER, TEX_CONTAINER_SPECULAR,
					TEX_VIRI, TEX_VIRI_SPECULAR,
					TEX_GRADIENT, TEX_GRADIENT_SPECULAR};
enum material_enum { MAT_1 = 0 };
enum mesh_enum { MESH_QUAD = 0 };

class Game
{
private:
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	float dt;
	float curTime;
	float lastTime;

	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	Camera camera;
	glm::vec3 cameraOrigin;

	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;

	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	std::vector<Shader*> shaders;

	std::vector<Texture*> textures;

	std::vector<Material*> materials;

	std::vector<Model*> models;
	std::vector<Model*> enemies;
	std::vector<Model*> blocks;

	std::vector<Model*> targets;

	int score = 0;

	std::vector<PointLight*> pointLights;

	void initGLFW();
	void initWindow(
		const char* title,
		bool resizable
	);
	void initGLEW(); //AFTER CONTEXT CREATION!!!
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initModels();
	void initPointLights();
	void initLights();
	void initUniforms();

	void updateUniforms();


public:
	Game(
		const char* title,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
		bool resizable
	);
	virtual ~Game();

	void moveEnemies(float, float, int);
	bool checkCollisionBlock(glm::vec3);
	bool checkCollisionEnemy();	
	bool checkCollisionTarget();

	void setWindowShouldClose();
	int getWindowShouldClose();

	int increaseScore() {
		this->score += 1;
		return this->score;
	}

	bool isWin() {
		if (score == 4) {
			return true;
		}
		else {
			return false;
		}
	}


	void updateDt();
	void updateMouseInput();
	void updateKeyboardInput();
	void updateInput();
	void update();
	void render();
};