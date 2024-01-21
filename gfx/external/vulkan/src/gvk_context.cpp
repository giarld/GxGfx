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

#include "gfx/gvk_context.h"

#include <gfx/gvk_fence.h>

#include <gx/debug.h>

#include <array>


namespace gfx
{

bool GVkContext::create(GVkInstance *instance, const VkPhysicalDeviceFeatures &features,
                        const std::vector<const char *> &enabledDeviceExtensions,
                        uint32_t gpuIndex, VkQueueFlags queueFlags)
{
    Log("GVkContext create");

    mVkInstance = instance;
    VkPhysicalDevice physicalDevice = instance->getPhysicalDevice(gpuIndex);

    if (physicalDevice == VK_NULL_HANDLE) {
        Log("GVkContext::create get physicalDevice is null handle");
        return false;
    }

    DeviceRequirements requirements;
    requirements.queues = queueFlags;
    if (!enabledDeviceExtensions.empty()) {
        for (auto i : enabledDeviceExtensions) {
            requirements.extensions.push_back(i);
        }
    }
    requirements.features = features;
    requirements.extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

#ifdef VK_KHR_get_memory_requirements2
    requirements.extensions.push_back(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
#endif

#ifdef VK_KHR_bind_memory2
    requirements.extensions.push_back(VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);
#endif

    mVkDevice.create(mVkInstance, physicalDevice, requirements);

    if (!mVkDevice.isCreated()) {
        Log("GVkContext::create create vkDevice success");
        return false;
    }

    return true;
}

void GVkContext::destroy()
{
    if (!mVkDevice.isCreated()) {
        return;
    }
    Log("GVkContext release");

    if (mPipelineCache != VK_NULL_HANDLE) {
        vkDestroyPipelineCache(mVkDevice, mPipelineCache, nullptr);
    }

    mGraphCmdPool.destroy();
    mTransferCmdPool.destroy();
    mComputeCmdPool.destroy();

    mVkDevice.destroy();
}

bool GVkContext::isCreated()
{
    return mVkDevice.isCreated();
}

GVkInstance *GVkContext::gvkInstance() const
{
    return mVkInstance;
}

VkInstance GVkContext::vkInstance()
{
    return mVkInstance->vkInstance();
}

GVkDevice *GVkContext::gvkDevice()
{
    return &mVkDevice;
}

VkDevice &GVkContext::vkDevice()
{
    return mVkDevice.vkDevice();
}

GVkQueue * GVkContext::graphicsQueue()
{
    return &mVkDevice.getGraphicsQueue();
}

GVkQueue * GVkContext::transferQueue()
{
    return &mVkDevice.getTransferQueue();
}

GVkQueue * GVkContext::computeQueue()
{
    return &mVkDevice.getComputeQueue();
}

GVkCommandPool *GVkContext::graphicsCmdPool()
{
    if (mGraphCmdPool.vkCommandPool() == VK_NULL_HANDLE) {
        mGraphCmdPool.create(graphicsQueue(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    }
    return &mGraphCmdPool;
}

GVkCommandPool *GVkContext::transferCmdPool()
{
    if (mTransferCmdPool.vkCommandPool() == VK_NULL_HANDLE) {
        mTransferCmdPool.create(transferQueue(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    }
    return &mTransferCmdPool;
}

GVkCommandPool * GVkContext::computeCmdPool()
{
    if (mComputeCmdPool.vkCommandPool() == VK_NULL_HANDLE) {
        mComputeCmdPool.create(computeQueue(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    }
    return &mComputeCmdPool;
}

VkPipelineCache & GVkContext::pipelineCache()
{
    if (mPipelineCache == VK_NULL_HANDLE) {
        VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
        pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        VK_CHECK_RESULT(vkCreatePipelineCache(vkDevice(), &pipelineCacheCreateInfo,
                                              nullptr, &mPipelineCache));
    }
    return mPipelineCache;
}

uint32_t GVkContext::getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32 *memTypeFound)
{
    return mVkDevice.getMemoryType(typeBits, properties, memTypeFound);
}

VkCommandBuffer GVkContext::createCommandBuffer(VkCommandBufferLevel level, GVkCommandPool *pool, bool begin)
{
    VkCommandBuffer cmdBuffer = pool->allocateCommandBuffer(level);
    if (begin) {
        VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();
        VK_CHECK_RESULT(vkBeginCommandBuffer(cmdBuffer, &cmdBufInfo));
    }
    return cmdBuffer;
}

void GVkContext::flushCommandBuffer(VkCommandBuffer commandBuffer, GVkCommandPool *pool, bool free)
{
    if (commandBuffer == VK_NULL_HANDLE) {
        return;
    }

    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

    GVkQueue *queue = pool->queue();

    GVkFence fence;
    fence.create(queue->device(), VK_FLAGS_NONE);
    queue->submit({}, {commandBuffer}, {}, fence);
    fence.wait();
    fence.destroy();

    if (free) {
        pool->freeCommandBuffer(commandBuffer);
    }
}

}
