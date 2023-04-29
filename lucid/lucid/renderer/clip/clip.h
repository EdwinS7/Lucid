#pragma once
#include <stack>

struct clip_info_t {
    std::stack < std::pair<bool, rect_t>> m_old_clips;

    bool m_clipping;
    rect_t m_clip;

    void clear() {
        m_clipping = false;
        m_old_clips = {};
        m_clip = {};
    }
};