#include "gas_window_manager.h"

using namespace gas;

void WindowManager::Draw(sfmx::RenderWindow* window) {
    main_window_->Draw(window);
}

bool WindowManager::Update(const sfmx::Event& event, const sfmx::RenderWindow& window) {
    return main_window_->Update(event, window);
}
