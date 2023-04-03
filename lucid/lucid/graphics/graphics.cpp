#include "graphics.h"

bool lucid_engine::graphics::create_direct_3d() {
	direct_3d = Direct3DCreate9(D3D_SDK_VERSION);

	direct_3d_paramaters.Windowed = true;
    direct_3d_paramaters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    direct_3d_paramaters.BackBufferFormat = D3DFMT_UNKNOWN;
    direct_3d_paramaters.EnableAutoDepthStencil = TRUE;
    direct_3d_paramaters.AutoDepthStencilFormat = D3DFMT_D16;

    /* to use VSync, use D3DPRESENT_INTERVAL_ONE */
    direct_3d_paramaters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    direct_3d_paramaters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
    direct_3d_paramaters.MultiSampleQuality = 0;

    return true;
}

bool lucid_engine::graphics::create_device() {
    direct_3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, lucid_engine::window::get_instance().get_hwnd(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &direct_3d_paramaters, &direct_3d_device);

    return true;
}

bool lucid_engine::graphics::begin_scene() {
    direct_3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(255, 0, 0, NULL), 1.f, 0);
    direct_3d_device->BeginScene();

    return true;
}

bool lucid_engine::graphics::end_scene() {
    direct_3d_device->EndScene();
    direct_3d_device->Present(NULL, NULL, NULL, NULL);

    return true;
}