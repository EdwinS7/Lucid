#include "../../lucid.h"

struct texture_t {
    LPDIRECT3DTEXTURE9 m_texture{ };

    texture_t() {}
    texture_t(IDirect3DDevice9* device, BYTE texture[], vec2_t size) {
        D3DXCreateTextureFromFileInMemoryEx(device, texture, INT_MAX, size.x,
            size.y, D3DX_DEFAULT, NULL, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &m_texture);
       
        if (!m_texture)
            printf("failed to create texture\n");
    }

    texture_t(IDirect3DDevice9* device, std::string file_name, vec2_t size) {
        D3DXCreateTextureFromFile(device, file_name.c_str(), &m_texture);

        if (!m_texture)
            printf("failed to create texture\n");
    }

    texture_t(LPDIRECT3DTEXTURE9 texture) {
        m_texture = texture;
    }
};