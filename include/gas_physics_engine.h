#ifndef GAS_PHYSICS_ENGINE_H_
#define GAS_PHYSICS_ENGINE_H_

#include "gas_engine.h"
#include "gas_subengine.h"

namespace gas {

class PhysicsEngine : public SubEngine {
public:
    void UpdateOneByOne     (molecule_iter it1,                    float delta_time) override;
    void UpdateCollidedPairs(molecule_iter it1, molecule_iter it2, float delta_time) override;

    PhysicsEngine(Engine* engine);
    virtual ~PhysicsEngine() override = default;

private:
    void UpdateMotion   (molecule_iter it1,                    float delta_time);
    void UpdateCollision(molecule_iter it1, molecule_iter it2, float delta_time);
};

}; // namespace

#endif // GAS_PHYSICS_ENGINE_H_
