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

#ifndef GFX_GVK_BUFFER_H
#define GFX_GVK_BUFFER_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>
#include <gfx/gvk_commandpool.h>

#include <cassert>


namespace gfx
{

class GVkDevice;

class GX_API GVkBuffer
{
CLASS_DEF(GVkBuffer)

public:
    void create(GVkDevice *device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags,
                VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE, VkDeviceSize size = 0, void *data = nullptr);

    void destroy();

public:
    bool isCreated() const;

    operator VkBuffer() const;

    VkBuffer vkBuffer() const;

    VkDeviceMemory vkDeviceMemory() const;

    VkDeviceSize size() const;

    VkDescriptorBufferInfo *descriptor(uint64_t offset = 0, uint64_t range = VK_WHOLE_SIZE);

    void *map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    void unmap();

    bool isMapped() const;

    void copyToBuffer(const void *data, VkDeviceSize size);

    void transfer(VkCommandBuffer cmdBuffer, GVkBuffer *src, VkDeviceSize srcOffset = 0, VkDeviceSize dstOffset = 0,
                  VkDeviceSize size = VK_WHOLE_SIZE) const;

    void transfer(GVkCommandPool *pool, GVkBuffer *src, VkDeviceSize srcOffset = 0, VkDeviceSize dstOffset = 0,
                  VkDeviceSize size = VK_WHOLE_SIZE) const;

private:
    GVkDevice *mDevice = nullptr;
    VkBuffer mHandle = VK_NULL_HANDLE;
    VkDeviceMemory mMemory = VK_NULL_HANDLE;
    VkDeviceSize mSize = 0;
    VkMemoryPropertyFlags mMemoryPropertyFlags{};
    void *mMapped = nullptr;

    VkDescriptorBufferInfo mDescriptor{};
};

}

#endif //GFX_GVK_BUFFER_H
