#ifndef GAS_CHAMBER_WINDOW_H_
#define GAS_CHAMBER_WINDOW_H_

#include "gas_controller.h"
#include "gas_engine.h"
#include "tgp_window.h"

namespace gas {

class ChamberWindow : public tgp::WindowBase {
public:
    ChamberWindow(Controller* controller,
                  const sfmx::vec2u& window_size, 
                  const sfmx::vec2u& window_pos, 
                  std::array<sfmx::Sprite, Molecule::kNumTypes>& sprites);

    void Draw(sfmx::RenderWindow* window) override;
    bool Update(const sfmx::Event& event, const sfmx::RenderWindow& window) override;

private:
    Controller* controller_ = nullptr;
    
    sfmx::vec2u window_size_;
    sfmx::vec2u window_pos_;
    std::array<sfmx::Sprite, Molecule::kNumTypes> sprites_;
};

};

#endif // GAS_CHAMBER_WINDOW_H_
