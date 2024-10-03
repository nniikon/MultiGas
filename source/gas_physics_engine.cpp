#include "gas_physics_engine.h"
#include <iostream>
#include <ostream>

using namespace gas;

PhysicsEngine::PhysicsEngine(Engine* engine)
    : SubEngine(engine)
{}

void PhysicsEngine::UpdateOneByOne(molecule_iter it1, float delta_time) {
    UpdateMotion(it1, delta_time);
}

void PhysicsEngine::UpdateCollidedPairs(molecule_iter it1, molecule_iter it2, float delta_time) {
    UpdateCollision(it1, it2, delta_time);
}

void PhysicsEngine::UpdateMotion(molecule_iter it1, float delta_time) {
    float  left_wall_x_ = 0.f;
    float right_wall_x_ = GetEngine()->GetPistonPos();
    float  left_wall_y_ = 0.f;
    float right_wall_y_ = 1.f;
    float  left_wall_z_ = 0.f;
    float right_wall_z_ = 1.f;

    Molecule& molecule = *it1;

    molecule.position += molecule.velocity * delta_time;

    if (molecule.position.x < left_wall_x_) {
        molecule.position.x = left_wall_x_;
        molecule.velocity.x *= -1.f;
    }
    else if (molecule.position.x > right_wall_x_) {
        molecule.position.x = right_wall_x_;
        molecule.velocity.x *= -1.f;
    }

    if (molecule.position.y < left_wall_y_) {
        molecule.position.y = left_wall_y_;
        molecule.velocity.y *= -1.f;
    }
    else if (molecule.position.y > right_wall_y_) {
        molecule.position.y = right_wall_y_;
        molecule.velocity.y *= -1.f;
    }

    if (molecule.position.z < left_wall_z_) {
        molecule.position.z = left_wall_z_;
        molecule.velocity.z *= -1.f;
    }
    else if (molecule.position.z > right_wall_z_) {
        molecule.position.z = right_wall_z_;
        molecule.velocity.z *= -1.f;
    }
}

void PhysicsEngine::UpdateCollision(molecule_iter it1, molecule_iter it2, float delta_time) {
    // unused parameters
    (void)delta_time;
    std::cout << "HandleCollision" << std::endl;

    Molecule& m1 = *it1;
    Molecule& m2 = *it2;

    const vec3f collision_normal = sfmx::Norm(m1.position - m2.position);

    const float relative_velocity = sfmx::Dot(m1.velocity - m2.velocity, collision_normal);

    // If the molecules are going different ways
    if (relative_velocity > 0.f) {
        return;
    }

    const auto m1_mass = static_cast<float>(m1.mass);
    const auto m2_mass = static_cast<float>(m2.mass);

    m1.velocity = (m1.velocity * (m1_mass - m2_mass) + m2.velocity * (2.f * m2_mass)) / (m1_mass + m2_mass);
    m2.velocity = (m2.velocity * (m2_mass - m1_mass) + m1.velocity * (2.f * m1_mass)) / (m1_mass + m2_mass);
}
