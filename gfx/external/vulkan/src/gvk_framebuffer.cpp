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

#include "gfx/gvk_framebuffer.h"

#include <gfx/gvk_device.h>
#include <gfx/gvk_renderpass.h>
#include <gfx/gvk_tools.h>


namespace gfx
{

void GVkFrameBuffer::create(GVkDevice *device, GVkRenderPass *renderPass, const std::vector<VkImageView> &attachments,
                            uint32_t width, uint32_t height, uint32_t layers)
{
    mDevice = device;

    VkFramebufferCreateInfo framebufferCreateInfo{};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = renderPass ? renderPass->vkRenderPass() : VK_NULL_HANDLE;
    framebufferCreateInfo.attachmentCount = static_cast<uint32_t >(attachments.size());
    framebufferCreateInfo.pAttachments = attachments.data();
    framebufferCreateInfo.width = mWidth = width;
    framebufferCreateInfo.height = mHeight = height;
    framebufferCreateInfo.layers = layers;

    VK_CHECK_RESULT(vkCreateFramebuffer(*device, &framebufferCreateInfo, nullptr, &mHandle));
}

void GVkFrameBuffer::destroy()
{
    if (mHandle != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(*mDevice, mHandle, nullptr);
    }
    mHandle = VK_NULL_HANDLE;
}

GVkFrameBuffer::operator VkFramebuffer()
{
    return mHandle;
}

VkFramebuffer &GVkFrameBuffer::vkFramebuffer()
{
    return mHandle;
}

uint32_t GVkFrameBuffer::width()
{
    return mWidth;
}

uint32_t GVkFrameBuffer::height()
{
    return mHeight;
}

}