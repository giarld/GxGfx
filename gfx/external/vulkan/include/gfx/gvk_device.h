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

#ifndef GFX_GVK_DEVICE_H
#define GFX_GVK_DEVICE_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>

#include <gfx/gvk_instance.h>
#include <gfx/gvk_queue.h>

#include <utility>
#include <vector>


namespace gfx
{

struct DeviceRequirements
{
    VkQueueFlags queues = 0;
    VkPhysicalDeviceFeatures features = {};
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    std::vector<const char *> extensions;
};

bool queryDevice(GVkInstance *instance, VkPhysicalDevice device, const DeviceRequirements &requirements);

std::vector<VkPhysicalDevice> queryDevices(GVkInstance *instance,
                                           const DeviceRequirements &requirements);

class GX_API GVkDevice
{
CLASS_DEF(GVkDevice)

public:
    void create(GVkInstance *instance, const DeviceRequirements &requirements);

    void create(GVkInstance *instance, VkPhysicalDevice physicalDevice, const DeviceRequirements &requirements);

    void destroy();

public:
    bool isCreated();

    operator VkPhysicalDevice();

    operator VkDevice();

    VkPhysicalDevice &physicalDevice();

    VkDevice &vkDevice();

    const VkPhysicalDeviceProperties &deviceProperties() const;

    const VkPhysicalDeviceFeatures &deviceFeatures() const;

    const VkPhysicalDeviceMemoryProperties &deviceMemoryProperties() const;

    uint32_t getQueueCount() const;

    GVkQueue &getQueue(uint32_t index = 0);

    GVkQueue &getGraphicsQueue();

    GVkQueue &getComputeQueue();

    GVkQueue &getTransferQueue();

    GVkQueue &getSparseBindingQueue();

public:
    uint32_t getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32 *memTypeFound = nullptr);

    void waitIdle();

private:
    void createLogicalDevice(const DeviceRequirements &requirements);

    std::vector<VkDeviceQueueCreateInfo> setupQueueCreateInfos(const DeviceRequirements &requirements);

    void createQueues();

private:
    GVkInstance *mInstance = nullptr;
    VkPhysicalDevice mPhysical = VK_NULL_HANDLE;
    VkDevice mLogical = VK_NULL_HANDLE;

    VkPhysicalDeviceProperties mProperties{};
    VkPhysicalDeviceFeatures mFeatures{};
    VkPhysicalDeviceMemoryProperties mMemoryProperties{};

    struct QueueInfo
    {
        uint32_t familyIndex = 0;
        VkQueueFlags flags = VK_QUEUE_GRAPHICS_BIT;
    };
    std::vector<QueueInfo> mQueueInfos;
    std::vector<GVkQueue> mQueues;
    GVkQueue mNullQueue;
};

}

#endif //GFX_GVK_DEVICE_H
