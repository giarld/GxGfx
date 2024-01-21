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

#ifndef GFX_GVK_OFFSCREEN_SWAPCHAIN_H
#define GFX_GVK_OFFSCREEN_SWAPCHAIN_H

#include <gfx/compiler.h>
#include <gfx/gvk_base_swapchain.h>

#include <vector>


namespace gfx
{

class GVkDevice;
class GVkImage;

/**
 * 离屏SwapChain
 * BufferMode, 当跟随屏幕同步控制时使用DoubleBuffer足矣，当脱离屏幕垂直同步时推荐使用TripleBuffer，可以保证更低的输入延迟同时不撕裂
 * 注意：需要主动调用SwapBuffer来完成缓冲区交换
 */
class GX_API GVkOffscreenSwapChain : public GVkBaseSwapChain
{
CLASS_DEF(GVkOffscreenSwapChain)

public:
    enum BufferMode
    {
        DoubleBuffer = 0,
        TripleBuffer = 1
    };

public:
    void create(GVkDevice *device, uint32_t width, uint32_t height, VkFormat colorFormat,
                BufferMode bufferMode = DoubleBuffer);

    void destroy() override;

    VkResult acquireNextImage(uint32_t *bufferIndex) override;

    VkResult queuePresent(GVkQueue *queue, uint32_t bufferIndex, VkSemaphore waitSemaphore) override;

    uint32_t bufferCount() override;

    GVkSwapChainBuffer *getSwapChainBuffer(uint32_t index) override;

    uint32_t frontBufferIndex() const;

    GVkImage *frontImage() const;

    VkFormat colorFormat() override;

    VkSemaphore getImageAvailableSemaphore() override;

    uint32_t width() override;

    uint32_t height() override;

private:
    void init();

    void clean();

    /**
     * 切换缓冲区
     */
    void swapBuffer();

private:
    GVkDevice *mDevice = nullptr;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    BufferMode mBufferMode = BufferMode::DoubleBuffer;
    VkFormat mColorFormat = VK_FORMAT_R8G8B8A8_UNORM;
    std::vector<GVkSwapChainBuffer> mBuffers;
    uint32_t mBufferCount = 0;
    uint32_t mFrontBufferIndex = 0;      // 前缓冲区，可被读取的缓冲区
    uint32_t mBackBufferIndex = 0;       // 后缓冲区，可被写入的缓冲区
    uint32_t mSwapBufferIndex = 0;       // 交换缓冲区，仅三缓冲时有用
};

}

#endif //GFX_GVK_OFFSCREEN_SWAPCHAIN_H
