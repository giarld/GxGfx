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

#include "gfx/gvk_device.h"

#include <gfx/gvk_tools.h>
#include <gfx/gvk_queue.h>

#include <gx/debug.h>

#include <cstring>


namespace gfx
{

static bool deviceFeatureIncludes(const VkPhysicalDeviceFeatures *a,
                                  const VkPhysicalDeviceFeatures *b)
{
    const VkBool32 *pa = reinterpret_cast<const VkBool32 *>(a);
    const VkBool32 *pb = reinterpret_cast<const VkBool32 *>(b);
    unsigned count = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);
    for (unsigned i = 0; i < count; i++)
        if (pa[i] && !pb[i]) return false;
    return true;
}

bool queryDevice(GVkInstance *instance, VkPhysicalDevice device, const DeviceRequirements &requirements)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    uint32_t n;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &n, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(n);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &n, queueFamilyProperties.data());

    VkQueueFlags foundQueues = 0;
    uint32_t i = 0;
    for (; i < n && (foundQueues & requirements.queues) != requirements.queues; i++) {
        VkQueueFlags flags = queueFamilyProperties[i].queueFlags;

        if (requirements.surface != VK_NULL_HANDLE && (flags & VK_QUEUE_GRAPHICS_BIT)) {
            VkBool32 surfaceSupported;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, requirements.surface,
                                                 &surfaceSupported);
            if (!surfaceSupported) flags &= ~VK_QUEUE_GRAPHICS_BIT;
        }

        foundQueues |= flags;
    }

    if ((foundQueues & requirements.queues) != requirements.queues) return false;

    VkPhysicalDeviceFeatures foundFeatures;
    vkGetPhysicalDeviceFeatures(device, &foundFeatures);
    if (!deviceFeatureIncludes(&requirements.features, &foundFeatures)) return false;

    if (!requirements.extensions.empty()) {
        vkEnumerateDeviceExtensionProperties(device, nullptr, &n, nullptr);
        std::vector<VkExtensionProperties> extensionProperties(n);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &n,
                                             extensionProperties.data());

        bool missing = false;
        for (i = 0; i < requirements.extensions.size() && !missing; i++) {
            uint32_t j = 0;
            for (; j < n && strcmp(requirements.extensions[i],
                                   extensionProperties[j].extensionName); j++);
            missing = j == n;
        }
        if (missing) return false;
    }

    return true;
}

std::vector<VkPhysicalDevice> queryDevices(GVkInstance *instance,
                                           const DeviceRequirements &requirements)
{
    std::vector<VkPhysicalDevice> devices = instance->physicalDevices();
    std::vector<VkPhysicalDevice> results;
    for (VkPhysicalDevice device : devices) {
        if (queryDevice(instance, device, requirements))
            results.push_back(device);
    }
    return results;
}


void GVkDevice::create(GVkInstance *instance, const DeviceRequirements &requirements)
{
    auto result = queryDevices(instance, requirements);
    GX_ASSERT_S(!result.empty(), "queryDevices failure");
    create(instance, result[0], requirements);
}

void GVkDevice::create(GVkInstance *instance, VkPhysicalDevice physicalDevice, const DeviceRequirements &requirements)
{
    GX_ASSERT(physicalDevice);
    mInstance = instance;
    mPhysical = physicalDevice;

    vkGetPhysicalDeviceProperties(mPhysical, &mProperties);
    vkGetPhysicalDeviceFeatures(mPhysical, &mFeatures);
    vkGetPhysicalDeviceMemoryProperties(mPhysical, &mMemoryProperties);

    createLogicalDevice(requirements);
    createQueues();
}

void GVkDevice::destroy()
{
    mQueueInfos.clear();
    mQueues.clear();

    if (mLogical != VK_NULL_HANDLE) {
        vkDestroyDevice(mLogical, nullptr);
    }

    mLogical = VK_NULL_HANDLE;
    mPhysical = VK_NULL_HANDLE;
}

bool GVkDevice::isCreated()
{
    return mLogical != VK_NULL_HANDLE;
}

GVkDevice::operator VkPhysicalDevice()
{
    return mPhysical;
}

GVkDevice::operator VkDevice()
{
    return mLogical;
}

VkPhysicalDevice &GVkDevice::physicalDevice()
{
    return mPhysical;
}

VkDevice &GVkDevice::vkDevice()
{
    return mLogical;
}

const VkPhysicalDeviceProperties &GVkDevice::deviceProperties() const
{
    return mProperties;
}

const VkPhysicalDeviceFeatures &GVkDevice::deviceFeatures() const
{
    return mFeatures;
}

const VkPhysicalDeviceMemoryProperties &GVkDevice::deviceMemoryProperties() const
{
    return mMemoryProperties;
}


uint32_t GVkDevice::getQueueCount() const
{
    return mQueues.size();
}

GVkQueue &GVkDevice::getQueue(uint32_t index)
{
    if (index >= mQueues.size()) {
        return mNullQueue;
    }
    return mQueues[index];
}

GVkQueue &GVkDevice::getGraphicsQueue()
{
    for (auto i = 0; i < getQueueCount(); i++) {
        if (mQueueInfos[i].flags & VK_QUEUE_GRAPHICS_BIT)
            return mQueues[i];
    }
    GX_ASSERT_S(false, "Can not find graphics queue");
    return mNullQueue;
}

GVkQueue &GVkDevice::getComputeQueue()
{
    for (auto i = 0; i < getQueueCount(); i++) {
        if (mQueueInfos[i].flags & VK_QUEUE_COMPUTE_BIT)
            return mQueues[i];
    }
    // 如果找不到则返回graphics queue，因为它是全能的
    return getGraphicsQueue();
}

GVkQueue &GVkDevice::getTransferQueue()
{
    for (auto i = 0; i < getQueueCount(); i++) {
        if (mQueueInfos[i].flags & VK_QUEUE_TRANSFER_BIT)
            return mQueues[i];
    }
    // 如果找不到则返回graphics queue，因为它是全能的
    return getGraphicsQueue();
}

GVkQueue &GVkDevice::getSparseBindingQueue()
{
    for (auto i = 0; i < getQueueCount(); i++) {
        if (mQueueInfos[i].flags & VK_QUEUE_SPARSE_BINDING_BIT)
            return mQueues[i];
    }
    // 如果找不到则返回graphics queue，因为它是全能的
    return getGraphicsQueue();
}

uint32_t GVkDevice::getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32 *memTypeFound)
{
    for (uint32_t i = 0; i < mMemoryProperties.memoryTypeCount; i++) {
        if ((typeBits & 1) == 1) {
            if ((mMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                if (memTypeFound) {
                    *memTypeFound = true;
                }
                return i;
            }
        }
        typeBits >>= 1;
    }

    if (memTypeFound) {
        *memTypeFound = false;
    }
    return 0;
}

void GVkDevice::waitIdle()
{
    VK_CHECK_RESULT(vkDeviceWaitIdle(mLogical));
}

void GVkDevice::createLogicalDevice(const DeviceRequirements &requirements)
{
    auto queueCreateInfos = setupQueueCreateInfos(requirements);

    VkDeviceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    info.pQueueCreateInfos = queueCreateInfos.data();

    std::vector<const char *> enabledExtensions;
    for (auto ext : requirements.extensions)
        enabledExtensions.push_back(ext);

    info.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
    info.ppEnabledExtensionNames = enabledExtensions.data();
    info.pEnabledFeatures = &requirements.features;

    VK_CHECK_RESULT(vkCreateDevice(mPhysical, &info, nullptr, &mLogical));
}

std::vector<VkDeviceQueueCreateInfo> GVkDevice::setupQueueCreateInfos(const DeviceRequirements &requirements)
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    static const float priority = 1.f;

    do {
        if (requirements.queues & VK_QUEUE_GRAPHICS_BIT) {
            int32_t qfi = -1;
            if (requirements.surface != VK_NULL_HANDLE)
                qfi = vks::tools::findSurfaceQueueFamilyIndex(mPhysical, requirements.surface);
            else
                qfi = vks::tools::findQueueFamilyIndex(mPhysical, VK_QUEUE_GRAPHICS_BIT);
            if (qfi == -1)
                break;
            queueCreateInfos.push_back({VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0,
                                        static_cast<uint32_t>(qfi), 1, &priority});
            mQueueInfos.push_back({static_cast<uint32_t>(qfi), VK_QUEUE_GRAPHICS_BIT});
        }
    } while (false);

    do {
        if (requirements.queues & VK_QUEUE_COMPUTE_BIT) {
            int32_t qfi = vks::tools::findQueueFamilyIndex(mPhysical, VK_QUEUE_COMPUTE_BIT);
            if (qfi == -1)
                break;

            bool found = false;
            for (auto &q : mQueueInfos) {
                if (q.familyIndex == qfi) {
                    q.flags |= VK_QUEUE_COMPUTE_BIT;
                    found = true;
                    break;
                }
            }

            if (!found) {
                queueCreateInfos.push_back(
                        {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0,
                         static_cast<uint32_t>(qfi), 1, &priority});
                mQueueInfos.push_back({static_cast<uint32_t>(qfi), VK_QUEUE_COMPUTE_BIT});
            }
        }
    } while (false);

    do {
        if (requirements.queues & VK_QUEUE_TRANSFER_BIT) {
            int32_t qfi = vks::tools::findQueueFamilyIndex(mPhysical, VK_QUEUE_TRANSFER_BIT);
            if (qfi == -1)
                break;

            bool found = false;
            for (auto &q : mQueueInfos) {
                if (q.familyIndex == qfi) {
                    q.flags |= VK_QUEUE_TRANSFER_BIT;
                    found = true;
                    break;
                }
            }

            if (!found) {
                queueCreateInfos.push_back(
                        {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0,
                         static_cast<uint32_t>(qfi), 1, &priority});
                mQueueInfos.push_back({static_cast<uint32_t>(qfi), VK_QUEUE_TRANSFER_BIT});
            }
        }
    } while (false);

    do {
        if (requirements.queues & VK_QUEUE_SPARSE_BINDING_BIT) {
            int32_t qfi = vks::tools::findQueueFamilyIndex(mPhysical, VK_QUEUE_SPARSE_BINDING_BIT);
            if (qfi == -1)
                break;

            bool found = false;
            for (auto &q : mQueueInfos) {
                if (q.familyIndex) {
                    q.flags |= VK_QUEUE_SPARSE_BINDING_BIT;
                    found = true;
                    break;
                }
            }

            if (!found) {
                queueCreateInfos.push_back(
                        {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0,
                         static_cast<uint32_t>(qfi), 1, &priority});
                mQueueInfos.push_back(
                        {static_cast<uint32_t>(qfi), VK_QUEUE_SPARSE_BINDING_BIT});
            }
        }
    } while (false);

    return queueCreateInfos;
}

void GVkDevice::createQueues()
{
    for (auto &q : mQueueInfos) {
        GVkQueue queue;
        queue.create(this, q.familyIndex, 0);
        mQueues.push_back(queue);
    }
}

}
