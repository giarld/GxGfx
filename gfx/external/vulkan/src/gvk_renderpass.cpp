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

#include "gfx/gvk_renderpass.h"

#include <gfx/gvk_tools.h>
#include <gfx/gvk_device.h>
#include <gfx/gvk_framebuffer.h>


namespace gfx
{

void GVkRenderPass::create(GVkDevice *device,
                           const std::vector<VkAttachmentDescription> &attachDescs,
                           const std::vector<VkSubpassDescription> &subpassDesc,
                           const std::vector<VkSubpassDependency> &subpassDepend)
{
    GX_ASSERT_S(!isCreated(), "GVkRenderPass is created");
    if (isCreated()) {
        return;
    }
    mDevice = device;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachDescs.size());
    renderPassInfo.pAttachments = attachDescs.data();
    renderPassInfo.subpassCount = static_cast<uint32_t>(subpassDesc.size());
    renderPassInfo.pSubpasses = subpassDesc.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(subpassDepend.size());
    renderPassInfo.pDependencies = subpassDepend.data();

    VK_CHECK_RESULT(vkCreateRenderPass(*mDevice, &renderPassInfo, nullptr, &mHandle));
}

void GVkRenderPass::destroy()
{
    if (mHandle != VK_NULL_HANDLE) {
        vkDestroyRenderPass(*mDevice, mHandle, nullptr);
    }
    mHandle = VK_NULL_HANDLE;
}

bool GVkRenderPass::isCreated() const
{
    return mHandle != VK_NULL_HANDLE;
}

GVkRenderPass::operator VkRenderPass()
{
    return mHandle;
}

VkRenderPass GVkRenderPass::vkRenderPass() const
{
    return mHandle;
}

void GVkRenderPass::renderBegin(VkCommandBuffer commandBuffer, GVkFrameBuffer *frameBuffer, const VkRect2D &renderArea,
                                const std::vector<VkClearValue> &clearValues,
                                VkSubpassContents subpassContents)
{
    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = mHandle;
    renderPassBeginInfo.renderArea = renderArea;
    renderPassBeginInfo.clearValueCount = clearValues.size();
    renderPassBeginInfo.pClearValues = clearValues.data();
    // Set target frame buffer
    renderPassBeginInfo.framebuffer = frameBuffer->vkFramebuffer();

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, subpassContents);
}

void GVkRenderPass::renderEnd(VkCommandBuffer commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer);
}

}