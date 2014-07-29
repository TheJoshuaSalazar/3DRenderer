#pragma once
#ifndef RENDERER
#define RENDERER
#include "ExportHeader.h"
#include "Camera.h"
#include <GL\glew.h>
#include "glm\glm.hpp"
#include "Logger.h"
#include <QtOpenGL\qglwidget>
#include "Qt\qtimer.h"
using std::vector;

typedef unsigned int uint;

class ENGINE_SHARED Renderer : public QGLWidget
{
	Q_OBJECT
		QTimer timer;

public:
	bool twoScreen;
	bool notMove;
	bool normalSky;
	GLuint FramebufferName;
	GLuint depthTexture;
	GLuint cubeMap;
	GLuint cubeBeachMap;
	QImage texture_data[6];
	QImage beachTexture_data[6];

	int noise3DTexSize;
	GLuint noise3DTexName;
	GLubyte *noise3DTexPtr;
	float disintegration;
	float alphaSlider;
	bool alpha;
	bool shadows;
	bool doNormal;
	bool doDiffuse;
	float totalTime;
	bool vertexDisplacement;

	float xLight;
	float yLight;
	float zLight;
	bool specularOn;
	bool diffuseOn;
	bool otherTexture;
	GLuint texIDs[3];

	uint ProgramID;

	Camera myCamera;

	static const uint UNIFORMS_MAX_CAPACITY = 100;
	static const uint UNIFORMNAME_MAX_CAPACITY = 64;
	static const uint UNIFORMDATA_MAX_CAPACITY = 64;

	static const uint MAX_NUM_UNIFORMS = 100;
	uint uniformBuffer[MAX_NUM_UNIFORMS];
	uint nextUniform;

	float ambientUniform;
	float lightColorUniform;
	float LightDirectionUniform;
	float HalfVectorUniform;
	float shininessUniform;
	float strengthUniform;
	float shapeColorUniform;

	ENGINE_SHARED enum uniformtype
	{
		GLMVEC3,
		GLMVEC4,
		GLMMAT3,
		GLMMAT4,
		GLMMAT4_AFFECTEDBYVIEWPERSPECTIVE,
		GLMMAT4_AFFECTEDBYVIEW,
		FLOAT,
		INT
	};

	ENGINE_SHARED struct UniformInfo
	{
		uniformtype format;
		char name[UNIFORMNAME_MAX_CAPACITY];
		char data[UNIFORMDATA_MAX_CAPACITY];

		void updateInfo(uint datasize, void* data);
		void initialize(uniformtype formatIn, const char* nameIn, uint dataSize, void* dataIn);
	};

	ENGINE_SHARED struct Renderable
	{
		uint geometryID;
		glm::mat4 place;
		uint shaderProgramID;
		bool visible;
		bool depth;
		bool opaque;
		uint numUniforms;
		UniformInfo uniforms[UNIFORMS_MAX_CAPACITY];
		Renderable() : visible(true){numUniforms = 0;}

		void initialize(uint geometryID, uint programIndex, bool visible, bool usesDepthTest, bool isOpaque);
		UniformInfo* addUniform(uniformtype format, const char* name, uint dataSize, void* data);
	};	

	static const uint MAX_NUM_RENDERABLES = 100;
	Renderable renderables[MAX_NUM_RENDERABLES];
	uint nextRenderableIndex;

	GLuint vertexBufferID;
	GLuint indexBufferID;
	GLuint vertexShaderID;
	GLuint fragShaderID;
	GLuint programID;
	GLuint numIndices;

	ENGINE_SHARED struct GeometryInfo
	{
		GLuint vertexBufferSize;
		GLuint vertexBufferOffset;
		GLuint indexBufferSize;
		GLuint indexBufferOffset;
		GLuint numIndices;
		GLchar glRender;
		GLenum depth;
		GLenum opaque;
	};

	static const uint MAX_NUM_GEOMETRIES = 100;
	GeometryInfo geometries[MAX_NUM_GEOMETRIES];
	uint nextGeometryIndex;

	struct ShaderInfo
	{
		GLuint glProgramID;
		GLuint vertexShaderID;
		GLuint fragShaderID;
	};

	static const uint MAX_NUM_SHADER = 100;
	ShaderInfo  shaderInfos[MAX_NUM_SHADER];
	uint nextShaderInfoIndex;

	static const uint BUFFERSIZE = 0x100000;

	std::vector<uint> vertexBufferInfos;
	uint nextVertexBufferIndex;
	GLuint currentVertexOffset;
	
	std::vector<uint> indexBufferInfos;
	uint nextIndexBufferIndex;
	GLuint currentIndexOffset;

	Renderer();
	~Renderer();
	uint addToOpenVertexBuffer(void* verticeData, GLuint vertexBufferSize, GeometryInfo shape);
	uint addToOpenIndexBuffer(unsigned short* indices, uint numIndices, GeometryInfo shape); 
	uint addGeometry(void* verticeData, GLuint vertexBufferSize, 
					unsigned short* indices, uint numIndices,
					GLchar glRenderer, GLenum depth);
	uint addShader(const char* vertexShaderFileName, const char* fragShaderFileName);
	Renderable* addRenderable(uint geometryIndex, const glm::mat4& where,
						uint shaderProgramID, bool opaque);
	void enableUniforms(uint i, glm::mat4 mvp, uint shader);
	void createFrameBuffer();
	void compileShader(const char* fileName, GLuint shaderID);
	void generateBuffers();
	void enableAttrib(GLuint vertexBufferOffset);
	uint getUniforms(GLuint programId, GLchar*name);
	void paintScreen(Camera ScreenCamera);
	void disableMethod();
	void paintShadows(Camera ScreenCamera);
	void debugShadowMap();
	void createCubeMap();
	void make3DNoiseTexture();
	void makeParticles();

protected:
	void initializeGL();
	void shutdown();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);

signals:
	void renderInitialized();

public slots:
	void timeUpdate();

};

#endif