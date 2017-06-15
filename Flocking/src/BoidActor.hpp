#ifndef BOID_ACTOR_HPP
#define BOID_ACTOR_HPP

#include <dusk/Dusk.hpp>
#include <random>
#include "Constants.hpp"
#include "Octant.hpp"

class BoidActor : public dusk::Actor
{
public:

    BoidActor(dusk::Scene * scene,
              const std::string& name,
              unsigned int group,
              glm::vec4 color);

    void Update(const dusk::Event& event) override;

    void SetVelocity(const glm::vec3& vel) { _velocity = vel; }
    glm::vec3 GetVelocity() const { return _velocity; }

    void SetAcceleration(const glm::vec3& acc) { _acceleration = acc; }
    glm::vec3 GetAcceleration() const { return _acceleration; }

    unsigned int GetGroup() const { return _group; }

private:

    std::random_device _rand;

    glm::vec3 _velocity;
    glm::vec3 _acceleration;

    unsigned int _group;

    unsigned int _quadIndex;

};

extern OctantNode<BoidActor *> _RootNodes[BOID_GROUPS];

#endif // BOID_ACTOR_HPP
