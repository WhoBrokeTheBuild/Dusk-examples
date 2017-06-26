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
                     unsigned int group,
                     glm::vec4 color)
    : dusk::Actor(scene)
{
    static std::mt19937 _R;

    _group = group;

    dusk::App * app = dusk::App::GetInst();
    dusk::Shader * shader = app->GetShader("default_shader");

    std::shared_ptr<dusk::Material> mat = dusk::Material::Create(
        color, glm::vec4(0), glm::vec4(0),
        0.0f, 0.0f,
        "", "", "", "");

    std::unique_ptr<dusk::Model> model(new dusk::Model(shader));
    model->SetRotation(glm::vec3(0, 0, -glm::pi<float>() * 0.5f));

    std::shared_ptr<dusk::Mesh> mesh = dusk::ConeMesh::Create(mat, 5, 3.0f, 8.0f);
    model->AddMesh(mesh);

    AddComponent(std::unique_ptr<dusk::Component>(new dusk::ModelComponent(this, std::move(model))));
    AddComponent(std::unique_ptr<dusk::Component>(new FlockingComponent(this)));

    std::uniform_real_distribution<float> random_position(LOWER_BOUND.x, UPPER_BOUND.x);
    std::uniform_real_distribution<float> random_angle(-1, 1);

    SetPosition(glm::vec3(random_position(_R),
                          random_position(_R),
                          random_position(_R)));

    _RootNodes[_group].AddMember(GetPosition(), this);
    _quadIndex = _RootNodes[_group].GetIndex(GetPosition());

    glm::vec3 vel = glm::vec3(random_angle(_R),
                              random_angle(_R),
                              random_angle(_R));
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
