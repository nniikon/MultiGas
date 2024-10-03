#ifndef GAS_CONTROLLER_H_
#define GAS_CONTROLLER_H_

#include "gas_molecule_types.h"
#include <list>

namespace gas {

class Engine;
class MainWindow;
struct Molecule;

class Controller {
public:
    Controller(Engine* engine, MainWindow* drawer);
    const std::list<Molecule>& GetMolecules() const;

    void CreateRandomMolecule(Molecule::Type type);
    void RemoveRandomMolecule(Molecule::Type type);

    void MovePiston(float delta);
    float GetPistonPos() const;

    float GetTemp() const;

    void SetDrawer(MainWindow* drawer);
    void SetEngine(Engine* engine);

private:
    Engine*     engine_;
    MainWindow* drawer_;
};

}; // namespace

#endif // GAS_CONTROLLER_H_
