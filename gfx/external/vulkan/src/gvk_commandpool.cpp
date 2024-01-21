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

#include "gfx/gvk_commandpool.h"

#include <gfx/gvk_tools.h>
#include <gfx/gvk_device.h>


namespace gfx
{

void GVkCommandPool::create(GVkQueue *queue, VkCommandPoolCreateFlags flags)
{
    mQueue = queue;

    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = queue->queueFamilyIndex();
    cmdPoolInfo.flags = flags;
    VK_CHECK_RESULT(vkCreateCommandPool(queue->vkDevice(), &cmdPoolInfo, nullptr, &mHandle));
}

void GVkCommandPool::destroy()
{
    if (mHandle != VK_NULL_HANDLE) {
        vkDestroyCommandPool(mQueue->vkDevice(), mHandle, nullptr);
    }
    mHandle = VK_NULL_HANDLE;
}

bool GVkCommandPool::isCreated() const
{
    return mHandle != VK_NULL_HANDLE;
}

GVkCommandPool::operator VkCommandPool()
{
    return mHandle;
}

VkCommandPool GVkCommandPool::vkCommandPool()
{
    return mHandle;
}

GVkQueue *GVkCommandPool::queue()
{
    return mQueue;
}

VkCommandBuffer GVkCommandPool::allocateCommandBuffer(VkCommandBufferLevel level)
{
    VkCommandBufferAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool = mHandle;
    info.level = level;
    info.commandBufferCount = 1;

    VkCommandBuffer cmdBuffer;
    VK_CHECK_RESULT(vkAllocateCommandBuffers(mQueue->vkDevice(), &info, &cmdBuffer));
    return cmdBuffer;
}

std::vector<VkCommandBuffer> GVkCommandPool::allocateCommandBuffers(uint32_t count, VkCommandBufferLevel level)
{
    VkCommandBufferAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool = mHandle;
    info.level = level;
    info.commandBufferCount = count;

    std::vector<VkCommandBuffer> cmdBuffers(count);
    VK_CHECK_RESULT(vkAllocateCommandBuffers(mQueue->vkDevice(), &info, cmdBuffers.data()));
    return cmdBuffers;
}

void GVkCommandPool::freeCommandBuffer(VkCommandBuffer &cmdBuffer)
{
    vkFreeCommandBuffers(mQueue->vkDevice(), mHandle, 1, &cmdBuffer);
}

void GVkCommandPool::freeCommandBuffers(std::vector<VkCommandBuffer> &cmdBuffers)
{
    vkFreeCommandBuffers(mQueue->vkDevice(), mHandle, static_cast<uint32_t>(cmdBuffers.size()),
                         cmdBuffers.data());
}

}