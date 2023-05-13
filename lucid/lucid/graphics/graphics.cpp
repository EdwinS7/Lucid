#include "graphics.h"

bool lucid_engine::graphics::create_direct_3d() {
	m_direct_3d = Direct3DCreate9(D3D_SDK_VERSION);

	m_direct_3d_paramaters.Windowed = TRUE;
    m_direct_3d_paramaters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_direct_3d_paramaters.BackBufferFormat = D3DFMT_UNKNOWN;
    m_direct_3d_paramaters.EnableAutoDepthStencil = TRUE;
    m_direct_3d_paramaters.AutoDepthStencilFormat = D3DFMT_D16;

    /* to use VSync, use D3DPRESENT_INTERVAL_DEFAULT. to use NVSync use, D3DPRESENT_INTERVAL_IMMEDIATE */
    m_direct_3d_paramaters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    /* to use Anti Aliasing, you need this. Higher sample rate gives better picture quality but it comes at the cost of a performance loss */
    m_direct_3d_paramaters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
    m_direct_3d_paramaters.MultiSampleQuality = 0;

    return true;
}

void  lucid_engine::graphics::setup_render_states() { 
    m_direct_3d_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_direct_3d_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    m_direct_3d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_direct_3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_direct_3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_direct_3d_device->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_direct_3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_direct_3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    m_direct_3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_direct_3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_direct_3d_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
    m_direct_3d_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
    m_direct_3d_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
    m_direct_3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    m_direct_3d_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    m_direct_3d_device->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
    m_direct_3d_device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
    m_direct_3d_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    m_direct_3d_device->SetRenderState(D3DRS_CLIPPING, TRUE);
    m_direct_3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_direct_3d_device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
    m_direct_3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

    m_direct_3d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_direct_3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_direct_3d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    m_direct_3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_direct_3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_direct_3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    m_direct_3d_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    m_direct_3d_device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    m_direct_3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_direct_3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    m_direct_3d_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    m_direct_3d_device->SetTexture(NULL, NULL);
    m_direct_3d_device->SetVertexShader(nullptr);
    m_direct_3d_device->SetPixelShader(nullptr);
}

bool lucid_engine::graphics::create_device() {
    if (m_direct_3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
        g_window->get_hwnd(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_direct_3d_paramaters, &m_direct_3d_device) < 0) {
        throw std::runtime_error{ "CreateDevice error" };
        return false;
    }

    setup_render_states();

    return true;
}

bool lucid_engine::graphics::begin_scene(color_t color) {
    m_direct_3d_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(color.r, color.g, color.b, NULL), 1.f, 0);
    m_direct_3d_device->BeginScene();

    return true;
}

bool lucid_engine::graphics::end_scene() {
    m_direct_3d_device->EndScene();
    m_direct_3d_device->Present(NULL, NULL, NULL, NULL);

    return true;
}

void lucid_engine::graphics::reset_device() {
    m_direct_3d_device->Release();
    create_device();

    g_renderer->destroy_objects();
    g_renderer->create_objects();
}

void lucid_engine::graphics::release() {
    g_renderer->destroy_objects();
    m_direct_3d_device->Release();
    m_direct_3d->Release();
}