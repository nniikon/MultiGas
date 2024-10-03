#include "gas_subengine.h"

using namespace gas;

SubEngine::SubEngine(Engine* engine)
    : engine_(engine)
{}

void SubEngine::MoleculeAdd(const Molecule& molecule) {
    engine_->MoleculeAdd(molecule);
}

void SubEngine::MoleculeRemove(molecule_iter it) {
    engine_->MoleculeRemove(it);
}

const Engine* SubEngine::GetEngine() const {
    return engine_;
}
