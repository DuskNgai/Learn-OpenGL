#ifndef _LEARN_OPENGL_COMMON_CUH_
#define _LEARN_OPENGL_COMMON_CUH_

#include <iostream>
#include <memory>
#include <stdexcept>

#include <fmt/core.h>

// clang-format off
#define LEARN_OPENGL_NAMESPACE_BEGIN namespace LearnGL {
#define LEARN_OPENGL_NAMESPACE_END                     }

LEARN_OPENGL_NAMESPACE_BEGIN

//! Constants
template<typename T> T constexpr PI          = 3.14159265358979323846l;
template<typename T> T constexpr INV_PI      = 0.31830988618379067154l;
template<typename T> T constexpr FRAC_1_2PI  = 0.15915494309189533577l;
template<typename T> T constexpr FRAC_1_4PI  = 0.07957747154594766788l;
template<typename T> T constexpr FRAC_PI_2   = 1.57079632679489661923l;
template<typename T> T constexpr FRAC_PI_4   = 0.78539816339744830961l;
template<typename T> T constexpr FRAC_PI_180 = 0.01745329251994329577l;
template<typename T> T constexpr FRAC_180_PI = 57.2957795130823208768l;
template<typename T> T constexpr SQRT_2      = 1.41421356237309504880l;
template<typename T> T constexpr SQRT_3      = 1.73205080756887729353l;
//! Constants

#define STRINGIFY(x) #x
#define STR(x) STRINGIFY(x)
#define FILE_AND_LINE "file " __FILE__ " line " STR(__LINE__)

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_COMMON_CUH_
