#pragma once

#include "../../lucid.h"

struct font_flags_t {
    bool anti_alias, drop_shadow, outline;

    font_flags_t() {
        anti_alias = false;
        drop_shadow = false;
        outline = false;
    }

    font_flags_t(bool _anti_alias, bool _drop_shadow, bool _outline)
        : anti_alias(_anti_alias), drop_shadow(_drop_shadow), outline(_outline) {}
};

struct font_t {
    LPD3DXFONT dx_font = {};
    font_flags_t font_flags;

    font_t() {}
    font_t(IDirect3DDevice9* pDevice, const char* name, int size, int weight, font_flags_t _font_flags) {
        D3DXCreateFontA(pDevice, size, 0, weight, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, _font_flags.anti_alias ? CLEARTYPE_NATURAL_QUALITY /* LOL */ : DEFAULT_QUALITY, DEFAULT_PITCH, name, &dx_font);
        font_flags = _font_flags;
    }
};