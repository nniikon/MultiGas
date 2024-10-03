#ifndef GAS_GAS_GRAPH_H_
#define GAS_GAS_GRAPH_H_

#include "gas_controller.h"
#include "tgp_window.h"

namespace gas {

using sfmx::vec2u;

class GraphWindow : public tgp::WindowBase {
public:
    void Draw(sfmx::RenderWindow* window) override;
    bool Update(const sfmx::Event& event, const sfmx::RenderWindow& window) override;

    GraphWindow(Controller* controller, const vec2u& pos);
    ~GraphWindow() override;

private:
    constexpr static float kGraphScale = 0.1f;

    Controller* controller_ = nullptr;
    vec2u pos_;

    constexpr static const char graph_texture_path_[] = "textures/graph.png";
    sfmx::Texture graph_texture_;
    constexpr static const char cross_texture_path_[] = "textures/cross.png";
    sfmx::Texture cross_texture_;

    sfmx::Sprite graph_sprite_;
    sfmx::Sprite cross_sprite_;

    constexpr static const unsigned int kGraphWidth  = 800u;
    constexpr static const unsigned int kGraphHeight = 400u;

    std::vector<float> graph_data_;
    size_t graph_cursor_ = 0ul;
};

}; // namespace gas

#endif  // GAS_GAS_GRAPH_H_
