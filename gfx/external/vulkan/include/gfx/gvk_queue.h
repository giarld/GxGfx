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

#ifndef GFX_GVK_QUEUE_H
#define GFX_GVK_QUEUE_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>

#include <vector>


namespace gfx
{

class GVkDevice;

class GX_API GVkQueue
{
CLASS_DEF(GVkQueue)

public:
    void create(GVkDevice *device, uint32_t queueFamilyIndex, uint32_t queueIndex);

    void destroy();

public:
    bool isCreated();

    void submit(
            const std::vector<std::pair<VkSemaphore, VkPipelineStageFlags>> &waitSemaphores,
            const std::vector<VkCommandBuffer> &cmdBuffers,
            const std::vector<VkSemaphore> &signalSemaphores, VkFence fence = VK_NULL_HANDLE);

    bool waitIdle();

    operator VkQueue();

    VkDevice vkDevice();

    GVkDevice *device();

    uint32_t queueFamilyIndex();

    uint32_t queueIndex();

    VkQueue &vkQueue();

private:
    GVkDevice *mDevice = nullptr;
    uint32_t mQueueFamilyIndex = 0;
    uint32_t mQueueIndex = 0;
    VkQueue mHandle = VK_NULL_HANDLE;
};

}

#endif //GFX_GVK_QUEUE_H
