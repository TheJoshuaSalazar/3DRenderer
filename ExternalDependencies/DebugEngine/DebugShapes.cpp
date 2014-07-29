#include "DebugShapes.h"
#include <glm\gtx\transform.hpp>
#include <fstream>
#include <iostream>
#include "Logger.h"
#include <Qt\qdebug.h>

#define _180_PI 180/PI;

DebugShapes DebugShapes::DebugShapeInstance;

DebugShapes::DebugShapes()
{
	
}

void DebugShapes::createRender()
{
	Renderers = new Renderer;
	Logger::getInstance().logInitialize("DebugShapes has been initialized", "DebugShapes.cpp", 21);
}

void DebugShapes::initializeDrawData()
{
	Arrow = Neumont::ShapeGenerator::makeArrow();
	ArrowID = Renderers->addGeometry(Arrow.verts, Arrow.vertexBufferSize(), Arrow.indices,
							Arrow.numIndices, GL_TRIANGLES, true);
	NormalShader = Renderers->addShader("MyvertexShaderCode.sh", "MyfragShaderCode.sh");

	Vector = Neumont::ShapeGenerator::makeVector(20);
	VectorID = Renderers->addGeometry(Vector.verts, Vector.vertexBufferSize(),
	Vector.indices, Vector.numIndices, GL_TRIANGLES, true);

	Sphere = Neumont::ShapeGenerator::makeSphere(30);
	SphereID = Renderers ->addGeometry(Sphere.verts, Sphere.vertexBufferSize(), 
	Sphere.indices, Sphere.numIndices, GL_TRIANGLES, true);

	Cube = Neumont::ShapeGenerator::makeCube();
	CubeID = Renderers->addGeometry(Cube.verts, Cube.vertexBufferSize(), 
	Cube.indices, Cube.numIndices, GL_TRIANGLES, true);

	Teapot = Neumont::ShapeGenerator::makeTeapot(15, glm::mat4());
	teapotID = Renderers->addGeometry(Teapot.verts, Teapot.vertexBufferSize(), 
	Teapot.indices, Teapot.numIndices, GL_TRIANGLES, true);

	Plane = Neumont::ShapeGenerator::makePlane(25,25);
	PlaneID = Renderers ->addGeometry(Plane.verts, Plane.vertexBufferSize(),
	Plane.indices, Plane.numIndices, GL_TRIANGLES, true);

	Cylinder = Neumont::ShapeGenerator::makeCylinder(30);
	CylinderID = Renderers->addGeometry(Cylinder.verts, Cylinder.vertexBufferSize(),
	Cylinder.indices, Cylinder.numIndices, GL_TRIANGLES, true);

	CubeWireFrame = Neumont::ShapeGenerator::makeWireFrameCube();
	CubeWireFrameID = Renderers->addGeometry(CubeWireFrame.verts, CubeWireFrame.vertexBufferSize(),
	CubeWireFrame.indices, CubeWireFrame.numIndices, GL_LINES, true);
}

void DebugShapes::initializeDrawData(const char *vertexShader, const char *fragShader)
{
	Arrow = Neumont::ShapeGenerator::makeArrow();
	ArrowID = Renderers->addGeometry(Arrow.verts, Arrow.vertexBufferSize(), Arrow.indices,
							Arrow.numIndices, GL_TRIANGLES, true);
	NormalShader = Renderers->addShader(vertexShader, fragShader);

	Vector = Neumont::ShapeGenerator::makeVector(20);
	VectorID = Renderers->addGeometry(Vector.verts, Vector.vertexBufferSize(),
	Vector.indices, Vector.numIndices, GL_TRIANGLES, true);

	Sphere = Neumont::ShapeGenerator::makeSphere(30);
	SphereID = Renderers ->addGeometry(Sphere.verts, Sphere.vertexBufferSize(), 
	Sphere.indices, Sphere.numIndices, GL_TRIANGLES, true);

	Cube = Neumont::ShapeGenerator::makeCube();
	CubeID = Renderers->addGeometry(Cube.verts, Cube.vertexBufferSize(), 
	Cube.indices, Cube.numIndices, GL_TRIANGLES, true);

	Teapot = Neumont::ShapeGenerator::makeTeapot(15, glm::mat4());
	teapotID = Renderers->addGeometry(Teapot.verts, Teapot.vertexBufferSize(), 
	Teapot.indices, Teapot.numIndices, GL_TRIANGLES, true);

	Plane = Neumont::ShapeGenerator::makePlane(25, 25);
	PlaneID = Renderers ->addGeometry(Plane.verts, Plane.vertexBufferSize(),
	Plane.indices, Plane.numIndices, GL_TRIANGLES, true);

	Cylinder = Neumont::ShapeGenerator::makeCylinder(30);
	CylinderID = Renderers->addGeometry(Cylinder.verts, Cylinder.vertexBufferSize(),
	Cylinder.indices, Cylinder.numIndices, GL_TRIANGLES, true);

	CubeWireFrame = Neumont::ShapeGenerator::makeWireFrameCube();
	CubeWireFrameID = Renderers->addGeometry(CubeWireFrame.verts, CubeWireFrame.vertexBufferSize(),
	CubeWireFrame.indices, CubeWireFrame.numIndices, GL_LINES, true);
}

void DebugShapes::AddVector(const glm::vec3& start, const glm::vec3& end)
{
	Logger::getInstance().logInitialize("A vector was added", "DebugShapes", 58);
	ushort indices[6] = {0,1,2,3,4,5};

	vertexs[0].position = glm::vec3(start.x, start.y, start.z);
	vertexs[0].color = glm::vec4(1,0,0,0);
	vertexs[1].position = glm::vec3(end.x, end.y, end.z);
	vertexs[1].color = glm::vec4(0,1,1,0);

	unsigned int CrossID = Renderers->addGeometry(vertexs, sizeof(vertexs), 
					indices, sizeof(indices), GL_LINES, true);
	Renderers ->addRenderable(CrossID, glm::translate(glm::vec3()), NormalShader, false);
}

void DebugShapes::AddCross(const glm::vec3& position)
{
	ushort indices[6] = {0,1,2,3,4,5};

	vertexs[0].position = glm::vec3(1,0, 0);
	vertexs[0].color = glm::vec4(1,0,0,0);
	vertexs[1].position = glm::vec3(-1,0,0);
	vertexs[1].color = glm::vec4(0,0,0,0);
	vertexs[2].position = glm::vec3(0,1, 0);
	vertexs[2].color = glm::vec4(0,1,0,0);
	vertexs[3].position = glm::vec3(0,-1,0);
	vertexs[3].color = glm::vec4(0,0,0,0);
	vertexs[4].position = glm::vec3(0,0, 1);
	vertexs[4].color = glm::vec4(0,0,1,0);
	vertexs[5].position = glm::vec3(0,0,-1);
	vertexs[5].color = glm::vec4(0,0,0,0);

	unsigned int CrossID = Renderers->addGeometry(vertexs, sizeof(vertexs), indices,
					sizeof(indices),GL_LINES, true);
	Renderers ->addRenderable(CrossID, glm::scale(glm::vec3(2,2,2)) * glm::translate(position), NormalShader, false);
}

void DebugShapes::AddArrow(const glm::vec3& position, bool opaque)
{
	Logger::getInstance().logInitialize("An arrow shape was added", "DebugShapes", 95);
	Renderers ->addRenderable(ArrowID, glm::translate(glm::vec3(position)), NormalShader, opaque);
}

void DebugShapes::Add3DVector(const glm::vec3& start, const glm::vec3& end)
{
	glm::vec3 rot = end - start;
	float rotationAngle =  acosf(glm::dot(glm::normalize(glm::vec3(0.0f,0.0f,1.0f)),glm::normalize(rot)));
	float convertedRotationAngle = rotationAngle * 180/PI;
	glm::vec3 cross = glm::cross(glm::vec3(0,0,0.5f), rot);
	if(cross.x == 0.0f && cross.y == 0.0f && cross.z == 0.0f)
	{
		cross.x = 1.0f;
		convertedRotationAngle = 0;
	}

	glm::mat4 transform = glm::translate(start) * glm::rotate(convertedRotationAngle,cross)
		* glm::scale(1.0f, 1.0f, glm::length(rot)) * glm::translate(0.0f, -0.015625f, 0.5f)
		* glm::scale(0.0625f, 0.0625f,-0.5f);

	Renderers ->addRenderable(VectorID, transform, NormalShader, false);
}

void DebugShapes::AddSphere(const glm::vec3& position, bool opaque)
{
	Logger::getInstance().logInitialize("A Sphere was added", "DebugShapes", 120);
	Renderers ->addRenderable(SphereID, glm::translate(position), NormalShader, opaque);
}

void DebugShapes::AddSphere(const glm::vec3& position, bool opaque, const char *vertexShader, const char *fragShader)
{
	unsigned int shader = Renderers->addShader(vertexShader, fragShader);
	Renderers ->addRenderable(SphereID, glm::translate(position), shader, opaque);
}

void DebugShapes::AddTeapot(const glm::vec3& position, bool opaque)
{
	Renderers ->addRenderable(teapotID, glm::translate(position), NormalShader, opaque);
}

void DebugShapes::AddCube(const glm::vec3& position, bool opaque)
{
	Logger::getInstance().logInitialize("A cube was added", "DebugShapes", 126);
	Renderers->addRenderable(CubeID,  glm::translate(position), NormalShader, opaque);
}

void DebugShapes::AddCube(const glm::vec3& position, bool opaque, const char *vertexShader, const char *fragShader)
{
	unsigned int shader = Renderers->addShader(vertexShader, fragShader);
	Renderers ->addRenderable(CubeID, glm::translate(position), shader, opaque);
}

void DebugShapes::AddPlane(const glm::vec3& position)
{
	Logger::getInstance().logInitialize("A plane was added", "DebugShapes", 132);
	Renderers ->addRenderable(PlaneID, glm::translate(position), NormalShader, false);
}

void DebugShapes::AddPlane(const glm::vec3& position, const char *vertexShader, const char *fragShader)
{
	unsigned int shader = Renderers->addShader(vertexShader, fragShader);
	Renderers ->addRenderable(PlaneID, glm::translate(position), shader, false);
}

void DebugShapes::AddCylinder(const glm::vec3& position, bool opaque)
{
	Logger::getInstance().logInitialize("A cylinder was added", "DebugShapes", 137);
	Renderers ->addRenderable(CylinderID, glm::translate(position), NormalShader, opaque);
}

void DebugShapes::AddCubeWireFrame(const glm::vec3& position)
{
	Logger::getInstance().logInitialize("A Wire frame cube was added", "DebugShapes", 142);
	Renderers ->addRenderable(CubeWireFrameID,  glm::translate(position), NormalShader, false);
}

void DebugShapes::AddMayaShape(const char* fileName, glm::vec3& position, bool opaque)
{
	Logger::getInstance().logInitialize("A Maya shape was added", "DebugShapes", 150);
	std::ifstream in(fileName, std::ios::in | std::ios::binary);
	in.seekg(0, std::ios::end);

	const unsigned int BUFFER_SIZE = in.tellg();

	if(BUFFER_SIZE == 4294967295)
		Logger::getInstance().InternaLog("Incorrect File", "DebugShapes", 157); 
	char* buffer = new char[BUFFER_SIZE];

	in.seekg(0, std::ios::beg);
	in.read(buffer, BUFFER_SIZE);
	in.close();

	unsigned int numShapes = *reinterpret_cast<unsigned int*>(buffer);

	Neumont::ShapeData* shapeDatas = reinterpret_cast<Neumont::ShapeData*>(buffer + sizeof(unsigned int));

	for(unsigned int i = 0; i < numShapes; i++)
	{
		Neumont::ShapeData& d = shapeDatas[i];
		d.verts = reinterpret_cast<Neumont::Vertex*>(buffer + (unsigned int)(d.verts));
		d.indices = reinterpret_cast<unsigned short*>(buffer + (unsigned int)(d.indices));

		Neumont::ShapeData shapeRenderer = shapeDatas[i];
		unsigned int shapeRendererID = Renderers->addGeometry(shapeRenderer.verts, shapeRenderer.vertexBufferSize(),
			shapeRenderer.indices, shapeRenderer.numIndices, GL_TRIANGLES, true);
		Renderers ->addRenderable(shapeRendererID, glm::translate(position), NormalShader, opaque);
	} 
	Renderers->repaint();
}

void DebugShapes::AddMayaShape(const char* fileName, glm::vec3& position, bool opaque, const char *vertexShader, const char *fragShader)
{
	std::ifstream in(fileName, std::ios::in | std::ios::binary);
	in.seekg(0, std::ios::end);

	unsigned int shader = Renderers->addShader(vertexShader, fragShader);

	const unsigned int BUFFER_SIZE = in.tellg();

	if(BUFFER_SIZE == 4294967295)
		Logger::getInstance().InternaLog("Incorrect File", "DebugShapes", 157); 
	char* buffer = new char[BUFFER_SIZE];

	in.seekg(0, std::ios::beg);
	in.read(buffer, BUFFER_SIZE);
	in.close();

	unsigned int numShapes = *reinterpret_cast<unsigned int*>(buffer);

	Neumont::ShapeData* shapeDatas = reinterpret_cast<Neumont::ShapeData*>(buffer + sizeof(unsigned int));

	for(unsigned int i = 0; i < numShapes; i++)
	{
		Neumont::ShapeData& d = shapeDatas[i];
		d.verts = reinterpret_cast<Neumont::Vertex*>(buffer + (unsigned int)(d.verts));
		d.indices = reinterpret_cast<unsigned short*>(buffer + (unsigned int)(d.indices));

		Neumont::ShapeData shapeRenderer = shapeDatas[i];
		unsigned int shapeRendererID = Renderers->addGeometry(shapeRenderer.verts, shapeRenderer.vertexBufferSize(),
		shapeRenderer.indices, shapeRenderer.numIndices, GL_TRIANGLES, true);
		Renderers ->addRenderable(shapeRendererID, glm::translate(glm::vec3( position.x , position.y , position.z)), shader, opaque);
	} 
}

glm::mat4 DebugShapes::direction(const glm::vec3& originalOrientation, const glm::vec3& newOrientation)
{
	glm::vec3 original = glm::vec3(originalOrientation);
	glm::vec3 NewOrientation = glm::normalize(newOrientation);
	float yaw = acos(glm::dot(original, glm::normalize(glm::vec3(NewOrientation.x, 0,
							NewOrientation.z)))) * _180_PI;
	float pitch = 0.0f;

	if(yaw * 0 != 0)
	{
		yaw = 0;
		pitch = acos(glm::dot(original, glm::normalize(NewOrientation))) * _180_PI;
	}
	else 
	{
		if(NewOrientation.x < 0)
		yaw = -yaw;
		glm::vec3 pitchVector = glm::vec3(glm::vec4(original, 0) * glm::rotate(yaw, glm::vec3(0,1,0)));
		pitch = acos(glm::dot(pitchVector, glm::normalize(NewOrientation))) * _180_PI;
	}

	if(pitch * 0 != 0)
	{
		pitch = 0;
	}
	else if(newOrientation.y > 0)
	{
		pitch = -pitch;
	}

	return glm::rotate(pitch, 1.0f,0.0f,0.0f) * 
		(-yaw, 0.0f,1.0f,0.0f);
}

void DebugShapes::update()
{
	
}

DebugShapes& DebugShapes::getInstance()
{
	return DebugShapeInstance;
}