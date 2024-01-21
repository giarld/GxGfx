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
#include "gfx/gvk_buffer.h"
#include <gfx/gvk_tools.h>
#include <gfx/gvk_device.h>
#include <gfx/gvk_fence.h>

#include <gx/debug.h>

#include <memory.h>
#include <math.h>


namespace gfx
{

void GVkBuffer::create(GVkDevice *device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags,
                       VkSharingMode sharingMode, VkDeviceSize size, void *data)
{
    GX_ASSERT_S(!isCreated(), "GVkBuffer::create is created");
    if (isCreated()) {
        return;
    }
    mDevice = device;

    VkBufferCreateInfo bufCreateInfo{};
    bufCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufCreateInfo.usage = usageFlags;
    bufCreateInfo.size = size;
    bufCreateInfo.sharingMode = sharingMode;
    VK_CHECK_RESULT(vkCreateBuffer(*device, &bufCreateInfo, nullptr, &mHandle));

    VkMemoryRequirements memReqs;
    VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
    vkGetBufferMemoryRequirements(*device, mHandle, &memReqs);
    memAlloc.allocationSize = memReqs.size;

    memAlloc.memoryTypeIndex = device->getMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags);
    VK_CHECK_RESULT(vkAllocateMemory(*device, &memAlloc, nullptr, &mMemory));

    mMemoryPropertyFlags = memoryPropertyFlags;
    mSize = size;

    if (data != nullptr) {
        copyToBuffer(data, size);
    }

    VK_CHECK_RESULT(vkBindBufferMemory(*mDevice, mHandle, mMemory, 0));
}

void GVkBuffer::destroy()
{
    if (mHandle != VK_NULL_HANDLE) {
        unmap();
        if (mMemory != VK_NULL_HANDLE) {
            vkFreeMemory(*mDevice, mMemory, nullptr);
        }
        vkDestroyBuffer(*mDevice, mHandle, nullptr);
    }
    mHandle = VK_NULL_HANDLE;
    mMemory = VK_NULL_HANDLE;
}

bool GVkBuffer::isCreated() const
{
    return mHandle != VK_NULL_HANDLE;
}

GVkBuffer::operator VkBuffer() const
{
    return mHandle;
}

VkBuffer GVkBuffer::vkBuffer() const
{
    return mHandle;
}

VkDeviceMemory GVkBuffer::vkDeviceMemory() const
{
    return mMemory;
}

VkDeviceSize GVkBuffer::size() const
{
    return mSize;
}

VkDescriptorBufferInfo *GVkBuffer::descriptor(uint64_t offset, uint64_t range)
{
    mDescriptor.buffer = mHandle;
    mDescriptor.offset = offset;
    mDescriptor.range = range;

    return &mDescriptor;
}

void *GVkBuffer::map(VkDeviceSize size, VkDeviceSize offset)
{
    if (mMapped) {
        return mMapped;
    }
    VK_CHECK_RESULT(vkMapMemory(*mDevice, mMemory, offset, size, 0, &mMapped));
    return mMapped;
}

VkResult GVkBuffer::flush(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = mMemory;
    mappedRange.offset = offset;
    mappedRange.size = size;
    return vkFlushMappedMemoryRanges(*mDevice, 1, &mappedRange);
}

void GVkBuffer::unmap()
{
    if (mMapped) {
        vkUnmapMemory(*mDevice, mMemory);
        mMapped = nullptr;
    }
}

bool GVkBuffer::isMapped() const
{
    return mMapped;
}

void GVkBuffer::copyToBuffer(const void *data, VkDeviceSize size)
{
    if (!data) {
        return;
    }
    bool mapped = isMapped();
    if (!mapped) {
        map();
    }
    memcpy(mMapped, data, size);
    if ((mMemoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
        VK_CHECK_RESULT(flush());
    if (!mapped) {
        unmap();
    }
}

void GVkBuffer::transfer(VkCommandBuffer cmdBuffer,
                         GVkBuffer *src,
                         VkDeviceSize srcOffset,
                         VkDeviceSize dstOffset,
                         VkDeviceSize size) const
{
    if (cmdBuffer == VK_NULL_HANDLE) {
        return;
    }
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = srcOffset;
    copyRegion.dstOffset = dstOffset;

    if (size == VK_WHOLE_SIZE)
        copyRegion.size = std::min(this->size() - dstOffset, src->size() - srcOffset);
    else
        copyRegion.size = size;

    vkCmdCopyBuffer(cmdBuffer, *src, mHandle, 1, &copyRegion);
}

void GVkBuffer::transfer(GVkCommandPool *pool,
                         GVkBuffer *src,
                         VkDeviceSize srcOffset,
                         VkDeviceSize dstOffset,
                         VkDeviceSize size) const
{
    VkCommandBuffer cmdBuffer = pool->allocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();
    VK_CHECK_RESULT(vkBeginCommandBuffer(cmdBuffer, &cmdBufInfo));

    transfer(cmdBuffer, src, srcOffset, dstOffset, size);

    VK_CHECK_RESULT(vkEndCommandBuffer(cmdBuffer));

    GVkQueue *queue = pool->queue();
    GVkFence fence;
    fence.create(queue->device(), VK_FLAGS_NONE);
    queue->submit({}, {cmdBuffer}, {}, fence);
    fence.wait();
    fence.destroy();

    pool->freeCommandBuffer(cmdBuffer);
}

}
