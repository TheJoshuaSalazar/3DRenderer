#pragma once
#ifndef DEBUGSHAPES_H
#define DEBUGSHAPES_H	
#include "Renderer.h"
#include "ExportHeader.h"
#include "ShapeGenerator.h"

class __declspec( dllexport ) DebugShapes
{

public:
	Renderer* Renderers;
	
	bool visible;

	Neumont::Vertex vertexs[6];

	Neumont::ShapeData Arrow;
	unsigned int ArrowID;

	Neumont::ShapeData Vector;
	unsigned int VectorID;

	Neumont::ShapeData Sphere;
	unsigned int SphereID;

	Neumont::ShapeData Cube;
	unsigned int CubeID;

	Neumont::ShapeData Plane;
	unsigned int PlaneID;

	Neumont::ShapeData Teapot;
	unsigned int teapotID;

	Neumont::ShapeData Cylinder;
	unsigned int CylinderID;

	Neumont::ShapeData CubeWireFrame;
	unsigned int CubeWireFrameID;

	unsigned int NormalShader;

	void AddVector(const glm::vec3& start, const glm::vec3& end);
	void AddCross(const glm::vec3& position);
	void AddArrow(const glm::vec3& position, bool opaque);
	void Add3DVector(const glm::vec3& start, const glm::vec3& end);
	void AddSphere(const glm::vec3& position, bool opaque);
	void AddSphere(const glm::vec3& position, bool opaque, const char *vertexShader, const char *fragShader);
	void AddTeapot(const glm::vec3& position, bool opaque);
	void AddCube(const glm::vec3& position, bool opaque);
	void AddCube(const glm::vec3& position, bool opaque, const char *vertexShader, const char *fragShader);
	void AddPlane(const glm::vec3& position);
	void AddPlane(const glm::vec3& position, const char *vertexShader, const char *fragShader);
	void AddCylinder(const glm::vec3& position, bool opaque);
	void AddCubeWireFrame(const glm::vec3& position);

	void AddMayaShape(const char* fileName, glm::vec3& position, bool opaque);
	void AddMayaShape(const char* fileName, glm::vec3& position, bool opaque, const char *vertexShader, const char *fragShader);

	glm::mat4 direction(const glm::vec3& originalOrientation, const glm::vec3& newOrientation);
	void update();

	static DebugShapes& getInstance();
	void debugUpdate();
	void createRender();
	void initializeDrawData();
	void initializeDrawData(const char *vertexShader, const char *fragShader);

private:
	DebugShapes();
	DebugShapes(const DebugShapes&);
	DebugShapes& operator=(const DebugShapes&);
	static DebugShapes DebugShapeInstance;
};

#define myDebugShape DebugShapes::getInstance();

#endif