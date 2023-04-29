#pragma once

#include "../../lucid.h"

struct font_flags_t {
    bool m_anti_alias,
         m_drop_shadow,
         m_outline;

    font_flags_t() {
        m_anti_alias  = false;
        m_drop_shadow = false;
        m_outline     = false;
    }

    font_flags_t(bool anti_alias, bool drop_shadow, bool outline)
        : m_anti_alias(anti_alias), m_drop_shadow(drop_shadow), m_outline(outline) {}
};

struct font_t {
    LPD3DXFONT   m_dx_font    = {};
    font_flags_t m_font_flags = {};

    font_t() {}
    font_t(IDirect3DDevice9* device, const char* name, int size, int weight, font_flags_t font_flags) {
        D3DXCreateFontA(device, size, 0, weight, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, font_flags.m_anti_alias ? CLEARTYPE_QUALITY /* LOL */ : DEFAULT_QUALITY, DEFAULT_PITCH, name, &m_dx_font);
        m_font_flags = font_flags;
    }
};