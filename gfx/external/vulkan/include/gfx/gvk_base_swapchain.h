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

#ifndef GFX_GVK_BASE_SWAPCHAIN_H
#define GFX_GVK_BASE_SWAPCHAIN_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>

namespace gfx
{

class GVkImage;

class GVkQueue;

struct GX_API GVkSwapChainBuffer
{
    GVkImage *gvkImage = nullptr;
    VkImageView view = VK_NULL_HANDLE;
};

class GX_API GVkBaseSwapChain
{
CLASS_DEF(GVkBaseSwapChain)

public:
    virtual void destroy() = 0;

    virtual VkResult acquireNextImage(uint32_t *bufferIndex) = 0;

    virtual VkResult queuePresent(GVkQueue *queue, uint32_t bufferIndex, VkSemaphore waitSemaphore) = 0;

    virtual uint32_t bufferCount() = 0;

    virtual GVkSwapChainBuffer *getSwapChainBuffer(uint32_t index) = 0;

    virtual VkFormat colorFormat() = 0;

    virtual VkSemaphore getImageAvailableSemaphore() = 0;

    virtual uint32_t width() = 0;

    virtual uint32_t height() = 0;
};

}

#endif //GFX_GVK_BASE_SWAPCHAIN_H
