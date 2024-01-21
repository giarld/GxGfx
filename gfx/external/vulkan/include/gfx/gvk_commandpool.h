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

#ifndef GFX_GVK_COMMAND_POOL_H
#define GFX_GVK_COMMAND_POOL_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>
#include <gfx/gvk_queue.h>

namespace gfx
{

class GX_API GVkCommandPool
{
CLASS_DEF(GVkCommandPool)

public:
    void create(GVkQueue *queue, VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    void destroy();

    bool isCreated() const;

public:
    operator VkCommandPool();

    VkCommandPool vkCommandPool();

    GVkQueue *queue();

    VkCommandBuffer allocateCommandBuffer(
            VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    std::vector<VkCommandBuffer> allocateCommandBuffers(
            uint32_t count, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    void freeCommandBuffer(VkCommandBuffer &cmdBuffer);

    void freeCommandBuffers(std::vector<VkCommandBuffer>& cmdBuffers);

private:
    GVkQueue *mQueue = nullptr;
    VkCommandPool mHandle = VK_NULL_HANDLE;
};

}

#endif //GFX_GVK_COMMAND_POOL_H
