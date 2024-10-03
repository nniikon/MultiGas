#ifndef GAS_GAS_ACTIONS_H_
#define GAS_GAS_ACTIONS_H_

#include "gas_controller.h"
#include "tgp_action.h"

namespace gas {

class ChangeNumMoleculesAction : public tgp::Action {
public:
    ChangeNumMoleculesAction(Controller* controller, int delta, Molecule::Type type);
    
    void Do()         override;
    bool Undo()       override;
    bool SaveState()  override;

    ~ChangeNumMoleculesAction() override = default;

private:
    Controller*    controller_ = nullptr;
    int            delta_      = 0;
    Molecule::Type type_ = Molecule::Type::Count;
};

class MovePistonAction : public tgp::Action {
public:
    MovePistonAction(Controller* controller, float delta);
    
    void Do()         override;
    bool Undo()       override;
    bool SaveState()  override;

private:
    Controller* controller_ = nullptr;
    float       delta_      = 0.f;
};

}; // namespace gas

#endif  // GAS_GAS_ACTIONS_H_
