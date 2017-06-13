#include <dusk/Dusk.hpp>
#include <sstream>
#include <random>

const unsigned int BOID_COUNT = 100;
const glm::vec3 UPPER_BOUND = glm::vec3(300.0f);
const glm::vec3 LOWER_BOUND = glm::vec3(-300.0f);

std::random_device _R;

class BoidActor;

class FlockingComponenet : public dusk::Component
{
public:

    FlockingComponenet(dusk::Actor * parent)
        : dusk::Component(parent)
    {
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

    const double NEIGHBORS_MAX_TIMEOUT = 0.3;
    const double COHESION_RADIUS = 50;
    const double SEPARATION_RADIUS = 25;
    const double ALIGNMENT_RADIUS = 50;
    const float COHESION_WEIGHT = 1.0;
    const float SEPARATION_WEIGHT = 1.5;
    const float ALIGNMENT_WEIGHT = 1.0;

    double _neighborsTimeout = 0;
    std::vector<BoidActor *> _cohesionNeighbors;
    std::vector<BoidActor *> _separationNeighbors;
    std::vector<BoidActor *> _alignmentNeighbors;

};

class BoidActor : public dusk::Actor
{
public:

    BoidActor(dusk::Scene * scene, const std::string& name)
        : dusk::Actor(scene, name)
    {
        GetScene()->AddActorTag(this, "Boid");

        dusk::App * app = dusk::App::GetInst();
        dusk::Shader * shader = app->GetShader("default_shader");
        dusk::Mesh * mesh = new dusk::ConeMesh(shader, nullptr, 5, 3.0f, 8.0f);
        mesh->SetRotation(glm::vec3(0, 0, M_PI * 0.5f));

        AddComponent(new dusk::MeshComponent(this, mesh));
        AddComponent(new FlockingComponenet(this));

        std::default_random_engine reng(_R());
        std::uniform_real_distribution<float> uniform_dist(-1, 1);
        glm::vec3 vel = glm::vec3(uniform_dist(reng),
                                  uniform_dist(reng),
                                  uniform_dist(reng));

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

    const float SPEED = 1000.0f;

private:

    glm::vec3 _velocity;
    glm::vec3 _acceleration;

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
        std::uniform_real_distribution<float> uniform_dist(NEIGHBORS_MAX_TIMEOUT / 2, NEIGHBORS_MAX_TIMEOUT);
        _neighborsTimeout = uniform_dist(reng);

        glm::vec3 pos = GetActor()->GetPosition();

        _cohesionNeighbors.clear();
        _separationNeighbors.clear();
        _alignmentNeighbors.clear();

        std::vector<dusk::Actor *> allBoids = GetActor()->GetScene()->GetActorsByTag("Boid");
        for (dusk::Actor * actor : allBoids)
        {
            BoidActor * boid = (BoidActor *)actor;

            if (actor == GetActor()) continue;

            float dist = glm::distance(actor->GetPosition(), pos);
            if (dist < COHESION_RADIUS)
            {
                _cohesionNeighbors.push_back(boid);
            }
            if (dist < SEPARATION_RADIUS)
            {
                _separationNeighbors.push_back(boid);
            }
            if (dist < ALIGNMENT_RADIUS)
            {
                _alignmentNeighbors.push_back(boid);
            }
        }
    }

    // Calculate new velocity

    glm::vec3 velocity = boidActor->GetVelocity();

    glm::vec3 cohesionVel;
    if (!_cohesionNeighbors.empty())
    {
        glm::vec3 cohesionDest;
        for (BoidActor * boid : _cohesionNeighbors)
        {
            cohesionDest += boid->GetPosition();
        }
        cohesionDest = cohesionDest / (float)(_cohesionNeighbors.size());
        cohesionVel = glm::normalize(cohesionDest - GetActor()->GetPosition());
        cohesionVel *= boidActor->SPEED;
        cohesionVel -= velocity;
        cohesionVel = glm::clamp(cohesionVel, glm::vec3(-boidActor->SPEED), glm::vec3(boidActor->SPEED));
        //DuskLogInfo("%f %f %f", cohesionDest.x, cohesionDest.y, cohesionDest.z);
    }

    glm::vec3 separationVel;
    if (!_separationNeighbors.empty())
    {
        glm::vec3 separationDest;
        for (BoidActor * boid : _separationNeighbors)
        {
            separationDest += boid->GetPosition();
        }
        separationDest = separationDest / (float)(_separationNeighbors.size());
        separationVel = glm::normalize(separationDest - GetActor()->GetPosition());

        if (glm::length(separationVel) > 0)
        {
            separationVel *= boidActor->SPEED;
            separationVel -= velocity;
            separationVel *= -1.0f;
            separationVel = glm::clamp(separationVel, glm::vec3(-boidActor->SPEED), glm::vec3(boidActor->SPEED));
        }
        //DuskLogInfo("%f %f %f", separationDest.x, separationDest.y, separationDest.z);
    }

    glm::vec3 alignmentVel;
    if (!_alignmentNeighbors.empty())
    {
        for (BoidActor * boid : _alignmentNeighbors)
        {
            alignmentVel += boid->GetVelocity();
        }
        alignmentVel = glm::normalize(alignmentVel / (float)_alignmentNeighbors.size());
        alignmentVel -= velocity;
        alignmentVel = glm::clamp(alignmentVel, glm::vec3(-boidActor->SPEED), glm::vec3(boidActor->SPEED));
    }

    glm::vec3 acceleration = (cohesionVel * COHESION_WEIGHT) +
                             (separationVel * SEPARATION_WEIGHT) +
                             (alignmentVel * ALIGNMENT_WEIGHT);
    boidActor->SetAcceleration(acceleration);
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
    for (unsigned int i = 0; i < BOID_COUNT; ++i)
    {
        ss.clear();
        ss.str("boid-");
        ss << i;

        DuskLogInfo("Spawning boid #%d", i + 1);

        dusk::Actor * actor = (dusk::Actor *)new BoidActor(scene, ss.str());
        app->GetScene()->AddActor(actor);
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
