#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "ExportHeader.h"
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <QtOpenGL\qglwidget>

class Camera
{
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 view;

	glm::vec3 positionVM;
	glm::vec3 upVM;
	glm::vec3 viewVM;

	float speed;
	float mouseSpeed;

	int lastMouseX;
	int lastMouseY;

public:
	glm::mat4 camera;
	glm::mat4 rotationCamera;

	ENGINE_SHARED Camera();

	ENGINE_SHARED void moveForward();
	ENGINE_SHARED void moveBack();
	ENGINE_SHARED void moveLeft();
	ENGINE_SHARED void moveRight();
	ENGINE_SHARED void moveUp();
	ENGINE_SHARED void moveDown();
	ENGINE_SHARED void moveCamera( int mouseX, int mouseY);
	ENGINE_SHARED glm::mat4 getViewMatrix() const;
	ENGINE_SHARED glm::mat4 getRotationView() const;
	ENGINE_SHARED glm::vec3 getPosition();
	ENGINE_SHARED glm::vec3 getView();

	ENGINE_SHARED void mouseMoveEvent(QMouseEvent*);
	ENGINE_SHARED void keyPressEvent(QKeyEvent*);
};

#endif
