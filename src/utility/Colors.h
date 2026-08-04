#pragma once
#include <cstdint>

namespace Colors {

    inline uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return (r << 0) | (g << 8) | (b << 16) | (a << 24);
    }
    const uint32_t black = rgba(0, 0, 0, 255);
    const uint32_t white = rgba(255, 255, 255, 255);
    const uint32_t red = rgba(255, 0, 0, 255);
    const uint32_t green = rgba(0, 255, 0, 255);
    const uint32_t blue = rgba(0, 0, 255, 255);
    const uint32_t yellow = rgba(255, 255, 0, 255);
    const uint32_t cyan = rgba(0, 255, 255, 255);
    const uint32_t magenta = rgba(255, 0, 255, 255);
    const uint32_t orange = rgba(255, 165, 0, 255);
    const uint32_t purple = rgba(128, 0, 128, 255);
    const uint32_t brown = rgba(148, 86, 0, 255);
    const uint32_t transparent = rgba(0, 0, 0, 0);
}