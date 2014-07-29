#ifndef PARTICLE_H
#define PARTICLE_H

namespace Engine
{
struct Particle
{
public:
	Particle();
	void Initialize(Vector2 p, Vector2 v);
	void Draw(Core::Graphics& graphics);
	void Update(float dt);
	Engine::Vector2 position;
	Engine::Vector2 velocity;
	Core::RGB color;
};

}
#endif