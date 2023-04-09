#include "lucid/lucid.h"

struct SnowFlake_t {
    float x = 0.0f, y = 0.0f, fall = 0.0f, drift = 0.0f;
    int size = 1;
};

int main() {
    lucid_engine::window::get_instance().create_window("lucid engine", 100, 100, 1280, 720);
    lucid_engine::graphics::get_instance().create_direct_3d();
    lucid_engine::graphics::get_instance().create_device();
    lucid_engine::renderer::get_instance().create_objects();

    while (true) {
        if (!lucid_engine::window::get_instance().dispatch_messages())
            break;

        lucid_engine::io::get_instance().update();

        lucid_engine::window::get_instance().set_window_title(std::format("lucid engine {} fps", lucid_engine::io::get_instance().frame_rate_average).c_str());

        lucid_engine::graphics::get_instance().begin_scene();
        {
            //lucid_engine::renderer::get_instance().filled_rectangle(vec2_t(10, 10), vec2_t(100, 100), color_t(255, 255, 255, 255));
            //lucid_engine::renderer::get_instance().filled_rectangle(vec2_t(150, 200), vec2_t(100, 100), color_t(255, 0, 0, 255));
            lucid_engine::renderer::get_instance().render_draw_data();
        }
        lucid_engine::graphics::get_instance().end_scene();

        lucid_engine::io::get_instance().reset();
    }

    lucid_engine::graphics::get_instance().release();

    return 0;
}