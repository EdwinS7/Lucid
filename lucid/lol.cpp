#include "lucid/lucid.h"

int main() {
    lucid_engine::g_window.get()->create_window("lucid engine", 100, 100, 1280, 720);
    lucid_engine::g_graphics.get()->create_direct_3d();
    lucid_engine::g_graphics.get()->create_device();
    lucid_engine::g_renderer.get()->create_objects();
    lucid_engine::g_io.get()->create();

    while (true) {
        if (!lucid_engine::g_window.get()->dispatch_messages())
            break;

        lucid_engine::g_io.get()->update();

        lucid_engine::g_window.get()->set_window_title(std::vformat("lucid engine {} fps", std::make_format_args(lucid_engine::g_io.get()->m_frame_rate_average)).c_str());

        lucid_engine::g_graphics.get()->begin_scene({ 156, 129, 98 });
        {
            lucid_engine::g_io.get()->demo_window(true);
        }
        lucid_engine::g_renderer.get()->render_draw_data();
        lucid_engine::g_graphics.get()->end_scene();

        lucid_engine::g_io.get()->reset();
    }

    lucid_engine::g_graphics.get()->release();

    return 0;
}