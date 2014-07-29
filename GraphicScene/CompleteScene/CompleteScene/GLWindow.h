#pragma once
#pragma once
#ifndef GL_WINDOW
#define GL_WINDOW
#include <QtGui\qwidget.h>
#include "Renderer.h"

class GLWindow : public QWidget
{
	Q_OBJECT
		QTimer *timer;	

public:
	GLWindow();

private:
	float disintegration;
	float alphaSlider;
	bool alpha;
	bool shadows;
	bool doNormal;
	bool doDiffuse;
	bool VertexDisplacement;

public slots:
	void createObjects();
	void update();
	
};

#endif