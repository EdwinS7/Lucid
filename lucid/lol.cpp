#include "lucid/lucid.h"

int main() {
    lucid_engine::g_window.create_window("lucid engine", 100, 100, 1280, 720);
    lucid_engine::g_graphics.create_direct_3d();
    lucid_engine::g_graphics.create_device();
    lucid_engine::g_renderer.create_objects();
    lucid_engine::g_io.create();

    while (true) {
        if (!lucid_engine::g_window.dispatch_messages())
            break;

        lucid_engine::g_io.update();

        lucid_engine::g_window.set_window_title(std::vformat("lucid engine {} fps", std::make_format_args(lucid_engine::g_io.frame_rate_average)).c_str());

        lucid_engine::g_graphics.begin_scene(156, 129, 98);
        {
            lucid_engine::g_io.demo_window(true);
        }
        lucid_engine::g_renderer.render_draw_data();
        lucid_engine::g_graphics.end_scene();

        lucid_engine::g_io.reset();
    }

    lucid_engine::g_graphics.release();

    return 0;
}