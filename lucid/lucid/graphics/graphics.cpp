#include "graphics.h"

bool lucid_engine::graphics::create_direct_3d() {
	direct_3d = Direct3DCreate9(D3D_SDK_VERSION);

	direct_3d_paramaters.Windowed = TRUE;
    direct_3d_paramaters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    direct_3d_paramaters.BackBufferFormat = D3DFMT_UNKNOWN;
    direct_3d_paramaters.EnableAutoDepthStencil = TRUE;
    direct_3d_paramaters.AutoDepthStencilFormat = D3DFMT_D16;

    /* to use VSync, use D3DPRESENT_INTERVAL_DEFAULT. to use NVSync use, D3DPRESENT_INTERVAL_IMMEDIATE */
    direct_3d_paramaters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    /* to use Anti Aliasing, you need this. Higher sample rate gives better picture quality but it comes at the cost of a performance loss */
    direct_3d_paramaters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
    direct_3d_paramaters.MultiSampleQuality = 0;

    return true;
}

bool lucid_engine::graphics::create_device() {
    if (direct_3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, lucid_engine::window::get_instance().get_hwnd(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &direct_3d_paramaters, &direct_3d_device) < 0) {
        throw std::runtime_error{ "CreateDevice error" };
        return false;
    }

    direct_3d_device->SetPixelShader(nullptr);
    direct_3d_device->SetVertexShader(nullptr);
    direct_3d_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    direct_3d_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    direct_3d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    direct_3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    direct_3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    direct_3d_device->SetRenderState(D3DRS_ZENABLE, FALSE);
    direct_3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    direct_3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    direct_3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    direct_3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    direct_3d_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
    direct_3d_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
    direct_3d_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
    direct_3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    direct_3d_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    direct_3d_device->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
    direct_3d_device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
    direct_3d_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    direct_3d_device->SetRenderState(D3DRS_CLIPPING, TRUE);
    direct_3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);
    direct_3d_device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
    direct_3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    direct_3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    direct_3d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    direct_3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    direct_3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    direct_3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    direct_3d_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    direct_3d_device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    direct_3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    direct_3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    direct_3d_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    direct_3d_device->SetTexture(NULL, NULL);
    direct_3d_device->SetPixelShader(nullptr);
    direct_3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

    return true;
}

bool lucid_engine::graphics::begin_scene(int r, int g, int b) {
    direct_3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(r, g, b, NULL), 1.f, 0);
    direct_3d_device->BeginScene();

    return true;
}

bool lucid_engine::graphics::end_scene() {
    direct_3d_device->EndScene();
    direct_3d_device->Present(NULL, NULL, NULL, NULL);

    return true;
}

void lucid_engine::graphics::reset_device() {
    direct_3d_device->Release();
    create_device();

    lucid_engine::renderer::get_instance().destroy_objects();
    lucid_engine::renderer::get_instance().create_objects();
}

void lucid_engine::graphics::release() {
    lucid_engine::renderer::get_instance().destroy_objects();
    direct_3d_device->Release();
    direct_3d->Release();
}