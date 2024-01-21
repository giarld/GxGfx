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

#ifndef GFX_GVK_FRAMEBUFFER_H
#define GFX_GVK_FRAMEBUFFER_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>

#include <vector>


namespace gfx
{

class GVkDevice;

class GVkRenderPass;

class GX_API GVkFrameBuffer
{
CLASS_DEF(GVkFrameBuffer)

public:
    void create(GVkDevice *device, GVkRenderPass *renderPass, const std::vector<VkImageView> &attachments,
            uint32_t width, uint32_t height, uint32_t layers = 1);

    void destroy();

public:
    operator VkFramebuffer();

    VkFramebuffer &vkFramebuffer();

    uint32_t width();

    uint32_t height();

private:
    GVkDevice *mDevice = nullptr;
    VkFramebuffer mHandle = VK_NULL_HANDLE;

    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
};

}

#endif //GFX_GVK_FRAMEBUFFER_H
