#ifndef GAS_BUTTON_TRANSITIONS_H_
#define GAS_BUTTON_TRANSITIONS_H_

class TransitionFunctor {
public:
    virtual float operator()(float current_factor, float speed) const = 0;
};

class LinearTransition : public TransitionFunctor {
public:
    virtual float operator()(float current_factor, float speed) const override;
};

class SinTransition : public TransitionFunctor {
public:
    virtual float operator()(float current_factor, float speed) const override;
};

class EaseOutTransition : public TransitionFunctor {
public:
    virtual float operator()(float current_factor, float speed) const override;
};

class EaseInOutTransition : public TransitionFunctor {
public:
    virtual float operator()(float current_factor, float speed) const override;
};

class DumpTransition : public TransitionFunctor {
public:
    virtual float operator()(float current_factor, float speed) const override;
};

#endif // GAS_BUTTON_TRANSITIONS_H_
