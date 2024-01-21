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

#ifndef GFX_GVK_CONTEXT_H
#define GFX_GVK_CONTEXT_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>
#include <gfx/gvk_instance.h>
#include <gfx/gvk_device.h>
#include <gfx/gvk_commandpool.h>
#include <gfx/gvk_tools.h>

#if GX_PLATFORM_ANDROID
#include <sys/system_properties.h>
#endif

namespace gfx
{

class GX_API GVkContext
{
CLASS_DEF(GVkContext)

public:
    bool create(GVkInstance *instance, const VkPhysicalDeviceFeatures &features,
                const std::vector<const char *> &enabledDeviceExtensions, uint32_t gpuIndex,
                VkQueueFlags queueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT);

    void destroy();

    bool isCreated();

public:
    GVkInstance *gvkInstance() const;

    VkInstance vkInstance();

    GVkDevice *gvkDevice();

    VkDevice &vkDevice();

    GVkQueue *graphicsQueue();

    GVkQueue *transferQueue();

    GVkQueue *computeQueue();

    GVkCommandPool *graphicsCmdPool();

    GVkCommandPool *transferCmdPool();

    GVkCommandPool *computeCmdPool();

    VkPipelineCache &pipelineCache();

public:
    uint32_t getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32 *memTypeFound = nullptr);

    static VkCommandBuffer createCommandBuffer(VkCommandBufferLevel level, GVkCommandPool *pool, bool begin);

    static void flushCommandBuffer(VkCommandBuffer commandBuffer, GVkCommandPool *pool, bool free);

private:
    GVkInstance *mVkInstance = nullptr;

    GVkDevice mVkDevice;

    VkPipelineCache mPipelineCache = VK_NULL_HANDLE;

    // Command buffer pool
    GVkCommandPool mGraphCmdPool;
    GVkCommandPool mTransferCmdPool;
    GVkCommandPool mComputeCmdPool;
};

}

#undef CALL_INIT_FUNC

#endif //GFX_GVK_CONTEXT_H
