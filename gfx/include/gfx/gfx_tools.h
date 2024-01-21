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

#ifndef GX_GFX_TOOLS_H
#define GX_GFX_TOOLS_H

#include <gfx/gfx_base.h>
#include <gfx/gfx_def.h>
#include <gx/gstring.h>

namespace gfx
{

GX_API void printDeviceInfo(const DeviceInfo &deviceInfo);

GX_API std::string getAPIVersion2String(uint32_t version);

/**
 * 依据给出的对齐规则分配内存
 *
 * @param size
 * @param alignment
 * @return
 */
GX_API void *alignedAlloc(size_t size, size_t alignment);

/**
 * 回收按对齐规则分配的内存
 *
 * @param data
 */
GX_API void alignedFree(void *data);

GX_API uint32_t calcMipLevels(uint32_t width, uint32_t height, uint32_t depth);

/**
 * 通过纹理的usage和aspect信息，推算Image layout
 *
 * @param usageFlags    图像的用途
 * @param aspectFlags   图形的样式
 * @param computeUsage  是否实在计算着色器使用
 * @return
 */
GX_API ImageLayout::Enum getImageLayoutFromUsage(TextureUsageFlags usageFlags,
                                                  TextureAspectFlags aspectFlags,
                                                  bool computeUsage);

}

#endif //GX_GFX_TOOLS_H
