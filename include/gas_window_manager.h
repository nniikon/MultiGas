#ifndef GAS_GAS_WINDOW_MANAGER_H_
#define GAS_GAS_WINDOW_MANAGER_H_

#include "tgp_window.h"

namespace gas {

class WindowManager : public tgp::WindowManager {
public:
    void Draw(sfmx::RenderWindow* window) override;
    bool Update(const sfmx::Event& event, const sfmx::RenderWindow& window) override;

    ~WindowManager() override = default; 
};

}; // namespace

#endif  // GAS_GAS_WINDOW_MANAGER_H_
