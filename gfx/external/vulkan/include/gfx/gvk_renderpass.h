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

#ifndef GFX_GVK_RENDERPASS_H
#define GFX_GVK_RENDERPASS_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>

#include <vector>


namespace gfx
{

class GVkDevice;
class GVkFrameBuffer;

class GX_API GVkRenderPass
{
CLASS_DEF(GVkRenderPass)

public:
    void create(GVkDevice *device,
                const std::vector<VkAttachmentDescription> &attachDescs,      // (Render Target)资源附件
                const std::vector<VkSubpassDescription> &subpassDesc,         // subpass描述
                const std::vector<VkSubpassDependency> &subpassDepend         // subpass关联控制
    );

    void destroy();

    bool isCreated() const;

public:
    operator VkRenderPass();

    VkRenderPass vkRenderPass() const;

    void renderBegin(VkCommandBuffer commandBuffer, GVkFrameBuffer *frameBuffer, const VkRect2D &renderArea,
                     const std::vector<VkClearValue> &clearValues, VkSubpassContents subpassContents);

    void renderEnd(VkCommandBuffer commandBuffer);

private:
    GVkDevice *mDevice = nullptr;
    VkRenderPass mHandle = VK_NULL_HANDLE;
};

}

#endif //GFX_GVK_RENDERPASS_H