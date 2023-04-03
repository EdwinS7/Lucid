#include "lucid/window/wnd.h"

int main() {
    lucid_engine::window::get_instance().create_window("lucid engine", 100, 100, 1280, 720);
    lucid_engine::graphics::get_instance().create_direct_3d();
    lucid_engine::graphics::get_instance().create_device();

    while (true) {
        if (!lucid_engine::window::get_instance().dispatch_messages())
            break;

        lucid_engine::graphics::get_instance().begin_scene();
        {
            lucid_engine::renderer::get_instance().filled_rectangle(vec2_t(0, 0), vec2_t(100, 100), color_t(255, 255, 255, 255));
        }
        lucid_engine::graphics::get_instance().end_scene();
    }

    return 0;
}