#include "gas_actions.h"
#include <cassert>

using namespace gas;

//-----------------------------------------------------------------------------
// ChangeNumMoleculesAction
//-----------------------------------------------------------------------------

ChangeNumMoleculesAction::ChangeNumMoleculesAction(Controller* controller,
                                                   int delta,
                                                   Molecule::Type type)
    : controller_(controller), 
      delta_(delta), 
      type_(type) {}

void ChangeNumMoleculesAction::Do() {
    if (delta_ < 0) {
        controller_->RemoveRandomMolecule(type_);
    }
    else if (delta_ > 0) {
        controller_->CreateRandomMolecule(type_);
    }
    else {
        assert(0);
    }
}

bool ChangeNumMoleculesAction::Undo() {
    return false;
}

bool ChangeNumMoleculesAction::SaveState() {
    return false;
}

//-----------------------------------------------------------------------------
// MovePistonAction
//-----------------------------------------------------------------------------

MovePistonAction::MovePistonAction(Controller* controller, float delta)
    : controller_(controller),
      delta_(delta) {}

void MovePistonAction::Do() {
    controller_->MovePiston(delta_);
}

bool MovePistonAction::Undo() {
    controller_->MovePiston(-delta_);
    return true;
}

bool MovePistonAction::SaveState() {
    return false;
}
