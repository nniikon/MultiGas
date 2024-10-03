#ifndef GAS_MOLECULE_TYPES_H_
#define GAS_MOLECULE_TYPES_H_

#include "sfmx_vectors.h"
#include <SFML/Window/Event.hpp>

namespace gas {

struct Molecule {

    enum class Type {
        Circular,
        Squared,
        Count, // number of types
    };

    static constexpr float kRadiusScale = 1e-2f;

    struct Squared {
        static constexpr unsigned int kMass   = 5u;
        static constexpr float        kRadius = 5.f * kRadiusScale;
        unsigned int charge = 1u;
    };

    struct Circular {
        static constexpr unsigned int kMass   = 1u;
        static constexpr float        kRadius = 1.f * kRadiusScale;
    };

    static constexpr auto kNumTypes = static_cast<size_t>(Molecule::Type::Count);

    sfmx::vec3f position;
    sfmx::vec3f velocity;
    unsigned int mass;
    float radius;
    Molecule::Type type;
    union {
        Molecule::Squared squared;
        Molecule::Circular circular;
    };
};

}; // namespace

#endif // GAS_MOLECULE_TYPES_H_
