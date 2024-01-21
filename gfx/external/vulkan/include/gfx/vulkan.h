/*
 * Copyright (c) 2023 Gxin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GFX_VULKAN_H
#define GFX_VULKAN_H

#include <gfx/compiler.h>


#if GX_PLATFORM_ANDROID
#   define VK_USE_PLATFORM_ANDROID_KHR
#   define KHR_SURFACE_EXTENSION_NAME VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
//# define VK_IMPORT_INSTANCE_PLATFORM VK_IMPORT_INSTANCE_ANDROID
#elif GX_PLATFORM_LINUX
//# define VK_USE_PLATFORM_MIR_KHR
#   define VK_USE_PLATFORM_XLIB_KHR
//# define VK_USE_PLATFORM_XCB_KHR
//# define VK_USE_PLATFORM_WAYLAND_KHR
#   define KHR_SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME
//# define VK_IMPORT_INSTANCE_PLATFORM VK_IMPORT_INSTANCE_LINUX
#elif GX_PLATFORM_WINDOWS
#   define VK_USE_PLATFORM_WIN32_KHR
#   define KHR_SURFACE_EXTENSION_NAME  VK_KHR_WIN32_SURFACE_EXTENSION_NAME
//#	define VK_IMPORT_INSTANCE_PLATFORM VK_IMPORT_INSTANCE_WINDOWS
#elif GX_PLATFORM_OSX || GX_PLATFORM_IOS
#   define VK_USE_PLATFORM_MACOS_MVK
#   define KHR_SURFACE_EXTENSION_NAME  VK_MVK_MACOS_SURFACE_EXTENSION_NAME
//#	define VK_IMPORT_INSTANCE_PLATFORM VK_IMPORT_INSTANCE_MACOS
#else
#   define KHR_SURFACE_EXTENSION_NAME ""
#   define VK_IMPORT_INSTANCE_PLATFORM
#endif // GX_PLATFORM_*

#include "volk.h"

#endif //GFX_VULKAN_H
