#include <glm\gtx\transform.hpp>
#include "DebugDualScreen.h"
#include <iostream>
#include "Renderer.h"
#include <ShapeData.h>
#include <QtGui\QKeyEvent>
#include <QtGui\QMouseEvent>
#include <Qt\qdebug.h>
#include <noise\noise.h>
#include "Clock.h"

DualScreen dualScreen;
typedef unsigned int uint;

Renderer::Renderer()
{
	Logger::getInstance().logInitialize("Renderer has been initialize", "Renderer", 16); 
	nextRenderableIndex = 0;
	nextGeometryIndex = 0;
	nextShaderInfoIndex = 0;

	nextVertexBufferIndex = 0;
	nextIndexBufferIndex = 0;

	currentIndexOffset = 0;
	currentVertexOffset = 0;

	nextUniform = 0;

	ambientUniform = 1;
	lightColorUniform =1;
	LightDirectionUniform =1;
	HalfVectorUniform =1;
	shininessUniform =1;
	strengthUniform =1;
	shapeColorUniform =1;

	twoScreen = false;
	notMove = false;
	shadows = false;
	
	specularOn = true;
	diffuseOn = true;
	otherTexture = true;
	xLight = 0;
	zLight = 0;
	yLight = 0;
	totalTime = 0;

	noise3DTexSize = 128;
	noise3DTexName = 0;
}

void Renderer::initializeGL()
{
	glewInit();
	glClearColor(0,0,0,1);
	setMinimumHeight(800);

	generateBuffers();

	setFocusPolicy(Qt::StrongFocus);	
	glDepthFunc(GL_LESS);
	resize(1200, 900);

	//start the timer and connects the timer to the timeUpdate which updates every tick
	timer.start(0);
	myClock.initialize();
	myClock.start();
	//Logger::getInstance().logInitialize("Timer has been initialize", "Renderer", 47);
	emit renderInitialized();
	connect(&timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
}

Renderer::~Renderer()
{

}

void Renderer::UniformInfo::initialize(uniformtype formatIn, const char* nameIn, uint dataSize, void* dataIn)
{
	assert(UNIFORMS_MAX_CAPACITY >= strlen(nameIn));
	memset(name, '\0', UNIFORMS_MAX_CAPACITY);
	memcpy(name,nameIn, strlen(nameIn));

	format = formatIn;
	memcpy(data, dataIn, dataSize);
}

void Renderer::UniformInfo::updateInfo(uint dataSize, void* dataIn)
{
	memcpy(data, dataIn, dataSize);
}

void Renderer::shutdown()
{
	//Logger::getInstance().logInitialize("Renderer was shutdown", "Renderer.cpp", 69);
	//Logger::getInstance().shutdown();
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);
}

void Renderer::paintGL()
{
	if(!twoScreen)
	{
		if(!shadows)
			paintShadows(myCamera);

		paintScreen(myCamera);	

		if(!shadows)
			debugShadowMap();
	}
	else
	{
		dualScreen.paintGL(width(), height(), this);
	}
	myClock.nextFrame();
}

//Gets a uniform from and returns its location
uint Renderer::getUniforms(GLuint programId, GLchar*name)
{
	GLuint uniformLocation = glGetUniformLocation(programId, name);
	return uniformLocation;
}

//Enables both the position and color
void Renderer::enableAttrib(GLuint vertexBufferOffset)
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 
			BUFFER_OFFSET(vertexBufferOffset + Neumont::Vertex::POSITION_OFFSET));
		
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,4, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 
			BUFFER_OFFSET(vertexBufferOffset + Neumont::Vertex::COLOR_OFFSET));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 
		BUFFER_OFFSET(vertexBufferOffset + Neumont::Vertex::NORMAL_OFFSET));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE,
		BUFFER_OFFSET(vertexBufferOffset + Neumont::Vertex::UV_OFFSET));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 
		BUFFER_OFFSET(vertexBufferOffset + Neumont::Vertex::TANGENT_OFFSET));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, 
		BUFFER_OFFSET(vertexBufferOffset + Neumont::Vertex::BITANGENT_OFFSET));
}

void Renderer::timeUpdate()
{
	if(!notMove)
	{
		myCamera.moveBack();
		notMove = true;
	}

	repaint();
}

//generates 1 vertex and index buffer
void Renderer::generateBuffers()
{
 	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &indexBufferID);

	createCubeMap();
	createFrameBuffer();
	make3DNoiseTexture();
}

void Renderer::createCubeMap()
{
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(4, texIDs);
	//glGenTextures(1, &cubeBeachMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texIDs[0]);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, 10, GL_RGBA8, 2048, 2048);

	char* beachTexName = "posx.PNG";
	QImage beachTimg = QGLWidget::convertToGLFormat(QImage(beachTexName, "PNG"));
	beachTexture_data[0] = beachTimg;

	beachTexName = "negx.PNG";
	beachTimg = QGLWidget::convertToGLFormat(QImage(beachTexName, "PNG"));
	beachTexture_data[1] = beachTimg;

	beachTexName = "negy.PNG";
	beachTimg = QGLWidget::convertToGLFormat(QImage(beachTexName, "PNG"));
	beachTexture_data[2] = beachTimg;

	beachTexName = "posy.PNG";
	beachTimg = QGLWidget::convertToGLFormat(QImage(beachTexName, "PNG"));
	beachTexture_data[3] = beachTimg;

	beachTexName = "posz.PNG";
	beachTimg = QGLWidget::convertToGLFormat(QImage(beachTexName, "PNG"));
	beachTexture_data[4] = beachTimg;
	
	beachTexName = "negz.PNG";
	beachTimg = QGLWidget::convertToGLFormat(QImage(beachTexName, "PNG"));
	beachTexture_data[5] = beachTimg;

	for(int face = 0; face < 6; face++)
	{
		GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
		glTexSubImage2D(target, 0, 0, 0, beachTexture_data[face].width(),
			beachTexture_data[face].height(), GL_RGBA, GL_UNSIGNED_BYTE, 
			beachTexture_data[face].bits());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Renderer::createFrameBuffer()
{
	FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width(), height(), 0, 
		GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDrawBuffer(GL_NONE);

	if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		qDebug() << "Did not Work";
	
	//glGenTextures(3, texIDs);
	
	char * texName = "RugNormal.PNG";
	QImage timg = QGLWidget::convertToGLFormat(QImage(texName, "PNG"));
	
	glActiveTexture(GL_TEXTURE2);	 
	glBindTexture(GL_TEXTURE_2D, texIDs[2]);
	 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(), timg.height(), 
				0, GL_RGBA, GL_UNSIGNED_BYTE, timg.bits());
	 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	texName = "Blue.PNG";
	QImage normalTwoImage =
		QGLWidget::convertToGLFormat(QImage(texName, "PNG"));

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texIDs[3]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, normalTwoImage.width(),
		normalTwoImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, normalTwoImage.bits());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

//Adds vertex shape data to the vertex buffer
uint Renderer::addToOpenVertexBuffer(void* verticeData, GLuint vertexBufferSize, GeometryInfo shape)
{	
	if(vertexBufferInfos.size() == vertexBufferInfos.max_size())
		Logger::getInstance().InternaLog("Index Buffer size reached max", "Renderer", 144);

	vertexBufferInfos.push_back(vertexBufferID);

	unsigned int vertexBuffer = vertexBufferInfos[nextVertexBufferIndex];

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, BUFFERSIZE, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, shape.vertexBufferOffset, vertexBufferSize, verticeData);

	if(currentVertexOffset >= BUFFERSIZE)
	{
		return nextVertexBufferIndex++;
	}

	return nextVertexBufferIndex;
}

//Adds index shape data to the index buffer
uint Renderer::addToOpenIndexBuffer(unsigned short* indices, GLuint indexBufferSize, GeometryInfo shape)
{
	if(indexBufferInfos.size() == indexBufferInfos.max_size())
		Logger::getInstance().InternaLog("Index Buffer size reached max", "Renderer", 144);

	indexBufferInfos.push_back(indexBufferID);
	unsigned int indexBuffer = indexBufferInfos[nextIndexBufferIndex];
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, BUFFERSIZE, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferOffset, indexBufferSize, indices);

	if(currentIndexOffset >= BUFFERSIZE)
	{
		return nextIndexBufferIndex++;
	}	
	return nextIndexBufferIndex;
}

//Takes in the vertex and indices data from the geometry that you want to draw
uint Renderer::addGeometry(void* verticeData, GLuint vertexBufferSize ,
							unsigned short* indices, uint numIndices, 
							GLchar glRender, GLenum depth)
{
	if(nextGeometryIndex >= MAX_NUM_GEOMETRIES)
			Logger::getInstance().InternaLog("nextGeometryIndex < MAX_NUM_GEOMETRIES", "Renderer", 170);
	assert(nextGeometryIndex < MAX_NUM_GEOMETRIES);

	GeometryInfo& geometry = geometries[nextGeometryIndex];

	geometry.indexBufferOffset = currentIndexOffset;
	geometry.indexBufferSize  = numIndices * sizeof(ushort);
	geometry.numIndices = numIndices;
	geometry.vertexBufferOffset = currentVertexOffset;
	geometry.vertexBufferSize = vertexBufferSize;
	geometry.glRender = glRender;
	geometry.depth = depth;

	addToOpenVertexBuffer(verticeData, geometry.vertexBufferSize, geometry);
	addToOpenIndexBuffer(indices, geometry.indexBufferSize, geometry);

	currentVertexOffset += geometry.vertexBufferSize;
	currentIndexOffset += geometry.indexBufferSize;

	return nextGeometryIndex++;
}

//Takes in a vertex and frag shader to compile and link to the program
uint Renderer::addShader(const char* vertexShaderFileName, const char* fragShaderFileName)
{
	if(nextShaderInfoIndex >= MAX_NUM_SHADER)
		Logger::getInstance().InternaLog("Added too many shaders", "Renderer", 190);
	assert(nextShaderInfoIndex < MAX_NUM_SHADER);
	ShaderInfo& shader = shaderInfos[nextShaderInfoIndex++];
	
	shader.vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	shader.fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	shader.glProgramID = glCreateProgram();
	ProgramID = shader.glProgramID;

	compileShader(vertexShaderFileName, shader.vertexShaderID);
	compileShader(fragShaderFileName, shader.fragShaderID);

	glAttachShader(ProgramID, shader.vertexShaderID);
	glAttachShader(ProgramID, shader.fragShaderID);
	glLinkProgram(ProgramID);

	return ProgramID;
}

//Connects the geometry, location, and shader to the shape that going to be renderer
Renderer::Renderable* Renderer::addRenderable(uint geometryIndex, const glm::mat4& place, 
								uint shaderProgramID, bool opaque)
{
	if(nextRenderableIndex >= MAX_NUM_RENDERABLES)
		Logger::getInstance().InternaLog("Added too many Renderables", "Renderer", 220);
	assert(nextRenderableIndex < MAX_NUM_RENDERABLES);
	Renderable& render = renderables[nextRenderableIndex++];
	render.geometryID = geometryIndex;
	render.place = place;
	render.shaderProgramID = shaderProgramID;
	render.opaque = opaque;
	
	return &render;
}

Renderer::UniformInfo* Renderer::Renderable::addUniform(uniformtype formatIn, const char* name,
										uint dataSize, void* data)
{
	if(numUniforms >= UNIFORMS_MAX_CAPACITY)
		Logger::getInstance().InternaLog("Reached max uniforms", "Renderer", 234);

	assert(numUniforms < UNIFORMS_MAX_CAPACITY);

	uniforms[numUniforms].initialize(formatIn, name, dataSize, data);
	return &(uniforms[numUniforms++]);
}

//Draws every object according to their mvp location. 
void Renderer::paintScreen(Camera ScreenCamera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,width(),height());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 camera = ScreenCamera.camera;
	glm::mat4 projection = glm::perspective(60.0f,(float)width()/height(), 0.1f,100.0f);
	glm::mat4 model;
	glm::mat4 mvp;
	uint shader;

	glm::mat4 viewPerspective = projection * camera;
	glm::mat3 ModelView3x3Matrix;

	for(unsigned int i = 0; i < nextRenderableIndex; i++)
	{	
		shader = (renderables[i].shaderProgramID); 

		glUseProgram(shader);

		if(!renderables[i].visible)
			continue;

		if(renderables[i].opaque)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, alpha);
		}

		if(geometries[renderables[i].geometryID].depth)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		GeometryInfo& geometry = geometries[renderables[i].geometryID];

		enableAttrib(geometry.vertexBufferOffset);

		model = renderables[i].place;
		mvp = projection * camera * model;
		glm::mat4 cameraRotation = projection  * ScreenCamera.getRotationView() * model;
		glm::mat4 viewModel =  model;
		viewPerspective = ScreenCamera.getRotationView() * projection ;
		ModelView3x3Matrix = glm::mat3(model);

		glm::mat4 biasMatrix = glm::mat4(glm::vec4(0.5f, 0, 0, 0),
								 glm::vec4(0, 0.5f, 0, 0),
								 glm::vec4(0, 0, 0.5f, 0),
								 glm::vec4(0.5f, 0.5f, 0.5f, 1));

		glm::vec3 lightInvDir = glm::vec3(renderables[0].place[3][0], renderables[0].place[3][1], renderables[0].place[3][2]);
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4 depthModelMatrix = model;

		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
		
		glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

		enableUniforms(i, mvp, shader);

		uint depthMVPUniform = getUniforms(shader, "depthBiasMVP");
		glUniformMatrix4fv(depthMVPUniform, 1, GL_FALSE, &depthBiasMVP[0][0]);

		uint ModelViewUniform = getUniforms(shader, "model");
		glUniformMatrix4fv(ModelViewUniform, 1, GL_FALSE, &model[0][0]);

		uint ViewUniform = getUniforms(shader, "ViewMatrix");
		glUniformMatrix4fv(ViewUniform, 1, GL_FALSE, &camera[0][0]);

		uint ViewProjectionUniform = getUniforms(shader, "viewProjection");
		glUniformMatrix4fv(ViewProjectionUniform, 1, GL_FALSE, &viewPerspective[0][0]);

		uint ViewModelUniform = getUniforms(shader, "ViewModel");
		glUniformMatrix4fv(ViewModelUniform, 1, GL_FALSE, &viewModel[0][0]);

		uint EyeDirectionUniform = getUniforms(shader, "EyeDirection");
		glUniform3f(EyeDirectionUniform, myCamera.getView().x, myCamera.getView().y, myCamera.getView().z);

		glActiveTexture(GL_TEXTURE0);
		uint cubeMapLocation = getUniforms(shader, "tex");
		glUniform1i(cubeMapLocation, 0);

		if(!shadows)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthTexture);
			uint shadowMapUniformLocation = getUniforms(shader, "shadowMap");
			glUniform1i(shadowMapUniformLocation, 1);
		}
		else
		{
			uint shadowMapUniformLocation = getUniforms(shader, "shadowMap");
			glUniform1i(shadowMapUniformLocation, 0);
		}

		uint doNormalLocation = getUniforms(shader, "doNormal");
		glUniform1f(doNormalLocation, !doNormal);

		uint doDiffuseLocation = getUniforms(shader, "doDiffuse");
		glUniform1f(doDiffuseLocation, !doDiffuse);

		uint normalMapTexLocation = getUniforms(shader, "NormalMap");
		glUniform1i(normalMapTexLocation, 2);

		uint diffuseMapTexLocation = getUniforms(shader, "DiffuseMap");
		glUniform1i(diffuseMapTexLocation, 3);

		uint noiseTexUniformLocation = getUniforms(shader, "Noise");
		glUniform1i(noiseTexUniformLocation, 6);

		uint noiseWoodTexUniformLocation = getUniforms(shader, "NoiseWood");
		glUniform1i(noiseWoodTexUniformLocation, 6);

		uint particleTexUniformLocation = getUniforms(shader, "particleTex");
		glUniform1i(particleTexUniformLocation, 6);

		glm::mat4 slice = glm::mat4() * glm::translate(glm::vec3(-0.5)) * glm::scale(glm::vec3(20)) * glm::rotate(40.0f, glm::vec3(1));
		uint sliceUniformLocation = getUniforms(shader, "Slice");
		glUniformMatrix4fv(sliceUniformLocation, 1, GL_FALSE, &slice[0][0]);

		glDrawElements(geometry.glRender, geometry.numIndices,
			GL_UNSIGNED_SHORT, BUFFER_OFFSET(geometry.indexBufferOffset));
	}
}

void Renderer::paintShadows(Camera ScreenCamera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glViewport(0,0,width(),height());
	//glViewport(0,-50,width()/8,height()/4);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 camera = ScreenCamera.camera;
	glm::mat4 projection = glm::perspective(60.0f,(float)width()/height(), 0.1f,100.0f);
	glm::mat4 model;
	uint shader;

	for(unsigned int i = 0; i < nextRenderableIndex; i++)
	{	
		shader = 3;//(renderables[i].shaderProgramID); 
		if(!renderables[i].visible)
			continue;

		if(geometries[renderables[i].geometryID].depth)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		GeometryInfo& geometry = geometries[renderables[i].geometryID];

		glUseProgram(shader); 
		enableAttrib(geometry.vertexBufferOffset);

		model = renderables[i].place;
		glm::mat4 mvp = projection * camera * model;
		enableUniforms(i, mvp, shader);

		glm::vec3 lightInvDir = glm::vec3(renderables[0].place[3][0], renderables[0].place[3][1], renderables[0].place[3][2]);
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4 depthModelMatrix = model;

		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
		
		uint depthMVPUniform = getUniforms(shader, "depthBiasMVP");
		glUniformMatrix4fv(depthMVPUniform, 1, GL_FALSE, &depthMVP[0][0]);

		glDrawElements(geometry.glRender, geometry.numIndices,
			GL_UNSIGNED_SHORT, BUFFER_OFFSET(geometry.indexBufferOffset));
	}
}

void Renderer::debugShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,-50,width()/8,height()/4);

	glm::mat4 model;
	uint shader = 3;

	for(unsigned int i = 0; i < nextRenderableIndex; i++)
	{		
		if(!renderables[i].visible)
			continue;

		if(geometries[renderables[i].geometryID].depth)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		GeometryInfo& geometry = geometries[renderables[i].geometryID];

		glUseProgram(shader); 
		enableAttrib(geometry.vertexBufferOffset);

		model = renderables[i].place;

		glm::vec3 lightInvDir = glm::vec3(renderables[0].place[3][0], renderables[0].place[3][1], renderables[0].place[3][2]);
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-5, 5, -5, 5, -5, 10);
		glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4 depthModelMatrix = model;

		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
		
		uint depthMVPUniform = getUniforms(shader, "depthBiasMVP");
		glUniformMatrix4fv(depthMVPUniform, 1, GL_FALSE, &depthMVP[0][0]);

		glDrawElements(geometry.glRender, geometry.numIndices,
			GL_UNSIGNED_SHORT, BUFFER_OFFSET(geometry.indexBufferOffset));
	}
}

void Renderer::enableUniforms(uint i, glm::mat4 mvp, uint shader)
{
	uint mvpUniform = getUniforms(shader, "mvp");
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvp[0][0]);

	glm::mat4 modelToWorld = renderables[i].place;

	GLuint modelToWorldToNormalUniformLocation = getUniforms(shader, "modelToWorldNormalMatrix");
	glm::mat3 temp = glm::mat3(modelToWorld);
	glUniformMatrix3fv(modelToWorldToNormalUniformLocation, 1, GL_FALSE, &temp[0][0]);

	glm::vec3 ambientLight = glm::vec3(0.9);
	if(diffuseOn == false)
		ambientLight = glm::vec3(0.0);
	uint ambientUniform = getUniforms(shader, "Ambient");
	glUniform3f(ambientUniform, ambientLight.x, ambientLight.y, ambientLight.z);

	glm::vec3 lightColor = glm::vec3(0.4, 0.4, 0.4);
	uint lightColorUniform = getUniforms(shader, "LightColor");
	glUniform3f(lightColorUniform,  lightColor.x, lightColor.y, lightColor.z);

	glm::vec3 lightPlace = glm::vec3(renderables[0].place[3][0], renderables[0].place[3][1], renderables[0].place[3][2]);
	uint lightPositionUniform = getUniforms(shader, "LightPosition");
	glUniform3f(lightPositionUniform,  lightPlace.x, lightPlace.y, lightPlace.z);

	uint lightDirectionUniform = getUniforms(shader, "LightDirection");
	glUniform3f(lightDirectionUniform,  lightPlace.x, lightPlace.y, lightPlace.z);

	glm::vec3 halfVector = glm::vec3(0.4);
	uint halfVectorUniform = getUniforms(shader, "HalfVector");
	glUniform3f(halfVectorUniform,  halfVector.x, halfVector.y, halfVector.z);

	uint ShininessUniform = getUniforms(shader, "Shininess");
	glUniform1f(ShininessUniform, 0.5f);

	GLfloat strength = 0.2f;
	uint strengthUniform = getUniforms(shader, "Strength");
	glUniform1f(strengthUniform,  strength);

	uint meterialAmbientUniform = getUniforms(shader, "materialAmbient");
	glUniform3f(meterialAmbientUniform,  0.9f, 0.9f, 0.9f);

	GLfloat specOn = 0;
	if(specularOn == false)
		specOn = 1;
	uint specularOnLocation = getUniforms(shader, "specularOn");
	glUniform1f(specularOnLocation, specOn);

	glm::vec4 shapeColor = glm::vec4(shapeColorUniform);
	uint shapeColorUniform = getUniforms(shader, "shapeColor");
	glUniform4f(shapeColorUniform, 0.4f, 0.4f, 0.4f, 1);

	uint skyColorUniform = getUniforms(shader, "SkyColor");
	glUniform4f(skyColorUniform,  0.3f, 0.3f, 0.9f, 1);

	uint cloudColorUniform = getUniforms(shader, "CloudColor");
	glUniform4f(cloudColorUniform, 1, 1, 1, 1);

	uint lowThresholdOnLocation = getUniforms(shader, "LowThreshold");
	glUniform1f(lowThresholdOnLocation, disintegration);

	uint highThresholdOnLocation = getUniforms(shader, "HighThreshold");
	glUniform1f(highThresholdOnLocation, 0.999f);

	int doDisplacement = 1;
	if(vertexDisplacement)
		doDisplacement = 0;
	int timeOffset = 1;
	if(!shadows)
		timeOffset = 2;
	totalTime += (myClock.timeElapsedLastFrame()/timeOffset) * doDisplacement;
	uint timeUniformLocation = getUniforms(shader, "time");
	glUniform1f(timeUniformLocation, totalTime);
	
	uint waveNumberUniformLocation = getUniforms(shader, "waveNumber");
	glUniform1f(waveNumberUniformLocation, 1);

	uint velocityUniformLocation = getUniforms(shader, "velocity");
	glUniform1f(velocityUniformLocation, 1);
	
	uint ampUniformLocation = getUniforms(shader, "amp");
	glUniform1f(ampUniformLocation, 1);

	uint alphaSLiderUniform = getUniforms(shader, "alpha");
	glUniform1f(alphaSLiderUniform, alphaSlider);
}

void Renderer::make3DNoiseTexture()
{
	int width = 128;
	int height = 128;
	noise::module::Perlin perlinNoise;
	
	perlinNoise.SetFrequency(4.0);
	GLubyte *data = new GLubyte[width * height * 4];

	double xRange = 1.0;
	double yRange = 1.0;
	double xFactor = xRange / width;
	double yFactor = yRange / height;

	for (int oct = 0; oct < 4; oct++)
	{
		perlinNoise.SetOctaveCount(oct + 1);

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				double x = xFactor * i;
				double y = yFactor * j;
				double z = 0.0;

				float val = 0.0f;
				double a, b, c, d;

				a = perlinNoise.GetValue(x, y, z);
				b = perlinNoise.GetValue(x + xRange, y, z);
				c = perlinNoise.GetValue(x, y + yRange, z);
				d = perlinNoise.GetValue(x + xRange, y + yRange, z);

				double xmin = 1.0 - x / xRange;
				double ymin = 1.0 - y / yRange;
				double x1 = glm::mix(a, b, xmin);
				double x2 = glm::mix(x, d, xmin);

				val = glm::mix(x1, x2, ymin);
				val = (val + 1.0f) * 0.5f;

				val = val > 1.0 ? 1.0 :val;
				val = val < 0.0 ? 0.0 :val;

				data[((j * width + i) * 4) + oct] = (GLubyte) (val * 255.0f);
			}
		}
	}

	glGenTextures(1, &noise3DTexName);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, noise3DTexName);

	glTexImage2D(GL_TEXTURE_2D , 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Renderer::makeParticles()
{
	uint nParticles = 10;
	GLuint initVel = 0;

	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];

	for (GLuint i = 0; i < nParticles; i++)
	{
		float randFloat = (float)rand()/(float)(RAND_MAX/1);

		theta = glm::mix(0.0f, (float)PI / 6.0f, randFloat);
		phi = glm::mix(0.0f, (float)(2 * PI), randFloat);

		v.x = sinf(theta )* cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(1.25f, 1.5f, randFloat);

		v = v * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[4 * i + 2] = v.z;
	}

	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * 3 * sizeof(float), data);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	GLuint startTime = 0;
	float *particleData = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;

	for(unsigned int i = 0; i <nParticles; i++)
	{
		particleData[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), particleData);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
}

//Reads in the shader file
void Renderer::compileShader(const char* fileName, GLuint shaderID)
{
	QFile input(fileName);
	if(!input.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<<"File failed to Open: " <<fileName;
		Logger::getInstance().InternaLog("File failed to Shader" , "Renderer", 272);
		exit(1);
	}

	QTextStream stream(&input);
	QString shaderCodeText = stream.readAll();
	input.close();

	std::string temp = shaderCodeText.toStdString();
	const char* shaderSource = temp.c_str();
	const char* buf[1];
	buf[0] = shaderSource;
	glShaderSource(shaderID, 1, buf, NULL);
	glCompileShader(shaderID);
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

	if(status!=GL_TRUE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* buffer = new char [infoLogLength];
		GLsizei bitBucket;
		glGetShaderInfoLog(shaderID, infoLogLength, &bitBucket, buffer);
		std::cout<<buffer<<std::endl;
		delete[] buffer;
	}
}

//Controls the camera view. Not inverted controls
void Renderer::mouseMoveEvent(QMouseEvent *event)
{	
	if(!twoScreen)
	{
		myCamera.mouseMoveEvent(event);
	}
	else
		dualScreen.mouseMoveEvent(event);

}

//use "W,A,S,D,R,F" to move the camera
void Renderer::keyPressEvent(QKeyEvent *event)
{	
	if(!twoScreen)
		myCamera.keyPressEvent(event);
	else
		dualScreen.keyPressEvent(event);

	switch(event->key()) 
	{
		case Qt::Key_Right:
			renderables[0].place = glm::translate(renderables[0].place, glm::vec3(0.1, 0, 0));
			break;
		case Qt::Key_Left:
			renderables[0].place = glm::translate(renderables[0].place, glm::vec3(-0.1, 0, 0));
			break;
		case Qt::Key_Down:		
			renderables[0].place = glm::translate(renderables[0].place, glm::vec3(0, -0.1, 0));
			break;
		case Qt::Key_Up:
			renderables[0].place = glm::translate(renderables[0].place, glm::vec3(0, 0.1, 0));
			break;
		case Qt::Key_Comma:
			renderables[0].place = glm::translate(renderables[0].place, glm::vec3(0, 0, 0.1));
			break;
		case Qt::Key_Period:
			renderables[0].place = glm::translate(renderables[0].place, glm::vec3(0, 0, -0.1));
			break;
		default:
			event->ignore();
			break;
	}
}