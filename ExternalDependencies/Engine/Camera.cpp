#include "Camera.h"
#include "Logger.h"
#include <glm\gtx\transform.hpp>
#include <QtGui\QKeyEvent>
#include <QtGui\QMouseEvent>

Camera::Camera()
{
	position = glm::vec3(0,0,5); 
	view = glm::vec3(0,0,0); 
	up = glm::vec3(0,1,0);

	positionVM = position;
	viewVM = view;
	upVM = up;

	camera = glm::lookAt(
			position, view, up);

	rotationCamera = glm::lookAt(positionVM,
		viewVM, upVM );

	speed = 0.5f;

	lastMouseX = 0;
	lastMouseY = 0;
}

void Camera::moveCamera( int mouseX, int mouseY)
{
	int difX = mouseX - lastMouseX;
	int difY = mouseY - lastMouseY;

	lastMouseX = mouseX;
	lastMouseY = mouseY;

	if((difX >100) || (difY > 100) || (difX< -100) || (difY< -100) ) return;

	glm::mat4 rotationX = glm::rotate(difX * 0.3f, up);
	view = glm::vec3(rotationX * glm::vec4(view,1));

	glm::mat4 rotationY = glm::rotate(difY * 0.3f, glm::cross(up, view));
	view = glm::vec3(rotationY * glm::vec4(view,1));

	camera = glm::lookAt(position, view, up);

	glm::mat4 rotationXVM = glm::rotate(difX * 0.3f, upVM);
	viewVM = glm::vec3(rotationXVM * glm::vec4(viewVM,1));

	glm::mat4 rotationYVM = glm::rotate(difY * 0.3f, glm::cross(upVM, viewVM));
	viewVM = glm::vec3(rotationYVM * glm::vec4(viewVM,1));

	rotationCamera = glm::lookAt(positionVM, viewVM, upVM);
}

void Camera::moveForward()
{
	position.z -= speed;
	view.z -= speed;
	camera = glm::lookAt(position, view, up);
}

void Camera::moveBack()
{
	position.z += speed;
	view.z += speed;
	camera = glm::lookAt(position, view, up);
}

void Camera::moveRight()
{
	position.x += speed;
	view.x += speed;
	camera = glm::lookAt(position, view, up);
}

void Camera::moveLeft()
{
	position.x -= speed;
	view.x -= speed;
	camera = glm::lookAt(position, view, up);
}

void Camera::moveUp()
{
	position.y += speed;
	view.y += speed;
	camera = glm::lookAt(position, view, up);
}

void Camera::moveDown()
{
	position.y -= speed;
	view.y -= speed;
	camera = glm::lookAt(position, view, up);
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, view, up);
}

glm::mat4 Camera::getRotationView() const
{
	return glm::lookAt(positionVM, view, up);
}

glm::vec3 Camera::getPosition()
{
	return position;
}

glm::vec3 Camera::getView()
{
	return view;
}

void Camera::mouseMoveEvent(QMouseEvent *event)
{
	moveCamera(-event -> x(), event -> y());
	getViewMatrix();
}

void Camera::keyPressEvent(QKeyEvent *event)
{
	switch(event->key()) 
	{
		case Qt::Key_W:
			moveForward();
			getViewMatrix();
			break;
		case Qt::Key_A:
			moveLeft();
			getViewMatrix();
			break;
		case Qt::Key_S:		
			moveBack();
			getViewMatrix();
			break;
		case Qt::Key_D:
			moveRight();
			getViewMatrix();
			break;
		case Qt::Key_R:
			moveUp();
			getViewMatrix();
			break;
		case Qt::Key_F:
			moveDown();
			getViewMatrix();
			break;
		case Qt::Key_Escape:
			exit(0);
			break;
		default:
			event->ignore();
			break;
	}
}
