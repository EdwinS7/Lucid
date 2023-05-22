#include "lucid/lucid.h"
using namespace lucid_engine;

int main() {
    g_window->create_window("lucid engine", 100, 100, 1280, 720);

    if (!g_graphics->create_direct_3d())
        throw std::runtime_error{ "failed to create direct context" };

    if (!g_graphics->create_device())
        throw std::runtime_error{ "failed to create device" };

    g_renderer->create_objects();
    g_io->create();

    while (g_window->dispatch_messages()) {
        g_io->update();

        g_window->set_window_title(std::vformat("lucid engine {} fps", std::make_format_args(g_io->m_frame_rate_average)).c_str());

        g_graphics->begin_scene({ 156, 129, 98 });
        {
            g_io->demo_window(true);
        }
        g_renderer->render_draw_data();
        g_graphics->end_scene();
        g_io->reset();
    }

    g_graphics->release();

    return EXIT_SUCCESS;
}