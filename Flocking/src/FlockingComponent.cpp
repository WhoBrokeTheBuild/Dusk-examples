#include "FlockingComponent.hpp"

#include "BoidActor.hpp"

const double NEIGHBORS_MAX_TIMEOUT = 2.0;

const double COHESION_RADIUS   = 80;
const double SEPARATION_RADIUS = 50;
const double ALIGNMENT_RADIUS  = 80;

const float COHESION_WEIGHT   = 0.5;
const float SEPARATION_WEIGHT = 0.5;
const float ALIGNMENT_WEIGHT  = 0.6;

FlockingComponent::FlockingComponent(dusk::Actor * parent)
    : dusk::Component(parent)
{
    GetActor()->AddComponentType<FlockingComponent>(this);
    GetActor()->AddEventListener((dusk::EventID)dusk::Actor::Events::UPDATE,
        this, &FlockingComponent::Update);

    _neighborsTimeout = NEIGHBORS_MAX_TIMEOUT;
}

FlockingComponent::~FlockingComponent()
{
    GetActor()->RemoveEventListener((dusk::EventID)dusk::Actor::Events::UPDATE,
        this, &FlockingComponent::Update);
}

void FlockingComponent::Update(const dusk::Event& event)
{
    const auto data = event.GetDataAs<dusk::UpdateEventData>();

    BoidActor * boidActor = (BoidActor *)GetActor();

    // Update neighbors
    _neighborsTimeout -= data->GetElapsedTime();
    if (_neighborsTimeout <= 0)
    {
        std::default_random_engine reng(_rand());
        std::uniform_real_distribution<double> uniform_dist(NEIGHBORS_MAX_TIMEOUT / 2, NEIGHBORS_MAX_TIMEOUT);
        _neighborsTimeout = uniform_dist(reng);

        glm::vec3 pos = GetActor()->GetPosition();

        _cohesionNeighbors.clear();
        _separationNeighbors.clear();
        _alignmentNeighbors.clear();

        float cohesionSquared   = COHESION_RADIUS * COHESION_RADIUS;
        float separationSquared = SEPARATION_RADIUS * SEPARATION_RADIUS;
        float alignmentSquared  = ALIGNMENT_RADIUS * ALIGNMENT_RADIUS;

        std::vector<BoidActor *> allBoids = _RootNodes[boidActor->GetGroup()].GetAdjacentMembers(pos);
        for (BoidActor * boid : allBoids)
        {

        //std::vector<BoidActor *> allBoids = GetActor()->GetScene()->GetActorsByType<BoidActor>();
        //for (BoidActor * boid : allBoids)
        //{
            if ((dusk::Actor *)boid == GetActor()) continue;

            float dist = glm::distance2(boid->GetPosition(), pos);
            if (dist < cohesionSquared)
            {
                _cohesionNeighbors.push_back(boid);
            }
            if (dist < separationSquared)
            {
                _separationNeighbors.push_back(boid);
            }
            if (dist < alignmentSquared)
            {
                _alignmentNeighbors.push_back(boid);
            }
        }
    }

    // Calculate new velocity

    glm::vec3 velocity = boidActor->GetVelocity();

    glm::vec3 cohesionVel;
    glm::vec3 separationVel;
    glm::vec3 alignmentVel;

    if (!_cohesionNeighbors.empty())
    {
        glm::vec3 center;

        for (BoidActor * boid : _cohesionNeighbors)
            center += boid->GetPosition();

        center /= (float)_cohesionNeighbors.size();
        cohesionVel = center - GetActor()->GetPosition();

        if (glm::length2(cohesionVel) > 0)
        {
            cohesionVel = glm::normalize(cohesionVel) * SPEED;
        }
    }

    if (!_separationNeighbors.empty())
    {
        glm::vec3 center;

        for (BoidActor * boid : _separationNeighbors)
            center += boid->GetPosition();

        center /= (float)_separationNeighbors.size();

        separationVel = GetActor()->GetPosition() - center;

        if (glm::length2(separationVel) > 0)
        {
            separationVel = glm::normalize(separationVel) * SPEED;
        }
    }

    if (!_alignmentNeighbors.empty())
    {
        for (BoidActor * boid : _alignmentNeighbors)
            alignmentVel += boid->GetVelocity();

        alignmentVel /= (float)_alignmentNeighbors.size();
    }

    float totalWeight = COHESION_WEIGHT + SEPARATION_WEIGHT + ALIGNMENT_WEIGHT;

    glm::vec3 targetVelocity;
    targetVelocity += cohesionVel * (COHESION_WEIGHT / totalWeight);
    targetVelocity += separationVel * (SEPARATION_WEIGHT / totalWeight);
    targetVelocity += alignmentVel * (ALIGNMENT_WEIGHT / totalWeight);

    if (glm::length2(targetVelocity) > 0)
    {
        targetVelocity = glm::normalize(targetVelocity) * SPEED;
        targetVelocity -= velocity;
        velocity += (targetVelocity * data->GetDelta() * BLENDING_SPEED);
    }
    boidActor->SetVelocity(velocity);
}
