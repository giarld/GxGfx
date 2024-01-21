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

#ifndef GFX_GVK_SWAPCHAIN_H
#define GFX_GVK_SWAPCHAIN_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>
#include <gfx/gvk_semaphore.h>
#include <gfx/gvk_base_swapchain.h>

#include <vector>


namespace gfx
{

class GVkInstance;
class GVkDevice;

class GX_API GVkSurfaceSwapChain : public GVkBaseSwapChain
{
CLASS_DEF(GVkSurfaceSwapChain)

public:
    void create(GVkDevice *device, VkSurfaceKHR surface, uint32_t width, uint32_t height, bool vsync);

    void destroy() override;

    bool isCreated() const;
public:
    VkResult acquireNextImage(uint32_t *bufferIndex) override;

    VkResult queuePresent(GVkQueue *queue, uint32_t bufferIndex, VkSemaphore waitSemaphore) override;

    uint32_t bufferCount() override;

    GVkSwapChainBuffer *getSwapChainBuffer(uint32_t index) override;

    VkFormat colorFormat() override;

    VkSemaphore getImageAvailableSemaphore() override;

    uint32_t width() override;

    uint32_t height() override;

private:
    void init();

private:
    GVkDevice *mDevice = nullptr;

    bool mVsync = true;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;

    VkSurfaceKHR mSurface = VK_NULL_HANDLE;
    VkSwapchainKHR mHandle = VK_NULL_HANDLE;

    VkFormat mColorFormat = VK_FORMAT_B8G8R8_UNORM;
    VkColorSpaceKHR mColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    uint32_t mImageCount = 0;
    std::vector<GVkSwapChainBuffer> mBuffers;
    GVkSemaphore mImageAvailableSemaphore;
};

}

#endif //GFX_GVK_SWAPCHAIN_H
