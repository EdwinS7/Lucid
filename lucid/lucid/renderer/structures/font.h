#pragma once

#include "../../lucid.h"

struct font_t {
    LPD3DXFONT   m_dx_font{ };

    font_t() {}
    font_t(IDirect3DDevice9* device, const char* name, int size, int weight, bool anti_alias) {
        D3DXCreateFontA(device, size, 0, weight, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, anti_alias ? CLEARTYPE_QUALITY /* LOL */ : DEFAULT_QUALITY, DEFAULT_PITCH, name, &m_dx_font);
    }
};