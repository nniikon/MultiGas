#include "gas_buttons.h"
#include "sfmx_mouse.h"
#include "sfmx_keyboard.h"

#include <cassert>

//-----------------------------------------------------------------------------
// HoveringButtonByTexture
//-----------------------------------------------------------------------------

HoveringButtonByTexture::HoveringButtonByTexture(HoveringButtonTextureInfo& info)
    : ButtonBase(std::move(info.action)),
      sprite_      (*info.texture, {int(info.texture_pos.x)         , int(info.texture_pos.y)         , int(info.size.x), int(info.size.y)}),
      sprite_hover_(*info.texture, {int(info.texture_hovering_pos.x), int(info.texture_hovering_pos.y), int(info.size.x), int(info.size.y)}),
      transition_factor_(0.f) {

    sprite_      .SetScale(info.scale, info.scale);
    sprite_hover_.SetScale(info.scale, info.scale);

    sprite_      .SetPosition((float)info.window_pos.x, (float)info.window_pos.y);
    sprite_hover_.SetPosition((float)info.window_pos.x, (float)info.window_pos.y);
}

void HoveringButtonByTexture::Draw(sfmx::RenderWindow* window) {
    assert(0.f <= transition_factor_ && transition_factor_ <= 1.f);

    auto  idleAlpha = static_cast<uint8_t>(255.f * (1.f - transition_factor_));
    auto hoverAlpha = static_cast<uint8_t>(255.f * transition_factor_);

    sprite_      .SetColor(255u, 255u, 255u,  idleAlpha);
    sprite_hover_.SetColor(255u, 255u, 255u, hoverAlpha);

    window->Draw(sprite_hover_);
    window->Draw(sprite_);
}

bool HoveringButtonByTexture::HandleUpdate(const sfmx::Event& event, const sfmx::RenderWindow& window) {
    sfmx::vec2i mouse_pos_i = sfmx::Mouse::GetPosition(window);
    sfmx::vec2f mouse_pos_f{static_cast<float>(mouse_pos_i.x),
                            static_cast<float>(mouse_pos_i.y)};

    if (!sprite_.GetGlobalBounds().Contains(mouse_pos_f.x, mouse_pos_f.y)) {
        OnIdle();
        return false;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
    switch (event.type) {
        case sfmx::Event::EventType::MouseButtonPressed:  OnPress  (); break;
        case sfmx::Event::EventType::MouseButtonReleased: OnRelease(); break;
        default:                                          OnHover  (); break;
    }
#pragma GCC diagnostic pop

    return true;
}

//-----------------------------------------------------------------------------
// ButtonAddMolecule
//-----------------------------------------------------------------------------

ButtonAddMolecule::ButtonAddMolecule(Controller* controller,
                                     HoveringButtonTextureInfo& info,
                                     const TransitionFunctor& transitionFunc)
    : HoveringButtonByTexture(info),
      transitionFunc_(transitionFunc),
      controller_(controller) {}

void ButtonAddMolecule::ChangeTransitionFactor(float value) {
    transition_factor_ = transitionFunc_(transition_factor_, value);

    if (transition_factor_ < 0.f) {
        transition_factor_ = 0.f; 
    }

    if (transition_factor_ > 1.f) {
        transition_factor_ = 1.f;
    }
}

void ButtonAddMolecule::OnIdle() {
    ChangeTransitionFactor(-kTransitionSpeed);
}

void ButtonAddMolecule::OnHover() {
    ChangeTransitionFactor(+kTransitionSpeed);
}

void ButtonAddMolecule::OnPress() {}

void ButtonAddMolecule::OnRelease() {
    (*action_)();
}

bool ButtonAddMolecule::Update(const sfmx::Event& event, const sfmx::RenderWindow& window) {
    return HandleUpdate(event, window);
}

//-----------------------------------------------------------------------------
// ButtonMovePiston
//-----------------------------------------------------------------------------

ButtonMovePiston::ButtonMovePiston(Controller* controller,
                                   HoveringButtonTextureInfo& info)
    : HoveringButtonByTexture(info),
      controller_(controller),
      is_ctrl_pressed_(false),
      is_pressed_(false) {}

bool ButtonMovePiston::Update(const sfmx::Event& event, const sfmx::RenderWindow& window) {
    transition_factor_ = controller_->GetPistonPos();
    is_ctrl_pressed_   = sfmx::Keyboard::isKeyPressed(sfmx::Keyboard::Key::LControl);

    if (is_pressed_) {
        MovePiston();
    }

    return HandleUpdate(event, window);
}

void ButtonMovePiston::OnIdle()  {}
void ButtonMovePiston::OnHover() {}
void ButtonMovePiston::OnPress() {
    is_pressed_ = true;
}

void ButtonMovePiston::OnRelease() {
    is_pressed_ = false;
}

void ButtonMovePiston::MovePiston() {
    if (is_ctrl_pressed_) {
        action_->Undo();
    } else {
        action_->Do();
    }
}
