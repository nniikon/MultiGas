#ifndef GAS_ENGINE_H_
#define GAS_ENGINE_H_

#include "sfmx_vectors.h"
#include "gas_molecule_types.h"

#include <array>
#include <list>
#include <memory>
#include <set>
#include <vector>

namespace gas {

class PhysicsEngine;
class ChemistryEngine;
class SubEngine;

using sfmx::vec3f;
using sfmx::vec2u;

using molecule_iter = std::list<Molecule>::iterator;

class MoleculeIteratorCompare {
public:
    bool operator()(const molecule_iter& lhs,
                    const molecule_iter& rhs) const {
        return &(*lhs) < &(*rhs);
    }
};

using molecules_set = std::set<molecule_iter, MoleculeIteratorCompare>;
using molecules_vec = std::vector<Molecule>;

class Engine {
public:
    const std::list<Molecule>& GetMolecules() const;

    void CreateRandomMolecule(Molecule::Type type);
    void RemoveRandomMolecule(Molecule::Type type);

    void MoleculeAdd   (const Molecule& molecule);
    void MoleculeRemove(molecule_iter it);

    float GetPistonPos() const;
    void MovePiston  (float delta);

    float GetTemp() const;

    void Update         (float delta_time);

    Engine(const vec2u& window_size,
           const vec2u& window_pos);

    ~Engine();

private:
    std::vector<std::unique_ptr<SubEngine>> sub_engines_;

    std::list<Molecule> molecules_; 

    molecules_set to_remove_;
    molecules_vec to_add_;

    float piston_pos_ = 1.0f;

    vec2u window_size_;
    vec2u window_pos_;

    std::array<size_t, Molecule::kNumTypes> n_molecules_;
    void SetNMolucules();

    void UpdateSubEngine    (SubEngine* sub_engine, float delta_time);
    void UpdateOneByOne     (SubEngine* sub_engine, float delta_time);
    void UpdateCollidedPairs(SubEngine* sub_engine, float delta_time);
};

}; // namespace

#endif // GAS_ENGINE_H_
