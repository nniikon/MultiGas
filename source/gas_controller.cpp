#include "gas_controller.h"

#include "gas_engine.h"
#include "gas_drawer.h"

using namespace gas;

Controller::Controller(Engine* engine, MainWindow* drawer)
    : engine_(engine),
      drawer_(drawer)
{}

const std::list<Molecule>& Controller::GetMolecules() const {
    return engine_->GetMolecules();
}

void Controller::CreateRandomMolecule(Molecule::Type type) {
    engine_->CreateRandomMolecule(type);
}

void Controller::RemoveRandomMolecule(Molecule::Type type) {
    engine_->RemoveRandomMolecule(type);
}

void Controller::MovePiston(float delta) {
    engine_->MovePiston(delta);
}

float Controller::GetPistonPos() const {
    return engine_->GetPistonPos();
}

float Controller::GetTemp() const {
    return engine_->GetTemp();
}

void Controller::SetDrawer(MainWindow* drawer) { drawer_ = drawer; }
void Controller::SetEngine(Engine*     engine) { engine_ = engine; }
