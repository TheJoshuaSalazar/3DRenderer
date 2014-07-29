#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H
#include "ExportHeader.h"
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "ExportHeader.h"


class DebugCamera
{
	glm::vec3 position;
	glm::vec3 view;
	glm::vec3 up;

	float speed;
	float mouseSpeed;

	int lastMouseX;
	int lastMouseY;

public:
	glm::mat4 camera;

	ENGINE_SHARED DebugCamera();

	ENGINE_SHARED void moveForward();
	ENGINE_SHARED void moveBack();
	ENGINE_SHARED void moveLeft();
	ENGINE_SHARED void moveRight();
	ENGINE_SHARED void moveUp();
	ENGINE_SHARED void moveDown();
	ENGINE_SHARED void moveCamera( int mouseX, int mouseY);
	ENGINE_SHARED glm::mat4 getViewMatrix() const;
	ENGINE_SHARED glm::vec3 getPosition();
};

#endif
