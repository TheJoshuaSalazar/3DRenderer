#include "DebugDualScreen.h"
#include <QtGui\QKeyEvent>
#include <QtGui\QMouseEvent>

void DualScreen::paintGL(int width, int height, Renderer *const renderer)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0,0,width/2, height);
	renderer->paintScreen(renderer->myCamera);
	glViewport(width/2, 0, width/2, height);
	renderer->paintScreen(secondCamera);
}

void DualScreen::mouseMoveEvent(QMouseEvent *event)
{
	secondCamera.mouseMoveEvent(event);
}

void DualScreen::keyPressEvent(QKeyEvent *event)
{
	secondCamera.keyPressEvent(event);
}
