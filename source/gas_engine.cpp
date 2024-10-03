#include "gas_engine.h"

#include <cassert>
#include <vector>

#include "gas_subengine.h"
#include "gas_physics_engine.h"
#include "gas_chemistry_engine.h"

using namespace gas;

static float DistanceSquared(const vec3f& pos1, const vec3f& pos2);
static bool  AreColliding(const Molecule& lhs, const Molecule& rhs);
static float GenerateUniform();
static vec3f GenerateRandomVelocity();
static vec3f GenerateRandomPosition();
static void RemoveMoleculesFromSet(std::list<Molecule>* molecules, const molecules_set& to_remove);
static void AddMoleculesFromVector(std::list<Molecule>* molecules, const molecules_vec& to_add);
static Molecule InitMoleculeByType(Molecule::Type type);

Engine::Engine(const vec2u& window_size,
               const vec2u& window_pos)
    : sub_engines_(),
      molecules_(),
      to_remove_(),
      to_add_(),
      window_size_(window_size),
      window_pos_ (window_pos),
      n_molecules_() {
    sub_engines_.push_back(std::make_unique<PhysicsEngine  >(this));
    sub_engines_.push_back(std::make_unique<ChemistryEngine>(this));
}

Engine::~Engine() = default;

void Engine::Update(float delta_time) {
    to_remove_.clear();
    to_add_.clear();

    for (auto& sub_engine : sub_engines_) {
        UpdateSubEngine(sub_engine.get(), delta_time);
    }

    RemoveMoleculesFromSet(&molecules_, to_remove_);
    AddMoleculesFromVector(&molecules_, to_add_);
}

void Engine::UpdateSubEngine(SubEngine* sub_engine, float delta_time) {
    UpdateOneByOne     (sub_engine, delta_time);
    UpdateCollidedPairs(sub_engine, delta_time);
}

void Engine::UpdateOneByOne(SubEngine* sub_engine, float delta_time) {
    for (auto it1 = molecules_.begin(); it1 != molecules_.end(); it1++) {
        sub_engine->UpdateOneByOne(it1, delta_time);
    }
}

void Engine::UpdateCollidedPairs(SubEngine* sub_engine, float delta_time) {
    for (auto it1 = molecules_.begin(); it1 != molecules_.end(); it1++) {
        for (auto it2 = std::next(it1); it2 != molecules_.end(); it2++) {
            if (AreColliding(*it1, *it2)) {
                sub_engine->UpdateCollidedPairs(it1, it2, delta_time);
            }
        }
    }
}

static bool AreColliding(const Molecule& lhs, const Molecule& rhs) {
    auto dist_squared = DistanceSquared(lhs.position, rhs.position);
    auto radius_sum = lhs.radius + rhs.radius;

    return (dist_squared <= radius_sum * radius_sum);
}

static Molecule InitMoleculeByType(Molecule::Type type) {
    Molecule molecule = {.type = type};

    switch(type) {
        case Molecule::Type::Circular:
            molecule.radius = Molecule::Circular::kRadius;
            molecule.mass   = Molecule::Circular::kMass;
            break;

        case Molecule::Type::Squared:
            molecule.radius         = Molecule::Squared::kRadius;
            molecule.mass           = Molecule::Squared::kMass;
            molecule.squared.charge = 10u;
            break;

        default:
        case Molecule::Type::Count:
            assert(0);
    }

    return molecule;
}

void Engine::SetNMolucules() {
    n_molecules_ = {};
    for (const auto& molecule : molecules_) {
        n_molecules_[(size_t)molecule.type]++;
    }
}

void Engine::CreateRandomMolecule(Molecule::Type type) {

    Molecule molecule = InitMoleculeByType(type);
    molecule.position = GenerateRandomPosition();
    molecule.velocity = GenerateRandomVelocity();

    molecules_.push_back(molecule);
}

void Engine::RemoveRandomMolecule(Molecule::Type type) {
    if (molecules_.empty()) {
        return;
    }

    std::vector<molecule_iter> candidates;
    for (auto it = molecules_.begin(); it != molecules_.end(); it++) {
        if (it->type == type) {
            candidates.push_back(it);
        }
    }

    if (candidates.empty()) {
        return;
    }

    auto random_index = static_cast<size_t>(GenerateUniform() * 
                                            static_cast<float>(candidates.size()));

    molecules_.erase(candidates[random_index]);
}

void Engine::MoleculeAdd(const Molecule& molecule) {
    to_add_.push_back(molecule);
}

void Engine::MoleculeRemove(molecule_iter it) {
    to_remove_.insert(it);
}

float Engine::GetPistonPos() const {
    return piston_pos_; 
}

void Engine::MovePiston(float delta) {
    piston_pos_ += delta;

    if (piston_pos_ > 1.f) {
        piston_pos_ = 1.f;
    }
    if (piston_pos_ < 0.15f) {
        piston_pos_ = 0.15f;
    }
}

float Engine::GetTemp() const {
    float kinetic_energy = 0.f;

    for (const auto& molecule : molecules_) {
        if (__finite(Len(molecule.velocity))) {
            kinetic_energy += static_cast<float>(molecule.mass) * Len(molecule.velocity) * Len(molecule.velocity);
        }
    }

    return kinetic_energy / static_cast<float>(molecules_.size());
}

static float DistanceSquared(const vec3f& pos1, const vec3f& pos2) {
    return (pos1.x - pos2.x) * (pos1.x - pos2.x) +
           (pos1.y - pos2.y) * (pos1.y - pos2.y) +
           (pos1.z - pos2.z) * (pos1.z - pos2.z);
}

static void RemoveMoleculesFromSet(std::list<Molecule>* molecules, const molecules_set& to_remove) {
    for (auto it : to_remove) {
        molecules->erase(it);
    }
}

static void AddMoleculesFromVector(std::list<Molecule>* molecules, const molecules_vec& to_add) {
    for (auto molecule : to_add) {
        molecules->push_back(molecule);
    }
}

const std::list<Molecule>& Engine::GetMolecules() const {
    return molecules_;
}

static float GenerateUniform() {
    return (float)rand() / ((float)(RAND_MAX) + 1.f);
}

static vec3f GenerateRandomVelocity() {
    return { GenerateUniform() - 0.5f,
             GenerateUniform() - 0.5f,
             GenerateUniform() - 0.5f };
}

static vec3f GenerateRandomPosition() {
    return { GenerateUniform(),
             GenerateUniform(),
             GenerateUniform() };
}
