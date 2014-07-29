#pragma once
#ifndef DEBUG_DUAL_SCREEN
#define DEBUG_DUAL_SCREEN
#include "Camera.h"
#include "Renderer.h"
#include "ExportHeader.h"

class __declspec( dllexport ) DualScreen
{

public:
	Camera secondCamera;
	void paintGL(int width, int height, Renderer *const render);
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
};

#endif