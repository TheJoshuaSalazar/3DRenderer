#include "Particle.h"
#include "Vector2.h"
#include "Core.h"
#include "math.h"
#include <cstdlib>

using Engine::Vector2;
using Core::Graphics;
using Core::RGB;
using Engine::Particle;

Particle::Particle()
{
}

void Particle::Initialize(Vector2 p, Vector2 v)
{
	position = p;
	velocity = v;
}

void Particle::Draw(Graphics& graphics)
{
	//Draws 2 pixal particles in random colors 
	graphics.DrawLine(position.getX(),position.getY(),position.getX()+2,position.getY()+2);
	graphics.SetColor(RGB(rand()%255+1,rand()%255+1,rand()%255+1));
}

void Particle::Update(float dt)
{
	position = position + velocity * dt;
}