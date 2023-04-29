#pragma once

#include "../lucid.h"

namespace lucid_engine {
	class graphics {
	private:
	public:
		IDirect3D9* direct_3d;
		IDirect3DDevice9* direct_3d_device;
		D3DPRESENT_PARAMETERS direct_3d_paramaters;

		bool create_direct_3d();
		bool create_device();

		bool begin_scene(int r = 0, int g = 0, int b = 0);
		bool end_scene();

		void reset_device();
		void release();
	};
	inline graphics g_graphics;
}