#include "lucid/lucid.h"

int main() {
    lucid_engine::window::get_instance().create_window("lucid engine", 100, 100, 1280, 720);
    lucid_engine::graphics::get_instance().create_direct_3d();
    lucid_engine::graphics::get_instance().create_device();
    lucid_engine::renderer::get_instance().create_objects();

    while (true) {
        if (!lucid_engine::window::get_instance().dispatch_messages())
            break;

        lucid_engine::graphics::get_instance().begin_scene();
        {
            lucid_engine::renderer::get_instance().filled_rectangle(vec2_t(0, 0), vec2_t(100, 100), color_t(255, 0, 255, 255));
            lucid_engine::renderer::get_instance().render_draw_data();
        }
        lucid_engine::graphics::get_instance().end_scene();
    }

    return 0;
}