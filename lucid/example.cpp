#include "lucid/lucid.h"
using namespace lucid_engine;

int main() {
    g_window.get()->create_window("lucid engine", 100, 100, 1280, 720);
    g_graphics.get()->create_direct_3d();
    g_graphics.get()->create_device();
    g_renderer.get()->create_objects();
    g_io.get()->create();

    while (true) {
        if (!g_window.get()->dispatch_messages())
            break;

        g_io.get()->update();

        g_window.get()->set_window_title(std::vformat("lucid engine {} fps", std::make_format_args(g_io.get()->m_frame_rate_average)).c_str());

        g_graphics.get()->begin_scene({ 156, 129, 98 });
        {
            g_io.get()->demo_window(true);
        }
        g_renderer.get()->render_draw_data();
        g_graphics.get()->end_scene();

        g_io.get()->reset();
    }

    g_graphics.get()->release();

    return 0;
}