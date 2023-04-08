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

            static std::vector<SnowFlake_t> SnowFlakes;
            static const int Count = 2000;
            static bool Init = false;

            if (!Init)
            {
                for (int n = 0; n < Count; n++)
                {
                    SnowFlake_t Flake = {};
                    Flake.x = static_cast<float>(lucid_engine::io::get_instance().random_int(0, 1280));
                    Flake.y = -static_cast<float>(lucid_engine::io::get_instance().random_int(50, 500));
                    Flake.fall = static_cast<float>(lucid_engine::io::get_instance().random_int(50, 100));
                    Flake.drift = static_cast<float>(lucid_engine::io::get_instance().random_int(5, 20));
                    Flake.size = lucid_engine::io::get_instance().random_int(1, 4) == 4 ? 2 : 1;
                    SnowFlakes.push_back(Flake);
                }

                Init = true;
            }

            for (auto& Flake : SnowFlakes)
            {
                Flake.x += sinf(lucid_engine::io::get_instance().delta_time) * Flake.drift;
                Flake.y += Flake.fall * lucid_engine::io::get_instance().delta_time;

                float Alpha = lucid_engine::io::get_instance().map(Flake.y, 0.0f, static_cast<float>(720 - 200), 1.0f, 0.0f);

                if (Alpha <= 0.0f)
                {
                    Flake.x = static_cast<float>(lucid_engine::io::get_instance().random_int(0, 1280));
                    Flake.y = -static_cast<float>(lucid_engine::io::get_instance().random_int(50, 500));
                    Flake.fall = static_cast<float>(lucid_engine::io::get_instance().random_int(50, 100));
                    Flake.drift = static_cast<float>(lucid_engine::io::get_instance().random_int(5, 20));
                    Flake.size = lucid_engine::io::get_instance().random_int(1, 4) == 4 ? 2 : 1;
                }

                color_t clr = { 255, 255, 255, static_cast<int>(Alpha * 255.0f) };
                lucid_engine::renderer::get_instance().filled_rectangle(vec2_t(static_cast<int>(Flake.x), static_cast<int>(Flake.y)), vec2_t(Flake.size, Flake.size), clr);
            }

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