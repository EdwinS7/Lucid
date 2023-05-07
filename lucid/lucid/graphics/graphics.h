#pragma once

#include "../lucid.h"

namespace lucid_engine {
	class graphics {
	private:
	public:
		IDirect3D9*			  m_direct_3d{ nullptr };
		IDirect3DDevice9*	  m_direct_3d_device{ nullptr };
		D3DPRESENT_PARAMETERS m_direct_3d_paramaters{ };

		bool create_direct_3d();
		void setup_render_states();
		bool create_device();

		bool begin_scene(color_t color);
		bool end_scene();

		void reset_device();
		void release();
	};
	inline const auto g_graphics = std::make_unique< graphics >();
}