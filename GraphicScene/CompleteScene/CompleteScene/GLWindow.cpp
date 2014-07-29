#include "GLWindow.h"
#include <glm\gtx\transform.hpp>
#include <QtGui\qvboxlayout>
#include "DebugShapes.h"
#include "DebugMenu.h"

GLWindow::GLWindow()
{
	setMinimumSize(1200, 900);
	QVBoxLayout* mainLayout;
	DebugShapes::getInstance().createRender();
	setLayout(mainLayout = new QVBoxLayout);

	timer = new QTimer(this);

	QVBoxLayout *layout = new QVBoxLayout;
	DebugMenu::getTheInstance().initialize(mainLayout);
	DebugShapes::getInstance().Renderers->normalSky = true;

	DebugMenu::getTheInstance().slideFloat("Disintegration", &disintegration);
	//DebugMenu::getTheInstance().slideFloat("Alpha control", &alphaSlider);
	DebugMenu::getTheInstance().watchCheckBox("Alpha On/Off", &alpha);
	DebugMenu::getTheInstance().watchCheckBox("Shadows Off", &shadows);
	DebugMenu::getTheInstance().watchCheckBox("Normal Texture Off", &doNormal);
	DebugMenu::getTheInstance().watchCheckBox("Color Texture Off", &doDiffuse);
	DebugMenu::getTheInstance().watchCheckBox("Vertex Displacement Off", &VertexDisplacement);

	mainLayout ->addWidget(DebugShapes::getInstance().Renderers);

	connect(DebugShapes::getInstance().Renderers, SIGNAL(renderInitialized()), 
		this, SLOT(createObjects()));

	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(0);
}

void GLWindow::update()
{
	DebugMenu::getTheInstance().update();
	DebugShapes::getInstance().Renderers->update();

	DebugShapes::getInstance().Renderers->disintegration = disintegration;
	DebugShapes::getInstance().Renderers->alphaSlider = alphaSlider;
	DebugShapes::getInstance().Renderers->alpha = alpha;
	DebugShapes::getInstance().Renderers->shadows = shadows;
	DebugShapes::getInstance().Renderers->doNormal = doNormal;
	DebugShapes::getInstance().Renderers->doDiffuse = doDiffuse;
	DebugShapes::getInstance().Renderers->vertexDisplacement = VertexDisplacement;
}

void GLWindow::createObjects()
{
	DebugShapes::getInstance().Renderers->addShader("DepthVertex.sh", "DepthFrag.sh");

	DebugShapes::getInstance().initializeDrawData("DisintegrationVertex.sh", "DisintegrationFrag.sh");

	DebugShapes::getInstance().AddSphere(glm::vec3(0, 0, 0), false, "EnvironmentMapVertex.sh", "EnvironmentMapFrag.sh");
	DebugShapes::getInstance().Renderers->renderables[0].place = glm::translate(glm::vec3(-1,3,0)) * glm::scale(glm::vec3(0.5));

	DebugShapes::getInstance().AddTeapot(glm::vec3(0), true);
	DebugShapes::getInstance().Renderers->renderables[1].place = glm::rotate(glm::mat4(),
		-90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(glm::vec3(0,0,-1));

	DebugShapes::getInstance().AddCube(glm::vec3(), true);
	DebugShapes::getInstance().Renderers->renderables[2].place = glm::scale(glm::vec3(0.4f, 4.0f, 0.4f))
		* glm::translate(glm::vec3(-8,-1.5f,4));
	DebugShapes::getInstance().AddCube(glm::vec3(), true);
	DebugShapes::getInstance().Renderers->renderables[3].place = glm::scale(glm::vec3(0.4f, 4.0f, 0.4f))
		* glm::translate(glm::vec3(8,-1.5f,4));
	DebugShapes::getInstance().AddCube(glm::vec3(), true);
	DebugShapes::getInstance().Renderers->renderables[4].place = glm::scale(glm::vec3(0.4f, 4.0f, 0.4f))
		* glm::translate(glm::vec3(-8,-1.5f,-4));
	DebugShapes::getInstance().AddCube(glm::vec3(), true);
	DebugShapes::getInstance().Renderers->renderables[5].place = glm::scale(glm::vec3(0.4f, 4.0f, 0.4f))
		* glm::translate(glm::vec3(8,-1.5f,-4));
	DebugShapes::getInstance().AddCube(glm::vec3(), true);
	DebugShapes::getInstance().Renderers->renderables[6].place = glm::scale(glm::vec3(6.0f, 0.4f, 4.0f))
		* glm::translate(glm::vec3(0, -4.0f, 0));

	DebugShapes::getInstance().AddCube(glm::vec3(), false, "SkyBoxVertex.sh", "SkyBoxFrag.sh");
	DebugShapes::getInstance().Renderers->renderables[7].place = glm::translate(glm::vec3(0, 15, 0)) * glm::scale(glm::mat4(), glm::vec3(25)) * glm::rotate(glm::mat4(), 180.0f, 0.0f, 1.0f, 0.0f);

	DebugShapes::getInstance().AddPlane(glm::vec3(), "DirectionalVertex.sh", "DirectionalFrag.sh");
	DebugShapes::getInstance().Renderers->renderables[8].place = glm::scale(glm::vec3(0.48f, 1.0f, 0.35f))
		* glm::translate(glm::vec3(0.4, -1.15f, 0.2));

	DebugShapes::getInstance().AddPlane(glm::vec3(0), "DisplacementVertex.sh", "DisplacementFrag.sh");
	DebugShapes::getInstance().Renderers->renderables[9].place = glm::translate(glm::vec3(0, 0, -15)) * glm::scale(glm::mat4(), glm::vec3(1)) * 
					glm::rotate(glm::mat4(), -90.0f, 1.0f, 0.0f, 0.0f);

	//DebugShapes::getInstance().AddMayaShape("WereWolf.bin", glm::vec3(0), false);
}