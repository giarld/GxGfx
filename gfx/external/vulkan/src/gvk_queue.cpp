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

#include "gfx/gvk_queue.h"

#include <gfx/gvk_device.h>


namespace gfx
{

void GVkQueue::create(GVkDevice *device, uint32_t queueFamilyIndex, uint32_t queueIndex)
{
    mDevice = device;
    mQueueFamilyIndex = queueFamilyIndex;
    mQueueIndex = queueIndex;
    vkGetDeviceQueue(*device, queueFamilyIndex, queueIndex, &mHandle);
}

void GVkQueue::destroy()
{
    mHandle = VK_NULL_HANDLE;
    mDevice = nullptr;
}

void GVkQueue::submit(const std::vector<std::pair<VkSemaphore, VkPipelineStageFlags> > &waitSemaphores,
                      const std::vector<VkCommandBuffer> &cmdBuffers,
                      const std::vector<VkSemaphore> &signalSemaphores,
                      VkFence fence)
{
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());

    std::vector<VkSemaphore> waitSems;
    waitSems.reserve(waitSemaphores.size());
    std::vector<VkPipelineStageFlags> waitStages;
    waitStages.reserve(waitSemaphores.size());
    for (auto &w : waitSemaphores) {
        waitSems.push_back(w.first);
        waitStages.push_back(w.second);
    }

    submitInfo.pWaitSemaphores = waitSems.data();
    submitInfo.pWaitDstStageMask = waitStages.data();
    submitInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());
    submitInfo.pCommandBuffers = cmdBuffers.data();
    submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
    submitInfo.pSignalSemaphores = signalSemaphores.data();

    vkQueueSubmit(mHandle, 1, &submitInfo, fence);
}

bool GVkQueue::waitIdle()
{
    return vkQueueWaitIdle(mHandle) == VK_SUCCESS;
}

GVkQueue::operator VkQueue()
{
    return mHandle;
}

VkDevice GVkQueue::vkDevice()
{
    return *mDevice;
}

GVkDevice *GVkQueue::device()
{
    return mDevice;
}

uint32_t GVkQueue::queueFamilyIndex()
{
    return mQueueFamilyIndex;
}

uint32_t GVkQueue::queueIndex()
{
    return mQueueIndex;
}

VkQueue &GVkQueue::vkQueue()
{
    return mHandle;
}

bool GVkQueue::isCreated()
{
    return mHandle != VK_NULL_HANDLE;
}

}


