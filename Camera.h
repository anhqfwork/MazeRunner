#pragma once

#include<iostream>

#include<glew/glew.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/vec3.hpp>
#include<glm/mat4x4.hpp>
#include<glm/gtc\matrix_transform.hpp>

#include"Model.h"

enum direction { FORWARD = 0, BACKWARD, LEFT, RIGHT };

class Camera
{
private:
	glm::mat4 ViewMatrix;

	GLfloat movementSpeed;
	GLfloat sensitivity;

	glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	glm::vec3 camOrigin;

	void updateCameraVectors()
	{
		this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front.y = sin(glm::radians(this->pitch));
		this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

		this->front = glm::normalize(this->front);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
	{
		this->ViewMatrix = glm::mat4(1.f);

		this->movementSpeed = 3.f;
		this->sensitivity = 5.f;

		this->worldUp = worldUp;
		this->position = position;
		this->right = direction;
		this->up = worldUp;

		this->pitch = -30.f;
		this->yaw = 0.f;
		this->roll = 0.f;

		this->updateCameraVectors();
	}

	~Camera() {}

	//Accessors
	const glm::mat4 getViewMatrix()
	{
		this->updateCameraVectors();

		this->ViewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);

		return this->ViewMatrix;
	}

	//Functions
	glm::vec3 move(const float& dt, const int direction)
	{
		glm::vec3 newPos = this->position;
		//Update position vector
		switch (direction)
		{
		case FORWARD:
			newPos.x = this->position.x + this->front.x * this->movementSpeed * dt;
			newPos.z = this->position.z + this->front.z * this->movementSpeed * dt;
			return newPos;
			break;
		case BACKWARD:
			newPos.x = this->position.x - this->front.x * this->movementSpeed * dt;
			newPos.z = this->position.z - this->front.z * this->movementSpeed * dt;
			return newPos;
			break;
		case LEFT:
			newPos.x = this->position.x - this->right.x * this->movementSpeed * dt;
			newPos.z = this->position.z - this->right.z * this->movementSpeed * dt;
			return newPos;
			break;
		case RIGHT:
			newPos.x = this->position.x + this->right.x * this->movementSpeed * dt;
			newPos.z = this->position.z + this->right.z * this->movementSpeed * dt;
			return newPos;
			break;
		}
	}

	void updateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
	{
		//Update pitch yaw and roll
		this->pitch += static_cast<GLfloat>(offsetY) * this->sensitivity * dt;
		this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity * dt;

		if (this->pitch > 80.f)
			this->pitch = 80.f;
		else if (this->pitch < -80.f)
			this->pitch = -80.f;

		if (this->yaw > 360.f || this->yaw < -360.f)
			this->yaw = 0.f;
	}

	void updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
	{
		this->updateMouseInput(dt, offsetX, offsetY);
	}

	void setCamOrigin(glm::vec3 camOrigin)
	{
		this->camOrigin = camOrigin;
	}

	glm::vec3 getCamOrigin() {
		return this->camOrigin;
	}

	glm::vec3 getPosition()
	{
		return this->position;
	}

	void setPosition(glm::vec3 newPosition) 
	{
		this->position = newPosition;
	}

};