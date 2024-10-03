#include "gas_chamber_window.h"

using namespace gas;

ChamberWindow::ChamberWindow(Controller* controller,
                             const sfmx::vec2u& window_size, 
                             const sfmx::vec2u& window_pos, 
                             std::array<sfmx::Sprite, Molecule::kNumTypes>& sprites)
    : WindowBase(),
      controller_(controller),
      window_size_(window_size),
      window_pos_(window_pos),
      sprites_(sprites) {}

void ChamberWindow::Draw(sfmx::RenderWindow* window) {
    auto molecules = controller_->GetMolecules();

    for (const auto& molecule : molecules) {
        sprites_[(size_t)molecule.type].SetPosition(
            static_cast<float>(window_pos_.x) + molecule.position.x * static_cast<float>(window_size_.x),
            static_cast<float>(window_pos_.y) + molecule.position.y * static_cast<float>(window_size_.y)
        );

        window->Draw(sprites_[(size_t)molecule.type]);
    }
}

bool ChamberWindow::Update(const sfmx::Event& event, const sfmx::RenderWindow& window) {
    (void)event;
    (void)window;
    return false;
}
