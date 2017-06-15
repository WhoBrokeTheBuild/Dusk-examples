#include <dusk/Dusk.hpp>
#include <sstream>
#include <random>

const unsigned int BOID_COUNT = 100;

const glm::vec4 BOID_COLORS[] = {
    glm::vec4(1, 0, 0, 1),
    glm::vec4(0, 1, 0, 1),
    glm::vec4(0, 0, 1, 1),
    glm::vec4(1, 0, 1, 1),
};
const unsigned int BOID_GROUPS = sizeof(BOID_COLORS) / sizeof(glm::vec4);

const float SPEED = 1000.0f;
const float BLENDING_SPEED = 50.0f;

const glm::vec3 UPPER_BOUND = glm::vec3(300.0f);
const glm::vec3 LOWER_BOUND = glm::vec3(-300.0f);

const double NEIGHBORS_MAX_TIMEOUT = 2.0;

const double COHESION_RADIUS   = 80;
const double SEPARATION_RADIUS = 50;
const double ALIGNMENT_RADIUS  = 80;

const float COHESION_WEIGHT   = 0.5;
const float SEPARATION_WEIGHT = 0.5;
const float ALIGNMENT_WEIGHT  = 0.6;

std::random_device _R;

class BoidActor;

class FlockingComponenet : public dusk::Component
{
public:

    FlockingComponenet(dusk::Actor * parent)
        : dusk::Component(parent)
    {
        GetActor()->AddComponentType<FlockingComponenet>(this);
        GetActor()->AddEventListener((dusk::EventID)dusk::Actor::Events::UPDATE,
            this, &FlockingComponenet::Update);

        _neighborsTimeout = NEIGHBORS_MAX_TIMEOUT;
    }

    virtual ~FlockingComponenet()
    {
        GetActor()->RemoveEventListener((dusk::EventID)dusk::Actor::Events::UPDATE,
            this, &FlockingComponenet::Update);
    }

    inline bool Load() override
    { return true; }

    inline void Free() override
    { }

    void Update(const dusk::Event& event);

private:

    double _neighborsTimeout = 0;

    std::vector<BoidActor *> _cohesionNeighbors;
    std::vector<BoidActor *> _separationNeighbors;
    std::vector<BoidActor *> _alignmentNeighbors;

};

class BoidActor : public dusk::Actor
{
public:

    BoidActor(dusk::Scene * scene, const std::string& name, unsigned int group)
        : dusk::Actor(scene, name)
    {
        GetScene()->AddActorType<BoidActor>(this);
        GetScene()->AddActorTag(this, "Boid");

        _group = group;

        std::stringstream ss;
        ss << "Boid-group-" << _group;
        _groupTag = ss.str();

        GetScene()->AddActorTag(this, _groupTag);

        dusk::App * app = dusk::App::GetInst();
        dusk::Shader * shader = app->GetShader("default_shader");

        dusk::Material * mat = new dusk::Material(BOID_COLORS[group],
                                                  glm::vec4(0),
                                                  glm::vec4(0),
                                                  0.0f, 0.0f,
                                                  "", "", "", "");

        dusk::Mesh * mesh = new dusk::ConeMesh(shader, mat, 5, 3.0f, 8.0f);
        mesh->SetRotation(glm::vec3(0, 0, -glm::pi<float>() * 0.5f));

        AddComponent(new dusk::MeshComponent(this, mesh));
        AddComponent(new FlockingComponenet(this));

        std::default_random_engine reng(_R());
        std::uniform_real_distribution<float> random_position(LOWER_BOUND.x, UPPER_BOUND.x);
        std::uniform_real_distribution<float> random_angle(-1, 1);

        SetPosition(glm::vec3(random_position(reng),
                              random_position(reng),
                              random_position(reng)));

        glm::vec3 vel = glm::vec3(random_angle(reng),
                                  random_angle(reng),
                                  random_angle(reng));
        SetVelocity(vel * SPEED);
    }

    void Update(const dusk::Event& event) override
    {
        const auto data = event.GetDataAs<dusk::UpdateEventData>();

        dusk::Actor::Update(event);

        SetVelocity(GetVelocity() + GetAcceleration() * data->GetDelta());
        SetAcceleration(glm::vec3(0));

        glm::vec3 pos = GetPosition();
        pos += GetVelocity() * data->GetDelta();
        //DuskLogInfo("Position: %f %f %f", pos.x, pos.y, pos.z);

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

        glm::vec3 forward = glm::normalize(_velocity);
        float angle = glm::dot(glm::vec3(1, 0, 0), forward);
        glm::quat q = glm::angleAxis(acosf(angle), glm::cross(glm::vec3(1, 0, 0), forward));

        SetRotation(glm::eulerAngles(q));
    }

    void SetVelocity(const glm::vec3& vel) { _velocity = vel; }
    glm::vec3 GetVelocity() const { return _velocity; }

    void SetAcceleration(const glm::vec3& acc) { _acceleration = acc; }
    glm::vec3 GetAcceleration() const { return _acceleration; }

    std::string GetGroupTag() const { return _groupTag; }

private:

    glm::vec3 _velocity;
    glm::vec3 _acceleration;

    unsigned int _group;
    std::string _groupTag;

};

void FlockingComponenet::Update(const dusk::Event& event)
{
    const auto data = event.GetDataAs<dusk::UpdateEventData>();

    BoidActor * boidActor = (BoidActor *)GetActor();

    // Update neighbors
    _neighborsTimeout -= data->GetElapsedTime();
    if (_neighborsTimeout <= 0)
    {
        std::default_random_engine reng(_R());
        std::uniform_real_distribution<double> uniform_dist(NEIGHBORS_MAX_TIMEOUT / 2, NEIGHBORS_MAX_TIMEOUT);
        _neighborsTimeout = uniform_dist(reng);

        glm::vec3 pos = GetActor()->GetPosition();

        _cohesionNeighbors.clear();
        _separationNeighbors.clear();
        _alignmentNeighbors.clear();

        float cohesionSquared   = COHESION_RADIUS * COHESION_RADIUS;
        float separationSquared = SEPARATION_RADIUS * SEPARATION_RADIUS;
        float alignmentSquared  = ALIGNMENT_RADIUS * ALIGNMENT_RADIUS;

        //std::vector<BoidActor *> allBoids = GetActor()->GetScene()->GetActorsByType<BoidActor>();
        //for (BoidActor * boid : allBoids)
        //{
        std::vector<dusk::Actor *> allBoids = GetActor()->GetScene()->GetActorsByTag(boidActor->GetGroupTag());
        for (dusk::Actor * actor : allBoids)
        {
            BoidActor * boid = (BoidActor *)actor;

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

struct LightingData
{
    alignas(4) glm::vec3 LightPos;
    alignas(4) glm::vec3 CameraPos;
};
LightingData lighting_data;

void AppUpdate(const dusk::Event& event)
{
    dusk::App * app = dusk::App::GetInst();

    glm::vec3 cam_pos = app->GetScene()->GetCamera()->GetPosition();

    lighting_data.LightPos  = cam_pos;
    lighting_data.CameraPos = cam_pos;
    dusk::Shader::UpdateData("LightingData", &lighting_data, sizeof(lighting_data));
}

void AppStart(const dusk::Event& event)
{
    dusk::App * app = dusk::App::GetInst();

    std::stringstream ss;
    dusk::Scene * scene = app->GetScene();
    for (unsigned int g = 0; g < BOID_GROUPS; ++g)
    {
        for (unsigned int i = 0; i < BOID_COUNT; ++i)
        {
            ss.clear();
            ss.str("boid-");
            ss << g << "-" << i;

            dusk::Actor * actor = (dusk::Actor *)new BoidActor(scene, ss.str(), g);
            app->GetScene()->AddActor(actor);
        }
    }
}

int main(int argc, char** argv)
{
    dusk::App app(argc, argv);

    dusk::Shader::DefineData("LightingData", sizeof(LightingData));
    //app.RegisterActorType<BoidActor>("Boid");

	app.LoadConfig("assets/config/Flocking/app.json");
    app.AddEventListener((dusk::EventID)dusk::App::Events::UPDATE, &AppUpdate);
    app.AddEventListener((dusk::EventID)dusk::App::Events::START, &AppStart);

    app.Run();
	return 0;
}
