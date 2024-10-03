#include "gas_chemistry_engine.h"
#include <cassert>
#include <iostream>
#include <ostream>

using namespace gas;

ChemistryEngine::ChemistryEngine(Engine* engine)
    : SubEngine(engine)
{}

ChemistryEngine::HandleReactionFunc ChemistryEngine::GetHandleReactionFunc(Molecule::Type type1,
                                                                           Molecule::Type type2) {
    static const HandleReactionFunc kHandleFunctions[] = {
        &ChemistryEngine::HandleReactionCircularCircular, &ChemistryEngine::HandleReactionCircularSquared,
        &ChemistryEngine::HandleReactionSquaredCircular , &ChemistryEngine::HandleReactionSquaredSquared,
    };

    return kHandleFunctions[size_t(type1) * size_t(Molecule::Type::Count) + size_t(type2)];
}

void ChemistryEngine::UpdateOneByOne(molecule_iter it1, float delta_time) {
    // unused parameters
    (void)it1;
    (void)delta_time;

    // Molecules can't react with themselves
}

void ChemistryEngine::UpdateCollidedPairs(molecule_iter it1,
                                          molecule_iter it2, 
                                          float delta_time) {
    HandleReaction(it1, it2);
    // unused parameters
    (void)delta_time;
}

static float GenerateUniform() {
    return (float)rand() / ((float)(RAND_MAX) + 1.f);
}

void ChemistryEngine::HandleReactionCircularCircular(molecule_iter it1,
                                                     molecule_iter it2) {
    assert(it1->type == Molecule::Type::Circular);
    assert(it2->type == Molecule::Type::Circular);

    std::cout << "Reaction Circular-Circular" << std::endl;

    Molecule new_squared{};

    new_squared.type           = Molecule::Type::Squared;
    new_squared.position       = (it1->position + it2->position) / 2.f;
    new_squared.velocity       = (it1->velocity + it2->velocity) / 2.f;
    new_squared.squared.charge = 2u;

    MoleculeAdd(new_squared);

    MoleculeRemove(it1);
    MoleculeRemove(it2);
}

void ChemistryEngine::HandleReactionCircularSquared(molecule_iter it1,
                                                    molecule_iter it2) {
    std::cout << "Reaction Circular-Squared" << std::endl;

    assert(it1->type == Molecule::Type::Circular);
    assert(it2->type == Molecule::Type::Squared);

    it2->squared.charge++;

    auto mass1 = static_cast<float>(it1->mass);
    auto mass2 = static_cast<float>(it2->mass);

    auto vel1 = it1->velocity;
    auto vel2 = it2->velocity;

    it2->velocity = (vel1 * mass1 + vel2 * mass2) / (mass1 + mass2);
    it2->position = it2->position; // stays the same

    MoleculeRemove(it1);
}

void ChemistryEngine::HandleReactionSquaredCircular(molecule_iter it1,
                                                    molecule_iter it2) {
    return HandleReactionCircularSquared(it2, it1);
}

static std::vector<vec3f> GenerateEvenlyDistributedDirections(unsigned int count);

void ChemistryEngine::HandleReactionSquaredSquared(molecule_iter it1,
                                                   molecule_iter it2) {
    std::cout << "Reaction Squared-Squared" << std::endl;

    assert(it1->type == Molecule::Type::Squared);
    assert(it2->type == Molecule::Type::Squared);

    unsigned int total_charge = it1->squared.charge + it2->squared.charge;

    const auto mass1 = static_cast<float>(it1->mass);
    const auto mass2 = static_cast<float>(it2->mass);

    auto& pos1 = it1->position;
    auto& pos2 = it2->position;

    auto& vel1 = it1->velocity;
    auto& vel2 = it2->velocity;

    vec3f collision_point = (pos1 + pos2) / 2.f;
    vec3f total_momentum = vel1 * mass1 + vel2 * mass2; 
    float total_mass = mass1 + mass2;
    float total_energy = 0.5f * (mass1 * Dot(vel1, vel1) +
                                 mass2 * Dot(vel2, vel2));
    unsigned int normalized_mass 
        = static_cast<unsigned int>(total_mass / static_cast<float>(total_charge));

    std::vector<vec3f> directions = GenerateEvenlyDistributedDirections(total_charge);

    for (unsigned int i = 0; i < total_charge; i++) {
        Molecule new_circular{};

        new_circular.type = Molecule::Type::Circular;
        new_circular.position = collision_point;
        new_circular.mass = normalized_mass;

        float speed = GenerateRandomSpeed(total_energy, total_charge, i);
        new_circular.velocity = directions[i] * speed;

        auto new_mass_f = static_cast<float>(new_circular.mass);
        total_momentum -= new_circular.velocity * new_mass_f;
        total_energy -= 0.5f * new_mass_f * speed * speed;

        MoleculeAdd(new_circular);
    }

    MoleculeRemove(it1);
    MoleculeRemove(it2);
}

// https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere
static std::vector<vec3f> GenerateEvenlyDistributedDirections(unsigned int count) {

    std::vector<vec3f> directions;
    directions.reserve(count);

    const float phi = M_PIf * (3.0f - std::sqrt(5.0f));

    for (unsigned int i = 0; i < count; i++) {
        assert(float(count - 1) > 0);
        float y = 1.0f - (float(i) / float(count - 1)) * 2.0f;
        float radius = std::sqrt(1.0f - y * y);
        float theta = phi * float(i);

        float x = std::cos(theta) * radius;
        float z = std::sin(theta) * radius;

        directions.emplace_back(x, y, z);
    }

    return directions;
}

float ChemistryEngine::GenerateRandomSpeed(float& total_energy, unsigned int total_count, unsigned int index) {
    assert(total_count > 0);
    assert(total_count - index > 0);

    float indexf = static_cast<float>(index);
    float total_countf = static_cast<float>(total_count);

    float ratio = (indexf + 1.f) / total_countf;
    float max_speed = std::sqrt(2.0f * total_energy / (total_countf - indexf));
    float min_speed = max_speed * 0.1f;

    float rand_uni = GenerateUniform();

    float speed = min_speed + (max_speed - min_speed) * std::pow(rand_uni, ratio);
    return speed;
}

void ChemistryEngine::HandleReaction(molecule_iter it1, molecule_iter it2) {
    (this->*GetHandleReactionFunc(it1->type, it2->type))(it1, it2);
}
