#pragma once

#include "../lucid.h"

namespace lucid_engine {
	class graphics : public singleton<graphics> {
	private:


	public:
		IDirect3D9* direct_3d;
		IDirect3DDevice9* direct_3d_device;
		D3DPRESENT_PARAMETERS direct_3d_paramaters;

		bool create_direct_3d();
		bool create_device();

		bool begin_scene();
		bool end_scene();
	};
}