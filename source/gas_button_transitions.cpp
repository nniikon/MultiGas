#include "gas_button_transitions.h"

#include <cmath>

float LinearTransition::operator()(float current_factor, float speed) const {
    return current_factor + speed;
}

float SinTransition::operator()(float current_factor, float speed) const {
    return current_factor + (0.99f - current_factor) * std::sin(speed * M_PIf / 2);
}

float EaseOutTransition::operator()(float current_factor, float speed) const {
    return current_factor + (speed * (1.f - (current_factor * current_factor)));
}

float EaseInOutTransition::operator()(float current_factor, float speed) const {
    if (current_factor < 0.5f) {
        return current_factor + (speed * 2.f * (current_factor + 0.5f) * (current_factor + 0.5f));
    }
    else {
        return current_factor + (speed * (-2.f * current_factor * current_factor + 4.f * current_factor - 1.f));
    }
}

float DumpTransition::operator()(float current, float speed) const {
    return speed > 0.f ? 1.f : 0.f;
}
