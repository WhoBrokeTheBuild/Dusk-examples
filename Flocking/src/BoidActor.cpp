#include "BoidActor.hpp"

#include "FlockingComponent.hpp"

const glm::vec3 UPPER_BOUND = glm::vec3(400.0f);
const glm::vec3 LOWER_BOUND = glm::vec3(-400.0f);
float OCTANT_SIZE = 80.0f;

OctantNode<BoidActor *> _RootNodes[BOID_GROUPS] = {
    OctantNode<BoidActor *>(LOWER_BOUND, UPPER_BOUND, OCTANT_SIZE),
    //OctantNode<BoidActor *>(LOWER_BOUND, UPPER_BOUND, OCTANT_SIZE),
    //OctantNode<BoidActor *>(LOWER_BOUND, UPPER_BOUND, OCTANT_SIZE),
    //OctantNode<BoidActor *>(LOWER_BOUND, UPPER_BOUND, OCTANT_SIZE),
};

BoidActor::BoidActor(dusk::Scene * scene,
                     const std::string& name,
                     unsigned int group,
                     glm::vec4 color)
    : dusk::Actor(scene, name)
{
    GetScene()->AddActorType<BoidActor>(this);
    GetScene()->AddActorTag(this, "Boid");

    _group = group;

    dusk::App * app = dusk::App::GetInst();
    dusk::Shader * shader = app->GetShader("default_shader");

    dusk::Material * mat = new dusk::Material(color,
                                              glm::vec4(0),
                                              glm::vec4(0),
                                              0.0f, 0.0f,
                                              "", "", "", "");

    dusk::Mesh * mesh = new dusk::ConeMesh(shader, mat, 5, 3.0f, 8.0f);
    mesh->SetRotation(glm::vec3(0, 0, -glm::pi<float>() * 0.5f));

    AddComponent(new dusk::MeshComponent(this, mesh));
    AddComponent(new FlockingComponent(this));

    std::default_random_engine reng(_rand());
    std::uniform_real_distribution<float> random_position(LOWER_BOUND.x, UPPER_BOUND.x);
    std::uniform_real_distribution<float> random_angle(-1, 1);

    SetPosition(glm::vec3(random_position(reng),
                          random_position(reng),
                          random_position(reng)));

    _RootNodes[_group].AddMember(GetPosition(), this);
    _quadIndex = _RootNodes[_group].GetIndex(GetPosition());

    glm::vec3 vel = glm::vec3(random_angle(reng),
                              random_angle(reng),
                              random_angle(reng));
    SetVelocity(vel * SPEED);
}

void BoidActor::Update(const dusk::Event& event)
{
    const auto data = event.GetDataAs<dusk::UpdateEventData>();

    dusk::Actor::Update(event);

    SetVelocity(GetVelocity() + GetAcceleration() * data->GetDelta());
    SetAcceleration(glm::vec3(0));

    glm::vec3 oldPos = GetPosition();
    glm::vec3 pos = oldPos;

    pos += GetVelocity() * data->GetDelta();

    // Wrap around
    for (unsigned int i = 0; i < 3; ++i)
    {
        if (pos[i] > UPPER_BOUND[i])
        {
            pos[i] = LOWER_BOUND[i];
        }
        else if (pos[i] < LOWER_BOUND[i])
        {
            pos[i] = UPPER_BOUND[i];
        }
    }

    SetPosition(pos);

    unsigned int newQuadIndex = _RootNodes[_group].GetIndex(pos);
    if (newQuadIndex != _quadIndex)
    {
        _quadIndex = newQuadIndex;
        _RootNodes[_group].RemoveMember(oldPos, this);
        _RootNodes[_group].AddMember(pos, this);
    }

    glm::vec3 forward = glm::normalize(_velocity);
    float angle = glm::dot(glm::vec3(1, 0, 0), forward);
    glm::quat q = glm::angleAxis(acosf(angle), glm::cross(glm::vec3(1, 0, 0), forward));

    SetRotation(glm::eulerAngles(q));
}
