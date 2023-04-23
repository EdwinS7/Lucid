#include "lucid/lucid.h"

int main() {
    lucid_engine::window::get_instance().create_window("lucid engine", 100, 100, 1280, 720);
    lucid_engine::graphics::get_instance().create_direct_3d();
    lucid_engine::graphics::get_instance().create_device();
    lucid_engine::renderer::get_instance().create_objects();
    lucid_engine::io::get_instance().create();

    while (true) {
        if (!lucid_engine::window::get_instance().dispatch_messages())
            break;

        lucid_engine::io::get_instance().update();

        lucid_engine::window::get_instance().set_window_title(std::format("lucid engine {} fps", lucid_engine::io::get_instance().frame_rate_average).c_str());

        lucid_engine::graphics::get_instance().begin_scene(156, 129, 98);
        {
            /*lucid_engine::renderer::get_instance().text(lucid_engine::renderer::get_instance().fonts.default_font, "edwnGui 2.0", vec2_t(0, 0), color_t(255, 255, 255, 255));
            lucid_engine::renderer::get_instance().rectangle({ 50, 50 }, { 100, 100 }, color_t(34, 108, 199, 255));
            lucid_engine::renderer::get_instance().rounded_rectangle({ 200, 50 }, { 100, 100 }, color_t(34, 108, 199, 255), 25);
            lucid_engine::renderer::get_instance().filled_rectangle({ 350, 50 }, { 100, 100 }, color_t(34, 108, 199, 255));
            lucid_engine::renderer::get_instance().filled_rounded_rectangle({ 500, 50 }, { 100, 100 }, color_t(34, 108, 199, 255), 25);
            lucid_engine::renderer::get_instance().gradient({ 950, 50 }, { 100, 100 }, color_t(34, 108, 199, 255), color_t(255, 255, 255, 255));
            lucid_engine::renderer::get_instance().gradient({ 1100, 50 }, { 100, 100 }, color_t(34, 108, 199, 255), color_t(255, 255, 255, 255), true);
            lucid_engine::renderer::get_instance().filled_gradient({ 50, 200 }, { 100, 100 }, color_t(34, 108, 199, 255), color_t(255, 255, 255, 255));
            lucid_engine::renderer::get_instance().filled_gradient({ 200, 200 }, { 100, 100 }, color_t(34, 108, 199, 255), color_t(255, 255, 255, 255), true);
            lucid_engine::renderer::get_instance().gradient_four({ 350, 200 }, { 100, 100 }, color_t(34, 108, 199, 255), color_t(255, 255, 255, 255), color_t(255, 0, 0, 255), color_t(0, 0, 255, 255));
            lucid_engine::renderer::get_instance().filled_gradient_four({ 500, 200 }, { 100, 100 }, color_t(34, 108, 199, 255), color_t(255, 255, 255, 255), color_t(255, 0, 0, 255), color_t(0, 0, 255, 255));
            lucid_engine::renderer::get_instance().triangle({ 650, 200 }, { 100, 100 }, color_t(34, 108, 199, 255));
            lucid_engine::renderer::get_instance().filled_triangle({ 800, 200 }, { 100, 100 }, color_t(34, 108, 199, 255));
            lucid_engine::renderer::get_instance().gradient_triangle({ 950, 200 }, { 100, 100 }, color_t(0, 0, 0, 0), color_t(34, 108, 199));
            lucid_engine::renderer::get_instance().circle({ 1150, 250 }, 50, 75, 0, color_t(34, 108, 199, 255));
            lucid_engine::renderer::get_instance().filled_circle({ 100, 400 }, 50, 75, 0, color_t(34, 108, 199, 255));
            lucid_engine::renderer::get_instance().gradient_circle({ 250, 400 }, 50, 75, 0, color_t(0, 0, 0, 0), color_t(34, 108, 199));*/

            lucid_engine::io::get_instance().demo_window(true);
        }
        lucid_engine::renderer::get_instance().render_draw_data();
        lucid_engine::graphics::get_instance().end_scene();

        lucid_engine::io::get_instance().reset();
    }

    lucid_engine::graphics::get_instance().release();

    return 0;
}