#include "../../lucid.h"

struct texture_t {
    LPDIRECT3DTEXTURE9  m_data{ };

    texture_t() {}
    texture_t(IDirect3DDevice9* device, BYTE texture[], vec2_t size) {
        if (D3DXCreateTextureFromFileInMemoryEx(device, texture, INT_MAX, size.x,
            size.y, D3DX_DEFAULT, NULL, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, NULL, NULL, NULL, &m_data) != D3D_OK)
            throw std::runtime_error{ "texture_t -> failed to create texture" };
    }

    texture_t(IDirect3DDevice9* device, std::string file_name, vec2_t size) {
        if (D3DXCreateTextureFromFile(device, file_name.c_str(), &m_data) != D3D_OK)
            throw std::runtime_error{ "texture_t -> failed to create texture" };
    }

    texture_t(LPDIRECT3DTEXTURE9 texture) {
        m_data = texture;
    }
};