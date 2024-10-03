#ifndef GAS_SUBENGINE_H_
#define GAS_SUBENGINE_H_

#include "gas_engine.h"

namespace gas {

class SubEngine {
public:
    virtual void UpdateOneByOne     (molecule_iter it1,                    float delta_time) = 0;
    virtual void UpdateCollidedPairs(molecule_iter it1, molecule_iter it2, float delta_time) = 0;

    SubEngine(Engine* engine);
    virtual ~SubEngine() = default;

protected:
    virtual void MoleculeAdd   (const Molecule& molecule);
    virtual void MoleculeRemove(molecule_iter it);

    const Engine* GetEngine() const;

private:
    Engine* engine_ = nullptr;
};

}; // namespace

#endif // GAS_SUBENGINE_H_
