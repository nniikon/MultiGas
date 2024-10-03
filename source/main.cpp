#include "gas_window_manager.h"
#include "gas_drawer.h"
#include "tgp_application.h"

const unsigned int kWindowWidth  = 800u;
const unsigned int kWindowHeight = 800u;
const unsigned int kButtonWidth  = 160u;

int main() {
    gas::Engine engine{{kWindowWidth, kWindowHeight}, {0u, 0u}};
    engine.CreateRandomMolecule(gas::Molecule::Type::Circular);
    engine.CreateRandomMolecule(gas::Molecule::Type::Squared);

    gas::Controller controller{nullptr, nullptr};
    std::unique_ptr<gas::WindowManager> manager = std::make_unique<gas::WindowManager>();
    std::unique_ptr<gas::MainWindow>    window  = std::make_unique<gas::MainWindow>(&controller);
    controller.SetDrawer(window.get());
    controller.SetEngine(&engine);

    manager->SetMainWindow(std::move(window));

    tgp::ApplicationBuilder application_builder{};
    tgp::Application application = application_builder
        .SetWindowPosition(sfmx::vec2u(0, 0))
        .SetWindowSize(sfmx::vec2u(kWindowWidth + 2 * kButtonWidth, kWindowHeight))
        .SetWindowName("TGPGas")
        .SetFramerate(60)
        .SetAdamWindow(std::move(manager))
        .Build();

    while (application.IsRunning()) {
        application.Run();

        engine.Update(0.02f);
    }
}
