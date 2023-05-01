#ifndef _LEARN_OPENGL_SIMPLE_SHAPES_HPP_
#define _LEARN_OPENGL_SIMPLE_SHAPES_HPP_

#include <array>

#include <common.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

// clang-format off
constexpr std::array<float, 192> CUBE_VERTICES {
    // Position(3), Normal(3), TexCoord(2)
    // Back
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    // Front
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
    // Left
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    // Right
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 0.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f, -1.0f, 0.0f, 0.0f,
    // Bottom
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f, -1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f, -1.0f, 1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f, -1.0f, 0.0f, 0.0f,
    // Top
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f, -1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  1.0f, -1.0f, 1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f, 0.0f, 0.0f,
};
constexpr std::array<uint32_t, 36> CUBE_INDICES {
     0,  1,  2,  0,  2,  3,
     4,  5,  6,  4,  6,  7,
     8,  9, 10,  8, 10, 11,
    12, 13, 14, 12, 14, 15,
    16, 17, 18, 16, 18, 19,
    20, 21, 22, 20, 22, 23,
};

constexpr std::array<float, 32> PLANE_VERTICES {
    // Position(3), Normal(3), TexCoord(2)
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
};
constexpr std::array<uint32_t, 6> PLANE_INDICES {
    0, 1, 2,
    0, 2, 3,
};

constexpr std::array<float, 28> TETRAHEDRON_VERTICES {
    // Position(3), Color(4)
     1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
};
constexpr std::array<uint32_t, 12> TETRAHEDRON_INDICES {
    0, 1, 2,
    0, 1, 3,
    0, 2, 3,
    1, 2, 3,
};

constexpr std::array<float, 44> UV_VERTICES {
    // Position(3), Color(4), TexCoord(2), TexIndex(1), TilingScale(1)
    -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};
constexpr std::array<uint32_t, 6> UV_INDICES {
    0, 1, 2,
    0, 2, 3,
};
// clang-format on

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_SIMPLE_SHAPES_HPP_
