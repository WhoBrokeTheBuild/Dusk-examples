#ifndef FLOCKING_COMPONENT_HPP
#define FLOCKING_COMPONENT_HPP

#include <dusk/Dusk.hpp>
#include <random>
#include "BoidActor.hpp"

class FlockingComponent : public dusk::Component
{
public:

    FlockingComponent(dusk::Actor * parent);

    virtual ~FlockingComponent();

    inline bool Load() override
    { return true; }

    inline void Free() override
    { }

    void Update(const dusk::Event& event);

private:

    std::random_device _rand;

    double _neighborsTimeout = 0;

    std::vector<BoidActor *> _cohesionNeighbors;
    std::vector<BoidActor *> _separationNeighbors;
    std::vector<BoidActor *> _alignmentNeighbors;

};

const float SPEED = 1000.0f;
const float BLENDING_SPEED = 50.0f;

#endif // FLOCKING_COMPONENT_HPP
