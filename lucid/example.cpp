#include "lucid/lucid.h"
using namespace lucid_engine;

int main() {
    g_window->create_window("lucid engine", 100, 100, 1280, 720);
    g_graphics->create_direct_3d();
    g_graphics->create_device();
    g_renderer->create_objects();
    g_io->create();

    while (true) {
        if (!g_window->dispatch_messages())
            break;

        g_io->update();

        g_window->set_window_title(std::vformat("lucid engine {} fps", std::make_format_args(g_io->m_frame_rate_average)).c_str());

        g_graphics->begin_scene({ 156, 129, 98 });
        {
            vec2_t text_size = g_renderer->get_text_size(g_renderer->m_defualt_font, "big nigger test balls");
            g_renderer->filled_rectangle({ 0, 0 }, text_size, { 0, 0, 0, 255 });
            g_renderer->text(g_renderer->m_defualt_font, "big nigger test balls", {0, 0}, {255, 255, 255, 255});
            g_io->demo_window(true);
        }
        g_renderer->render_draw_data();
        g_graphics->end_scene();

        g_io->reset();
    }

    g_graphics->release();

    return 0;
}