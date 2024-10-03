#ifndef GAS_DRAWER_H_
#define GAS_DRAWER_H_

#include "gas_buttons.h"
#include "tgp_window.h"

#include "gas_controller.h"
#include "gas_engine.h"

using tgp::Action;

namespace gas {

class MainWindow : public tgp::WindowContainerBase {
public:
    MainWindow(Controller* controller); 

    void Draw(sfmx::RenderWindow* window) override;
    bool Update(const sfmx::Event& event, const sfmx::RenderWindow& window) override;

    ~MainWindow() override;

private:
    static constexpr char buttons_texture_path_[] = "textures/buttons.png";
    static constexpr char   frame_texture_path_[] = "textures/frame.png";

    sfmx::Texture buttons_texture_;
    sfmx::Texture   frame_texture_;

    Controller* controller_ = nullptr;

    sfmx::Sprite CreateButtonSprite(const vec2u& texture_pos,
                                    const vec2u& size,
                                    float scale) const;

    std::array<HoveringButtonTextureInfo, 5> button_infos_;

    void LoadTextures();

    void AddChamber();
    void AddGraphs();
    void AddButtons();
};

}; // namespace

#endif // GAS_DRAWER_H_
