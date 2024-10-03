#ifndef GAS_GAS_CHEMISTRY_ENGINE_H_
#define GAS_GAS_CHEMISTRY_ENGINE_H_

#include "gas_engine.h"
#include "gas_subengine.h"

namespace gas {

class ChemistryEngine : public SubEngine {
public:
    void UpdateOneByOne     (molecule_iter it1,                    float delta_time) override;
    void UpdateCollidedPairs(molecule_iter it1, molecule_iter it2, float delta_time) override;

    ChemistryEngine(Engine* engine);
    virtual ~ChemistryEngine() override = default;

private:
    using HandleReactionFunc = void (ChemistryEngine::*)(molecule_iter, molecule_iter);

    HandleReactionFunc GetHandleReactionFunc(Molecule::Type type1,
                                             Molecule::Type type2);
    void HandleReaction(molecule_iter it1, molecule_iter it2);

    void HandleReactionCircularCircular(molecule_iter it1, molecule_iter it2);
    void HandleReactionCircularSquared (molecule_iter it1, molecule_iter it2);
    void HandleReactionSquaredCircular (molecule_iter it1, molecule_iter it2);
    void HandleReactionSquaredSquared  (molecule_iter it1, molecule_iter it2);

    float              GenerateRandomSpeed(float& total_energy, unsigned int total_count, unsigned int index);
};

}; // namespace

#endif  // GAS_GAS_CHEMISTRY_ENGINE_H_
