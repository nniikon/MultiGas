#ifndef GAS_BUTTONS_H_
#define GAS_BUTTONS_H_

#include "gas_button_transitions.h"

#include "gas_controller.h"
#include "tgp_action.h"
#include "tgp_button.h"
#include <memory>

using namespace gas;

using sfmx::vec2u;

struct HoveringButtonTextureInfo {
    const sfmx::Texture* texture;
    vec2u window_pos;
    vec2u texture_pos;
    vec2u texture_hovering_pos;
    vec2u size;
    float scale;
    std::unique_ptr<tgp::Action> action;
};

class HoveringButtonByTexture : public tgp::ButtonBase {
public:
    HoveringButtonByTexture(HoveringButtonTextureInfo& info);

    virtual void Draw(sfmx::RenderWindow* window) override;
    bool HandleUpdate(const sfmx::Event& event, const sfmx::RenderWindow& window);

protected:
    sfmx::Sprite sprite_;
    sfmx::Sprite sprite_hover_;

    float transition_factor_ = 0.f;
};

class ButtonAddMolecule : public HoveringButtonByTexture {
public:
    ButtonAddMolecule(Controller* controller,
                      HoveringButtonTextureInfo& info,
                      const TransitionFunctor& transitionFunc);

    void OnIdle()    override;
    void OnHover()   override;
    void OnPress()   override;
    void OnRelease() override;

    bool Update(const sfmx::Event& event, const sfmx::RenderWindow& window) override;

    ~ButtonAddMolecule() override = default;

private:
    constexpr static const float kTransitionSpeed = 0.01f;

    const TransitionFunctor& transitionFunc_;

    Controller* controller_ = nullptr;

    void ChangeTransitionFactor(float value);
};

class ButtonMovePiston : public HoveringButtonByTexture {
public:
    ButtonMovePiston(Controller* controller,
                     HoveringButtonTextureInfo& info);

    void OnIdle()    override;
    void OnHover()   override;
    void OnPress()   override;
    void OnRelease() override;

    bool Update(const sfmx::Event& event, const sfmx::RenderWindow& window) override;

    ~ButtonMovePiston() override = default;

private:
    void MovePiston();

    Controller* controller_ = nullptr;

    bool is_ctrl_pressed_ = false;
    bool is_pressed_      = false;
};

#endif // GAS_BUTTONS_H_
