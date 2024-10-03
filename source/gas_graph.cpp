#include "gas_graph.h"
#include <iostream>
#include <ostream>

using namespace gas;

GraphWindow::GraphWindow(Controller* controller, const vec2u& pos)
    : controller_(controller),
      pos_(pos),
      graph_data_(kGraphWidth),
      graph_cursor_(0ul) {

    if (!graph_texture_.LoadFromFile(graph_texture_path_)) {
        throw std::runtime_error("Failed to load graph texture");
    }

    if (!cross_texture_.LoadFromFile(cross_texture_path_)) {
        throw std::runtime_error("Failed to load graph cross texture");
    }

    graph_sprite_.SetTexture(graph_texture_);
    cross_sprite_.SetTexture(cross_texture_);

    graph_sprite_.SetPosition(float(pos_.x), float(pos_.y));
}

GraphWindow::~GraphWindow() = default;

void GraphWindow::Draw(sfmx::RenderWindow* window) {
    window->Draw(graph_sprite_);

    for (float i = 0.f; i < float(kGraphWidth); i++) {
        cross_sprite_.SetPosition(float(pos_.x) + i, float(pos_.y) + float(kGraphHeight) / 2 - graph_data_[size_t(i)] * kGraphHeight / 2);
        window->Draw(cross_sprite_);
    }
}

bool GraphWindow::Update(const sfmx::Event& event, const sfmx::RenderWindow& window) {
    std::cout << "Temp: " << controller_->GetTemp() << std::endl;
    graph_data_[graph_cursor_] = controller_->GetTemp();

    graph_cursor_ = (graph_cursor_ + 1) % kGraphWidth;

    return false;
}
