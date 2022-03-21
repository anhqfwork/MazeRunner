#include "Game.h"

//Private functions
void Game::initGLFW()
{
	//INIT GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initWindow(
	const char* title,
	bool resizable
)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	
	glfwMakeContextCurrent(this->window); //IMPORTANT!!
}

void Game::initGLEW()
{
	//INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	//Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

void Game::initShaders()
{
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTextures()
{
	//TEXTURE 0
	this->textures.push_back(new Texture("Images/viri.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/viri.png", GL_TEXTURE_2D));

	//TEXTURE 1
	this->textures.push_back(new Texture("Images/brickwall_new.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/brickwall_specular.png", GL_TEXTURE_2D));

	//TEXTURE 2
	this->textures.push_back(new Texture("Images/red_new.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/red_new.png", GL_TEXTURE_2D));

	//TEXTURE 3
	this->textures.push_back(new Texture("Images/gradient.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/gradient.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0, 1));
}

void Game::initModels()
{
	// Plane Model
	std::vector<Mesh*> meshPlane;

	meshPlane.push_back(
		new Mesh(
			&Quad(),
			glm::vec3(0.f, -1.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(-90.f, 0.f, 0.f),
			glm::vec3(50.f)
		)
	);

	this->models.push_back(new Model(
		glm::vec3(2.f, -1.f, 2.f),
		this->materials[0],
		this->textures[TEX_PINK],
		this->textures[TEX_PINK_SPECULAR],
		meshPlane));

	// Sphere mesh init
	std::vector<Vertex> sphere;
	sphere = loadOBJ("Models/sphere.obj");

	std::vector<Mesh*> meshSphere;
	meshSphere.push_back(new Mesh(sphere.data(), sphere.size(), NULL, 0,
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(0.2f)));

	// Block mesh init
	std::vector<Vertex> block;
	block = loadOBJ("Models/block.obj");

	std::vector<Mesh*> meshBlock;

	meshBlock.push_back(new Mesh(block.data(), block.size(), NULL, 0,
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(1.f)));

	// Target mesh init
	std::vector<Mesh*> meshTarget;

	meshTarget.push_back(new Mesh(block.data(), block.size(), NULL, 0,
		glm::vec3(0.f, 0.0f, 0.f),
		glm::vec3(0.f),
		glm::vec3(0.f, 45.f, 30.f),
		glm::vec3(0.3f)));

	std::vector<std::vector<int>> map = { 
				{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
				{1, 2, 1, 1, 4, 0, 0, 0, 0, 1, 1, 1},
				{1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 4, 1},
				{1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
				{1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1},
				{1, 4, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1},
				{1, 1, 0, 1, 3, 1, 0, 1, 1, 0, 1, 1},
				{1, 1, 0, 1, 0, 1, 3, 1, 1, 0, 1, 1},
				{1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1},
				{1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 4, 1},
				{1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
				{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	std::cout << "===========================" << "\n";
	std::cout << "MAZE MAP" << "\n";
	std::cout << "==========================" << "\n";

	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map.size(); j++) {
			if (map[i][j] == 2)
			{
				this->camera.setPosition(glm::vec3(2.f * (i - 1), 0.f, 2.f * (j - 1)));
				this->camera.setCamOrigin(glm::vec3(2.f * (i - 1), 0.f, 2.f * (j - 1)));
				std::cout << "O";
			}
			else if (map[i][j] == 3)
			{
				this->enemies.push_back(new Model(
					glm::vec3(2.f * (i - 1), 0.f, 2.f * (j - 1)),
					this->materials[0],
					this->textures[TEX_VIRI],
					this->textures[TEX_VIRI_SPECULAR],
					meshSphere));
			}
			else if (map[i][j] == 1)
			{
				this->blocks.push_back(new Model(
					glm::vec3(2.f * (i - 1), 0.f, 2.f * (j - 1)),
					this->materials[0],
					this->textures[TEX_CONTAINER],
					this->textures[TEX_CONTAINER_SPECULAR],
					meshBlock));
				std::cout << "X";
				if (j == 11) {
					std::cout << "\n";
				}
			}
			else if (map[i][j] == 4) {
				this->targets.push_back(new Model(
					glm::vec3(2.f * (i - 1), 0.f, 2.f * (j - 1)),
					this->materials[0],
					this->textures[TEX_GRADIENT],
					this->textures[TEX_GRADIENT_SPECULAR],
					meshTarget));
				std::cout << "A";
			}
			else {
				std::cout << " ";
			}
		}
	}

	for (auto*& i : meshBlock)
		delete i;
}

void Game::initPointLights()
{
	this->pointLights.push_back(new PointLight(glm::vec3(0.f, 1.f, 0.f)));
}

void Game::initLights()
{
	this->initPointLights();
}

void Game::initUniforms()
{
	//INIT UNIFORMS
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	for each (PointLight * pl in this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
}

void Game::updateUniforms()
{
	//Update view matrix (camera)
	this->ViewMatrix = this->camera.getViewMatrix();

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");

	for each (PointLight * pl in this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}

	//Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

//Constructors / Destructors
Game::Game(
	const char* title,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable
)
	:
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	GL_VERSION_MAJOR(GL_VERSION_MAJOR),
	GL_VERSION_MINOR(GL_VERSION_MINOR),
	camera(glm::vec3(0.f, 0.5f, 4.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f))
{
	//Init variables
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	this->camPosition = glm::vec3(0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->dt = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();

	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initModels();
	this->initLights();
	this->initUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (auto*& i : this->models)
		delete i;

	for (size_t i = 0; i < this->pointLights.size(); i++)
		delete this->pointLights[i];
}

int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void Game::updateDt()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

bool Game::checkCollisionBlock(glm::vec3 camNewPos)
{
	for (size_t i = 0; i < this->blocks.size(); i++) {
		glm::vec3 blockPos = this->blocks[i]->getPosition();
		float d = sqrt(pow(blockPos.x - camNewPos.x, 2) + pow(blockPos.z - camNewPos.z, 2) * 1.0);
		if (d < 1.5f) {
			return true;
		}
	}
	return false;
}

bool Game::checkCollisionEnemy()
{
	glm::vec3 camPos = this->camera.getPosition();
	for (size_t i = 0; i < this->enemies.size(); i++) {
		glm::vec3 enemyPos = this->enemies[i]->getPosition();
		float d = sqrt(pow(enemyPos.x - camPos.x, 2) + pow(enemyPos.z - camPos.z, 2) * 1.0);
		if (d < 1.f) {
			return true;
		}
	}
	return false;
}

bool Game::checkCollisionTarget()
{
	glm::vec3 camPos = this->camera.getPosition();
	for (size_t i = 0; i < this->targets.size(); i++) {
		glm::vec3 targetPos = this->targets[i]->getPosition();
		float d = sqrt(pow(targetPos.x - camPos.x, 2) + pow(targetPos.z - camPos.z, 2) * 1.0);
		if (d < 1.f) {
			this->targets[i]->setPosition(targetPos + glm::vec3(100.f, 2.f, 100.f));
			int score = this->increaseScore();
			std::cout << "You have " << score << " points.\n";
			return true;
		}
	}
	return false;
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	//Calc offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	//Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

void Game::updateKeyboardInput()
{
	//Program
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}
	
	glm::vec3 camNewPos = this->camera.getPosition();
	//Camera
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camNewPos = this->camera.move(this->dt, FORWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camNewPos = this->camera.move(this->dt, BACKWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camNewPos = this->camera.move(this->dt, LEFT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camNewPos = this->camera.move(this->dt, RIGHT);
	}

	if (!this->checkCollisionBlock(camNewPos)) {
		this->camera.setPosition(camNewPos);
	}
}

void Game::updateInput()
{
	glfwPollEvents();

	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
	this->pointLights[0]->setPosition(this->camera.getPosition());
}

void Game::moveEnemies(float init_x, float final_x, int enemy_index) {
	if (this->enemies[enemy_index]->getPosition().x <= init_x) {
		this->enemies[enemy_index]->setFront(true);
	} 
	else if (this->enemies[enemy_index]->getPosition().x >= final_x) {
		this->enemies[enemy_index]->setFront(false);
	}

	if (this->enemies[enemy_index]->isFront()) {
		this->enemies[enemy_index]->move(glm::vec3(0.02f, 0.f, 0.f));
	}
	else
	{
		this->enemies[enemy_index]->move(glm::vec3(-0.02f, 0.f, 0.f));
	}
}

void Game::update()
{
	//UPDATE INPUT ---
	this->updateDt();
	this->updateInput();


	this->moveEnemies(10, 18, 0);
	this->moveEnemies(0, 12, 1);


	this->checkCollisionTarget();
	if (this->checkCollisionEnemy()) {
		this->camera.setPosition(this->camera.getCamOrigin());
	}

	for (auto& i : this->targets) {
		i->rotate(glm::vec3(0.f, 1.f, 1.f));
	}

	if (this->isWin()) {
		std::cout << "YOU WIN!" << "\n";
		this->setWindowShouldClose();
	}
	//this->moveEnemies();
	//this->models[0]->rotate(glm::vec3(0.f, 1.f, 0.f));
	//this->blocks[1]->rotate(glm::vec3(0.f, 1.f, 0.f));
	//this->models[2]->rotate(glm::vec3(0.f, 1.f, 0.f));
}

void Game::render()
{
	//UPDATE --- 
	//updateInput(window);

	//DRAW ---
	//Clear
	glClearColor(0.529f, 0.808f, 0.922f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update the uniforms
	this->updateUniforms();

	//Render models
	for (auto& i : this->models)
		i->render(this->shaders[SHADER_CORE_PROGRAM]);

	for (auto& i : this->blocks)
		i->render(this->shaders[SHADER_CORE_PROGRAM]);

	for (auto& i : this->enemies)
		i->render(this->shaders[SHADER_CORE_PROGRAM]);

	for (auto& i : this->targets)
		i->render(this->shaders[SHADER_CORE_PROGRAM]);

	//End Draw
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
