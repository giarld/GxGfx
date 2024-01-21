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

#include "gfx/gvk_offscreen_swapchain.h"

#include <gfx/gvk_device.h>
#include <gfx/gvk_image.h>
#include <gfx/gvk_tools.h>


namespace gfx
{

void GVkOffscreenSwapChain::create(GVkDevice *device, uint32_t width, uint32_t height,
                                   VkFormat colorFormat, BufferMode bufferMode)
{
    mDevice = device;
    mWidth = width;
    mHeight = height;
    mColorFormat = colorFormat;
    mBufferMode = bufferMode;

    init();
}

void GVkOffscreenSwapChain::destroy()
{
    if (mDevice) {
        clean();
    }
    mDevice = nullptr;
}

VkResult GVkOffscreenSwapChain::acquireNextImage(uint32_t *bufferIndex)
{
    if (bufferIndex) {
        *bufferIndex = mBackBufferIndex;
    }
    return VK_SUCCESS;
}

VkResult GVkOffscreenSwapChain::queuePresent(GVkQueue *queue, uint32_t bufferIndex, VkSemaphore waitSemaphore)
{
    if (mBufferMode == BufferMode::TripleBuffer) {
        mSwapBufferIndex = mSwapBufferIndex ^ mBackBufferIndex;
        mBackBufferIndex = mSwapBufferIndex ^ mBackBufferIndex;
        mSwapBufferIndex = mSwapBufferIndex ^ mBackBufferIndex;
    }
    swapBuffer();
    return VK_SUCCESS;
}

void GVkOffscreenSwapChain::swapBuffer()
{
    if (mBufferMode == BufferMode::DoubleBuffer) {
        mFrontBufferIndex = mBackBufferIndex;
        mBackBufferIndex = (mBackBufferIndex + 1) % 2;
    } else {
        mFrontBufferIndex = mFrontBufferIndex ^ mSwapBufferIndex;
        mSwapBufferIndex = mFrontBufferIndex ^ mSwapBufferIndex;
        mFrontBufferIndex = mFrontBufferIndex ^ mSwapBufferIndex;
    }
}

uint32_t GVkOffscreenSwapChain::bufferCount()
{
    return mBufferCount;
}

GVkSwapChainBuffer *GVkOffscreenSwapChain::getSwapChainBuffer(uint32_t index)
{
    return &mBuffers[index];
}

uint32_t GVkOffscreenSwapChain::frontBufferIndex() const
{
    return mFrontBufferIndex;
}

GVkImage *GVkOffscreenSwapChain::frontImage() const
{
    return mBuffers[mFrontBufferIndex].gvkImage;
}

VkFormat GVkOffscreenSwapChain::colorFormat()
{
    return mColorFormat;
}

VkSemaphore GVkOffscreenSwapChain::getImageAvailableSemaphore()
{
    return VK_NULL_HANDLE;
}

uint32_t GVkOffscreenSwapChain::width()
{
    return mWidth;
}

uint32_t GVkOffscreenSwapChain::height()
{
    return mHeight;
}

void GVkOffscreenSwapChain::init()
{
    clean();

    if (mBufferMode == BufferMode::DoubleBuffer) {
        mBufferCount = 2;
        mFrontBufferIndex = 0;
        mBackBufferIndex = 1;
    } else {
        mBufferCount = 3;
        mFrontBufferIndex = 0;
        mSwapBufferIndex = 2;
        mBackBufferIndex = 1;
    }

    mBuffers.resize(mBufferCount);

    for (int i = 0; i < mBufferCount; i++) {
        GVkImage *color = GX_NEW(GVkImage);
        color->create(
                mDevice,
                VK_IMAGE_TYPE_2D,
                mColorFormat,
                mWidth,
                mHeight,
                1,
                VK_SAMPLE_COUNT_1_BIT,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_IMAGE_ASPECT_COLOR_BIT,
                VK_SHARING_MODE_EXCLUSIVE,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        mBuffers[i].gvkImage = color;

        VkImageViewCreateInfo colorImageView{};
        colorImageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
        colorImageView.format = mColorFormat;
        colorImageView.subresourceRange = {};
        colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        colorImageView.subresourceRange.baseMipLevel = 0;
        colorImageView.subresourceRange.levelCount = 1;
        colorImageView.subresourceRange.baseArrayLayer = 0;
        colorImageView.subresourceRange.layerCount = 1;
        colorImageView.image = *color;
        VK_CHECK_RESULT(vkCreateImageView(*mDevice, &colorImageView, nullptr, &mBuffers[i].view));
    }
}

void GVkOffscreenSwapChain::clean()
{
    for (auto &buf : mBuffers) {
        if (buf.gvkImage) {
            buf.gvkImage->destroy();
            GX_DELETE(buf.gvkImage);
        }
        vkDestroyImageView(*mDevice, buf.view, nullptr);
    }
    mBuffers.clear();
}

}