#include "gas_drawer.h"
#include "gas_buttons.h"
#include "gas_chamber_window.h"
#include "gas_actions.h"
#include "gas_graph.h"

#include <cassert>
#include <stdexcept>

using namespace gas;

// https://stackoverflow.com/questions/154469/why-should-you-prefer-unnamed-namespaces-over-static-functionsnamespace {
namespace {

const LinearTransition main_transition_;

}; // anon namespace

MainWindow::MainWindow(Controller* controller)
    : controller_(controller),
      button_infos_({{
            {&buttons_texture_, {800u, 160u}, {1 * 16u, 3 * 16u}, {7  * 16u, 0 * 16u}, {16u, 16u}, 10.f, std::make_unique<ChangeNumMoleculesAction>(controller_,  1, Molecule::Type::Squared)},
            {&buttons_texture_, {800u,   0u}, {1 * 16u, 0 * 16u}, {7  * 16u, 0 * 16u}, {16u, 16u}, 10.f, std::make_unique<ChangeNumMoleculesAction>(controller_,  1, Molecule::Type::Circular)},
            {&buttons_texture_, {960u, 160u}, {4 * 16u, 4 * 16u}, {10 * 16u, 1 * 16u}, {16u, 16u}, 10.f, std::make_unique<ChangeNumMoleculesAction>(controller_, -1, Molecule::Type::Squared)},
            {&buttons_texture_, {960u,   0u}, {4 * 16u, 1 * 16u}, {10 * 16u, 1 * 16u}, {16u, 16u}, 10.f, std::make_unique<ChangeNumMoleculesAction>(controller_, -1, Molecule::Type::Circular)},
            {&buttons_texture_, {800u, 320u}, {3 * 16u, 5 * 16u}, {9  * 16u, 2 * 16u}, {16u, 16u}, 10.f, std::make_unique<MovePistonAction>(controller_,  0.001f)}
        }}){

    LoadTextures();

    AddButtons();
    AddGraphs();
    AddChamber();
}

MainWindow::~MainWindow() = default;

void MainWindow::LoadTextures() {
    if (buttons_texture_.LoadFromFile(buttons_texture_path_) == false) {
        throw std::runtime_error("Can't find buttons' texture");
    }

    if (frame_texture_.LoadFromFile(frame_texture_path_) == false) {
        throw std::runtime_error("Can't find frame's texture");
    }
}

void MainWindow::AddChamber() {
    vec2u window_size = {800u, 480u};
    vec2u window_pos  = {  0u,   0u};

    // FIXME:
    sfmx::Sprite sprite1(buttons_texture_, sfmx::IntRect{9 * 16, 11 * 16, 16, 16});
    sfmx::Sprite sprite2(buttons_texture_, sfmx::IntRect{0     ,  0     , 16, 16});

    std::array<sfmx::Sprite, Molecule::kNumTypes> sprites = {{sprite1, sprite2}};
    
    windows_.push_back(std::make_unique<ChamberWindow>(
        controller_,
        window_size,
        window_pos,
        sprites
    ));
}

void MainWindow::AddGraphs() {
    windows_.emplace_back(std::make_unique<GraphWindow>(controller_, vec2u{0u, 480u}));
}

void MainWindow::AddButtons() {
    windows_.emplace_back(std::make_unique<ButtonAddMolecule>(controller_, button_infos_[0], main_transition_));
    windows_.emplace_back(std::make_unique<ButtonAddMolecule>(controller_, button_infos_[1], main_transition_));
    windows_.emplace_back(std::make_unique<ButtonAddMolecule>(controller_, button_infos_[2], main_transition_));
    windows_.emplace_back(std::make_unique<ButtonAddMolecule>(controller_, button_infos_[3], main_transition_));
    windows_.emplace_back(std::make_unique<ButtonMovePiston >(controller_, button_infos_[4]));
}

void MainWindow::Draw(sfmx::RenderWindow* window) {
    assert(window);

    for (const auto& buttons : windows_) {
        buttons->Draw(window);
    }
}

bool MainWindow::Update(const sfmx::Event& event, const sfmx::RenderWindow& window) {
    sfmx::Event idle_event{};
    idle_event.type = sfmx::Event::EventType::Count;

    // idle event
    for (const auto& buttons : windows_) {
        buttons->Update(idle_event, window);
    }

    // CoR event
    for (const auto& buttons : windows_) {
        if (buttons->Update(event, window)) {
            return true;
        }
    }

    return false;
}
