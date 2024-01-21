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

#ifdef USE_GP_API_VULKAN

#include "gfx_p_vk.h"

#include "gfx_context.h"
#include "gfx_element.h"
#include "gfx_private.h"

#include <gfx/gvk_surface_swapchain.h>

#include <gfx/gfx_tools.h>
#include <gfx/gvk_debug.h>

#include <cstring>
#include <string>
#include <sstream>
#include <math.h>

#endif //USE_GP_API_VULKAN


#ifdef Always
#undef Always
#endif

#ifdef None
#undef None
#endif

#ifdef Success
#undef Success
#endif


namespace gfx
{
namespace vk
{

Instance_P *createInstanceP()
{
#ifdef USE_GP_API_VULKAN
    return GX_NEW(InstanceVk);
#else
    return GFX_NULL_HANDLE;
#endif
}

void destroyInstanceP(Instance_P *obj)
{
#ifdef USE_GP_API_VULKAN
    auto *objT = dynamic_cast<InstanceVk *>(obj);
    objT->destroy();
    GX_DELETE(objT);
#endif
}

#ifdef USE_GP_API_VULKAN

static inline GVkContext *getGVkContext(Context_T *contextT)
{
    GX_ASSERT(contextT);
    auto *context = dynamic_cast<ContextVk *>(contextT->contextP());
    GX_ASSERT(context);
    return context->vkContext();
}

/// ============ InstanceVk ============ ///
bool InstanceVk::init(const CreateInstanceInfo &createInfo)
{
    if (!mVkInstance.create(createInfo.appName, USE_VK_API_VER,
                            transInstanceExt(createInfo.exts), createInfo.enableValidation)) {
        LogE("Create Vulkan Instance failure!");
        return false;
    }

    return true;
}

void InstanceVk::destroy()
{
    mVkInstance.destroy();
}

uint32_t InstanceVk::deviceCount()
{
    return mVkInstance.physicalDeviceCount();
}

DeviceInfo InstanceVk::deviceInfo(uint32_t deviceIndex)
{
    return tranVkDeviceInfo(deviceIndex);
}

std::vector<const char *> InstanceVk::transInstanceExt(const std::vector<GfxEXT> &exts)
{
    std::vector<const char *> vkInstanceExts;

    return vkInstanceExts;
}

bool InstanceVk::isCreated()
{
    return mVkInstance.vkInstance() != VK_NULL_HANDLE;
}

Context_P *InstanceVk::createContextP()
{
    uint16_t idx = mCtxIDAllocator.alloc();
    GX_ASSERT(mCtxIDAllocator.isValid(idx));
    return GX_NEW(ContextVk, idx);
}

void InstanceVk::destroyContextP(Context_P *obj)
{
    auto *objT = dynamic_cast<ContextVk *>(obj);
    mCtxIDAllocator.free(objT->idx());
    objT->destroy();
    GX_DELETE(objT);
}

GVkInstance *InstanceVk::vkInstance()
{
    return &mVkInstance;
}

DeviceInfo InstanceVk::tranVkDeviceInfo(uint32_t index)
{
    VkPhysicalDevice device = mVkInstance.getPhysicalDevice(index);
    GX_ASSERT_S(device != VK_NULL_HANDLE, "Not find device");

    VkPhysicalDeviceProperties deviceProperties{};
    VkPhysicalDeviceFeatures deviceFeatures{};

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    DeviceInfo deviceInfo = {};
    deviceInfo.deviceIndex = index;
    deviceInfo.deviceID = deviceProperties.deviceID;
    deviceInfo.apiVersion = deviceProperties.apiVersion;
    deviceInfo.driverVersion = deviceProperties.driverVersion;
    deviceInfo.deviceType =
            deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? gfx::DeviceType::IntegratedGPU :
            deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? gfx::DeviceType::DiscreteGPU :
            deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ? gfx::DeviceType::VirtualGPU :
            deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU ? gfx::DeviceType::CPU : gfx::DeviceType::Other;
    strcpy(deviceInfo.deviceName, deviceProperties.deviceName);

    // 担心数据结构会变，不直接内存拷贝
    deviceInfo.features.robustBufferAccess = deviceFeatures.robustBufferAccess;
    deviceInfo.features.fullDrawIndexUint32 = deviceFeatures.fullDrawIndexUint32;
    deviceInfo.features.imageCubeArray = deviceFeatures.imageCubeArray;
    deviceInfo.features.independentBlend = deviceFeatures.independentBlend;
    deviceInfo.features.geometryShader = deviceFeatures.geometryShader;
    deviceInfo.features.tessellationShader = deviceFeatures.tessellationShader;
    deviceInfo.features.sampleRateShading = deviceFeatures.sampleRateShading;
    deviceInfo.features.dualSrcBlend = deviceFeatures.dualSrcBlend;
    deviceInfo.features.logicOp = deviceFeatures.logicOp;
    deviceInfo.features.multiDrawIndirect = deviceFeatures.multiDrawIndirect;
    deviceInfo.features.drawIndirectFirstInstance = deviceFeatures.drawIndirectFirstInstance;
    deviceInfo.features.depthClamp = deviceFeatures.depthClamp;
    deviceInfo.features.depthBiasClamp = deviceFeatures.depthBiasClamp;
    deviceInfo.features.fillModeNonSolid = deviceFeatures.fillModeNonSolid;
    deviceInfo.features.depthBounds = deviceFeatures.depthBounds;
    deviceInfo.features.wideLines = deviceFeatures.wideLines;
    deviceInfo.features.largePoints = deviceFeatures.largePoints;
    deviceInfo.features.alphaToOne = deviceFeatures.alphaToOne;
    deviceInfo.features.multiViewport = deviceFeatures.multiViewport;
    deviceInfo.features.samplerAnisotropy = deviceFeatures.samplerAnisotropy;
    deviceInfo.features.textureCompressionETC2 = deviceFeatures.textureCompressionETC2;
    deviceInfo.features.textureCompressionASTC_LDR = deviceFeatures.textureCompressionASTC_LDR;
    deviceInfo.features.textureCompressionBC = deviceFeatures.textureCompressionBC;
    deviceInfo.features.occlusionQueryPrecise = deviceFeatures.occlusionQueryPrecise;
    deviceInfo.features.pipelineStatisticsQuery = deviceFeatures.pipelineStatisticsQuery;
    deviceInfo.features.vertexPipelineStoresAndAtomics = deviceFeatures.vertexPipelineStoresAndAtomics;
    deviceInfo.features.fragmentStoresAndAtomics = deviceFeatures.fragmentStoresAndAtomics;
    deviceInfo.features.shaderTessellationAndGeometryPointSize = deviceFeatures.shaderTessellationAndGeometryPointSize;
    deviceInfo.features.shaderImageGatherExtended = deviceFeatures.shaderImageGatherExtended;
    deviceInfo.features.shaderStorageImageExtendedFormats = deviceFeatures.shaderStorageImageExtendedFormats;
    deviceInfo.features.shaderStorageImageMultisample = deviceFeatures.shaderStorageImageMultisample;
    deviceInfo.features.shaderStorageImageReadWithoutFormat = deviceFeatures.shaderStorageImageReadWithoutFormat;
    deviceInfo.features.shaderStorageImageWriteWithoutFormat = deviceFeatures.shaderStorageImageWriteWithoutFormat;
    deviceInfo.features.shaderUniformBufferArrayDynamicIndexing = deviceFeatures.shaderUniformBufferArrayDynamicIndexing;
    deviceInfo.features.shaderSampledImageArrayDynamicIndexing = deviceFeatures.shaderSampledImageArrayDynamicIndexing;
    deviceInfo.features.shaderStorageBufferArrayDynamicIndexing = deviceFeatures.shaderStorageBufferArrayDynamicIndexing;
    deviceInfo.features.shaderStorageImageArrayDynamicIndexing = deviceFeatures.shaderStorageImageArrayDynamicIndexing;
    deviceInfo.features.shaderClipDistance = deviceFeatures.shaderClipDistance;
    deviceInfo.features.shaderCullDistance = deviceFeatures.shaderCullDistance;
    deviceInfo.features.shaderFloat64 = deviceFeatures.shaderFloat64;
    deviceInfo.features.shaderInt64 = deviceFeatures.shaderInt64;
    deviceInfo.features.shaderInt16 = deviceFeatures.shaderInt16;
    deviceInfo.features.shaderResourceResidency = deviceFeatures.shaderResourceResidency;
    deviceInfo.features.shaderResourceMinLod = deviceFeatures.shaderResourceMinLod;
    deviceInfo.features.sparseBinding = deviceFeatures.sparseBinding;
    deviceInfo.features.sparseResidencyBuffer = deviceFeatures.sparseResidencyBuffer;
    deviceInfo.features.sparseResidencyImage2D = deviceFeatures.sparseResidencyImage2D;
    deviceInfo.features.sparseResidencyImage3D = deviceFeatures.sparseResidencyImage3D;
    deviceInfo.features.sparseResidency2Samples = deviceFeatures.sparseResidency2Samples;
    deviceInfo.features.sparseResidency4Samples = deviceFeatures.sparseResidency4Samples;
    deviceInfo.features.sparseResidency8Samples = deviceFeatures.sparseResidency8Samples;
    deviceInfo.features.sparseResidency16Samples = deviceFeatures.sparseResidency16Samples;
    deviceInfo.features.sparseResidencyAliased = deviceFeatures.sparseResidencyAliased;
    deviceInfo.features.variableMultisampleRate = deviceFeatures.variableMultisampleRate;
    deviceInfo.features.inheritedQueries = deviceFeatures.inheritedQueries;

    return deviceInfo;
}

/// ============ ContextVk ============ ///
bool ContextVk::init(Instance_P *instance, Context_T *context, const CreateContextInfo &createInfo)
{
    auto *instanceVk = dynamic_cast<InstanceVk *>(instance);
    GX_ASSERT(instanceVk);
    GX_ASSERT(context);
    GX_ASSERT_S(!mVkContext.isCreated(), "Vulkan context is created");

    mParentCtx = context;
    mEnableValidation = instanceVk->vkInstance()->isEnableValidation();

    VkQueueFlags vkQueueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT;

    if (!mVkContext.create(instanceVk->vkInstance(),
                           getVkDeviceFeatures(createInfo.deviceIndex, instanceVk),
                           transDeviceExt(createInfo.exts),
                           createInfo.deviceIndex, vkQueueFlags)) {
        Log("Create vulkan device failure!");
        return false;
    }

    auto properties = mVkContext.gvkDevice()->deviceProperties();
    mSupportQueryTimestamp = (bool)properties.limits.timestampComputeAndGraphics;
    mTimestampPeriod = properties.limits.timestampPeriod;

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    initVma();
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR

    return true;
}

void ContextVk::destroy()
{
    // clear DescriptorPools
    for (auto iPool : mVkDescriptorPools) {
        if (iPool) {
            vkDestroyDescriptorPool(mVkContext.vkDevice(), iPool, nullptr);
        }
    }
    mVkDescriptorPools.clear();

    // clear RenderPasss
    mRwRPMapMutex.lock();
    for (auto &[k, v] : mRenderPassMap) {
        auto *obj = findRenderPass(v);
        if (obj != GFX_NULL_HANDLE) {
            destroyRenderPass(obj);
        }
    }
    mRenderPassMap.clear();
    mRwRPMapMutex.unlock();

    // clear PipelineLayout
    mRwPLMapMutex.lock();
    for (auto &[k, v] : mPipelineLayoutMap) {
        auto *obj = findPipelineLayout(v);
        if (obj != GFX_NULL_HANDLE) {
            destroyPipelineLayout(obj);
        }
    }
    mRwPLMapMutex.unlock();

    // clear DescriptorLayouts
    mRwDLMapMutex.lock();
    for (auto &[k, v] : mDescriptorLayoutMap) {
        auto *obj = findDescriptorLayout(v);
        if (obj != GFX_NULL_HANDLE) {
            destroyDescriptorLayout(obj);
        }
    }
    mRwDLMapMutex.unlock();

    checkLeak();

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    vmaDestroyAllocator(mVmaAllocator);
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR

    mVkContext.destroy();
}

GVkContext *ContextVk::vkContext()
{
    return &mVkContext;
}

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR

VmaAllocator ContextVk::getVmaAllocator()
{
    return mVmaAllocator;
}

#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR

bool ContextVk::samplerAnisotropy()
{
    return vkContext()->gvkDevice()->deviceFeatures().samplerAnisotropy;
}

float ContextVk::maxSamplerAnisotropy()
{
    if (samplerAnisotropy()) {
        return vkContext()->gvkDevice()->deviceProperties().limits.maxSamplerAnisotropy;
    }
    return 1.0f;
}

VkDescriptorSet ContextVk::allocVkDescriptorSet(VkDescriptorSetLayout vkLayout, VkDescriptorPool &pool)
{
    std::vector<VkDescriptorPool> &pools = getDescriptorPools();

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.pSetLayouts = &vkLayout;
    descriptorSetAllocateInfo.descriptorSetCount = 1;

    VkDescriptorSet descSet;

    // 正向迭代是对的，这样才能最大程度利用Pool资源
    for (auto iPool : pools) {
        descriptorSetAllocateInfo.descriptorPool = iPool;
        if (vkAllocateDescriptorSets(mVkContext.vkDevice(), &descriptorSetAllocateInfo, &descSet) == VK_SUCCESS) {
            pool = iPool;
            return descSet;
        }
    }

    pool = createDescriptorPool();
    descriptorSetAllocateInfo.descriptorPool = pool;

    if (vkAllocateDescriptorSets(mVkContext.vkDevice(), &descriptorSetAllocateInfo, &descSet) == VK_SUCCESS) {
        return descSet;
    } else {
        return VK_NULL_HANDLE;
    }
}

uint32_t ContextVk::getQueueIndex(QueueType::Enum queueType)
{
    switch (queueType) {
        case QueueType::Graphics:
            return mVkContext.graphicsQueue()->queueFamilyIndex();
        case QueueType::Compute:
            return mVkContext.computeQueue()->queueFamilyIndex();
        case QueueType::Transfer:
            return mVkContext.transferQueue()->queueFamilyIndex();
    }
    return QueueType::Graphics;
}

bool ContextVk::isEnableValidation() const
{
    return mEnableValidation;
}

uint64_t ContextVk::isSupportQueryTimestamp() const
{
    return mSupportQueryTimestamp;
}

float ContextVk::getTimestampPeriod() const
{
    return mTimestampPeriod;
}

VkPhysicalDeviceFeatures ContextVk::getVkDeviceFeatures(uint32_t deviceIndex, InstanceVk *instance)
{
    VkPhysicalDeviceFeatures vkFeatures{};

    ExtFeatures supportedFeatures = instance->deviceInfo(deviceIndex).features;

    vkFeatures.fullDrawIndexUint32 = supportedFeatures.fullDrawIndexUint32;
    vkFeatures.geometryShader = supportedFeatures.geometryShader;
    vkFeatures.tessellationShader = supportedFeatures.tessellationShader;
    vkFeatures.samplerAnisotropy = supportedFeatures.samplerAnisotropy;

    vkFeatures.textureCompressionASTC_LDR = supportedFeatures.textureCompressionASTC_LDR;
    vkFeatures.textureCompressionBC = supportedFeatures.textureCompressionBC;
    vkFeatures.textureCompressionETC2 = supportedFeatures.textureCompressionETC2;

    return vkFeatures;
}

std::vector<const char *> ContextVk::transDeviceExt(const std::vector<DeviceEXT> &exts)
{
    std::vector<const char *> vkDeviceExts;

//    vkDeviceExts.push_back(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME);  // 纹理环绕border模式自定义颜色扩展
    return vkDeviceExts;
}

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR

void ContextVk::initVma()
{
    VmaVulkanFunctions vmaVulkanFunctions{};
#if VMA_DYNAMIC_VULKAN_FUNCTIONS
    vmaVulkanFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vmaVulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
#else
    vmaVulkanFunctions.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;
    vmaVulkanFunctions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
    vmaVulkanFunctions.vkAllocateMemory = vkAllocateMemory;
    vmaVulkanFunctions.vkFreeMemory = vkFreeMemory;
    vmaVulkanFunctions.vkMapMemory = vkMapMemory;
    vmaVulkanFunctions.vkUnmapMemory = vkUnmapMemory;
    vmaVulkanFunctions.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
    vmaVulkanFunctions.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
    vmaVulkanFunctions.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
    vmaVulkanFunctions.vkBindBufferMemory = vkBindBufferMemory;
    vmaVulkanFunctions.vkBindImageMemory = vkBindImageMemory;
    vmaVulkanFunctions.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements;
    vmaVulkanFunctions.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements;
    vmaVulkanFunctions.vkCreateBuffer = vkCreateBuffer;
    vmaVulkanFunctions.vkDestroyBuffer = vkDestroyBuffer;
    vmaVulkanFunctions.vkCreateImage = vkCreateImage;
    vmaVulkanFunctions.vkDestroyImage = vkDestroyImage;
    vmaVulkanFunctions.vkCmdCopyBuffer = vkCmdCopyBuffer;
    vmaVulkanFunctions.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2KHR;
    vmaVulkanFunctions.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2KHR;
    vmaVulkanFunctions.vkBindBufferMemory2KHR = vkBindBufferMemory2KHR;
    vmaVulkanFunctions.vkBindImageMemory2KHR = vkBindImageMemory2KHR;
    if(USE_VK_API_VER >= VK_MAKE_VERSION(1, 1, 0)) {
        vmaVulkanFunctions.vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2;
    } else {
        vmaVulkanFunctions.vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2KHR;
    }

    vmaVulkanFunctions.vkGetDeviceBufferMemoryRequirements = vkGetDeviceBufferMemoryRequirements;
    vmaVulkanFunctions.vkGetDeviceImageMemoryRequirements = vkGetDeviceImageMemoryRequirements;
#endif

    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.pVulkanFunctions = &vmaVulkanFunctions;
    allocatorInfo.vulkanApiVersion = USE_VK_API_VER;
    allocatorInfo.physicalDevice = mVkContext.gvkDevice()->physicalDevice();
    allocatorInfo.device = mVkContext.gvkDevice()->vkDevice();
    allocatorInfo.instance = mVkContext.vkInstance();

    vmaCreateAllocator(&allocatorInfo, &mVmaAllocator);
}

#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR

VkDescriptorPool ContextVk::createDescriptorPool()
{
    std::vector<VkDescriptorPoolSize> poolSizes;

    poolSizes.push_back({VK_DESCRIPTOR_TYPE_SAMPLER, MAX_DESC_POOL_SIZE});
    poolSizes.push_back({VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_DESC_POOL_SIZE * 4});
    poolSizes.push_back({VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, MAX_DESC_POOL_SIZE});
    poolSizes.push_back({VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, MAX_DESC_POOL_SIZE});

    poolSizes.push_back({VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_DESC_POOL_SIZE * 4});
    poolSizes.push_back({VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MAX_DESC_POOL_SIZE * 2});
    poolSizes.push_back({VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, MAX_DESC_POOL_SIZE});
    poolSizes.push_back({VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, MAX_DESC_POOL_SIZE});

    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptorPoolInfo.pPoolSizes = poolSizes.data();
    descriptorPoolInfo.maxSets = MAX_DESC_SETS;

    VkDescriptorPool pool;
    VK_CHECK_RESULT(vkCreateDescriptorPool(mVkContext.vkDevice(), &descriptorPoolInfo, nullptr, &pool));

    mVkDescriptorPools.push_back(pool);

    return pool;
}

std::vector<VkDescriptorPool> &ContextVk::getDescriptorPools()
{
    return mVkDescriptorPools;
}

Format::Enum ContextVk::getSupportedDepthFormat()
{
    VkFormat depthFormat = VK_FORMAT_UNDEFINED;
    std::vector<VkFormat> depthFormats = {
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D24_UNORM_S8_UINT,
            VK_FORMAT_D16_UNORM_S8_UINT,
            VK_FORMAT_D16_UNORM
    };

    auto &vkPhysicalDevice = mVkContext.gvkDevice()->physicalDevice();
    for (auto &format : depthFormats) {
        VkFormatProperties formatProps;
        vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, format, &formatProps);
        // Format must support depth stencil attachment for optimal tiling
        if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            depthFormat = format;
            break;
        }
    }
    Format::Enum gFormat = fromVkFormat(depthFormat);
    if (gFormat == Format::Undefined) {
        gFormat = Format::D16_UNorm;
    }
    return gFormat;
}


std::vector<Format::Enum> ContextVk::formatSupported(const std::vector<Format::Enum> &formats,
                                                     FormatFeatureFlags featureFlags)
{
    VkFormatFeatureFlags vkFeatureFlags = toVkFormatFeatureFlags(featureFlags);

    std::vector<Format::Enum> ret;
    ret.reserve(formats.size());

    auto &vkPhysicalDevice = mVkContext.gvkDevice()->physicalDevice();
    for (auto &format : formats) {
        VkFormatProperties formatProps;
        vkGetPhysicalDeviceFormatProperties(mVkContext.gvkDevice()->physicalDevice(), toVkFormat(format), &formatProps);
        if (formatProps.optimalTilingFeatures & vkFeatureFlags) {
            ret.push_back(format);
        }
    }

    return ret;
}

size_t ContextVk::uniformBufferOffsetAlignment(size_t dataSize)
{
    uint64_t minAlign = mVkContext.gvkDevice()->deviceProperties().limits.minUniformBufferOffsetAlignment;
    size_t align = dataSize;
    if (minAlign > 0) {
        align = (align + minAlign - 1) & ~(minAlign - 1);
    }
    return align;
}

size_t ContextVk::storageBufferOffsetAlignment(size_t dataSize)
{
    uint64_t minAlign = mVkContext.gvkDevice()->deviceProperties().limits.minStorageBufferOffsetAlignment;
    size_t align = dataSize;
    if (minAlign > 0) {
        align = (align + minAlign - 1) & ~(minAlign - 1);
    }
    return align;
}

size_t ContextVk::texelBufferOffsetAlignment(size_t dataSize)
{
    uint64_t minAlign = mVkContext.gvkDevice()->deviceProperties().limits.minTexelBufferOffsetAlignment;
    size_t align = dataSize;
    if (minAlign > 0) {
        align = (align + minAlign - 1) & ~(minAlign - 1);
    }
    return align;
}

SampleCountFlag::Enum ContextVk::maxRenderTargetSampleCount()
{
    VkSampleCountFlags msc =
            std::min(mVkContext.gvkDevice()->deviceProperties().limits.framebufferColorSampleCounts,
                     mVkContext.gvkDevice()->deviceProperties().limits.framebufferDepthSampleCounts);
    if (msc & VK_SAMPLE_COUNT_64_BIT) {
        return SampleCountFlag::SampleCount_64;
    }
    if (msc & VK_SAMPLE_COUNT_32_BIT) {
        return SampleCountFlag::SampleCount_32;
    }
    if (msc & VK_SAMPLE_COUNT_16_BIT) {
        return SampleCountFlag::SampleCount_16;
    }
    if (msc & VK_SAMPLE_COUNT_8_BIT) {
        return SampleCountFlag::SampleCount_8;
    }
    if (msc & VK_SAMPLE_COUNT_4_BIT) {
        return SampleCountFlag::SampleCount_4;
    }
    if (msc & VK_SAMPLE_COUNT_2_BIT) {
        return SampleCountFlag::SampleCount_2;
    }
    return SampleCountFlag::SampleCount_1;
}

uint32_t ContextVk::maxTextureDimension1D()
{
    return mVkContext.gvkDevice()->deviceProperties().limits.maxImageDimension1D;
}

uint32_t ContextVk::maxTextureDimension2D()
{
    return mVkContext.gvkDevice()->deviceProperties().limits.maxImageDimension2D;
}

uint32_t ContextVk::maxTextureDimension3D()
{
    return mVkContext.gvkDevice()->deviceProperties().limits.maxImageDimension3D;
}

uint32_t ContextVk::maxTextureDimensionCube()
{
    return mVkContext.gvkDevice()->deviceProperties().limits.maxImageDimensionCube;
}

uint32_t ContextVk::maxTextureArrayLayers()
{
    return mVkContext.gvkDevice()->deviceProperties().limits.maxImageArrayLayers;
}

bool ContextVk::isSupportTextureCubeArray()
{
    return mVkContext.gvkDevice()->deviceFeatures().imageCubeArray;
}

uint32_t ContextVk::maxPerStageShaderSamplersCount()
{
    return mVkContext.gvkDevice()->deviceProperties().limits.maxPerStageDescriptorSamplers;
}

uint32_t ContextVk::maxPerStageShaderUniformBuffersCount()
{
    return mVkContext.gvkDevice()->deviceProperties().limits.maxPerStageDescriptorUniformBuffers;
}

uint32_t ContextVk::maxPerStageShaderStorageBuffersCount()
{
    return mVkContext.gvkDevice()->deviceProperties().limits.maxPerStageDescriptorStorageBuffers;
}

uint32_t ContextVk::maxPerStageShaderSampledImagesCount()
{
    return mVkContext.gvkDevice()->deviceProperties().limits.maxPerStageDescriptorSampledImages;
}

uint32_t ContextVk::maxPerStageShaderStorageImagesCount()
{
    return mVkContext.gvkDevice()->deviceProperties().limits.maxPerStageDescriptorStorageImages;
}

void ContextVk::waitIdle()
{
    mVkContext.gvkDevice()->waitIdle();
}

void ContextVk::submitCommandBlock(CommandBuffer cmdBuffer, uint32_t bufferIndex)
{
    GX_ASSERT(cmdBuffer);
    auto *cmdBufferP = dynamic_cast<CommandBufferVk *>(cmdBuffer);
    auto *cmdPool = cmdBufferP->mVkCommandPool;
    auto *queue = cmdPool->queue();

    if (!cmdBufferP->isCompiled()) {
        cmdBufferP->compile();
    }

    GVkFence fence;
    fence.create(queue->device(), VK_FLAGS_NONE);

    queue->submit({}, {cmdBufferP->getVkCommandBuffer(bufferIndex)}, {}, fence);

    fence.wait();
    fence.destroy();
}

void ContextVk::submitCommand(CommandBuffer cmdBuffer, uint32_t bufferIndex, Fence fence)
{
    GX_ASSERT(cmdBuffer);
    auto *cmdBufferP = dynamic_cast<CommandBufferVk *>(cmdBuffer);
    auto *cmdPool = cmdBufferP->mVkCommandPool;
    auto *queue = cmdPool->queue();
    FenceVk *fenceP = GFX_NULL_HANDLE;
    if (fence) {
        fenceP = dynamic_cast<FenceVk *>(fence);
    }

    if (!cmdBufferP->isCompiled()) {
        cmdBufferP->compile();
    }

    queue->submit({}, {cmdBufferP->getVkCommandBuffer(bufferIndex)}, {},
                  fenceP ? fenceP->vkFence()->vkFence() : VK_NULL_HANDLE);
}

void ContextVk::queueWaitIdle(QueueType::Enum queueType)
{
    switch (queueType) {
        case QueueType::Graphics:
            mVkContext.graphicsQueue()->waitIdle();
            break;
        case QueueType::Compute:
            mVkContext.computeQueue()->waitIdle();
            break;
        case QueueType::Transfer:
            mVkContext.transferQueue()->waitIdle();
            break;
    }
}

std::string ContextVk::dumpCommandBuffer(CommandBuffer commandBuffer)
{
    return dynamic_cast<CommandBufferVk *>(commandBuffer)->dump();
}

Fence_P *ContextVk::createFenceP(bool signaled)
{
    uint16_t oIdx = mFenceIDAlloc.alloc();
    GX_ASSERT(mFenceIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(FenceVk, genElementIdx(mIdx, oIdx, ElementType::Fence));
    if (obj && obj->init(mParentCtx, signaled)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createFenceP create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyFenceP(Fence obj)
{
    GX_ASSERT(obj);
    auto *objP = dynamic_cast<Fence_P *>(obj);
    GX_ASSERT(objP);

    uint8_t tId = getElementTypeIdx(objP->idx());
    GX_ASSERT(tId == ElementType::Fence);
    uint16_t oId = getElementObjectIdx(objP->idx());

    destroyElement(objP);
    mFenceIDAlloc.free(oId);
}

Fence_P *ContextVk::findFenceP(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::Fence, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<Fence_P *>(objE);
}

RenderPassVk *ContextVk::getRenderPass(const GetRenderPassInfo &getInfo)
{
    RenderPassVk *obj = GFX_NULL_HANDLE;

    GLockerGuard locker(mRwRPMapMutex);

    auto it = mRenderPassMap.find(getInfo);
    if (it != mRenderPassMap.end()) {
        obj = findRenderPass(it->second);
    }
    if (obj == GFX_NULL_HANDLE) {
        Log("ContextVk::getRenderPassP create render pass");
        obj = createRenderPass(getInfo);
        if (obj) {
            mRenderPassMap.emplace(getInfo, obj->idx());
        }
    }
    return obj;
}

RenderPassVk *ContextVk::findRenderPass(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::RenderPass, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<RenderPassVk *>(objE);
}

Frame_P *ContextVk::createFrameP(const CreateFrameInfo &createInfo)
{
    uint16_t oIdx = mFrameIDAlloc.alloc();
    GX_ASSERT(mFrameIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(FrameVk, genElementIdx(mIdx, oIdx, ElementType::Frame));
    if (obj && obj->init(mParentCtx, createInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createFrameP create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyFrameP(Frame obj)
{
    GX_ASSERT(obj);
    auto *objP = dynamic_cast<Frame_P *>(obj);
    GX_ASSERT(objP);

    uint8_t tId = getElementTypeIdx(objP->idx());
    GX_ASSERT(tId == ElementType::Frame);
    uint16_t oId = getElementObjectIdx(objP->idx());

    destroyElement(objP);
    mFrameIDAlloc.free(oId);
}

Frame_P *ContextVk::findFrameP(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::Frame, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<Frame_P *>(objE);
}

RenderTarget_P *ContextVk::createRenderTargetP(const CreateRenderTargetInfo &createInfo)
{
    return createRenderTargetP(createInfo, false);
}

RenderTarget_P *ContextVk::createRenderTargetP(const CreateRenderTargetInfo &createInfo, bool isSwapChain)
{
    uint16_t oIdx = mRenderTargetIDAlloc.alloc();
    GX_ASSERT(mRenderTargetIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(RenderTargetVk, genElementIdx(mIdx, oIdx, ElementType::RenderTarget));
    if (obj && obj->init(mParentCtx, createInfo, isSwapChain)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createRenderTargetP create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyRenderTargetP(RenderTarget obj)
{
    GX_ASSERT(obj);
    auto *objP = dynamic_cast<RenderTarget_P *>(obj);
    GX_ASSERT(objP);

    uint8_t tId = getElementTypeIdx(objP->idx());
    GX_ASSERT(tId == ElementType::RenderTarget);
    uint16_t oId = getElementObjectIdx(objP->idx());

    destroyElement(objP);
    mRenderTargetIDAlloc.free(oId);
}

RenderTarget_P *ContextVk::findRenderTargetP(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::RenderTarget, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<RenderTarget_P *>(objE);
}

Texture_P *ContextVk::createTextureP(const CreateTextureInfo &createInfo,
                                     SampleCountFlag::Enum sample,
                                     GVkImage *image)
{
    uint16_t oIdx = mTextureIDAlloc.alloc();
    GX_ASSERT(mTextureIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(TextureVk, genElementIdx(mIdx, oIdx, ElementType::Texture));
    if (obj && obj->init(mParentCtx, createInfo, sample, image)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createTextureP create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

Texture_P *ContextVk::createTextureP(const CreateTextureInfo &createInfo, SampleCountFlag::Enum sample)
{
    return createTextureP(createInfo, sample, nullptr);
}

void ContextVk::destroyTextureP(Texture obj)
{
    GX_ASSERT(obj);
    auto *objP = dynamic_cast<Texture_P *>(obj);
    GX_ASSERT(objP);

    uint8_t tId = getElementTypeIdx(objP->idx());
    GX_ASSERT(tId == ElementType::Texture);
    uint16_t oId = getElementObjectIdx(objP->idx());

    destroyElement(objP);
    mTextureIDAlloc.free(oId);
}

Texture_P *ContextVk::findTextureP(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::Texture, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<Texture_P *>(objE);
}

Sampler_P *ContextVk::createSamplerP(const CreateSamplerInfo &createInfo)
{
    uint16_t oIdx = mSamplerIDAlloc.alloc();
    GX_ASSERT(mSamplerIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(SamplerVk, genElementIdx(mIdx, oIdx, ElementType::Sampler));
    if (obj && obj->init(mParentCtx, createInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createSamplerP create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroySamplerP(Sampler obj)
{
    GX_ASSERT(obj);
    auto *objP = dynamic_cast<Sampler_P *>(obj);
    GX_ASSERT(objP);

    uint8_t tId = getElementTypeIdx(objP->idx());
    GX_ASSERT(tId == ElementType::Sampler);
    uint16_t oId = getElementObjectIdx(objP->idx());

    destroyElement(objP);
    mSamplerIDAlloc.free(oId);
}

Sampler_P *ContextVk::findSamplerP(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::Sampler, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<Sampler_P *>(objE);
}

Buffer_P *ContextVk::createBufferP(const CreateBufferInfo &createInfo)
{
    uint16_t oIdx = mBufferIDAlloc.alloc();
    GX_ASSERT(mBufferIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(BufferVk, genElementIdx(mIdx, oIdx, ElementType::Buffer));
    if (obj && obj->init(mParentCtx, createInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createBufferP create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyBufferP(Buffer obj)
{
    GX_ASSERT(obj);
    auto *objP = dynamic_cast<Buffer_P *>(obj);
    GX_ASSERT(objP);

    uint8_t tId = getElementTypeIdx(objP->idx());
    GX_ASSERT(tId == ElementType::Buffer);
    uint16_t oId = getElementObjectIdx(objP->idx());

    destroyElement(objP);
    mBufferIDAlloc.free(oId);
}

Buffer_P *ContextVk::findBufferP(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::Buffer, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<Buffer_P *>(objE);
}

Shader_P *ContextVk::createShaderP(const CreateShaderInfo &createInfo)
{
    uint16_t oIdx = mShaderIDAlloc.alloc();
    GX_ASSERT(mShaderIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(ShaderVk, genElementIdx(mIdx, oIdx, ElementType::Shader));
    if (obj && obj->init(mParentCtx, createInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createShaderP create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyShaderP(Shader obj)
{
    GX_ASSERT(obj);
    auto *objP = dynamic_cast<Shader_P *>(obj);
    GX_ASSERT(objP);

    uint8_t tId = getElementTypeIdx(objP->idx());
    GX_ASSERT(tId == ElementType::Shader);
    uint16_t oId = getElementObjectIdx(objP->idx());

    destroyElement(objP);
    mShaderIDAlloc.free(oId);
}

Shader_P *ContextVk::findShaderP(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::Shader, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<Shader_P *>(objE);
}

DescriptorLayoutVk *ContextVk::getDescriptorLayout(const ResourceLayoutInfo &createInfo)
{
    DescriptorLayoutVk *obj = GFX_NULL_HANDLE;

    GLockerGuard locker(mRwDLMapMutex);

    auto it = mDescriptorLayoutMap.find(createInfo);
    if (it != mDescriptorLayoutMap.end()) {
        obj = findDescriptorLayout(it->second);
    }
    if (obj == GFX_NULL_HANDLE) {
        Log("ContextVk::getDescriptorLayout create descriptor layout");
        obj = createDescriptorLayout(createInfo);
        if (obj) {
            mDescriptorLayoutMap.emplace(createInfo, obj->idx());
        }
    }

    return obj;
}

DescriptorLayoutVk *ContextVk::findDescriptorLayout(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::DescLayout, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<DescriptorLayoutVk *>(objE);
}

ResourceBinder_P *ContextVk::createResourceBinderP(const ResourceLayoutInfo &layoutInfo)
{
    uint16_t oIdx = mDescSetBinderIDAlloc.alloc();
    GX_ASSERT(mDescSetBinderIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(ResourceBinderVk, genElementIdx(mIdx, oIdx, ElementType::ResourceBinder));
    if (obj && obj->init(mParentCtx, layoutInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createResourceBinderP create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyResourceBinderP(ResourceBinder obj)
{
    GX_ASSERT(obj);
    auto *objP = dynamic_cast<ResourceBinder_P *>(obj);
    GX_ASSERT(objP);

    uint8_t tId = getElementTypeIdx(objP->idx());
    GX_ASSERT(tId == ElementType::ResourceBinder);
    uint16_t oId = getElementObjectIdx(objP->idx());

    destroyElement(objP);
    mDescSetBinderIDAlloc.free(oId);
}

ResourceBinder_P *ContextVk::findResourceBinderP(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::ResourceBinder, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<ResourceBinder_P *>(objE);
}

PipelineLayoutVk *ContextVk::getPipelineLayout(const PipelineLayoutInfo &info)
{
    PipelineLayoutVk *obj = GFX_NULL_HANDLE;

    GLockerGuard locker(mRwPLMapMutex);

    auto it = mPipelineLayoutMap.find(info);
    if (it != mPipelineLayoutMap.end()) {
        obj = findPipelineLayout(it->second);
    }
    if (obj == GFX_NULL_HANDLE) {
        Log("ContextVk::getPipelineLayout create pipeline layout");
        obj = createPipelineLayout(info);
        if (obj) {
            mPipelineLayoutMap.emplace(info, obj->idx());
        }
    }

    return obj;
}

PipelineLayoutVk *ContextVk::findPipelineLayout(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::PipelineLayout, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<PipelineLayoutVk *>(objE);
}

PipelineVk *ContextVk::getGraphicsPipeline(const CreateGraphicsPipelineStateInfo &createInfo)
{
    QueryGraphicsPipelineStateInfo queryInfo = createQueryGraphicsPipelineStateInfo(createInfo);

    PipelineVk *obj = GFX_NULL_HANDLE;

    GLockerGuard locker(mRwGPMapMutex);

    auto it = mGraphPipelineMap.find(queryInfo);
    if (it != mGraphPipelineMap.end()) {
        obj = findPipeline(it->second);
    }
    if (obj == GFX_NULL_HANDLE) {
        Log("ContextVk::getGraphicsPipelineP create graphics pipeline");
        obj = createGraphicsPipeline(createInfo);
        if (obj) {
            mGraphPipelineMap.emplace(queryInfo, obj->idx());
            for (auto *shader : createInfo.shaderPrograms) {
                dynamic_cast<ShaderVk *>(shader)->refPipeline(obj->idx());
            }
        }
    }

    return obj;
}

PipelineVk *ContextVk::getComputePipeline(const CreateComputePipelineStateInfo &createInfo)
{
    QueryComputePipelineStateInfo queryInfo = createQueryComputePipelineStateInfo(createInfo);

    PipelineVk *obj = GFX_NULL_HANDLE;

    GLockerGuard locker(mRwCPMapMutex);

    auto it = mCompPipelineMap.find(queryInfo);
    if (it != mCompPipelineMap.end()) {
        obj = findPipeline(it->second);
    }
    if (obj == GFX_NULL_HANDLE) {
        Log("ContextVk::getComputePipeline create compute pipeline");
        obj = createComputePipeline(createInfo);
        if (obj) {
            mCompPipelineMap.emplace(queryInfo, obj->idx());
            for (auto *shader : createInfo.shaderPrograms) {
                dynamic_cast<ShaderVk *>(shader)->refPipeline(obj->idx());
            }
        }
    }

    return obj;
}

PipelineVk *ContextVk::createGraphicsPipeline(const CreateGraphicsPipelineStateInfo &createInfo)
{
    uint16_t oIdx = mPipelineStateIDAlloc.alloc();
    GX_ASSERT(mPipelineStateIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(PipelineVk, genElementIdx(mIdx, oIdx, ElementType::PipelineState));
    if (obj && obj->init(mParentCtx, createInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createGraphicsPipeline create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

PipelineVk *ContextVk::findPipeline(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::PipelineState, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<PipelineVk *>(objE);
}

void ContextVk::destroyPipeline(PipelineVk *obj)
{
    GX_ASSERT(obj);
    {
        auto it = mGraphPipelineMap.begin();
        for (; it != mGraphPipelineMap.end(); it++) {
            if (it->second == obj->idx()) {
                mGraphPipelineMap.erase(it);
                break;
            }
        }
    }
    {
        auto it = mCompPipelineMap.begin();
        for (; it != mCompPipelineMap.end(); it++) {
            if (it->second == obj->idx()) {
                mCompPipelineMap.erase(it);
                break;
            }
        }
    }

    uint8_t tId = getElementTypeIdx(obj->idx());
    GX_ASSERT(tId == ElementType::PipelineState);
    uint16_t oId = getElementObjectIdx(obj->idx());

    destroyElement(obj);
    mPipelineStateIDAlloc.free(oId);
}

PipelineVk *ContextVk::createComputePipeline(const CreateComputePipelineStateInfo &createInfo)
{
    uint16_t oIdx = mPipelineStateIDAlloc.alloc();
    GX_ASSERT(mPipelineStateIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(PipelineVk, genElementIdx(mIdx, oIdx, ElementType::PipelineState));
    if (obj && obj->init(mParentCtx, createInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createComputePipeline create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

CommandBuffer_P *ContextVk::createCommandBufferP(const CreateCommandBufferInfo &createInfo)
{
    uint16_t oIdx = mCommandBufferIDAlloc.alloc();
    GX_ASSERT(mCommandBufferIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(CommandBufferVk, genElementIdx(mIdx, oIdx, ElementType::CommandBuffer));
    if (obj && obj->init(mParentCtx, createInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createCommandBuffer create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyCommandBufferP(CommandBuffer obj)
{
    GX_ASSERT(obj);
    auto *objP = dynamic_cast<CommandBuffer_P *>(obj);
    GX_ASSERT(objP);

    uint8_t tId = getElementTypeIdx(objP->idx());
    GX_ASSERT(tId == ElementType::CommandBuffer);
    uint16_t oId = getElementObjectIdx(objP->idx());

    destroyElement(objP);
    mCommandBufferIDAlloc.free(oId);
}

CommandBuffer_P *ContextVk::findCommandBufferP(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::CommandBuffer, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<CommandBuffer_P *>(objE);
}

Query_P *ContextVk::createQueryP(const CreateQueryInfo &createInfo)
{
    uint16_t oIdx = mQueryIDAlloc.alloc();
    GX_ASSERT(mQueryIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(QueryVk, genElementIdx(mIdx, oIdx, ElementType::Query));
    if (obj && obj->init(mParentCtx, createInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createQuery create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyQueryP(Query obj)
{
    GX_ASSERT(obj);
    auto *objP = dynamic_cast<Query_P *>(obj);
    GX_ASSERT(objP);

    uint8_t tId = getElementTypeIdx(objP->idx());
    GX_ASSERT(tId == ElementType::Query);
    uint16_t oId = getElementObjectIdx(objP->idx());

    destroyElement(objP);
    mQueryIDAlloc.free(oId);
}

Query_P *ContextVk::findQueryP(GfxIdxTy idx)
{
    auto *objE = findElement(ElementType::Query, idx);
    if (!objE) {
        return GFX_NULL_HANDLE;
    }
    return dynamic_cast<Query_P *>(objE);
}

RenderPassVk *ContextVk::createRenderPass(const GetRenderPassInfo &createInfo)
{
    uint16_t oIdx = mRenderPassIDAlloc.alloc();
    GX_ASSERT(mRenderPassIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(RenderPassVk, genElementIdx(mIdx, oIdx, ElementType::RenderPass));
    if (obj && obj->init(mParentCtx, createInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createRenderPass create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyRenderPass(RenderPassVk *obj)
{
    GX_ASSERT(obj);

    uint8_t tId = getElementTypeIdx(obj->idx());
    GX_ASSERT(tId == ElementType::RenderPass);
    uint16_t oId = getElementObjectIdx(obj->idx());

    destroyElement(obj);
    mRenderPassIDAlloc.free(oId);
}

DescriptorLayoutVk *ContextVk::createDescriptorLayout(const ResourceLayoutInfo &createInfo)
{
    uint16_t oIdx = mDescLayoutIDAlloc.alloc();
    GX_ASSERT(mDescLayoutIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(DescriptorLayoutVk, genElementIdx(mIdx, oIdx, ElementType::DescLayout));
    if (obj && obj->init(mParentCtx, createInfo)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createDescriptorLayout create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyDescriptorLayout(DescriptorLayoutVk *obj)
{
    GX_ASSERT(obj);

    uint8_t tId = getElementTypeIdx(obj->idx());
    GX_ASSERT(tId == ElementType::DescLayout);
    uint16_t oId = getElementObjectIdx(obj->idx());

    destroyElement(obj);
    mDescLayoutIDAlloc.free(oId);
}

PipelineLayoutVk *ContextVk::createPipelineLayout(const PipelineLayoutInfo &info)
{
    uint16_t oIdx = mPipelineLayoutIDAlloc.alloc();
    GX_ASSERT(mPipelineLayoutIDAlloc.isValid(oIdx));
    auto *obj = GX_NEW(PipelineLayoutVk, genElementIdx(mIdx, oIdx, ElementType::PipelineLayout));
    if (obj && obj->init(mParentCtx, info)) {
        GX_ASSERT(!containElementMap(obj->idx()));
        insertElementMap(obj);
        return obj;
    }
    GX_ASSERT_S(obj != nullptr, "ContextVk::createPipelineLayout create object failure");
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void ContextVk::destroyPipelineLayout(PipelineLayoutVk *obj)
{
    GX_ASSERT(obj);

    uint8_t tId = getElementTypeIdx(obj->idx());
    GX_ASSERT(tId == ElementType::PipelineLayout);
    uint16_t oId = getElementObjectIdx(obj->idx());

    destroyElement(obj);
    mPipelineLayoutIDAlloc.free(oId);
}

bool ContextVk::containElementMap(GfxIdxTy idx)
{
    GLockerGuard locker(mElementMapMutex);
    return mElementMap.find(idx) != mElementMap.end();
}

void ContextVk::insertElementMap(ElementHandle *obj)
{
    GLockerGuard locker(mElementMapMutex);
    mElementMap.insert(std::make_pair(obj->idx(), obj));
    GX_ASSERT_S(mElementMap.find(obj->idx()) != mElementMap.end(), "insertElementMap failure");
}

void ContextVk::removeElementMap(GfxIdxTy idx, ElementHandle *obj)
{
    GLockerGuard locker(mElementMapMutex);
    auto it = mElementMap.find(idx);
    if (it == mElementMap.end()) {
        return;
    }
    auto *objF = it->second;
    GX_ASSERT_S(obj == objF, "find idx(%d) element object mismatch", idx);
    mElementMap.erase(it);
}

void ContextVk::destroyElement(ElementHandle *obj)
{
    GX_ASSERT_S(obj->context() == this->mParentCtx, "Context mismatch");
    GfxIdxTy idx = obj->idx();
    removeElementMap(idx, obj);

    obj->destroy();
    GX_DELETE(obj);
}

ElementHandle *ContextVk::findElement(ElementType::Enum type, GfxIdxTy idx)
{
    // 先做类型校验
    GX_ASSERT(getElementContextIdx(idx) == mIdx);
    GX_ASSERT(getElementTypeIdx(idx) == (uint8_t)type);
    if (getElementTypeIdx(idx) != (uint8_t)type) {
        return GFX_NULL_HANDLE;
    }

    GLockerGuard locker(mElementMapMutex);

    auto it = mElementMap.find(idx);
    if (it == mElementMap.end()) {
        return GFX_NULL_HANDLE;
    }
    return it->second;
}

void ContextVk::checkLeak()
{
    bool hasLeak = !mElementMap.empty();
    uint32_t leakCounts[ElementType::Count] = {0};

    for (auto &[k, v] : mElementMap) {
        uint8_t tid = getElementTypeIdx(k);
        leakCounts[tid]++;
    }

    for (int i = 0; i < ElementType::Count; i++) {
        if (leakCounts[i] > 0) {
            Log("GFX Element(%s) leak, leak count = %d", ElementTypeNames[i], leakCounts[i]);
        }
    }
    GX_ASSERT_S(!hasLeak, "GFX Elements memory leak");
}

/// ============ FenceVk ============ ///

bool FenceVk::init(Context_T *context, bool signaled)
{
    mContextT = context;

    mVkFence.create(getGVkContext(context)->gvkDevice(), signaled ? VK_FENCE_CREATE_SIGNALED_BIT : VK_FLAGS_NONE);

    return true;
}

void FenceVk::destroy()
{
    mVkFence.destroy();

    mContextT = GFX_NULL_HANDLE;
}

Context_T *FenceVk::context()
{
    return mContextT;
}

FenceWaitRet::Enum FenceVk::wait(uint64_t timeout)
{
    return mVkFence.wait(timeout) == VK_TIMEOUT ? FenceWaitRet::Timeout : FenceWaitRet::Success;
}

void FenceVk::reset()
{
    mVkFence.reset();
}

GVkFence *FenceVk::vkFence()
{
    return &mVkFence;
}

bool FenceVk::isSignaled()
{
    return mVkFence.isSignaled();
}

/// ============ RenderPassVk ============ ///
bool RenderPassVk::init(Context_T *context, const GetRenderPassInfo &createInfo)
{
    mContextT = context;
    return initVkRenderPass(createInfo);
}

void RenderPassVk::destroy()
{
    mVkRenderPass.destroy();
    mContextT = GFX_NULL_HANDLE;
}

Context_T *RenderPassVk::context()
{
    return mContextT;
}

size_t RenderPassVk::hash()
{
    return mHash;
}

GVkRenderPass *RenderPassVk::vkRenderPass()
{
    return &mVkRenderPass;
}

uint32_t RenderPassVk::subPassCount() const
{
    return mSubPassCount;
}

uint32_t RenderPassVk::colorAttachmentCount() const
{
    return mColorAttachmentCount;
}

SampleCountFlag::Enum RenderPassVk::sampleCountFlag() const
{
    return mSampleCountFlag == 0 ? SampleCountFlag::SampleCount_1 : (SampleCountFlag::Enum) mSampleCountFlag;
}

bool RenderPassVk::initVkRenderPass(const GetRenderPassInfo &createInfo)
{
    std::hash<GetRenderPassInfo> hashFunc;
    mHash = hashFunc(createInfo);

    SampleCountFlag::Enum sample = createInfo.sample == 0 ? SampleCountFlag::SampleCount_1 : createInfo.sample;
    mSampleCountFlag = sample;

    const std::vector<AttachmentDesc> &colorAttachmentDescs = createInfo.colorAttachmentDescs;
    const AttachmentDesc &depthAttachmentDesc = createInfo.depthAttachmentDesc;
    const std::vector<SubPassDesc> &subPassDescs = createInfo.subPassDescs;
    const std::vector<SubPassDependency> &subPassDepends = createInfo.subPassDepends;
    bool hasDepth = depthAttachmentDesc.t != 0;
    bool msaa = sample != SampleCountFlag::SampleCount_1;

    mColorAttachmentCount = createInfo.colorAttachmentDescs.size() * (msaa ? 2 : 1);

    std::vector<VkAttachmentDescription> vkAttachments;
    std::vector<VkSubpassDescription> vkSubpassDescs;
    std::vector<VkSubpassDependency> vkDependencies;

    vkAttachments.resize((colorAttachmentDescs.size() + (int) hasDepth) * (msaa ? 2 : 1));

    // color
    uint32_t attachmentIndex = 0;
    for (uint32_t i = 0; i < colorAttachmentDescs.size(); i++) {
        const AttachmentDesc &gfxAttachDesc = colorAttachmentDescs[i];

        if (msaa) {
            // msaa color
            vkAttachments[attachmentIndex].format = toVkFormat(gfxAttachDesc.format);
            vkAttachments[attachmentIndex].samples = toVkSampleCount(sample);
            vkAttachments[attachmentIndex].loadOp = gfxAttachDesc.clear ? VK_ATTACHMENT_LOAD_OP_CLEAR
                                                                        : gfxAttachDesc.discard
                                                                          ? VK_ATTACHMENT_LOAD_OP_DONT_CARE
                                                                          : VK_ATTACHMENT_LOAD_OP_LOAD;
            vkAttachments[attachmentIndex].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            vkAttachments[attachmentIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            vkAttachments[attachmentIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            vkAttachments[attachmentIndex].initialLayout = toVkImageLayout(gfxAttachDesc.initLayout);
            vkAttachments[attachmentIndex].finalLayout = toVkImageLayout(gfxAttachDesc.finalLayout);
            attachmentIndex += 1;

            // color
            vkAttachments[attachmentIndex].format = toVkFormat(gfxAttachDesc.format);
            vkAttachments[attachmentIndex].samples = toVkSampleCount(SampleCountFlag::SampleCount_1);
            vkAttachments[attachmentIndex].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            vkAttachments[attachmentIndex].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            vkAttachments[attachmentIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            vkAttachments[attachmentIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            vkAttachments[attachmentIndex].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            vkAttachments[attachmentIndex].finalLayout = toVkImageLayout(gfxAttachDesc.finalLayout);
            attachmentIndex += 1;
        } else {
            vkAttachments[attachmentIndex].format = toVkFormat(gfxAttachDesc.format);
            vkAttachments[attachmentIndex].samples = toVkSampleCount(SampleCountFlag::SampleCount_1);
            vkAttachments[attachmentIndex].loadOp = gfxAttachDesc.clear ? VK_ATTACHMENT_LOAD_OP_CLEAR
                                                                        : gfxAttachDesc.discard
                                                                          ? VK_ATTACHMENT_LOAD_OP_DONT_CARE
                                                                          : VK_ATTACHMENT_LOAD_OP_LOAD;
            vkAttachments[attachmentIndex].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            vkAttachments[attachmentIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            vkAttachments[attachmentIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            vkAttachments[attachmentIndex].initialLayout =
                    gfxAttachDesc.discard ? VK_IMAGE_LAYOUT_UNDEFINED : toVkImageLayout(gfxAttachDesc.initLayout);
            vkAttachments[attachmentIndex].finalLayout = toVkImageLayout(gfxAttachDesc.finalLayout);
            attachmentIndex += 1;
        }
    }

    // depth
    uint32_t depthIndex = attachmentIndex;
    if (hasDepth) {
        if (msaa) {
            // msaa depth
            vkAttachments[depthIndex].format = toVkFormat(depthAttachmentDesc.format);
            vkAttachments[depthIndex].samples = toVkSampleCount(sample);
            vkAttachments[depthIndex].loadOp = depthAttachmentDesc.clear ? VK_ATTACHMENT_LOAD_OP_CLEAR
                                                                         : depthAttachmentDesc.discard
                                                                           ? VK_ATTACHMENT_LOAD_OP_DONT_CARE
                                                                           : VK_ATTACHMENT_LOAD_OP_LOAD;
            vkAttachments[depthIndex].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            vkAttachments[depthIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            vkAttachments[depthIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            vkAttachments[depthIndex].initialLayout = toVkImageLayout(depthAttachmentDesc.initLayout);
            vkAttachments[depthIndex].finalLayout = toVkImageLayout(depthAttachmentDesc.finalLayout);

            // depth
            vkAttachments[depthIndex + 1].format = toVkFormat(depthAttachmentDesc.format);
            vkAttachments[depthIndex + 1].samples = toVkSampleCount(SampleCountFlag::SampleCount_1);
            vkAttachments[depthIndex + 1].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            vkAttachments[depthIndex + 1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            vkAttachments[depthIndex + 1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            vkAttachments[depthIndex + 1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            vkAttachments[depthIndex + 1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            vkAttachments[depthIndex + 1].finalLayout = toVkImageLayout(depthAttachmentDesc.finalLayout);
        } else {
            vkAttachments[depthIndex].format = toVkFormat(depthAttachmentDesc.format);
            vkAttachments[depthIndex].samples = toVkSampleCount(SampleCountFlag::SampleCount_1);
            vkAttachments[depthIndex].loadOp = depthAttachmentDesc.clear ? VK_ATTACHMENT_LOAD_OP_CLEAR
                                                                         : depthAttachmentDesc.discard
                                                                           ? VK_ATTACHMENT_LOAD_OP_DONT_CARE
                                                                           : VK_ATTACHMENT_LOAD_OP_LOAD;
            vkAttachments[depthIndex].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            vkAttachments[depthIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            vkAttachments[depthIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            vkAttachments[depthIndex].initialLayout = toVkImageLayout(depthAttachmentDesc.initLayout);
            vkAttachments[depthIndex].finalLayout = toVkImageLayout(depthAttachmentDesc.finalLayout);
        }
    }

    mSubPassCount = subPassDescs.size();
    vkSubpassDescs.resize(subPassDescs.size());

    // 作用域要在这一层
    std::vector<std::vector<VkAttachmentReference>> colorAttachmentRefs;
    std::vector<std::vector<VkAttachmentReference>> inputAttachmentRefs;
    std::vector<std::vector<VkAttachmentReference>> resolveRefs;
    std::vector<VkAttachmentReference> depthAttachmentRefs;
    colorAttachmentRefs.resize(subPassDescs.size());
    inputAttachmentRefs.resize(subPassDescs.size());
    resolveRefs.resize(subPassDescs.size());
    depthAttachmentRefs.resize(subPassDescs.size());

    for (uint32_t i = 0; i < subPassDescs.size(); i++) {
        const SubPassDesc &gfxSubPassDesc = subPassDescs[i];
        std::vector<VkAttachmentReference> &colorReference = colorAttachmentRefs[i];
        std::vector<VkAttachmentReference> &resolveReference = resolveRefs[i];
        std::vector<VkAttachmentReference> &inputReference = inputAttachmentRefs[i];
        colorReference.resize(gfxSubPassDesc.colorAttachments.size());
        inputReference.resize(gfxSubPassDesc.inputAttachments.size());
        if (msaa) {
            resolveReference.resize(gfxSubPassDesc.colorAttachments.size());
        }

        for (uint32_t j = 0; j < colorReference.size(); j++) {
            if (msaa) {
                colorReference[j].attachment = gfxSubPassDesc.colorAttachments[j] * 2;
                colorReference[j].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                resolveReference[j].attachment = gfxSubPassDesc.colorAttachments[j] * 2 + 1;
                resolveReference[j].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            } else {
                colorReference[j].attachment = gfxSubPassDesc.colorAttachments[j];
                colorReference[j].layout = toVkImageLayout(
                        colorAttachmentDescs[colorReference[j].attachment].subPassLayout);
            }
        }

        for (uint32_t j = 0; j < inputReference.size(); j++) {
            inputReference[j].attachment = gfxSubPassDesc.inputAttachments[j];
            inputReference[j].layout = toVkImageLayout(
                    colorAttachmentDescs[inputReference[j].attachment].subPassLayout);
        }

        vkSubpassDescs[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        vkSubpassDescs[i].colorAttachmentCount = colorReference.size();
        vkSubpassDescs[i].pColorAttachments = colorReference.data();
        vkSubpassDescs[i].pResolveAttachments = resolveReference.empty() ? nullptr : resolveReference.data();
        vkSubpassDescs[i].inputAttachmentCount = inputReference.size();
        vkSubpassDescs[i].pInputAttachments = inputReference.data();

        if (hasDepth && gfxSubPassDesc.useDepthStencil) {
            VkAttachmentReference &depthStencilReference = depthAttachmentRefs[i];
            depthStencilReference.attachment = depthIndex;
            depthStencilReference.layout = toVkImageLayout(depthAttachmentDesc.subPassLayout);
            vkSubpassDescs[i].pDepthStencilAttachment = &depthStencilReference;
        } else {
            vkSubpassDescs[i].pDepthStencilAttachment = nullptr;
        }
    }

    vkDependencies.resize(subPassDepends.size());

    for (uint32_t i = 0; i < subPassDepends.size(); i++) {
        const SubPassDependency &gfxDependency = subPassDepends[i];

        vkDependencies[i].srcSubpass =
                gfxDependency.srcSubPass == SUBPASS_EXTERNAL ? VK_SUBPASS_EXTERNAL : gfxDependency.srcSubPass;
        vkDependencies[i].dstSubpass =
                gfxDependency.dstSubPass == SUBPASS_EXTERNAL ? VK_SUBPASS_EXTERNAL : gfxDependency.dstSubPass;
        if (gfxDependency.srcSubPass == SUBPASS_EXTERNAL) {
            vkDependencies[i].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            vkDependencies[i].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            vkDependencies[i].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            vkDependencies[i].dstAccessMask =
                    VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        } else if (gfxDependency.dstSubPass == SUBPASS_EXTERNAL) {
            vkDependencies[i].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            vkDependencies[i].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            vkDependencies[i].srcAccessMask =
                    VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            vkDependencies[i].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        } else {
            vkDependencies[i].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            vkDependencies[i].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            vkDependencies[i].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            vkDependencies[i].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        }
        vkDependencies[i].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    }

    mVkRenderPass.create(getGVkContext(mContextT)->gvkDevice(), vkAttachments, vkSubpassDescs, vkDependencies);
    return mVkRenderPass.isCreated();
}

/// ============ FrameVk ============ ///
bool FrameVk::init(Context_T *context, const CreateFrameInfo &createInfo)
{
    mContextT = context;
    mRenderTargetType = createInfo.targetType;
    mVSync = createInfo.vSync;
    mWidth = createInfo.frameWidth;
    mHeight = createInfo.frameHeight;

    bool ok;
    if (mRenderTargetType == FrameTargetType::SwapChain) {
        ok = initSwapChain((SwapChainInfo *)createInfo.pTarget) && initRenderTarget(true);
    } else {
        mRenderTarget = dynamic_cast<RenderTargetVk *>((RenderTarget)createInfo.pTarget);
        ok = mRenderTarget != nullptr && initFence(mRenderTarget->frameBufferCount());
    }
    if (!ok) {
        destroy();
    }
    return ok;
}

void FrameVk::destroy()
{
    if (mRenderTargetType == FrameTargetType::SwapChain) {
        mContextT->destroyRenderTarget(mRenderTarget);
    }

    for (auto *t : mColorTextures) {
        mContextT->destroyTexture(t);
    }
    mColorTextures.clear();

    if (mDepthTexture != GFX_NULL_HANDLE) {
        mContextT->destroyTexture(mDepthTexture);
    }

    mRenderSemaphore.destroy();
    for (auto &f : mFences) {
        f.destroy();
    }
    mFences.clear();

    if (mVkSwapChain) {
        mVkSwapChain->destroy();
        GX_DELETE(mVkSwapChain);
    }

    if (mVkSurface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(getGVkContext(mContextT)->vkInstance(), mVkSurface, nullptr);
    }

    mVkSwapChain = nullptr;
    mContextT = nullptr;
}

bool FrameVk::reset(uint32_t width, uint32_t height, bool vSync, bool enforce)
{
    if (mRenderTargetType == FrameTargetType::RenderTarget) {
        // 自定义RenderTarget的Frame，不支持reset，直接返回true
        return true;
    }

    bool resize = width != this->mWidth || height != this->mHeight;
    if (!enforce && !resize && vSync == this->mVSync) {
        return false;
    }
    mWidth = width;
    mHeight = height;
    mVSync = vSync;

    mContextT->waitIdle();

    initSwapChain(nullptr);

    if (mRenderTargetType == FrameTargetType::SwapChain) {
        initRenderTarget(true);
    }

    return true;
}

bool FrameVk::beginFrame()
{
    updateFrameState();

    GVkFence *fence = nullptr;

    if (mRenderTargetType == FrameTargetType::SwapChain) {
        VkResult result = mVkSwapChain->acquireNextImage(&mCurrentFrameIndex);
        if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
            Log("FrameVk::acquireNextImage result = %s", vks::tools::errorString(result).c_str());
            return false;
        } else if (result != VK_SUCCESS) {
            VK_CHECK_RESULT(result);
            return false;
        }
    } else if (mRenderTargetType == FrameTargetType::RenderTarget) {
        if (mRenderTarget == nullptr) {
            return false;
        }
        uint32_t nextFrameIndex = mCurrentFrameIndex + 1;
        mCurrentFrameIndex = nextFrameIndex % mRenderTarget->frameBufferCount();
    }

    if (!mFences.empty()) {
        fence = &mFences[mCurrentFrameIndex];
    }

    if (fence) {
        if (fence->wait() != VK_SUCCESS) {
            return false;
        }
        fence->reset();
    }

    return true;
}

void FrameVk::submit(CommandBuffer commandBuffer)
{
    GVkContext *gVkContext = getGVkContext(mContextT);
    GVkFence *fence = nullptr;

    auto *cmdBufferP = dynamic_cast<CommandBufferVk *>(commandBuffer);

    if (!cmdBufferP->isCompiled()) {
        cmdBufferP->compile(this);
    }

    if (!mFences.empty()) {
        fence = &mFences[mCurrentFrameIndex];
    }

    VkCommandBuffer vkCmdBuffer = cmdBufferP->getVkCommandBuffer(mCurrentFrameIndex);

    if (mVkSwapChain && mVkSwapChain->getImageAvailableSemaphore() != VK_NULL_HANDLE) {
        gVkContext->graphicsQueue()
                ->submit({{mVkSwapChain->getImageAvailableSemaphore(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}},
                         {vkCmdBuffer},
                         {mRenderSemaphore.vkSemaphore()},
                         fence ? fence->vkFence() : VK_NULL_HANDLE);
    } else {
        gVkContext->graphicsQueue()
                ->submit({},
                         {vkCmdBuffer},
                         {},
                         fence ? fence->vkFence() : VK_NULL_HANDLE);
    }
}

void FrameVk::endFrame(bool waitQueue)
{
    GVkContext *gVkContext = getGVkContext(mContextT);

    if (mRenderTargetType == FrameTargetType::SwapChain) {
        VkResult result = mVkSwapChain->queuePresent(gVkContext->graphicsQueue(), mCurrentFrameIndex, mRenderSemaphore);
        if (!((result == VK_SUCCESS) || (result == VK_SUBOPTIMAL_KHR))) {
            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                Log("FrameVk::endFrame result = %s", vks::tools::errorString(result).c_str());
                if (mSwapChainErrorCb) {
                    mSwapChainErrorCb();
                }
            } else {
                VK_CHECK_RESULT(result);
            }
        }
    }

    (waitQueue && gVkContext->graphicsQueue()->waitIdle());
}

void FrameVk::waitGraphicsQueueIdle()
{
    mContextT->queueWaitIdle(QueueType::Graphics);
}

void FrameVk::setSwapChainErrorCallback(const FrameSwapChainErrorCallback &callback)
{
    mSwapChainErrorCb = callback;
}

uint32_t FrameVk::frameBufferCount()
{
    return mRenderTarget->frameBufferCount();
}

uint32_t FrameVk::currentFrameIndex()
{
    return mCurrentFrameIndex;
}

RenderTarget FrameVk::renderTarget()
{
    return mRenderTarget;
}

uint32_t FrameVk::width()
{
    return mWidth;
}

uint32_t FrameVk::height()
{
    return mHeight;
}

Format::Enum FrameVk::getSwapChainColorFormat()
{
    return mColorFormat;
}

Format::Enum FrameVk::getSwapChainDepthFormat()
{
    return mDepthFormat;
}

uint64_t FrameVk::getFrameTime()
{
    return mFrameState.frameTime;
}

Context_T *FrameVk::context()
{
    return mContextT;
}

bool FrameVk::initSwapChain(const SwapChainInfo *swapChainInfo)
{
    if (mRenderTargetType == FrameTargetType::SwapChain) {
        uint32_t width = this->width();
        uint32_t height = this->height();

        GVkContext *vkContext = getGVkContext(mContextT);
        if (mVkSurface == VK_NULL_HANDLE) {
            GX_ASSERT(swapChainInfo != nullptr);
            GVkPlatformData vkPlatformData{};
            vkPlatformData.nativeDisplayType = swapChainInfo->nativeDisplayType;
            vkPlatformData.nativeWindowHandle = swapChainInfo->nativeWindowHandle;
            vkPlatformData.context = swapChainInfo->context;
            vkPlatformData.backBuffer = swapChainInfo->backBuffer;
            vkPlatformData.backBufferDS = swapChainInfo->backBufferDS;
            mVkSurface = vkContext->gvkInstance()->createSurfaceForWindow(vkPlatformData);
        }

        if (!mVkSwapChain) {
            auto *swapChain = GX_NEW(GVkSurfaceSwapChain);
            swapChain->create(vkContext->gvkDevice(),
                              mVkSurface,
                              width, height, mVSync);
            mVkSwapChain = swapChain;

            if (mVkSwapChain->getImageAvailableSemaphore() != VK_NULL_HANDLE) {
                mRenderSemaphore.create(vkContext->vkDevice());
            }
            initFence(mVkSwapChain->bufferCount());
        } else {
            auto *swapChain = dynamic_cast<GVkSurfaceSwapChain *>(mVkSwapChain);
            swapChain->create(vkContext->gvkDevice(),
                              mVkSurface,
                              width, height, mVSync);
        }

        if (!mColorTextures.empty()) {
            for (auto *t : mColorTextures) {
                mContextT->destroyTexture(t);
            }
        }
        uint32_t bufferCount = mVkSwapChain->bufferCount();
        mColorTextures.resize(bufferCount);

        if (mDepthTexture != GFX_NULL_HANDLE) {
            mContextT->destroyTexture(mDepthTexture);
        }

        Format::Enum colorFormat = fromVkFormat(mVkSwapChain->colorFormat());

        for (uint32_t i = 0; i < bufferCount; i++) {
            GVkImage *image = mVkSwapChain->getSwapChainBuffer(i)->gvkImage;

            mColorTextures[i] = dynamic_cast<TextureVk *>(
                    dynamic_cast<ContextVk *>(mContextT->contextP())->createTextureP(
                            {
                                    TextureType::Texture2D,
                                    colorFormat,
                                    TextureUsage::Attachment,
                                    TextureAspect::AspectColor,
                                    image->width(), image->height(),
                                    image->depth(), image->mipLevels(), image->arrayLayers()
                            },
                            SampleCountFlag::SampleCount_1, image));
        }

        mDepthTexture = dynamic_cast<TextureVk *>(
                mContextT->contextP()->createTextureP(
                        {
                                TextureType::Texture2D,
                                mContextT->getSupportedDepthFormat(),
                                TextureUsage::Attachment,
                                TextureAspect::AspectDepth | TextureAspect::AspectStencil,
                                width, height, 1, 1, 1
                        },
                        SampleCountFlag::SampleCount_1));
    }

    return true;
}

bool FrameVk::initRenderTarget(bool isSwapChain)
{
    if (mRenderTarget != GFX_NULL_HANDLE) {
        mContextT->destroyRenderTarget(mRenderTarget);
    }

    CreateRenderTargetInfo createRTInfo{};
    createRTInfo.colorAttachments.resize(mColorTextures.size());
    for (uint32_t i = 0; i < mColorTextures.size(); i++) {
        createRTInfo.colorAttachments[i].resize(1);
        createRTInfo.colorAttachments[i][0].texture = mColorTextures[i];
        createRTInfo.colorAttachments[i][0].mipLevel = 0;
        createRTInfo.colorAttachments[i][0].layer = 0;
    }

    createRTInfo.depthStencilAttachment.texture = mDepthTexture;
    createRTInfo.depthStencilAttachment.mipLevel = 0;
    createRTInfo.depthStencilAttachment.layer = 0;

    createRTInfo.sample = SampleCountFlag::SampleCount_1;

    auto *rt = dynamic_cast<ContextVk *>(mContextT->contextP())
            ->createRenderTargetP(createRTInfo, isSwapChain);
    mRenderTarget = dynamic_cast<RenderTargetVk *>(rt);
    return mRenderTarget != GFX_NULL_HANDLE;
}

bool FrameVk::initFence(uint32_t bufferCount)
{
    mFences.resize(bufferCount);
    for (auto &f : mFences) {
        // beginFrame中先wait再reset，所以一开始要设置为signaled状态
        f.create(getGVkContext(mContextT)->gvkDevice(), VK_FENCE_CREATE_SIGNALED_BIT);
        if (f.vkFence() == VK_NULL_HANDLE) {
            Log("FrameVk::initCommandBuffer create fence failure");
            return false;
        }
    }
    return true;
}

void FrameVk::updateFrameState()
{
    uint64_t timeDiff = GTime::currentSteadyTime().microSecsTo(mFrameState.time);
    mFrameState.frameTime = timeDiff;

    mFrameState.time.resetToSteadyClock();
}

/// ============ RenderTargetVk ============ ///
bool RenderTargetVk::init(Context_T *context, const CreateRenderTargetInfo &createInfo, bool isSwapChain)
{
    mContextT = context;

    GX_ASSERT(createInfo.sample <= mContextT->maxRenderTargetSampleCount());

    uint32_t attachSize = 0;
    if (!createInfo.colorAttachments.empty()) {
        attachSize += (mColorCount = createInfo.colorAttachments[0].size());

        mAttachColors.resize(createInfo.colorAttachments.size());
        for (uint32_t i = 0; i < createInfo.colorAttachments.size(); i++) {
            auto &attach = createInfo.colorAttachments[i];
            GX_ASSERT(attach.size() == attachSize);
            mAttachColors[i].resize(attach.size());
            for (uint32_t j = 0; j < attach.size(); j++) {
                mAttachColors[i][j] = attach[j];
            }
        }
    }
    if (createInfo.depthStencilAttachment.texture != nullptr) {
        attachSize += 1;
        mHasDepth = true;
        mAttachDepth = createInfo.depthStencilAttachment;
    }

    GX_ASSERT(attachSize > 0);

    initRenderPassBase(createInfo, isSwapChain);

    uint32_t fbCount = createInfo.colorAttachments.empty() ? 1 : createInfo.colorAttachments.size();
    mFBViews.resize(fbCount);

    SampleCountFlag::Enum sample = createInfo.sample == 0 ? SampleCountFlag::SampleCount_1 : createInfo.sample;
    if (sample != SampleCountFlag::SampleCount_1) {
        // create MSAA render target
        return initMSAARenderTarget(createInfo);
    }

    for (auto &t : mFBViews) {
        t.resize(attachSize);
    }

    if (createInfo.depthStencilAttachment.texture != nullptr) {
        auto &attachmentInfo = createInfo.depthStencilAttachment;
        auto *depthTexP = dynamic_cast<TextureVk *>(attachmentInfo.texture);
        TextureBindRange range{
                depthTexP->aspect(),
                attachmentInfo.mipLevel, 1,
                attachmentInfo.layer, 1
        };
        VkImageView rtImageView = depthTexP->createImageView(range, true);
        mAttachDepthImage = depthTexP->vkImage();

        for (auto &t : mFBViews) {
            t[t.size() - 1] = rtImageView;
        }
    }

    uint32_t colorFBCount = createInfo.colorAttachments.size();

    mFrameCount = colorFBCount == 0 ? 1 : colorFBCount;
    mAttachColorImages.resize(colorFBCount);

    for (uint32_t i = 0; i < mFrameCount; i++) {
        if (!createInfo.colorAttachments.empty() && !createInfo.colorAttachments[0].empty()) {
            for (uint32_t j = 0; j < createInfo.colorAttachments[i].size(); j++) {
                auto &attachmentInfo = createInfo.colorAttachments[i][j];
                auto *colorTexP = dynamic_cast<TextureVk *>(attachmentInfo.texture);
                TextureBindRange range{
                        colorTexP->aspect(),
                        attachmentInfo.mipLevel, 1,
                        attachmentInfo.layer, 1
                };
                VkImageView rtImageView = colorTexP->createImageView(range, true);
                mAttachColorImages[i].push_back(colorTexP->vkImage());

                mFBViews[i][j] = rtImageView;

                // 使用选定的mipLevel作为帧缓冲区尺寸
                uint32_t w = valueForLevel(attachmentInfo.mipLevel, colorTexP->width());
                uint32_t h = valueForLevel(attachmentInfo.mipLevel, colorTexP->height());

                if (j == 0) {
                    mWidth = w;
                    mHeight = h;
                } else {
                    GX_ASSERT(mWidth == w);
                    GX_ASSERT(mHeight == h);
                }
            }
        } else {
            auto &attachmentInfo = createInfo.depthStencilAttachment;

            mWidth = valueForLevel(attachmentInfo.mipLevel, createInfo.depthStencilAttachment.texture->width());
            mHeight = valueForLevel(attachmentInfo.mipLevel, createInfo.depthStencilAttachment.texture->height());
        }

        GX_ASSERT(mWidth > 0);
        GX_ASSERT(mHeight > 0);
    }
    return true;
}

void RenderTargetVk::destroy()
{
    if (mMsaaColorTexture != nullptr) {
        mContextT->destroyTexture(mMsaaColorTexture);
    }
    if (mMsaaDepthTexture != nullptr) {
        mContextT->destroyTexture(mMsaaDepthTexture);
    }

    for (auto &[k, v] : mFBCache) {
        for (auto *fb : v) {
            fb->destroy();
            GX_DELETE(fb);
        }
    }
    mFBCache.clear();
    mFBViews.clear();

    mContextT = GFX_NULL_HANDLE;
}

uint32_t RenderTargetVk::width()
{
    return mWidth;
}

uint32_t RenderTargetVk::height()
{
    return mHeight;
}

uint8_t RenderTargetVk::frameBufferCount()
{
    return mFrameCount;
}

uint8_t RenderTargetVk::colorAttachmentCount()
{
    return mColorCount;
}

bool RenderTargetVk::hasDepthStencil()
{
    return mHasDepth;
}

Attachment RenderTargetVk::getColorAttachment(uint8_t frameIndex, uint8_t attachIndex)
{
    GX_ASSERT(frameIndex < mFrameCount);
    GX_ASSERT(attachIndex < mColorCount);
    return mAttachColors[frameIndex][attachIndex];
}

Attachment RenderTargetVk::getDepthStencilAttachment()
{
    GX_ASSERT(mHasDepth);
    return mAttachDepth;
}

Context_T *RenderTargetVk::context()
{
    return mContextT;
}

GVkFrameBuffer *RenderTargetVk::getVkFrameBuffer(RenderPassVk *rp, uint32_t index)
{
    GfxIdxTy idx = rp->idx();

    auto it = mFBCache.find(idx);
    if (it == mFBCache.end()) {
        auto fbs = createFrameBuffer(rp);
        mFBCache.emplace(idx, fbs);
        it = mFBCache.find(idx);
    }

    auto &fbs = it->second;

    index = index % fbs.size();
    return fbs[index];
}

constexpr inline RenderTargetAttachmentFlag::Enum getRenderTargetAttachmentFlagsAt(uint32_t index) noexcept
{
    if (index == 0u) return RenderTargetAttachmentFlag::Color0;
    if (index == 1u) return RenderTargetAttachmentFlag::Color1;
    if (index == 2u) return RenderTargetAttachmentFlag::Color2;
    if (index == 3u) return RenderTargetAttachmentFlag::Color3;
    if (index == 4u) return RenderTargetAttachmentFlag::Color4;
    if (index == 5u) return RenderTargetAttachmentFlag::Color5;
    if (index == 6u) return RenderTargetAttachmentFlag::Color6;
    if (index == 7u) return RenderTargetAttachmentFlag::Color7;
    if (index == 8u) return RenderTargetAttachmentFlag::Depth;
    return RenderTargetAttachmentFlag::None;
}

RenderPassVk *RenderTargetVk::getRenderPass(const RenderPassInfo &rpInfo) const
{
    auto info = mGetRenderPassBase;
    const auto &subPassInfo = rpInfo.subPassInfo;

    for (uint32_t i = 0; i < info.colorAttachmentDescs.size(); i++) {
        if (i < RenderTargetAttachmentFlag::ColorCount) {
            RenderTargetAttachmentFlag::Enum target = getRenderTargetAttachmentFlagsAt(i);

            info.colorAttachmentDescs[i].clear = rpInfo.clear & target;
            info.colorAttachmentDescs[i].discard = rpInfo.discard & target;
        }
    }

    if (info.depthAttachmentDesc.t != 0) {
        RenderTargetAttachmentFlag::Enum target
                = getRenderTargetAttachmentFlagsAt(RenderTargetAttachmentFlag::ColorCount);
        info.depthAttachmentDesc.clear = rpInfo.clear & target;
        info.depthAttachmentDesc.discard = rpInfo.discard & target;
    }

    if (subPassInfo.subPassDescs.empty() || subPassInfo.subPassDepends.empty()) {
        info.subPassDescs.resize(1);
        if (!mAttachColors.empty() && !mAttachColors[0].empty()) {
            auto &colorAttach = mAttachColors[0];
            info.subPassDescs[0].colorAttachments.resize(colorAttach.size());
            for (uint32_t i = 0; i < colorAttach.size(); i++) {
                info.subPassDescs[0].colorAttachments[i] = i;
            }
        }
        info.subPassDescs[0].useDepthStencil = mAttachDepth.texture != nullptr;

        info.subPassDepends.push_back({SUBPASS_EXTERNAL, 0});
        info.subPassDepends.push_back({0, SUBPASS_EXTERNAL});
    } else {
        info.subPassDescs = subPassInfo.subPassDescs;
        info.subPassDepends = subPassInfo.subPassDepends;
    }

    RenderPassVk *rp = dynamic_cast<ContextVk *>(mContextT->contextP())->getRenderPass(info);
    GX_ASSERT(rp != GFX_NULL_HANDLE);

    return rp;
}

uint16_t RenderTargetVk::colorAttachmentCount() const
{
    return mColorCount;
}

bool RenderTargetVk::hasDepthStencilAttachment() const
{
    return mHasDepth;
}

GVkImage *RenderTargetVk::getAttachmentImage(uint32_t frameIndex, uint32_t attachIndex) const
{
    if (attachIndex < mColorCount) {
        frameIndex = frameIndex % mFrameCount;
        return mAttachColorImages[frameIndex][attachIndex];
    } else if (attachIndex == mColorCount && mHasDepth) {
        return mAttachDepthImage;
    }
    return nullptr;
}

bool RenderTargetVk::initMSAARenderTarget(const CreateRenderTargetInfo &createInfo)
{
    if (createInfo.colorAttachments.empty()) {
        Log("MSAA Render Target must contain color attachment");
        return false;
    }
    if (createInfo.colorAttachments.at(0).size() != 1) {
        Log("MSAA Render Target only one color attachment is supported");
        return false;
    }
    const auto &aAttach = createInfo.colorAttachments.at(0).at(0);
    Texture aColor = aAttach.texture;

    uint32_t attachSize = 2 + (createInfo.depthStencilAttachment.texture != nullptr ? 2 : 0);

    for (auto &t : mFBViews) {
        t.resize(attachSize);
    }

    SampleCountFlag::Enum sample = createInfo.sample == 0 ? SampleCountFlag::SampleCount_1 : createInfo.sample;

    {
        mMsaaColorTexture = mContextT->contextP()->createTextureP(
                {
                        aColor->type(),
                        aColor->format(),
                        aColor->usage(),
                        aColor->aspect(),
                        aColor->width(),
                        aColor->height(),
                        aColor->depth(),
                        aColor->mipLevels(),
                        aColor->layerCount()
                }, sample);

        auto *msaaColorTextureP = dynamic_cast<TextureVk *>(mMsaaColorTexture);
        TextureBindRange range {
                VK_IMAGE_ASPECT_COLOR_BIT,
                aAttach.mipLevel, 1,
                aAttach.layer, 1
        };
        VkImageView msaaColorImageView = msaaColorTextureP->createImageView(range, true);

        for (auto &t : mFBViews) {
            t[0] = msaaColorImageView;
        }
    }

    if (createInfo.depthStencilAttachment.texture != nullptr) {
        auto &attachmentInfo = createInfo.depthStencilAttachment;
        auto *depthTexP = dynamic_cast<TextureVk *>(attachmentInfo.texture);

        // msaa depth
        mMsaaDepthTexture = mContextT->contextP()->createTextureP(
                {
                        depthTexP->type(),
                        depthTexP->format(),
                        depthTexP->usage(),
                        depthTexP->aspect(),
                        depthTexP->width(),
                        depthTexP->height(),
                        depthTexP->depth(),
                        depthTexP->mipLevels(),
                        depthTexP->layerCount()
                }, sample);
        auto *msaaDepthTextureP = dynamic_cast<TextureVk *>(mMsaaDepthTexture);
        TextureBindRange range{
                msaaDepthTextureP->aspect(),
                aAttach.mipLevel, 1,
                aAttach.layer, 1
        };
        VkImageView msaaDepthImageView = msaaDepthTextureP->createImageView(range, true);

        for (auto &t : mFBViews) {
            t[t.size() - 2] = msaaDepthImageView;
        }

        // depth
        TextureBindRange rangeDepth{
                depthTexP->aspect(),
                attachmentInfo.mipLevel, 1,
                attachmentInfo.layer, 1
        };
        VkImageView rtImageView = depthTexP->createImageView(rangeDepth, true);
        mAttachDepthImage = depthTexP->vkImage();

        for (auto &t : mFBViews) {
            t[t.size() - 1] = rtImageView;
        }
    }

    uint32_t colorFBCount = createInfo.colorAttachments.size();

    mFrameCount = colorFBCount == 0 ? 1 : colorFBCount;
    mAttachColorImages.resize(colorFBCount);

    for (uint32_t i = 0; i < mFrameCount; i++) {
        auto &attachmentInfo = createInfo.colorAttachments[i][0];
        auto *colorTexP = dynamic_cast<TextureVk *>(attachmentInfo.texture);
        TextureBindRange range{
                colorTexP->aspect(),
                attachmentInfo.mipLevel, 1,
                attachmentInfo.layer, 1
        };
        VkImageView rtImageView = colorTexP->createImageView(range, true);
        mAttachColorImages[i].push_back(colorTexP->vkImage());

        mFBViews[i][1] = rtImageView;

        // 使用选定的mipLevel作为帧缓冲区尺寸
        mWidth = valueForLevel(attachmentInfo.mipLevel, colorTexP->width());
        mHeight = valueForLevel(attachmentInfo.mipLevel, colorTexP->height());
    }
    return true;
}

void RenderTargetVk::initRenderPassBase(const CreateRenderTargetInfo &createInfo, bool isSwapChain)
{
    auto &info = mGetRenderPassBase;
    info.isSwapChain = isSwapChain;
    if (!createInfo.colorAttachments.empty() && !createInfo.colorAttachments[0].empty()) {
        info.colorAttachmentDescs.resize(createInfo.colorAttachments[0].size());
        for (uint32_t i = 0; i < createInfo.colorAttachments[0].size(); i++) {
            auto &ca = createInfo.colorAttachments[0][i];
            info.colorAttachmentDescs[i].format = ca.texture->format();
            if (isSwapChain) {
                info.colorAttachmentDescs[i].initLayout = ImageLayout::ColorAttachment;
                info.colorAttachmentDescs[i].finalLayout = ImageLayout::ColorAttachment;
                info.colorAttachmentDescs[i].subPassLayout = ImageLayout::ColorAttachment;
            } else {
                ImageLayout::Enum layout =
                        TextureVk::getUsageImageLayout(ca.texture->usage(), ca.texture->aspect());
                info.colorAttachmentDescs[i].initLayout = layout;
                info.colorAttachmentDescs[i].finalLayout = layout;
                info.colorAttachmentDescs[i].subPassLayout = layout;
            }
        }
    }
    if (createInfo.depthStencilAttachment.texture != nullptr) {
        info.depthAttachmentDesc.format = createInfo.depthStencilAttachment.texture->format();
        if (isSwapChain) {
            info.depthAttachmentDesc.initLayout = ImageLayout::DepthStencilAttachment;
            info.depthAttachmentDesc.finalLayout = ImageLayout::DepthStencilAttachment;
            info.depthAttachmentDesc.subPassLayout = ImageLayout::DepthStencilAttachment;
        } else {
            Texture tex = createInfo.depthStencilAttachment.texture;
            ImageLayout::Enum layout =
                    TextureVk::getUsageImageLayout(tex->usage(), tex->aspect());
            info.depthAttachmentDesc.initLayout = layout;
            info.depthAttachmentDesc.finalLayout = layout;
            info.depthAttachmentDesc.subPassLayout = layout;
        }
    }
    info.sample = createInfo.sample;
}

std::vector<GVkFrameBuffer *> RenderTargetVk::createFrameBuffer(RenderPassVk *rp)
{
    std::vector<GVkFrameBuffer *> fbs;
    fbs.resize(mFBViews.size());

    for (uint32_t i = 0; i < fbs.size(); i++) {
        fbs[i] = GX_NEW(GVkFrameBuffer);
        fbs[i]->create(getGVkContext(mContextT)->gvkDevice(),
                                  rp->vkRenderPass(), mFBViews.at(i),
                                  mWidth, mHeight, 1);
        GX_ASSERT(fbs[i]->vkFramebuffer() != VK_NULL_HANDLE);
    }

    return fbs;
}

uint32_t RenderTargetVk::valueForLevel(uint8_t level, uint32_t baseLevelValue)
{
    return std::max((uint32_t) 1, baseLevelValue >> level);
}

/// ============ BufferVk ============ ///

bool BufferVk::init(Context_T *context, const CreateBufferInfo &createInfo)
{
    mContextT = context;

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    return initByVMA(createInfo);
#else
    return initByGVkBuffer(createInfo);
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR
}

void BufferVk::destroy()
{
    unmap();

    auto contextVk = getContextVk();
    auto *vkContext = contextVk->vkContext();

    for (auto &[k, p] : mVkBufferViews) {
        vkDestroyBufferView(vkContext->vkDevice(), p, nullptr);
    }

    mVkBufferViews.clear();

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    vmaDestroyBuffer(getContextVk()->getVmaAllocator(),
                     mBuffer, mAllocation);
#else
    mGVkBuffer.destroy();
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR

    mContextT = GFX_NULL_HANDLE;
}

Context_T *BufferVk::context()
{
    return mContextT;
}

uint64_t BufferVk::size()
{
    return mSize;
}

void *BufferVk::map()
{
#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    if (mBuffer == VK_NULL_HANDLE) {
        return nullptr;
    }
    if (mMapped != nullptr) {
        return mMapped;
    }
    auto *contextVk = getContextVk();

    VkMemoryPropertyFlags memFlags;
    vmaGetMemoryTypeProperties(contextVk->getVmaAllocator(), mAllocInfo.memoryType, &memFlags);

    if ((memFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0) {
        vmaMapMemory(contextVk->getVmaAllocator(), mAllocation, &mMapped);
        return mMapped;
    } else {
        return nullptr;
    }
#else
    if (!mGVkBuffer.isCreated()) {
        return nullptr;
    }

    return mGVkBuffer.map(VK_WHOLE_SIZE, 0);
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR
}

void BufferVk::flush()
{
#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    if (mBuffer == VK_NULL_HANDLE || mAllocation == nullptr) {
        return;
    }
    vmaFlushAllocation(getContextVk()->getVmaAllocator(), mAllocation, 0, VK_WHOLE_SIZE);
#else
    if (!mGVkBuffer.isCreated()) {
        return;
    }
    if (!mGVkBuffer.isMapped()) {
        Log("BufferVk::flush not mapped");
        return;
    }
    if (mMemoryUsage != BufferMemoryUsage::GpuOnly) {
        mGVkBuffer.flush(VK_WHOLE_SIZE, 0);
    }
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR
}

void BufferVk::unmap()
{
#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    if (mBuffer == VK_NULL_HANDLE || mAllocation == nullptr) {
        return;
    }
    if (mMapped == nullptr) {
        return;
    }
    vmaUnmapMemory(getContextVk()->getVmaAllocator(), mAllocation);
    mMapped = nullptr;
#else
    if (!mGVkBuffer.isCreated()) {
        return;
    }
    if (!mGVkBuffer.isMapped()) {
        return;
    }
    mGVkBuffer.unmap();
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR
}

VkBuffer BufferVk::vkBuffer()
{
#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    return mBuffer;
#else
    return mGVkBuffer.vkBuffer();
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR
}

VkDescriptorBufferInfo BufferVk::getVkDescriptorBufferInfo(uint64_t offset, uint64_t range)
{
    VkDescriptorBufferInfo info {};
#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    info.buffer = mBuffer;
#else
    info.buffer = mGVkBuffer.vkBuffer();
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR
    info.offset = offset;
    info.range = range;
    return info;
}

VkBufferView BufferVk::getVkBufferView(Format::Enum format, uint64_t offset, uint64_t size)
{
    CreateBufferViewInfo key{
        format, offset, size
    };

    VkBufferView vkBufferView;

    auto it = mVkBufferViews.find(key);
    if (it == mVkBufferViews.end()) {
        VkBufferViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;
        createInfo.buffer = vkBuffer();
        createInfo.format = toVkFormat(format);
        createInfo.offset = offset;
        createInfo.range = size;

        vkCreateBufferView(getGVkContext(mContextT)->vkDevice(), &createInfo, nullptr, &vkBufferView);

        mVkBufferViews.insert({key, vkBufferView});
    } else {
        vkBufferView = it->second;
    }

    return vkBufferView;
}

ContextVk *BufferVk::getContextVk()
{
    return dynamic_cast<ContextVk *>(mContextT->contextP());
}

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR

bool BufferVk::initByVMA(const CreateBufferInfo &createInfo)
{
    auto *contextVk = dynamic_cast<ContextVk *>(mContextT->contextP());

    mType = createInfo.type;
    mMemoryUsage = createInfo.memoryUsage;
    mSize = createInfo.size;

    VkBufferUsageFlags vkBufferUsage = 0;
    VmaMemoryUsage memoryUsage = VmaMemoryUsage::VMA_MEMORY_USAGE_UNKNOWN;

    if ((mType & BufferType::Staging) == BufferType::Staging) {
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if ((mType & BufferType::Uniform) == BufferType::Uniform) {
        if ((mType & BufferType::Texel) == BufferType::Texel) {
            vkBufferUsage |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
        } else {
            vkBufferUsage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if ((mType & BufferType::Storage) == BufferType::Storage) {
        if ((mType & BufferType::Texel) == BufferType::Texel) {
            vkBufferUsage |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
        } else {
            vkBufferUsage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        }
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if ((mType & BufferType::Index) == BufferType::Index) {
        vkBufferUsage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if ((mType & BufferType::Vertex) == BufferType::Vertex) {
        vkBufferUsage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if ((mType & BufferType::Indirect) == BufferType::Indirect) {
        vkBufferUsage |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }

    switch (createInfo.memoryUsage) {
        default:
        case BufferMemoryUsage::GpuOnly:
            memoryUsage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;
            break;
        case BufferMemoryUsage::CpuOnly:
            memoryUsage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_ONLY;
            break;
        case BufferMemoryUsage::CpuToGpu:
            memoryUsage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;
            break;
        case BufferMemoryUsage::GpuToCpu:
            memoryUsage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU;
            break;
        case BufferMemoryUsage::Count:
            GX_ASSERT_S(createInfo.memoryUsage < BufferMemoryUsage::Count, "Buffer memory usage invalid");
            break;
    }

    VkBufferCreateInfo bufferInfo = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    bufferInfo.size = mSize;
    bufferInfo.usage = vkBufferUsage;

    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = memoryUsage;

    return vmaCreateBuffer(contextVk->getVmaAllocator(), &bufferInfo, &allocCreateInfo,
                           &mBuffer, &mAllocation, &mAllocInfo) == VK_SUCCESS;
}

#else

bool BufferVk::initByGVkBuffer(const CreateBufferInfo &createInfo)
{
    auto *contextVk = dynamic_cast<ContextVk *>(mContextT->contextP());

    mType = createInfo.type;
    mMemoryUsage = createInfo.memoryUsage;
    mSize = createInfo.size;

    VkBufferUsageFlags vkBufferUsage = 0;
    VkMemoryPropertyFlags vkMemoryProperty = 0;

    if ((mType & BufferType::Staging) == BufferType::Staging) {
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if ((mType & BufferType::Uniform) == BufferType::Uniform) {
        if ((mType & BufferType::Texel) == BufferType::Texel) {
            vkBufferUsage |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
        } else {
            vkBufferUsage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if ((mType & BufferType::Storage) == BufferType::Storage) {
        if ((mType & BufferType::Texel) == BufferType::Texel) {
            vkBufferUsage |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
        } else {
            vkBufferUsage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        }
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if ((mType & BufferType::Index) == BufferType::Index) {
        vkBufferUsage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if ((mType & BufferType::Vertex) == BufferType::Vertex) {
        vkBufferUsage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }
    if ((mType & BufferType::Indirect) == BufferType::Indirect) {
        vkBufferUsage |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
        vkBufferUsage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }

    switch (createInfo.memoryUsage) {
        default:
        case BufferMemoryUsage::GpuOnly:
            vkMemoryProperty = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            break;
        case BufferMemoryUsage::CpuOnly:
            vkMemoryProperty = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            break;
        case BufferMemoryUsage::CpuToGpu:
            vkMemoryProperty = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            break;
        case BufferMemoryUsage::GpuToCpu:
            vkMemoryProperty = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
            break;
        case BufferMemoryUsage::Count:
            GX_ASSERT_S(createInfo.memoryUsage < BufferMemoryUsage::Count, "Buffer memory usage invalid");
            break;
    }

    mGVkBuffer.create(
            contextVk->vkContext()->gvkDevice(),
            vkBufferUsage,
            vkMemoryProperty,
            VK_SHARING_MODE_EXCLUSIVE,
            createInfo.size);

    return mGVkBuffer.isCreated();
}

#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR

/// ============ TextureVk ============ ///

bool TextureVk::init(Context_T *context, const CreateTextureInfo &createInfo,
                     SampleCountFlag::Enum sample, GVkImage *image)
{
    mContextT = context;

    std::hash<CreateTextureInfo> hashFunc;

    mHash = hashFunc(createInfo);
    mHash = hashOf(mHash, idx());

    return initVkTexture(createInfo, sample, image);
}

void TextureVk::destroy()
{
    auto vkDevice = getGVkContext(mContextT)->vkDevice();
    for (auto &[k, v] : mImageViewCache) {
        vkDestroyImageView(vkDevice, v, nullptr);
    }
    mImageViewCache.clear();
    if (!mIsFromImage) {
        mVkImage->destroy();
        GX_DELETE(mVkImage);
    }
    mContextT = GFX_NULL_HANDLE;
}

Context_T *TextureVk::context()
{
    return mContextT;
}

size_t TextureVk::hash()
{
    return mHash;
}

TextureType::Enum TextureVk::type()
{
    return mType;
}

Format::Enum TextureVk::format()
{
    return mFormat;
}

TextureUsageFlags TextureVk::usage()
{
    return mUsage;
}

TextureAspectFlags TextureVk::aspect()
{
    return mAspect;
}

uint32_t TextureVk::width()
{
    return mWidth;
}

uint32_t TextureVk::height()
{
    return mHeight;
}

uint32_t TextureVk::depth()
{
    return mDepth;
}

uint32_t TextureVk::layerCount()
{
    return mLayerCount;
}

uint32_t TextureVk::mipLevels()
{
    return mMipLevels;
}

uint64_t TextureVk::size()
{
    return mVkImage->size();
}

void TextureVk::setData(const void *data, uint64_t size, Fence fence)
{
    if (!data) {
        return;
    }
    Buffer stagingBuffer = mContextT->createBuffer(
            {BufferType::Staging, BufferMemoryUsage::CpuOnly, size});

    CommandBuffer cmdBuffer = mContextT->createCommandBuffer({QueueType::Graphics, 1});

    memcpy(stagingBuffer->map(), data, size);
    stagingBuffer->flush();
    stagingBuffer->unmap();

    ImageLayout::Enum dstLayout = getImageLayoutFromUsage(mUsage, mAspect, false);

    BufferImageCopyInfo copyInfo{};
    copyInfo.imageWidth = mWidth;
    copyInfo.imageHeight = mHeight;
    copyInfo.imageDepth = mDepth;
    copyInfo.imageOffsetX = 0;
    copyInfo.imageOffsetY = 0;
    copyInfo.layerCount = 1;
    copyInfo.mipLevel = 0;

    cmdBuffer->begin()
            ->imageMemoryBarrier(this, ImageLayout::Undefined, ImageLayout::TransferDst, {0, 0, 1, 1})
            ->copyBufferToImage(stagingBuffer, this, {copyInfo})
            ->imageMemoryBarrier(this, ImageLayout::TransferDst, dstLayout, {0, 0, 1, 1})
            ->end();

    if (fence == GFX_NULL_HANDLE) {
        mContextT->submitCommandBlock(cmdBuffer, 0);
    } else {
        mContextT->submitCommand(cmdBuffer, 0, fence);
    }

    mContextT->destroyBuffer(stagingBuffer);
    mContextT->destroyCommandBuffer(cmdBuffer);
}

void TextureVk::genMipmap(Fence fence)
{
    if (mMipLevels <= 1) {
        return;
    }

    CommandBuffer cmdBuffer = mContextT->createCommandBuffer({QueueType::Graphics, 1});
    cmdBuffer->begin();

    cmdBuffer->imageMemoryBarrier(this, ImageLayout::Undefined, ImageLayout::TransferSrc, {0, 0, 1, 1});

    uint32_t infoIndex = 0;
    for (uint32_t i = 0; i < mLayerCount; i++) {
        int32_t mipWidth = (int32_t)mWidth;
        int32_t mipHeight = (int32_t)mHeight;
        int32_t mipDepth= (int32_t)mDepth;
        for (uint32_t x = 1; x < mMipLevels; x++) {
            ImageBlitInfo blitInfo {};
            blitInfo.srcWidth = mipWidth;
            blitInfo.srcHeight = mipHeight;
            blitInfo.srcDepth = mipDepth;
            blitInfo.srcMipLevel = x - 1;
            blitInfo.srcLayerCount = 1;
            blitInfo.srcBaseArrayLayer = i;
            blitInfo.dstWidth = mipWidth > 1 ? mipWidth / 2 : 1;
            blitInfo.dstHeight = mipHeight > 1 ? mipHeight / 2 : 1;
            blitInfo.dstDepth = mipDepth > 1 ? mipDepth / 2 : 1;
            blitInfo.dstMipLevel = x;
            blitInfo.dstLayerCount = 1;
            blitInfo.dstBaseArrayLayer = i;

            // 需要严格控制Barrier的粒度
            cmdBuffer
                    // undefined -> dst
                    ->imageMemoryBarrier(this, ImageLayout::Undefined, ImageLayout::TransferDst, {x, i, 1, 1})
                    ->blitImage(this, this, {blitInfo}, BlitFilter::Linear)
                    // dst -> src
                    ->imageMemoryBarrier(this, ImageLayout::TransferDst, ImageLayout::TransferSrc, {x, i, 1, 1});

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
            if (mipDepth > 1) mipDepth /= 2;

            infoIndex++;
        }
    }

    ImageLayout::Enum dstLayout = getImageLayoutFromUsage(mUsage, mAspect, false);
    // dst -> layout
    cmdBuffer->imageMemoryBarrier(this, ImageLayout::TransferSrc, dstLayout, {0, 0});

    cmdBuffer->end();
    if (fence == GFX_NULL_HANDLE) {
        mContextT->submitCommandBlock(cmdBuffer, 0);
    } else {
        mContextT->submitCommand(cmdBuffer, 0, fence);
    }

    mContextT->destroyCommandBuffer(cmdBuffer);
}

GVkContext *TextureVk::vkContext()
{
    auto *context = dynamic_cast<ContextVk *>(mContextT->contextP());
    GX_ASSERT(context);
    return context->vkContext();
}

GVkImage *TextureVk::vkImage()
{
    return mVkImage;
}

VkImageView TextureVk::createImageView(const TextureBindRange &range, bool isAttachment)
{
    CreateImageViewInfo createInfo {
        range, isAttachment
    };

    // 处理默认值
    if (createInfo.range.aspect == TextureAspect::AspectNone) {
        if ((mAspect & TextureAspect::AspectColor) == TextureAspect::AspectColor) {
            createInfo.range.aspect = TextureAspect::AspectColor;
        } else if ((mAspect & TextureAspect::AspectDepth) == TextureAspect::AspectDepth) {
            createInfo.range.aspect = TextureAspect::AspectDepth;
        } else {
            createInfo.range.aspect = TextureAspect::AspectStencil;
        }
    }

    GX_ASSERT(createInfo.range.levelCount > 0);
    GX_ASSERT(createInfo.range.layerCount > 0);

    if (createInfo.range.levelCount == UINT32_MAX) {
        createInfo.range.levelCount = mMipLevels;
    }
    if (createInfo.range.layerCount == UINT32_MAX) {
        createInfo.range.layerCount = mLayerCount;
    }

    auto it = mImageViewCache.find(createInfo);
    if (it != mImageViewCache.end()) {
        return it->second;
    }

    VkImageViewType imageViewType;
    bool isCube = false;

    switch (mType) {
        case TextureType::Texture1D:
            imageViewType = VK_IMAGE_VIEW_TYPE_1D;
            break;
        case TextureType::Texture2D:
            imageViewType = VK_IMAGE_VIEW_TYPE_2D;
            break;
        case TextureType::Texture3D:
            imageViewType = VK_IMAGE_VIEW_TYPE_3D;
            break;
        case TextureType::Texture1DArray:
            imageViewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
            break;
        case TextureType::Texture2DArray:
            imageViewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            break;
        case TextureType::TextureCube:
            imageViewType = VK_IMAGE_VIEW_TYPE_CUBE;
            isCube = true;
            break;
        default:
            imageViewType = VK_IMAGE_VIEW_TYPE_2D;
            break;
    }

    VkComponentMapping swizzle = toVkComponentMapping(mSwizzleMapping);

    if (createInfo.range.aspect == TextureAspect::AspectColor) {
        if (swizzle.r == VK_COMPONENT_SWIZZLE_IDENTITY) {
            swizzle.r = VK_COMPONENT_SWIZZLE_R;
        }
        if (swizzle.g == VK_COMPONENT_SWIZZLE_IDENTITY) {
            swizzle.g = VK_COMPONENT_SWIZZLE_G;
        }
        if (swizzle.b == VK_COMPONENT_SWIZZLE_IDENTITY) {
            swizzle.b = VK_COMPONENT_SWIZZLE_B;
        }
        if (swizzle.a == VK_COMPONENT_SWIZZLE_IDENTITY) {
            swizzle.a = VK_COMPONENT_SWIZZLE_A;
        }
    }

    VkImageViewCreateInfo imageViewCreateInfo{};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    if (isCube) {
        imageViewCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    }
    imageViewCreateInfo.viewType = isAttachment ? VK_IMAGE_VIEW_TYPE_2D : imageViewType;
    imageViewCreateInfo.format = toVkFormat(mFormat);
    imageViewCreateInfo.subresourceRange.aspectMask = toVkAspectFlags(createInfo.range.aspect);
    imageViewCreateInfo.components = isAttachment ? VkComponentMapping{} : swizzle;
    imageViewCreateInfo.subresourceRange.baseMipLevel = createInfo.range.baseMipLevel;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = createInfo.range.baseArrayLayer;
    imageViewCreateInfo.subresourceRange.layerCount = isAttachment ? 1 : createInfo.range.layerCount;
    imageViewCreateInfo.subresourceRange.levelCount = isAttachment ? 1 : createInfo.range.levelCount;
    imageViewCreateInfo.image = mVkImage->vkImage();

    VkImageView imageView;
    VK_CHECK_RESULT(vkCreateImageView(
            getGVkContext(mContextT)->vkDevice(), &imageViewCreateInfo, nullptr, &imageView));
    if (imageView) {
        mImageViewCache.emplace(createInfo, imageView);
    }
    return imageView;
}

VkDescriptorImageInfo TextureVk::getDescriptor(VkImageView imageView, VkSampler sampler) const
{
    VkDescriptorImageInfo info{};
    info.imageLayout = toVkImageLayout(getUsageImageLayout(mUsage, mAspect));
    info.imageView = imageView;
    info.sampler = sampler;
    return info;
}

void TextureVk::imageMemoryBarrier(VkCommandBuffer cmdBuffer,
                                   ImageLayout::Enum srcLayout,
                                   ImageLayout::Enum dstLayout,
                                   const ImageSubResourceRange &subResRange)
{
    VkImageSubresourceRange vkSubResRange{};
    vkSubResRange.aspectMask = mVkImage->aspectMask();
    vkSubResRange.baseMipLevel = subResRange.baseMipLevel;
    vkSubResRange.levelCount = subResRange.levelCount == 0
                               ? (mMipLevels - subResRange.baseMipLevel) : subResRange.levelCount;
    vkSubResRange.baseArrayLayer = subResRange.baseArrayLayer;
    vkSubResRange.layerCount = subResRange.layerCount == 0
                               ? (mLayerCount - subResRange.baseArrayLayer) : subResRange.layerCount;

    mVkImage->imageMemoryBarrier(
            cmdBuffer,
            toVkImageLayout(srcLayout),
            toVkImageLayout(dstLayout),
            vkSubResRange,
            (srcLayout == ImageLayout::ComputeGeneral || dstLayout == ImageLayout::ComputeGeneral));
}

ImageLayout::Enum TextureVk::getUsageImageLayout(TextureUsageFlags usage, TextureAspectFlags aspect)
{
    if ((usage & TextureUsage::Attachment) == TextureUsage::Attachment) {
        if (((aspect & TextureAspect::AspectDepth) == TextureAspect::AspectDepth)
            || ((aspect & TextureAspect::AspectStencil) == TextureAspect::AspectStencil)) {
            return ImageLayout::DepthStencilAttachment;
        } else {
            return ImageLayout::ColorAttachment;
        }
    } else if ((usage & TextureUsage::Storage) == TextureUsage::Storage) {
        return ImageLayout::GraphicsGeneral;
    } else {
        return ImageLayout::ShaderReadOnly;
    }
}

bool TextureVk::initVkTexture(const CreateTextureInfo &createInfo,
                              SampleCountFlag::Enum sample,
                              GVkImage *image)
{
    if (createInfo.mipLevels < 1) {
        GX_ASSERT_S(false, "mipLevels must be >= 1");
        return false;
    }
    if (createInfo.arrayLayers < 1) {
        GX_ASSERT_S(false, "arrayLayers must be >= 1");
        return false;
    }

    bool isCube = createInfo.type == TextureType::TextureCube;

    VkImageType vkImageType = VK_IMAGE_TYPE_2D;
    VkImageUsageFlags vkImageUsage = 0;
    VkImageLayout vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    mWidth = createInfo.width;
    mHeight = createInfo.height;
    mDepth = createInfo.depth;
    mType = createInfo.type;
    mFormat = createInfo.format;
    mUsage = createInfo.usage;
    mAspect = createInfo.aspect;
    mMipLevels = createInfo.mipLevels;
    mLayerCount = createInfo.arrayLayers;
    mSwizzleMapping = createInfo.swizzle;

    if (image != nullptr) {
        mVkImage = image;
        mIsFromImage = true;
        return true;
    }

    mIsFromImage = false;

    // trans type
    switch (createInfo.type) {
        case TextureType::Texture1D:
        case TextureType::Texture1DArray:
            vkImageType = VK_IMAGE_TYPE_1D;
            break;
        case TextureType::Texture2D:
        case TextureType::Texture2DArray:
        case TextureType::TextureCube:
            vkImageType = VK_IMAGE_TYPE_2D;
            break;
        case TextureType::Texture3D:
            vkImageType = VK_IMAGE_TYPE_3D;
            break;
        default:
            GX_ASSERT_S(createInfo.type < TextureType::Count, "Texture type invalid");
            break;
    }

    // trans usage
    if ((createInfo.usage & TextureUsage::Sampled) == TextureUsage::Sampled) {
        vkImageUsage |= VK_IMAGE_USAGE_SAMPLED_BIT;
        vkImageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        vkImageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    if ((createInfo.usage & TextureUsage::Storage) == TextureUsage::Storage) {
        vkImageUsage |= VK_IMAGE_USAGE_STORAGE_BIT;
        vkImageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        vkImageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }
    if ((createInfo.usage & TextureUsage::Attachment) == TextureUsage::Attachment) {
        vkImageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        if (((createInfo.aspect & TextureAspect::AspectDepth) == TextureAspect::AspectDepth)
            || ((createInfo.aspect & TextureAspect::AspectStencil) == TextureAspect::AspectStencil)) {
            vkImageUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        } else {
            vkImageUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }
    }
    if (createInfo.usage & TextureUsage::InputAttachment) {
        vkImageUsage |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
    }

    // trans layout
    if ((createInfo.usage & TextureUsage::Attachment) == TextureUsage::Attachment) {
        if (((createInfo.aspect & TextureAspect::AspectDepth) == TextureAspect::AspectDepth)
            || ((createInfo.aspect & TextureAspect::AspectStencil) == TextureAspect::AspectStencil)) {
            vkImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        } else {
            vkImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
    } else if ((createInfo.usage & TextureUsage::Storage) == TextureUsage::Storage) {
        vkImageLayout = VK_IMAGE_LAYOUT_GENERAL;
    } else {
        vkImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }

    bool ok;
    if (createInfo.aspect == TextureAspect::AspectColor) {
        ok = createImage(createInfo.width, createInfo.height, createInfo.depth,
                         createInfo.mipLevels, createInfo.arrayLayers, vkImageType,
                         toVkFormat(createInfo.format), toVkSampleCount(sample),
                         VK_IMAGE_TILING_OPTIMAL, vkImageUsage, VK_IMAGE_ASPECT_COLOR_BIT,
                         VK_SHARING_MODE_EXCLUSIVE, vkImageLayout, isCube);
    } else if (((createInfo.aspect & TextureAspect::AspectDepth) == TextureAspect::AspectDepth)
               || ((createInfo.aspect & TextureAspect::AspectStencil) == TextureAspect::AspectStencil)) {
        VkImageAspectFlags vkImageAspect = 0;
        if ((createInfo.aspect & TextureAspect::AspectDepth) == TextureAspect::AspectDepth) {
            vkImageAspect |= VK_IMAGE_ASPECT_DEPTH_BIT;
        }
        if ((createInfo.aspect & TextureAspect::AspectStencil) == TextureAspect::AspectStencil) {
            vkImageAspect |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
        ok = createImage(createInfo.width, createInfo.height, createInfo.depth,
                         createInfo.mipLevels, createInfo.arrayLayers, vkImageType,
                         toVkFormat(createInfo.format), toVkSampleCount(sample),
                         VK_IMAGE_TILING_OPTIMAL, vkImageUsage, vkImageAspect,
                         VK_SHARING_MODE_EXCLUSIVE, vkImageLayout, false);
    } else {
        GX_ASSERT_S(false, "Unsupported aspect type (%x)", createInfo.aspect);
        ok = false;
    }

    return ok;
}

bool TextureVk::createImage(uint32_t width, uint32_t height, uint32_t depth,
                            uint32_t mipLevels,
                            uint32_t arrayLayers,
                            VkImageType imageType,
                            VkFormat format,
                            VkSampleCountFlagBits sampleCount,
                            VkImageTiling tiling,
                            VkImageUsageFlags usage,
                            VkImageAspectFlags aspect,
                            VkSharingMode sharingMode,
                            VkImageLayout layout,
                            bool isCube)
{
    VkFlags imageFlags = 0;
    if (isCube) {
        imageFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    }

    GVkContext *vkContext = getGVkContext(mContextT);

    mVkImage = GX_NEW(GVkImage);

    mVkImage->create(vkContext->gvkDevice(), imageType, format,
                    width, height, depth, sampleCount, tiling, usage, aspect,
                    sharingMode, layout, mipLevels, arrayLayers, imageFlags);

    return mVkImage->isCreated();
}

/// ============ SamplerVk ============ ///

bool SamplerVk::init(Context_T *context, const CreateSamplerInfo &createInfo)
{
    mContextT = context;

    mVkSampler.beginResetSampler();
    mVkSampler.setSamplerFilter(toVkFilter(createInfo.magFilter), toVkFilter(createInfo.minFilter));
    mVkSampler.setSamplerAddressMode(
            toVkSamplerAddressMode(createInfo.addressModeU),
            toVkSamplerAddressMode(createInfo.addressModeV),
            toVkSamplerAddressMode(createInfo.addressModeW));
    mVkSampler.setSamplerMipmapMode(toVkSamplerMipmapMode(createInfo.mipmapMode));

    // 0.25在vulkan中表示禁用mipmapping
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#textures-level-of-detail-operation
    mVkSampler.setSamplerMaxLod(createInfo.hasMips ? VK_LOD_CLAMP_NONE : 0.25f);

    mVkSampler.setSamplerCompareEnable(createInfo.compareEnable);
    mVkSampler.setSamplerCompareOp(toVkCompareOp(createInfo.compareOp));
    mVkSampler.setSamplerAnisotropyEnable(createInfo.anisotropyLog2 != 0);
    mVkSampler.setSamplerMaxAnisotropy((float)(1u << createInfo.anisotropyLog2));

    mVkSampler.endResetSampler();

    mVkSampler.create(getGVkContext(context)->gvkDevice());

    return true;
}

void SamplerVk::destroy()
{
    mVkSampler.destroy();
    mContextT = GFX_NULL_HANDLE;
}

Context_T *SamplerVk::context()
{
    return mContextT;
}

GVkSampler *SamplerVk::vkSampler()
{
    return &mVkSampler;
}

/// ============ ShaderVk ============ ///

bool ShaderVk::init(Context_T *context, const CreateShaderInfo &createInfo)
{
    mContextT = context;
    mTag = createInfo.tag;
    return createVkShaderModule(createInfo);
}

void ShaderVk::destroy()
{
    releaseRefPipeline();
    if (mVkShaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(getGVkContext(mContextT)->vkDevice(), mVkShaderModule, nullptr);
    }
    mVkShaderModule = VK_NULL_HANDLE;
    mContextT = GFX_NULL_HANDLE;
}

Context_T *ShaderVk::context()
{
    return mContextT;
}

size_t ShaderVk::hash()
{
    return mHash;
}

VkShaderModule ShaderVk::vkShaderModule() const
{
    return mVkShaderModule;
}

VkShaderStageFlagBits ShaderVk::vkShaderStage() const
{
    return mVkShaderStage;
}

void ShaderVk::refPipeline(GfxIdxTy idx)
{
    mPipelineRef.emplace(idx);
}

bool ShaderVk::createVkShaderModule(const CreateShaderInfo &createInfo)
{
    mHash = hashOf(idx());

    mVkShaderStage = toVkShaderStageFlagBits(createInfo.type);

    VkShaderModuleCreateInfo moduleCreateInfo{};
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.pCode = static_cast<const uint32_t *>(createInfo.pCode);
    moduleCreateInfo.codeSize = createInfo.codeSize;
    VK_CHECK_RESULT(vkCreateShaderModule(getGVkContext(mContextT)->vkDevice(), &moduleCreateInfo, nullptr,
                                         &mVkShaderModule));
    return mVkShaderModule != VK_NULL_HANDLE;
}

void ShaderVk::releaseRefPipeline()
{
    auto *contextP = dynamic_cast<ContextVk *>(mContextT->contextP());
    for (GfxIdxTy idx : mPipelineRef) {
        auto *obj = contextP->findPipeline(idx);
        if (obj != GFX_NULL_HANDLE) {
            contextP->destroyPipeline(obj);
        }
    }
    mPipelineRef.clear();
}

/// ============ DescriptorLayoutVk ============ ///

bool DescriptorLayoutVk::init(Context_T *context, const ResourceLayoutInfo &createInfo)
{
    mContextT = context;

    std::hash<ResourceLayoutInfo> hashFunc;
    mHash = hashFunc(createInfo);

    GVkContext *vkContext = getGVkContext(mContextT);
    VkDevice vkDevice = vkContext->vkDevice();

    //! [1] DescriptorSetLayout
    std::vector<VkDescriptorSetLayoutBinding> vkDescSetLayoutBindings;

    auto &bindingInfos = createInfo.bindingInfos;
    if (!bindingInfos.empty()) {
        vkDescSetLayoutBindings.resize(bindingInfos.size());
        for (uint32_t i = 0; i < bindingInfos.size(); i++) {
            mBindingInfo.insert(std::make_pair(i, bindingInfos[i]));

            vkDescSetLayoutBindings[i].binding = i;
            vkDescSetLayoutBindings[i].descriptorType = toVkDescriptorType(bindingInfos[i].descriptorType);
            vkDescSetLayoutBindings[i].descriptorCount = 1;
            vkDescSetLayoutBindings[i].stageFlags = toVkShaderStageFlags(bindingInfos[i].shaderStage);
        }
    }
    VkDescriptorSetLayoutCreateInfo descSetLayoutCreateInfo{};
    descSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(vkDescSetLayoutBindings.size());
    descSetLayoutCreateInfo.pBindings = vkDescSetLayoutBindings.data();

    return vkCreateDescriptorSetLayout(vkDevice, &descSetLayoutCreateInfo, nullptr, &mVkDescSetLayout) == VK_SUCCESS;
}

void DescriptorLayoutVk::destroy()
{
    VkDevice vkDevice = getGVkContext(mContextT)->vkDevice();

    vkDestroyDescriptorSetLayout(vkDevice, mVkDescSetLayout, nullptr);

    mVkDescSetLayout = VK_NULL_HANDLE;
    mBindingInfo.clear();
    mContextT = GFX_NULL_HANDLE;
}

Context_T *DescriptorLayoutVk::context()
{
    return mContextT;
}

size_t DescriptorLayoutVk::hash()
{
    return mHash;
}

bool DescriptorLayoutVk::equals(ElementHandle *b)
{
    return this->hash() == b->hash();
}

VkDescriptorSetLayout DescriptorLayoutVk::vkDescriptorSetLayout() const
{
    return mVkDescSetLayout;
}

/// ============ ResourceBinderVk ============ ///

bool ResourceBinderVk::init(Context_T *context, const ResourceLayoutInfo &layoutInfo)
{
    mContextT = context;

    auto *contextVk = dynamic_cast<ContextVk *>(context->contextP());
    VkDevice vkDevice = contextVk->vkContext()->vkDevice();

    mDescLayoutInfo = layoutInfo;

    mDescLayout = contextVk->getDescriptorLayout(layoutInfo);
    VkDescriptorSetLayout vkLayout = mDescLayout->vkDescriptorSetLayout();

    mDescSet = contextVk->allocVkDescriptorSet(vkLayout, mPool);
    GX_ASSERT(mDescSet != VK_NULL_HANDLE);

    return mDescSet != VK_NULL_HANDLE && initBindInfo();
}

void ResourceBinderVk::destroy()
{
    VkDevice vkDevice = vkContext()->vkDevice();

    VK_CHECK_RESULT(vkFreeDescriptorSets(vkDevice, mPool, 1, &mDescSet));

    mPool = VK_NULL_HANDLE;
    mContextT = GFX_NULL_HANDLE;
}

Context_T *ResourceBinderVk::context()
{
    return mContextT;
}

void ResourceBinderVk::bindBuffer(uint32_t binding, Buffer buffer)
{
    bindBufferRange(binding, buffer, 0, GFX_WHOLE_SIZE);
}

void ResourceBinderVk::bindBufferRange(uint32_t binding, Buffer buffer, uint64_t offset, uint64_t range)
{
    GX_ASSERT(binding < mBindDescInfo.size());

    auto &info = mBindDescInfo[binding];

    GX_ASSERT(info.index() == 0);

    if (info.index() == 0) {
        BindBufferInfo &pInfo = std::get<BindBufferInfo>(info);
        if (pInfo.buffer != buffer || pInfo.offset != offset || pInfo.range != range) {
            pInfo.buffer = buffer;
            pInfo.offset = offset;
            pInfo.range = range;
            pInfo.updated = false;
            mAllUpdated = false;
        }
    }
}

void ResourceBinderVk::bindTexelBuffer(uint32_t binding, Buffer buffer, Format::Enum format,
                                       uint64_t offset, uint64_t range)
{
    GX_ASSERT(binding < mBindDescInfo.size());

    auto &info = mBindDescInfo[binding];

    GX_ASSERT(info.index() == 2);

    if (info.index() == 2) {
        BindTexelBufferInfo &pInfo = std::get<BindTexelBufferInfo>(info);
        if (pInfo.buffer != buffer || pInfo.offset != offset || pInfo.range != range
            || pInfo.format != format) {
            pInfo.buffer = buffer;
            pInfo.offset = offset;
            pInfo.range = range;
            pInfo.format = format;
            pInfo.updated = false;
            mAllUpdated = false;
        }
    }
}

void ResourceBinderVk::bindTexture(uint32_t binding, Texture texture, Sampler sampler, const TextureBindRange &range)
{
    GX_ASSERT(binding < mBindDescInfo.size());

    auto &info = mBindDescInfo[binding];

    GX_ASSERT(info.index() == 1);

    if (info.index() == 1) {
        BindSamplerInfo &pInfo = std::get<BindSamplerInfo>(info);
        if (pInfo.texture != texture || pInfo.sampler != sampler) {
            pInfo.texture = texture;
            pInfo.sampler = sampler;
            pInfo.range = range;
            pInfo.updated = false;
            mAllUpdated = false;
        }
    }
}

void ResourceBinderVk::bindInputAttachment(uint32_t binding, Texture texture, const TextureBindRange &range)
{
    bindTexture(binding, texture, GFX_NULL_HANDLE, range);
}

GVkContext *ResourceBinderVk::vkContext()
{
    auto *context = dynamic_cast<ContextVk *>(mContextT->contextP());
    GX_ASSERT(context);
    return context->vkContext();
}

ResourceLayoutInfo ResourceBinderVk::getDescriptorLayoutInfo() const
{
    return mDescLayoutInfo;
}

VkDescriptorSet ResourceBinderVk::getVkDescriptorSet() const
{
    return mDescSet;
}

void ResourceBinderVk::bindResources()
{
    if (mAllUpdated) {
        return;
    }
    std::vector<VkWriteDescriptorSet> writeDescriptorSets;

    for (uint32_t binding = 0; binding < mBindDescInfo.size(); binding++) {
        auto &info = mBindDescInfo[binding];
        if (info.index() == 0) {
            BindBufferInfo &pInfo = std::get<BindBufferInfo>(info);
            if (pInfo.updated) {
                continue;
            }
            auto setVk = mDescSet;

            writeDescriptorSets.push_back({});
            auto &writeDescSet = writeDescriptorSets.back();

            writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescSet.dstSet = setVk;
            writeDescSet.descriptorType = toVkDescriptorType(pInfo.type);
            writeDescSet.dstBinding = binding;

            auto *bufferVk = dynamic_cast<BufferVk *>(pInfo.buffer);

            uint64_t range = pInfo.range == GFX_WHOLE_SIZE ? (bufferVk->size() - pInfo.offset) : pInfo.range;
            pInfo.writeInfo = bufferVk->getVkDescriptorBufferInfo(pInfo.offset, range);
            writeDescSet.pBufferInfo = &pInfo.writeInfo;

            writeDescSet.descriptorCount = 1;
            pInfo.updated = true;
        } else if (info.index() == 1) {
            BindSamplerInfo &pInfo = std::get<BindSamplerInfo>(info);
            if (pInfo.updated) {
                continue;
            }
            auto setVk = mDescSet;

            writeDescriptorSets.push_back({});
            auto &writeDescSet = writeDescriptorSets.back();

            writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescSet.dstSet = setVk;
            writeDescSet.descriptorType = toVkDescriptorType(pInfo.type);
            writeDescSet.dstBinding = binding;

            auto *textureVk = dynamic_cast<TextureVk *>(pInfo.texture);
            if (pInfo.type == ResourceType::InputAttachment || pInfo.sampler == nullptr) {
                VkImageView imageView = textureVk->createImageView(pInfo.range, false);
                pInfo.writeInfo = textureVk->getDescriptor(imageView, VK_NULL_HANDLE);
                writeDescSet.pImageInfo = &pInfo.writeInfo;
            } else {
                VkImageView imageView = textureVk->createImageView(pInfo.range, false);
                auto *samplerVk = dynamic_cast<SamplerVk *>(pInfo.sampler);
                pInfo.writeInfo = textureVk->getDescriptor(imageView, samplerVk->vkSampler()->vkSampler());
                writeDescSet.pImageInfo = &pInfo.writeInfo;
            }

            writeDescSet.descriptorCount = 1;
            pInfo.updated = true;
        } else if (info.index() == 2) {
            BindTexelBufferInfo &pInfo = std::get<BindTexelBufferInfo>(info);
            if (pInfo.updated) {
                continue;
            }
            auto setVk = mDescSet;

            writeDescriptorSets.push_back({});
            auto &writeDescSet = writeDescriptorSets.back();

            writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescSet.dstSet = setVk;
            writeDescSet.descriptorType = toVkDescriptorType(pInfo.type);
            writeDescSet.dstBinding = binding;

            auto *bufferVk = dynamic_cast<BufferVk *>(pInfo.buffer);

            uint64_t range = pInfo.range == GFX_WHOLE_SIZE ? (bufferVk->size() - pInfo.offset) : pInfo.range;

            pInfo.bufferView = bufferVk->getVkBufferView(pInfo.format, pInfo.offset, range);
            writeDescSet.pTexelBufferView = &pInfo.bufferView;

            writeDescSet.descriptorCount = 1;
            pInfo.updated = true;
        }
    }

    vkUpdateDescriptorSets(vkContext()->vkDevice(), static_cast<uint32_t>(writeDescriptorSets.size()),
                           writeDescriptorSets.data(), 0, VK_NULL_HANDLE);

    mAllUpdated = true;
//    Log("ResourceBinderVk::bindResources updated");
}

bool ResourceBinderVk::initBindInfo()
{
    mAllUpdated = true;

    auto layout = mDescLayout;
    const auto &lBindInfo = layout->mBindingInfo;
    mBindDescInfo.resize(lBindInfo.size());
    for (uint32_t j = 0; j < lBindInfo.size(); j++) {
        const auto &info = lBindInfo.at(j);
        switch (info.descriptorType) {
            case ResourceType::Sampler:
            case ResourceType::CombinedImageSampler:
            case ResourceType::SamplerImage:
            case ResourceType::StorageImage:
            case ResourceType::InputAttachment:
                mBindDescInfo[j] = BindSamplerInfo{
                        info.descriptorType, GFX_NULL_HANDLE, GFX_NULL_HANDLE, {}, true, {}
                };
                break;
            case ResourceType::UniformBuffer:
            case ResourceType::StorageBuffer:
            case ResourceType::UniformBufferDynamic:
            case ResourceType::StorageBufferDynamic:
                mBindDescInfo[j] = BindBufferInfo{
                        info.descriptorType, GFX_NULL_HANDLE, 0, GFX_WHOLE_SIZE, true, {}
                };
                break;
            case ResourceType::UniformTexelBuffer:
            case ResourceType::StorageTexelBuffer:
                mBindDescInfo[j] = BindTexelBufferInfo{
                        info.descriptorType, GFX_NULL_HANDLE, 0, GFX_WHOLE_SIZE, Format::Undefined, true, VK_NULL_HANDLE
                };
                break;
            default:
                GX_ASSERT_S(false, "Unknown DescriptorType");
                return false;
        }
    }
    return true;
}

/// ============ PipelineLayoutVk ============ ///

bool PipelineLayoutVk::init(Context_T *context, const PipelineLayoutInfo &info)
{
    mContextT = context;
    auto *contextP = dynamic_cast<ContextVk *>(context->contextP());
    auto *vkContext = contextP->vkContext();

    std::vector<VkDescriptorSetLayout> vkDescLayouts;
    const auto &layoutInfos = info.layoutInfos;
    if (!layoutInfos.empty()) {
        vkDescLayouts.resize(layoutInfos.size());
        for (uint32_t i = 0; i < layoutInfos.size(); i++) {
            auto *dsLayoutVk = contextP->getDescriptorLayout(layoutInfos[i]);
            GX_ASSERT(dsLayoutVk != GFX_NULL_HANDLE);
            vkDescLayouts[i] = dsLayoutVk->vkDescriptorSetLayout();
        }
    }

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = vkDescLayouts.size();
    pipelineLayoutCreateInfo.pSetLayouts = vkDescLayouts.data();
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

    return vkCreatePipelineLayout(vkContext->vkDevice(), &pipelineLayoutCreateInfo,
                                  nullptr, &mVkPipelineLayout) == VK_SUCCESS;
}

void PipelineLayoutVk::destroy()
{
    VkDevice vkDevice = getGVkContext(mContextT)->vkDevice();
    vkDestroyPipelineLayout(vkDevice, mVkPipelineLayout, nullptr);

    mVkPipelineLayout = VK_NULL_HANDLE;
    mContextT = GFX_NULL_HANDLE;
}

Context_T *PipelineLayoutVk::context()
{
    return mContextT;
}

VkPipelineLayout PipelineLayoutVk::getVkPipelineLayout() const
{
    return mVkPipelineLayout;
}

/// ============ PipelineVk ============ ///

bool PipelineVk::init(Context_T *context, const CreateGraphicsPipelineStateInfo &createInfo)
{
    mContextT = context;

    return createGraphPipeline(createInfo);
}

bool PipelineVk::init(Context_T *context, const CreateComputePipelineStateInfo &createInfo)
{
    mContextT = context;

    return createComputePipeline(createInfo);
}

void PipelineVk::destroy()
{
    VkDevice vkDevice = getGVkContext(mContextT)->vkDevice();

    vkDestroyPipeline(vkDevice, mVkPipeline, nullptr);

    mContextT = GFX_NULL_HANDLE;
}

Context_T *PipelineVk::context()
{
    return mContextT;
}

VkPipeline PipelineVk::vkPipeline() const
{
    return mVkPipeline;
}

bool PipelineVk::createGraphPipeline(const CreateGraphicsPipelineStateInfo &createInfo)
{
    auto *contextP = dynamic_cast<ContextVk *>(mContextT->contextP());
    GVkContext *gVkContext = contextP->vkContext();

    QueryGraphicsPipelineStateInfo queryInfo = createQueryGraphicsPipelineStateInfo(createInfo);
    std::hash<QueryGraphicsPipelineStateInfo> hashFunc;
    mHash = hashFunc(queryInfo);

    auto &stateInfo = createInfo.stateInfo;
    auto &vertexLayout = createInfo.vertexLayout;

    //! [1.1] VkVertexInputBindingDescriptions
    std::vector<VkVertexInputBindingDescription> vkVInputBindDescriptions;
    for (auto &ibi : vertexLayout.vertexInputBindingInfos) {
        if (ibi.stride > 0) {
            vkVInputBindDescriptions.push_back(
                    {
                            (uint32_t) ibi.binding, (uint32_t) ibi.stride, toVkVertexInputRate(ibi.inputRate)
                    });
        }
    }

    //! [1.2] VkVertexInputAttributeDescriptions
    std::vector<VkVertexInputAttributeDescription> vkVaoDescs;
    for (auto &iad : vertexLayout.vertexInputAttributeDescInfos) {
        if (iad.use) {
            vkVaoDescs.push_back(
                    {
                            (uint32_t) iad.location,
                            (uint32_t) iad.binding,
                            toVkVertexAttribType(iad.attrib, iad.normalized),
                            iad.offset
                    });
        }
    }

    //! [1] VkPipelineVertexInputStateCreateInfo
    VkPipelineVertexInputStateCreateInfo vertexInputState{};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexBindingDescriptionCount = vkVInputBindDescriptions.size();
    vertexInputState.pVertexBindingDescriptions = vkVInputBindDescriptions.data();
    vertexInputState.vertexAttributeDescriptionCount = vkVaoDescs.size();
    vertexInputState.pVertexAttributeDescriptions = vkVaoDescs.data();

    //! [2] VkPipelineRasterizationStateCreateInfo
    float depthBiasConstantFactor = stateInfo.paramValueInfo.depthBiasConstantFactor;
    float depthBiasSlopeFactor = stateInfo.paramValueInfo.depthBiasSlopeFactor;
    float depthBiasClamp = stateInfo.paramValueInfo.depthBiasClamp;
    VkPipelineRasterizationStateCreateInfo rasterizationState{};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.flags = 0;
    rasterizationState.depthClampEnable = VK_FALSE;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.polygonMode = toVkPolygonMode(stateInfo.rasterStateInfo.polygonMode);
    rasterizationState.cullMode = toVkCullModeFlags(stateInfo.rasterStateInfo.cullMode);
    rasterizationState.frontFace = toVkFrontFace(stateInfo.rasterStateInfo.frontFace);
    rasterizationState.depthBiasEnable =
            (depthBiasConstantFactor != 0 || depthBiasSlopeFactor != 0 || depthBiasClamp != 0) ? VK_TRUE : VK_FALSE;
    rasterizationState.depthBiasConstantFactor = depthBiasConstantFactor;
    rasterizationState.depthBiasClamp = depthBiasClamp;
    rasterizationState.depthBiasSlopeFactor = depthBiasSlopeFactor;
    rasterizationState.lineWidth = 1.0f;

    //! [3] VkPipelineInputAssemblyStateCreateInfo
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.topology = toVkPrimitiveTopology(stateInfo.rasterStateInfo.primitive);
    inputAssemblyState.flags = 0;
    inputAssemblyState.primitiveRestartEnable = stateInfo.rasterStateInfo.primitiveRestartEnable;

    auto *rpVk = contextP->findRenderPass(createInfo.renderPass);
    GX_ASSERT(rpVk != GFX_NULL_HANDLE);

    //! [4] VkPipelineColorBlendStateCreateInfo
    std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachmentStates;
    if (rpVk->colorAttachmentCount() > 0) {
        colorBlendAttachmentStates.resize(rpVk->colorAttachmentCount());

        bool blendEnable = !(stateInfo.rasterStateInfo.colorBlendOp == BlendOp::Add
                             && stateInfo.rasterStateInfo.alphaBlendOp == BlendOp::Add
                             && stateInfo.rasterStateInfo.srcColorBlendFactor == BlendFactor::One
                             && stateInfo.rasterStateInfo.srcAlphaBlendFactor == BlendFactor::One
                             && stateInfo.rasterStateInfo.dstColorBlendFactor == BlendFactor::Zero
                             && stateInfo.rasterStateInfo.dstAlphaBlendFactor == BlendFactor::Zero);

        colorBlendAttachmentStates[0].blendEnable = blendEnable;
        colorBlendAttachmentStates[0].srcColorBlendFactor = toVkBlendFactor(
                stateInfo.rasterStateInfo.srcColorBlendFactor);
        colorBlendAttachmentStates[0].dstColorBlendFactor = toVkBlendFactor(
                stateInfo.rasterStateInfo.dstColorBlendFactor);
        colorBlendAttachmentStates[0].colorBlendOp = toVkBlendOp(stateInfo.rasterStateInfo.colorBlendOp);
        colorBlendAttachmentStates[0].srcAlphaBlendFactor = toVkBlendFactor(
                stateInfo.rasterStateInfo.srcAlphaBlendFactor);
        colorBlendAttachmentStates[0].dstAlphaBlendFactor = toVkBlendFactor(
                stateInfo.rasterStateInfo.dstAlphaBlendFactor);
        colorBlendAttachmentStates[0].alphaBlendOp = toVkBlendOp(stateInfo.rasterStateInfo.alphaBlendOp);
        colorBlendAttachmentStates[0].colorWriteMask = toVkColorComponentFlags(
                stateInfo.rasterStateInfo.colorWriteMask);
        for (uint32_t i = 1; i < colorBlendAttachmentStates.size(); i++) {
            colorBlendAttachmentStates[i] = colorBlendAttachmentStates[0];
        }
    }

    VkPipelineColorBlendStateCreateInfo colorBlendState{};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.flags = 0;
    colorBlendState.attachmentCount = static_cast<uint32_t>(colorBlendAttachmentStates.size());
    colorBlendState.pAttachments = colorBlendAttachmentStates.data();
    colorBlendState.logicOpEnable = stateInfo.rasterStateInfo.logicOpEnable;
    colorBlendState.logicOp = toVkLogicOp(stateInfo.rasterStateInfo.logicOp);

    //! [5] VkPipelineDepthStencilStateCreateInfo
    VkPipelineDepthStencilStateCreateInfo depthStencilState{};
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.flags = 0;
    depthStencilState.depthTestEnable = stateInfo.rasterStateInfo.depthTestEnable;
    depthStencilState.depthWriteEnable = stateInfo.rasterStateInfo.depthWriteEnable;
    depthStencilState.depthCompareOp = toVkCompareOp(stateInfo.rasterStateInfo.depthCompareOp);
    depthStencilState.stencilTestEnable = stateInfo.rasterStateInfo.stencilTestEnable;
    depthStencilState.front = toVkStencilOpState(stateInfo.paramValueInfo.frontStencilOp,
                                                 createInfo.frontMR.compareMask,
                                                 createInfo.frontMR.writeMask,
                                                 createInfo.frontMR.reference);
    depthStencilState.back = toVkStencilOpState(stateInfo.paramValueInfo.backStencilOp,
                                                createInfo.backMR.compareMask,
                                                createInfo.backMR.writeMask,
                                                createInfo.backMR.reference);
    depthStencilState.depthBoundsTestEnable = VK_FALSE;
    depthStencilState.minDepthBounds = 0.0f;
    depthStencilState.maxDepthBounds = 1.0f;

    //! [6] VkPipelineViewportStateCreateInfo
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.flags = 0;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    //! [7] VkPipelineMultisampleStateCreateInfo
    VkPipelineMultisampleStateCreateInfo multisampleState{};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.flags = 0;
    multisampleState.rasterizationSamples = toVkSampleCount(rpVk->sampleCountFlag());
    multisampleState.sampleShadingEnable = stateInfo.rasterStateInfo.sampleShadingEnable;
    multisampleState.minSampleShading = stateInfo.rasterStateInfo.conservativeEnable ? 1.0f : 0.0f;
    multisampleState.pSampleMask = nullptr;
    multisampleState.alphaToCoverageEnable = stateInfo.rasterStateInfo.alphaToCoverageEnable;
    multisampleState.alphaToOneEnable = stateInfo.rasterStateInfo.alphaToOneEnable;

    //! [8] VkPipelineDynamicStateCreateInfo
    std::vector<VkDynamicState> dynamicStateEnables = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
            VK_DYNAMIC_STATE_LINE_WIDTH
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.flags = 0;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
    dynamicState.pDynamicStates = dynamicStateEnables.data();

    //! [9] VkPipelineShaderStageCreateInfo
    bool hasTessellationShader = false;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    shaderStages.resize(createInfo.shaderPrograms.size());
    for (int i = 0; i < shaderStages.size(); i++) {
        GX_ASSERT(createInfo.shaderPrograms[i]);
        auto *shaderP = dynamic_cast<ShaderVk *>(createInfo.shaderPrograms[i]);
        shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[i].module = shaderP->vkShaderModule();
        shaderStages[i].stage = shaderP->vkShaderStage();
        shaderStages[i].pName = "main";

        if ((shaderP->vkShaderStage() & VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT)
            || (shaderP->vkShaderStage() & VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT)) {
            hasTessellationShader = true;
        }
    }

    //! [10] VkPipelineTessellationStateCreateInfo
    VkPipelineTessellationStateCreateInfo tessellationState{};
    tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellationState.flags = 0;
    tessellationState.patchControlPoints = stateInfo.paramValueInfo.tessellationPatchControlPoints;

    //! [11] PipelineLayout
    VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
    if (createInfo.pipelineLayout != 0) {
        auto *pipelineLayout = contextP->findPipelineLayout(createInfo.pipelineLayout);
        GX_ASSERT(pipelineLayout != GFX_NULL_HANDLE);
        vkPipelineLayout = pipelineLayout->getVkPipelineLayout();
        GX_ASSERT(vkPipelineLayout != VK_NULL_HANDLE);
    }

    //! Create pipeline

    VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.flags = 0;
    pipelineCreateInfo.renderPass = rpVk->vkRenderPass()->vkRenderPass();
    pipelineCreateInfo.layout = vkPipelineLayout;
    pipelineCreateInfo.subpass = createInfo.subpassIndex;
    pipelineCreateInfo.pVertexInputState = &vertexInputState;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineCreateInfo.pStages = shaderStages.data();

    // A bug in apple system
    // https://github.com/KhronosGroup/MoltenVK/blob/master/MoltenVK/MoltenVK/GPUObjects/MVKPipeline.mm
    // line: 1441-1456
    if (hasTessellationShader) {
        pipelineCreateInfo.pTessellationState = &tessellationState;
    }

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(gVkContext->vkDevice(), gVkContext->pipelineCache(),
                                              1, &pipelineCreateInfo, nullptr, &mVkPipeline));

    return mVkPipeline != VK_NULL_HANDLE;
}

bool PipelineVk::createComputePipeline(const CreateComputePipelineStateInfo &createInfo)
{
    auto *contextP = dynamic_cast<ContextVk *>(mContextT->contextP());
    GVkContext *gVkContext = contextP->vkContext();

    QueryComputePipelineStateInfo queryInfo = createQueryComputePipelineStateInfo(createInfo);
    std::hash<QueryComputePipelineStateInfo> hashFunc;
    mHash = hashFunc(queryInfo);


    //! [1] PipelineLayout
    VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
    if (createInfo.pipelineLayout != 0) {
        auto *pipelineLayout = contextP->findPipelineLayout(createInfo.pipelineLayout);
        GX_ASSERT(pipelineLayout != GFX_NULL_HANDLE);
        vkPipelineLayout = pipelineLayout->getVkPipelineLayout();
        GX_ASSERT(vkPipelineLayout != VK_NULL_HANDLE);
    }

    //! [1] Shader
    GX_ASSERT(createInfo.shaderPrograms.size() == 1);
    GX_ASSERT(createInfo.shaderPrograms.at(0));
    auto *shaderP = dynamic_cast<ShaderVk *>(createInfo.shaderPrograms.at(0));
    GX_ASSERT(shaderP->vkShaderStage() == VK_SHADER_STAGE_COMPUTE_BIT);

    VkPipelineShaderStageCreateInfo shaderStage{};
    shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.module = shaderP->vkShaderModule();
    shaderStage.stage = shaderP->vkShaderStage();
    shaderStage.pName = "main";


    VkComputePipelineCreateInfo pipelineCreateInfo{};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.flags = 0;
    pipelineCreateInfo.stage = shaderStage;
    pipelineCreateInfo.layout = vkPipelineLayout;

    VK_CHECK_RESULT(vkCreateComputePipelines(gVkContext->vkDevice(), gVkContext->pipelineCache(),
                                             1, &pipelineCreateInfo, nullptr, &mVkPipeline));

    return mVkPipeline != VK_NULL_HANDLE;
}

size_t PipelineVk::hash()
{
    return mHash;
}

/// ============ CommandBufferVk ============ ///

bool CommandBufferVk::init(Context_T *context, const CreateCommandBufferInfo &createInfo)
{
    mContextT = context;
    auto *contextP = dynamic_cast<ContextVk *>(context->contextP());

    GVkContext *vkContext = contextP->vkContext();

    switch (createInfo.queueType) {
        default:
        case QueueType::Graphics:
            mVkCommandPool = vkContext->graphicsCmdPool();
            break;
        case QueueType::Compute:
            mVkCommandPool = vkContext->computeCmdPool();
            break;
        case QueueType::Transfer:
            mVkCommandPool = vkContext->transferCmdPool();
            break;
    }

    VkCommandBufferAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool = *mVkCommandPool;
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandBufferCount = createInfo.bufferCount;

    mVkCommandBuffers.resize(createInfo.bufferCount);
    VK_CHECK_RESULT(vkAllocateCommandBuffers(getGVkContext(mContextT)->vkDevice(), &info, mVkCommandBuffers.data()));

    mCommandBuffer.reset(CMD_BUFFER_SIZE);

    return true;
}

void CommandBufferVk::destroy()
{
    vkFreeCommandBuffers(getGVkContext(mContextT)->vkDevice(), *mVkCommandPool,
                         static_cast<uint32_t>(mVkCommandBuffers.size()), mVkCommandBuffers.data());
    mVkCommandBuffers.clear();
    mVkCommandPool = nullptr;
    mContextT = GFX_NULL_HANDLE;
}

Context_T *CommandBufferVk::context()
{
    return mContextT;
}

VkCommandBuffer CommandBufferVk::getVkCommandBuffer(uint32_t index)
{
    index = index % mVkCommandBuffers.size();
    return mVkCommandBuffers[index];
}

void CommandBufferVk::compile(FrameVk *frame)
{
    compileCommand(frame);
}

bool CommandBufferVk::isCompiled() const
{
    return mIsCompiled;
}

std::string CommandBufferVk::dump()
{
    if (mCommandBuffer.writePos() == 0) {
        return "";
    }

    GX_ASSERT_S(!mIsBegun, "Call end first to finish writing to the command buffer");

    if (mIsBegun) {
        return "";
    }

    mCommandBuffer.seekReadPos(SEEK_SET, 0);

    std::ostringstream out;

    uint8_t cmdKey;
    do {
        mCommandBuffer.read(cmdKey);

        if (cmdKey >= CommandKey::Count) {
            out << CommandKeyStr[CommandKey::None] << std::endl;
        } else {
            out << CommandKeyStr[cmdKey] << std::endl;
        }

        switch (cmdKey) {
            case CommandKey::SetClearColor: {
                ClearColor cc{};
                mCommandBuffer.read(cc);

                out << "    {" << "clearColor: {"
                    << "r: " << cc.r
                    << ", g: " << cc.g
                    << ", b: " << cc.b
                    << ", a: " << cc.a
                    << "}}" << std::endl;
            }
                break;
            case CommandKey::SetClearDepSte: {
                float depth;
                uint32_t stencil;
                mCommandBuffer.read(depth);
                mCommandBuffer.read(stencil);

                out << "    {"
                    << "depth: " << depth
                    << ", stencil: "
                    << stencil
                    << "}" << std::endl;
            }
                break;
            case CommandKey::BindRenderTarget: {
                GfxIdxTy idx;
                mCommandBuffer.read(idx);

                out << "    {" << "idx: " << idx << "}" << std::endl;
            }
                break;
            case CommandKey::BeginRenderPass: {
                Rect2D renderArea{};
                mCommandBuffer.read(renderArea);

                out << "    {" << "renderArea: {"
                    << "x: " << renderArea.x << ", "
                    << "y: " << renderArea.y << ", "
                    << "width: " << renderArea.width << ", "
                    << "height: " << renderArea.height << "}"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::SetGraphPipelineState: {
                GraphicsPipelineStateInfo stateInfo{};
                mCommandBuffer.read(stateInfo);

                out << "    {" << "stateInfo: {"
                    << "rasterStateInfo: {"
                    << "polygonMode: " << (int) stateInfo.rasterStateInfo.polygonMode
                    << ", primitive: " << (int) stateInfo.rasterStateInfo.primitive
                    << ", cullMode: " << (int) stateInfo.rasterStateInfo.cullMode
                    << ", frontFace: " << (int) stateInfo.rasterStateInfo.frontFace
                    << ", colorBlendOp: " << (int) stateInfo.rasterStateInfo.colorBlendOp
                    << ", primitiveRestartEnable: " << (int) stateInfo.rasterStateInfo.primitiveRestartEnable
                    << ", alphaBlendOp: " << (int) stateInfo.rasterStateInfo.alphaBlendOp
                    << ", logicOp: " << (int) stateInfo.rasterStateInfo.logicOp
                    << ", srcColorBlendFactor: " << (int) stateInfo.rasterStateInfo.srcColorBlendFactor
                    << ", srcAlphaBlendFactor: " << (int) stateInfo.rasterStateInfo.srcAlphaBlendFactor
                    << ", dstColorBlendFactor: " << (int) stateInfo.rasterStateInfo.dstColorBlendFactor
                    << ", dstAlphaBlendFactor: " << (int) stateInfo.rasterStateInfo.dstAlphaBlendFactor
                    << ", colorWriteMask: " << (int) stateInfo.rasterStateInfo.colorWriteMask
                    << ", logicOpEnable: " << (int) stateInfo.rasterStateInfo.logicOpEnable
                    << ", depthTestEnable: " << (int) stateInfo.rasterStateInfo.depthTestEnable
                    << ", depthWriteEnable: " << (int) stateInfo.rasterStateInfo.depthWriteEnable
                    << ", stencilTestEnable: " << (int) stateInfo.rasterStateInfo.stencilTestEnable
                    << ", depthCompareOp: " << (int) stateInfo.rasterStateInfo.depthCompareOp
                    << ", sampleShadingEnable: " << (int) stateInfo.rasterStateInfo.sampleShadingEnable
                    << ", conservativeEnable: " << (int) stateInfo.rasterStateInfo.conservativeEnable
                    << ", alphaToCoverageEnable: " << (int) stateInfo.rasterStateInfo.alphaToCoverageEnable
                    << ", alphaToOneEnable: " << (int) stateInfo.rasterStateInfo.alphaToOneEnable
                    << "},"
                    << "paramValueInfo: {"
                    << "depthBiasConstantFactor: " << stateInfo.paramValueInfo.depthBiasConstantFactor
                    << ", depthBiasClamp: " << stateInfo.paramValueInfo.depthBiasClamp
                    << ", depthBiasSlopeFactor: " << stateInfo.paramValueInfo.depthBiasSlopeFactor
                    << ", frontStencilOp: {failOp: " << (int) stateInfo.paramValueInfo.frontStencilOp.failOp
                    << ", passOp: " << (int) stateInfo.paramValueInfo.frontStencilOp.passOp
                    << ", depthFailOp: " << (int) stateInfo.paramValueInfo.frontStencilOp.depthFailOp
                    << ", compareOp: " << (int) stateInfo.paramValueInfo.frontStencilOp.compareOp
                    << "}"
                    << ", backStencilOp: {failOp: " << (int) stateInfo.paramValueInfo.backStencilOp.failOp
                    << ", passOp: " << (int) stateInfo.paramValueInfo.backStencilOp.passOp
                    << ", depthFailOp: " << (int) stateInfo.paramValueInfo.backStencilOp.depthFailOp
                    << ", compareOp: " << (int) stateInfo.paramValueInfo.backStencilOp.compareOp
                    << "}"
                    << ", tessellationPatchControlPoints: " << stateInfo.paramValueInfo.tessellationPatchControlPoints
                    << "}"
                    << "}"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::SetVertexLayout: {
                VertexLayout vertexLayout{};
                mCommandBuffer.read(vertexLayout);

                out << "    {";
                out << "vertexInputBindingInfos: [";
                int iindex = 0;
                for (auto &info : vertexLayout.vertexInputBindingInfos) {
                    if (info.stride > 0) {
                        if (iindex > 0) {
                            out << ", ";
                        }
                        iindex++;
                        out << "{"
                            << "binding: " << (int) info.binding
                            << ", inputRate: " << (int) info.inputRate
                            << ", stride: " << (int) info.stride
                            << "}";
                    }
                }
                out << "]"
                    << ", vertexInputAttributeDescInfos: [";
                iindex = 0;
                for (auto &info : vertexLayout.vertexInputAttributeDescInfos) {
                    if (info.use) {
                        if (iindex > 0) {
                            out << ", ";
                        }
                        iindex++;
                        out << "{"
                            << "binding: " << (int) info.binding
                            << ", location: " << (int) info.location
                            << ", attrib: " << (int) info.attrib
                            << ", normalized: " << (info.normalized ? "true" : "false")
                            << ", offset: " << info.offset
                            << "}";
                    }
                }
                out << "]";
                out << "}" << std::endl;
            }
                break;
            case CommandKey::SetShaders: {
                uint32_t size;
                GfxIdxTy idx;

                out << "    {shaders: [";

                mCommandBuffer.read(size);
                for (uint32_t k = 0; k < size; k++) {
                    if (k != 0) {
                        out << ", ";
                    }
                    mCommandBuffer.read(idx);
                    out << idx;
                }
                out << "]}" << std::endl;
            }
                break;
            case CommandKey::SetViewport: {
                Rect2D viewport{};
                float minDepth;
                float maxDepth;

                mCommandBuffer.read(viewport);
                mCommandBuffer.read(minDepth);
                mCommandBuffer.read(maxDepth);

                out << "    {" << "viewport: {"
                    << "x: " << viewport.x
                    << ", y: " << viewport.y
                    << ", width: " << viewport.width
                    << ", height: " << viewport.height
                    << ", minDepth: " << minDepth
                    << ", maxDepth: " << maxDepth
                    << "}" << std::endl;
            }
                break;
            case CommandKey::SetScissor: {
                Rect2D scissor{};
                mCommandBuffer.read(scissor);

                out << "    {" << "scissor: {"
                    << "x: " << scissor.x
                    << ", y: " << scissor.y
                    << ", width: " << scissor.width
                    << ", height: " << scissor.height
                    << "}" << std::endl;
            }
                break;
            case CommandKey::SetLineWidth: {
                float lineWidth;
                mCommandBuffer.read(lineWidth);

                out << "    {" << "lineWidth: " << lineWidth << "}" << std::endl;
            }
                break;
            case CommandKey::SetStencilCompMask: {
                uint8_t face;
                uint32_t mask;
                mCommandBuffer.read(face);
                mCommandBuffer.read(mask);

                out << "    {"
                    << "face: " << (int) face
                    << ", mask: " << std::hex << "0x" << mask << std::dec
                    << "}" << std::endl;
            }
                break;
            case CommandKey::SetStencilWriteMask: {
                uint8_t face;
                uint32_t mask;
                mCommandBuffer.read(face);
                mCommandBuffer.read(mask);

                out << "    {"
                    << "face: " << (int) face
                    << ", mask: " << std::hex << "0x" << mask << std::dec
                    << "}" << std::endl;
            }
                break;
            case CommandKey::SetStencilReference: {
                uint8_t face;
                uint32_t reference;
                mCommandBuffer.read(face);
                mCommandBuffer.read(reference);

                out << "    {"
                    << "face: " << (int) face
                    << ", reference: " << std::hex << "0x" << reference << std::dec
                    << "}" << std::endl;
            }
                break;
            case CommandKey::BindDescSet: {
                uint8_t bindPoint;
                uint32_t binderSize;

                mCommandBuffer.read(bindPoint);
                mCommandBuffer.read(binderSize);

                out << "    {"
                    << "bindPoint: " << (uint32_t)bindPoint
                    << ", binders: [";

                for (uint32_t x = 0; x < binderSize; x++) {
                    GfxIdxTy idx;
                    mCommandBuffer.read(idx);
                    if (x != 0) {
                        out << ", ";
                    }
                    out << idx;
                }

                out << "]"
                    << ", offsets = [";

                uint32_t offsetSize;

                mCommandBuffer.read(offsetSize);
                for (uint32_t x = 0; x < offsetSize; x++) {
                    uint32_t offset;
                    mCommandBuffer.read(offset);
                    if (x != 0) {
                        out << ", ";
                    }
                    out << offset;
                }

                out << "]}" << std::endl;
            }
                break;
            case CommandKey::BindVertexBuf: {
                uint32_t firstBinding;
                uint8_t size;

                mCommandBuffer.read(firstBinding);
                mCommandBuffer.read(size);
                GX_ASSERT(size > 0);

                out << "    {"
                    << "firstBinding: " << firstBinding
                    << ", buffers: [";

                for (uint32_t x = 0; x < size; x++) {
                    GfxIdxTy idx;
                    mCommandBuffer.read(idx);

                    if (x > 0) {
                        out << ", ";
                    }
                    out << idx;
                }
                out << "], offsets: [";
                for (uint32_t x = 0; x < size; x++) {
                    uint64_t offset;
                    mCommandBuffer.read(offset);
                    if (x > 0) {
                        out << ", ";
                    }
                    out << offset;
                }
                out << "]}" << std::endl;
            }
                break;
            case CommandKey::BindIndexBuf: {
                GfxIdxTy idx;
                uint32_t offset;
                IndexType::Enum indexType;
                mCommandBuffer.read(idx);
                mCommandBuffer.read(offset);
                mCommandBuffer.read(indexType);

                out << "    {"
                    << "buffer: " << idx
                    << ", offset: " << offset
                    << ", indexType: " << (int) indexType
                    << "}" << std::endl;
            }
                break;
            case CommandKey::Draw: {
                uint32_t vertexCount;
                uint32_t instanceCount;
                uint32_t firstVertex;
                uint32_t firstInstance;
                mCommandBuffer.read(vertexCount);
                mCommandBuffer.read(instanceCount);
                mCommandBuffer.read(firstVertex);
                mCommandBuffer.read(firstInstance);

                out << "    {"
                    << "vertexCount: " << vertexCount
                    << ", instanceCount: " << instanceCount
                    << ", firstVertex: " << firstVertex
                    << ", firstInstance: " << firstInstance
                    << "}" << std::endl;
            }
                break;
            case CommandKey::DrawIndexed: {
                uint32_t indexCount;
                uint32_t instanceCount;
                uint32_t firstIndex;
                int32_t vertexOffset;
                uint32_t firstInstance;
                mCommandBuffer.read(indexCount);
                mCommandBuffer.read(instanceCount);
                mCommandBuffer.read(firstIndex);
                mCommandBuffer.read(vertexOffset);
                mCommandBuffer.read(firstInstance);

                out << "    {"
                    << "indexCount: " << indexCount
                    << ", instanceCount: " << instanceCount
                    << ", firstIndex: " << firstIndex
                    << ", vertexOffset: " << vertexOffset
                    << ", firstInstance: " << firstInstance
                    << "}" << std::endl;
            }
                break;
            case CommandKey::DrawIndirect: {
                GfxIdxTy idx;
                uint32_t offset;
                uint32_t drawCount;
                uint32_t stride;

                mCommandBuffer.read(idx);
                mCommandBuffer.read(offset);
                mCommandBuffer.read(drawCount);
                mCommandBuffer.read(stride);

                out << "    {"
                    << "buffer: " << idx
                    << ", offset: " << offset
                    << ", drawCount: " << drawCount
                    << ", stride: " << stride
                    << "}" << std::endl;
            }
                break;
            case CommandKey::DrawIndexedIndirect: {
                GfxIdxTy idx;
                uint32_t offset;
                uint32_t drawCount;
                uint32_t stride;

                mCommandBuffer.read(idx);
                mCommandBuffer.read(offset);
                mCommandBuffer.read(drawCount);
                mCommandBuffer.read(stride);

                out << "    {"
                    << "buffer: " << idx
                    << ", offset: " << offset
                    << ", drawCount: " << drawCount
                    << ", stride: " << stride
                    << "}" << std::endl;
            }
                break;
            case CommandKey::Dispatch: {
                uint32_t groupCountX;
                uint32_t groupCountY;
                uint32_t groupCountZ;

                mCommandBuffer.read(groupCountX);
                mCommandBuffer.read(groupCountY);
                mCommandBuffer.read(groupCountZ);

                out << "    {"
                    << "groupCountX: " << groupCountX
                    << ", groupCountY: " << groupCountY
                    << ", groupCountZ: " << groupCountZ
                    << "}" << std::endl;
            }
                break;
            case CommandKey::DispatchIndirect: {
                GfxIdxTy idx;
                uint32_t offset;

                mCommandBuffer.read(idx);
                mCommandBuffer.read(offset);

                out << "    {"
                    << "buffer: " << idx
                    << ", offset: " << offset
                    << "}" << std::endl;
            }
                break;
            case CommandKey::PipelineBarrier: {
                PipelineStageMask srcStage;
                PipelineStageMask dstStage;

                mCommandBuffer.read(srcStage);
                mCommandBuffer.read(dstStage);

                out << "    {"
                    << "srcStage: " << std::hex << srcStage
                    << ", dstStage: " << dstStage << std::dec
                    << "}" << std::endl;
            }
                break;
            case CommandKey::BufferBarrier: {
                GfxIdxTy idx;
                BufferBarrierInfo barrierInfo{};

                mCommandBuffer.read(idx);
                mCommandBuffer.read(barrierInfo);

                out << "    {"
                    << "idx: " << idx
                    << ", barrierInfo: " << "{"
                    << "srcStage: " << std::hex << barrierInfo.srcStage
                    << ", dstStage: " << std::hex << barrierInfo.dstStage
                    << ", srcAccess: " << std::hex << barrierInfo.srcAccess
                    << ", dstAccess: " << std::hex << barrierInfo.dstAccess
                    << ", srcQueue: " << std::dec << barrierInfo.srcQueue
                    << ", dstQueue: " << std::dec << barrierInfo.dstQueue
                    << "}"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::ImageBarrier: {
                GfxIdxTy idx;
                uint8_t srcLayout;
                uint8_t dstLayout;
                ImageSubResourceRange subResRange{};

                mCommandBuffer.read(idx);
                mCommandBuffer.read(srcLayout);
                mCommandBuffer.read(dstLayout);
                mCommandBuffer.read(subResRange);

                out << "    {"
                    << "idx: " << idx
                    << ", srcLayout: " << (int) srcLayout
                    << ", dstLayout: " << (int) dstLayout
                    << ", subResRange: {"
                    << "baseMipLevel: " << subResRange.baseMipLevel
                    << ", levelCount: " << subResRange.levelCount
                    << ", baseArrayLayer: " << subResRange.baseArrayLayer
                    << ", layerCount: " << subResRange.layerCount
                    << "}"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::CopyBuffer: {
                GfxIdxTy srcIdx;
                GfxIdxTy dstIdx;
                uint64_t srcOffset;
                uint64_t dstOffset;
                uint64_t size;

                mCommandBuffer.read(srcIdx);
                mCommandBuffer.read(dstIdx);
                mCommandBuffer.read(srcOffset);
                mCommandBuffer.read(dstOffset);
                mCommandBuffer.read(size);

                out << "    {"
                    << "srcIdx: " << srcIdx
                    << ", dstIdx: " << dstIdx
                    << ", srcOffset: " << srcOffset
                    << ", dstOffset: " << dstOffset
                    << ", size: " << size
                    << "}" << std::endl;
            }
                break;
            case CommandKey::CopyImage: {
                GfxIdxTy srcIdx;
                GfxIdxTy dstIdx;
                uint32_t copyInfoSize;
                mCommandBuffer.read(srcIdx);
                mCommandBuffer.read(dstIdx);
                mCommandBuffer.read(copyInfoSize);

                out << "    {"
                    << "srcIdx: " << srcIdx
                    << ", dstIdx: " << dstIdx
                    << " ImageCopyInfos: [";

                if (copyInfoSize > 0) {
                    ImageCopyInfo copyInfo{};
                    for (uint32_t x = 0; x < copyInfoSize; x++) {
                        mCommandBuffer.read(copyInfo);

                        if (x > 0) {
                            out << ", ";
                        }
                        out << "{"
                            << "srcMipLevel: " << copyInfo.srcMipLevel
                            << ", srcBaseArrayLayer: " << copyInfo.srcBaseArrayLayer
                            << ", srcLayerCount: " << copyInfo.srcLayerCount
                            << ", srcOffsetX: " << copyInfo.srcOffsetX
                            << ", srcOffsetY: " << copyInfo.srcOffsetY
                            << ", srcOffsetZ: " << copyInfo.srcOffsetZ
                            << ", dstMipLevel: " << copyInfo.dstMipLevel
                            << ", dstBaseArrayLayer: " << copyInfo.dstBaseArrayLayer
                            << ", dstLayerCount: " << copyInfo.dstLayerCount
                            << ", dstOffsetX: " << copyInfo.dstOffsetX
                            << ", dstOffsetY: " << copyInfo.dstOffsetY
                            << ", dstOffsetZ: " << copyInfo.dstOffsetZ
                            << ", imageWidth: " << copyInfo.imageWidth
                            << ", imageHeight: " << copyInfo.imageHeight
                            << ", imageDepth: " << copyInfo.imageDepth
                            << ", srcAspectMask: " << copyInfo.srcAspectMask
                            << ", dstAspectMask: " << copyInfo.dstAspectMask
                            << "}";
                    }
                }
                out << "]"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::CopyBufferToImage: {
                GfxIdxTy srcIdx;
                GfxIdxTy dstIdx;
                uint32_t copyInfoSize;
                mCommandBuffer.read(srcIdx);
                mCommandBuffer.read(dstIdx);
                mCommandBuffer.read(copyInfoSize);

                out << "    {"
                    << "srcIdx: " << srcIdx
                    << ", dstIdx: " << dstIdx
                    << " BufferImageCopyInfos: [";

                if (copyInfoSize > 0) {
                    BufferImageCopyInfo tempInfo{};
                    for (uint32_t x = 0; x < copyInfoSize; x++) {
                        mCommandBuffer.read(tempInfo);
                        if (x > 0) {
                            out << ", ";
                        }
                        out << "{"
                            << "bufferOffset: " << tempInfo.bufferOffset
                            << ", bufferRowLength: " << tempInfo.bufferRowLength
                            << ", bufferImageHeight: " << tempInfo.bufferImageHeight
                            << ", mipLevel: " << tempInfo.mipLevel
                            << ", baseArrayLayer: " << tempInfo.baseArrayLayer
                            << ", layerCount: " << tempInfo.layerCount
                            << ", imageOffsetX: " << tempInfo.imageOffsetX
                            << ", imageOffsetY: " << tempInfo.imageOffsetY
                            << ", imageOffsetZ: " << tempInfo.imageOffsetZ
                            << ", imageWidth: " << tempInfo.imageWidth
                            << ", imageHeight: " << tempInfo.imageHeight
                            << ", imageDepth: " << tempInfo.imageDepth
                            << ", aspectMask: " << std::hex << (uint32_t)tempInfo.aspectMask << std::dec
                            << "}";
                    }
                }
                out << "]"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::CopyImageToBuffer: {
                GfxIdxTy srcIdx;
                GfxIdxTy dstIdx;
                uint32_t copyInfoSize;
                mCommandBuffer.read(srcIdx);
                mCommandBuffer.read(dstIdx);
                mCommandBuffer.read(copyInfoSize);

                out << "    {"
                    << "srcIdx: " << srcIdx
                    << ", dstIdx: " << dstIdx
                    << ", BufferImageCopyInfos: [";

                if (copyInfoSize > 0) {
                    BufferImageCopyInfo tempInfo{};
                    for (uint32_t x = 0; x < copyInfoSize; x++) {
                        mCommandBuffer.read(tempInfo);
                        if (x > 0) {
                            out << ", ";
                        }
                        out << "{"
                            << "bufferOffset: " << tempInfo.bufferOffset
                            << ", bufferRowLength: " << tempInfo.bufferRowLength
                            << ", bufferImageHeight: " << tempInfo.bufferImageHeight
                            << ", mipLevel: " << tempInfo.mipLevel
                            << ", baseArrayLayer: " << tempInfo.baseArrayLayer
                            << ", layerCount: " << tempInfo.layerCount
                            << ", imageOffsetX: " << tempInfo.imageOffsetX
                            << ", imageOffsetY: " << tempInfo.imageOffsetY
                            << ", imageOffsetZ: " << tempInfo.imageOffsetZ
                            << ", imageWidth: " << tempInfo.imageWidth
                            << ", imageHeight: " << tempInfo.imageHeight
                            << ", imageDepth: " << tempInfo.imageDepth
                            << ", aspectMask: " << std::hex << (uint32_t)tempInfo.aspectMask << std::dec
                            << "}";
                    }
                }

                out << "]"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::BlitImage: {
                GfxIdxTy srcIdx;
                GfxIdxTy dstIdx;
                uint8_t filter;
                uint32_t blitInfoSize;
                mCommandBuffer.read(srcIdx);
                mCommandBuffer.read(dstIdx);
                mCommandBuffer.read(filter);
                mCommandBuffer.read(blitInfoSize);

                out << "    {"
                    << "srcIdx: " << srcIdx
                    << ", dstIdx: " << dstIdx
                    << ", filter: " << (int) filter
                    << ", ImageBlitInfo: [";

                if (blitInfoSize > 0) {
                    ImageBlitInfo blitInfo{};
                    for (uint32_t x = 0; x < blitInfoSize; x++) {
                        mCommandBuffer.read(blitInfo);
                        if (x > 0) {
                            out << ", ";
                        }
                        out << "{"
                            << "srcMipLevel: " << blitInfo.srcMipLevel
                            << ", srcBaseArrayLayer: " << blitInfo.srcBaseArrayLayer
                            << ", srcLayerCount: " << blitInfo.srcLayerCount
                            << ", srcOffsetX: " << blitInfo.srcOffsetX
                            << ", srcOffsetY: " << blitInfo.srcOffsetY
                            << ", srcOffsetZ: " << blitInfo.srcOffsetZ
                            << ", srcWidth: " << blitInfo.srcWidth
                            << ", srcHeight: " << blitInfo.srcHeight
                            << ", srcDepth: " << blitInfo.srcDepth
                            << ", dstMipLevel: " << blitInfo.dstMipLevel
                            << ", dstBaseArrayLayer: " << blitInfo.dstBaseArrayLayer
                            << ", dstLayerCount: " << blitInfo.dstLayerCount
                            << ", dstOffsetX: " << blitInfo.dstOffsetX
                            << ", dstOffsetY: " << blitInfo.dstOffsetY
                            << ", dstOffsetZ: " << blitInfo.dstOffsetZ
                            << ", dstWidth: " << blitInfo.dstWidth
                            << ", dstHeight: " << blitInfo.dstHeight
                            << ", dstDepth: " << blitInfo.dstDepth
                            << ", srcAspectMask: " << blitInfo.srcAspectMask
                            << ", dstAspectMask: " << blitInfo.dstAspectMask
                            << "}";
                    }
                }
                out << "]"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::CopyRT: {
                GfxIdxTy srcIdx;
                GfxIdxTy dstIdx;
                uint8_t vFrameIndex;
                uint32_t copyInfoSize;
                std::vector<ImageCopyInfo> copyInfos;
                mCommandBuffer.read(srcIdx);
                mCommandBuffer.read(dstIdx);
                mCommandBuffer.read(vFrameIndex);
                mCommandBuffer.read(copyInfoSize);

                out << "    {"
                    << "srcIdx: " << srcIdx
                    << ", dstIdx: " << dstIdx
                    << ", vFrameIndex: " << (int) vFrameIndex
                    << ", ImageCopyInfos: [";

                if (copyInfoSize > 0) {
                    ImageCopyInfo copyInfo{};
                    for (uint32_t x = 0; x < copyInfoSize; x++) {
                        mCommandBuffer.read(copyInfo);
                        if (x > 0) {
                            out << ", ";
                        }
                        out << "{"
                            << "srcMipLevel: " << copyInfo.srcMipLevel
                            << ", srcBaseArrayLayer: " << copyInfo.srcBaseArrayLayer
                            << ", srcLayerCount: " << copyInfo.srcLayerCount
                            << ", srcOffsetX: " << copyInfo.srcOffsetX
                            << ", srcOffsetY: " << copyInfo.srcOffsetY
                            << ", srcOffsetZ: " << copyInfo.srcOffsetZ
                            << ", dstMipLevel: " << copyInfo.dstMipLevel
                            << ", dstBaseArrayLayer: " << copyInfo.dstBaseArrayLayer
                            << ", dstLayerCount: " << copyInfo.dstLayerCount
                            << ", dstOffsetX: " << copyInfo.dstOffsetX
                            << ", dstOffsetY: " << copyInfo.dstOffsetY
                            << ", dstOffsetZ: " << copyInfo.dstOffsetZ
                            << ", imageWidth: " << copyInfo.imageWidth
                            << ", imageHeight: " << copyInfo.imageHeight
                            << ", imageDepth: " << copyInfo.imageDepth
                            << ", srcAspectMask: " << copyInfo.srcAspectMask
                            << ", dstAspectMask: " << copyInfo.dstAspectMask
                            << "}";
                    }
                }
                out << "]"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::BlitRT: {
                GfxIdxTy srcIdx;
                GfxIdxTy dstIdx;
                uint8_t filter;
                uint8_t vFrameIndex;
                uint32_t blitInfoSize;
                mCommandBuffer.read(srcIdx);
                mCommandBuffer.read(dstIdx);
                mCommandBuffer.read(filter);
                mCommandBuffer.read(vFrameIndex);
                mCommandBuffer.read(blitInfoSize);

                out << "    {"
                    << "srcIdx: " << srcIdx
                    << ", dstIdx: " << dstIdx
                    << ", vFrameIndex: " << (int) vFrameIndex
                    << ", ImageBlitInfos: [";

                if (blitInfoSize > 0) {
                    ImageBlitInfo blitInfo{};
                    for (uint32_t x = 0; x < blitInfoSize; x++) {
                        mCommandBuffer.read(blitInfo);
                        if (x > 0) {
                            out << ", ";
                        }
                        out << "{"
                            << "srcMipLevel: " << blitInfo.srcMipLevel
                            << ", srcBaseArrayLayer: " << blitInfo.srcBaseArrayLayer
                            << ", srcLayerCount: " << blitInfo.srcLayerCount
                            << ", srcOffsetX: " << blitInfo.srcOffsetX
                            << ", srcOffsetY: " << blitInfo.srcOffsetY
                            << ", srcOffsetZ: " << blitInfo.srcOffsetZ
                            << ", srcWidth: " << blitInfo.srcWidth
                            << ", srcHeight: " << blitInfo.srcHeight
                            << ", srcDepth: " << blitInfo.srcDepth
                            << ", dstMipLevel: " << blitInfo.dstMipLevel
                            << ", dstBaseArrayLayer: " << blitInfo.dstBaseArrayLayer
                            << ", dstLayerCount: " << blitInfo.dstLayerCount
                            << ", dstOffsetX: " << blitInfo.dstOffsetX
                            << ", dstOffsetY: " << blitInfo.dstOffsetY
                            << ", dstOffsetZ: " << blitInfo.dstOffsetZ
                            << ", dstWidth: " << blitInfo.dstWidth
                            << ", dstHeight: " << blitInfo.dstHeight
                            << ", dstDepth: " << blitInfo.dstDepth
                            << ", srcAspectMask: " << blitInfo.srcAspectMask
                            << ", dstAspectMask: " << blitInfo.dstAspectMask
                            << "}";
                    }
                }
                out << "]"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::BlitRTToImage: {
                GfxIdxTy srcIdx;
                GfxIdxTy dstIdx;
                uint8_t filter;
                uint8_t attachIndex;
                uint8_t vFrameIndex;
                uint32_t blitInfoSize;
                mCommandBuffer.read(srcIdx);
                mCommandBuffer.read(dstIdx);
                mCommandBuffer.read(filter);
                mCommandBuffer.read(attachIndex);
                mCommandBuffer.read(vFrameIndex);
                mCommandBuffer.read(blitInfoSize);

                out << "    {"
                    << "srcIdx: " << srcIdx
                    << ", dstIdx: " << dstIdx
                    << ", filter: " << (int) filter
                    << ", attachIndex: " << (int) attachIndex
                    << ", vFrameIndex: " << (int) vFrameIndex
                    << ", ImageBlitInfos: [";

                if (blitInfoSize > 0) {
                    ImageBlitInfo blitInfo{};
                    for (uint32_t x = 0; x < blitInfoSize; x++) {
                        mCommandBuffer.read(blitInfo);
                        if (x > 0) {
                            out << ", ";
                        }
                        out << "{"
                            << "srcMipLevel: " << blitInfo.srcMipLevel
                            << ", srcBaseArrayLayer: " << blitInfo.srcBaseArrayLayer
                            << ", srcLayerCount: " << blitInfo.srcLayerCount
                            << ", srcOffsetX: " << blitInfo.srcOffsetX
                            << ", srcOffsetY: " << blitInfo.srcOffsetY
                            << ", srcOffsetZ: " << blitInfo.srcOffsetZ
                            << ", srcWidth: " << blitInfo.srcWidth
                            << ", srcHeight: " << blitInfo.srcHeight
                            << ", srcDepth: " << blitInfo.srcDepth
                            << ", dstMipLevel: " << blitInfo.dstMipLevel
                            << ", dstBaseArrayLayer: " << blitInfo.dstBaseArrayLayer
                            << ", dstLayerCount: " << blitInfo.dstLayerCount
                            << ", dstOffsetX: " << blitInfo.dstOffsetX
                            << ", dstOffsetY: " << blitInfo.dstOffsetY
                            << ", dstOffsetZ: " << blitInfo.dstOffsetZ
                            << ", dstWidth: " << blitInfo.dstWidth
                            << ", dstHeight: " << blitInfo.dstHeight
                            << ", dstDepth: " << blitInfo.dstDepth
                            << ", srcAspectMask: " << blitInfo.srcAspectMask
                            << ", dstAspectMask: " << blitInfo.dstAspectMask
                            << "}";
                    }
                }
                out << "]"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::CopyRTToImage: {
                GfxIdxTy srcIdx;
                GfxIdxTy dstIdx;
                uint8_t attachIndex;
                uint8_t vFrameIndex;
                uint32_t copyInfoSize;
                mCommandBuffer.read(srcIdx);
                mCommandBuffer.read(dstIdx);
                mCommandBuffer.read(attachIndex);
                mCommandBuffer.read(vFrameIndex);
                mCommandBuffer.read(copyInfoSize);

                out << "    {"
                    << "srcIdx: " << srcIdx
                    << ", dstIdx: " << dstIdx
                    << ", attachIndex: " << (int) attachIndex
                    << ", vFrameIndex: " << (int) vFrameIndex
                    << ", ImageCopyInfos: [";

                if (copyInfoSize > 0) {
                    ImageCopyInfo copyInfo{};
                    for (uint32_t x = 0; x < copyInfoSize; x++) {
                        mCommandBuffer.read(copyInfo);
                        if (x > 0) {
                            out << ", ";
                        }
                        out << "{"
                            << "srcMipLevel: " << copyInfo.srcMipLevel
                            << ", srcBaseArrayLayer: " << copyInfo.srcBaseArrayLayer
                            << ", srcLayerCount: " << copyInfo.srcLayerCount
                            << ", srcOffsetX: " << copyInfo.srcOffsetX
                            << ", srcOffsetY: " << copyInfo.srcOffsetY
                            << ", srcOffsetZ: " << copyInfo.srcOffsetZ
                            << ", dstMipLevel: " << copyInfo.dstMipLevel
                            << ", dstBaseArrayLayer: " << copyInfo.dstBaseArrayLayer
                            << ", dstLayerCount: " << copyInfo.dstLayerCount
                            << ", dstOffsetX: " << copyInfo.dstOffsetX
                            << ", dstOffsetY: " << copyInfo.dstOffsetY
                            << ", dstOffsetZ: " << copyInfo.dstOffsetZ
                            << ", imageWidth: " << copyInfo.imageWidth
                            << ", imageHeight: " << copyInfo.imageHeight
                            << ", imageDepth: " << copyInfo.imageDepth
                            << ", srcAspectMask: " << copyInfo.srcAspectMask
                            << ", dstAspectMask: " << copyInfo.dstAspectMask
                            << "}";
                    }
                }
                out << "]"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::CopyRTToBuffer: {
                GfxIdxTy srcIdx;
                GfxIdxTy dstIdx;
                uint8_t attachIndex;
                uint8_t vFrameIndex;
                uint32_t copyInfoSize;
                mCommandBuffer.read(srcIdx);
                mCommandBuffer.read(dstIdx);
                mCommandBuffer.read(attachIndex);
                mCommandBuffer.read(vFrameIndex);
                mCommandBuffer.read(copyInfoSize);

                out << "    {"
                    << "srcIdx: " << srcIdx
                    << ", dstIdx: " << dstIdx
                    << ", attachIndex: " << (int) attachIndex
                    << ", vFrameIndex: " << (int) vFrameIndex
                    << ", ImageCopyInfos: [";

                if (copyInfoSize > 0) {
                    BufferImageCopyInfo tempInfo{};
                    for (uint32_t x = 0; x < copyInfoSize; x++) {
                        mCommandBuffer.read(tempInfo);
                        if (x > 0) {
                            out << ", ";
                        }
                        out << "{"
                            << "bufferOffset: " << tempInfo.bufferOffset
                            << ", bufferRowLength: " << tempInfo.bufferRowLength
                            << ", bufferImageHeight: " << tempInfo.bufferImageHeight
                            << ", mipLevel: " << tempInfo.mipLevel
                            << ", baseArrayLayer: " << tempInfo.baseArrayLayer
                            << ", layerCount: " << tempInfo.layerCount
                            << ", imageOffsetX: " << tempInfo.imageOffsetX
                            << ", imageOffsetY: " << tempInfo.imageOffsetY
                            << ", imageOffsetZ: " << tempInfo.imageOffsetZ
                            << ", imageWidth: " << tempInfo.imageWidth
                            << ", imageHeight: " << tempInfo.imageHeight
                            << ", imageDepth: " << tempInfo.imageDepth
                            << ", aspectMask: " << std::hex << (uint32_t)tempInfo.aspectMask << std::dec
                            << "}";
                    }
                }
                out << "]"
                    << "}" << std::endl;
            }
                break;
            case CommandKey::FillBuffer: {
                GfxIdxTy idx;
                uint64_t offset;
                uint64_t size;
                uint32_t data;

                mCommandBuffer.read(idx);
                mCommandBuffer.read(offset);
                mCommandBuffer.read(size);
                mCommandBuffer.read(data);
                out << "   {"
                    << "buffer: " << idx
                    << ", offset" << offset
                    << ", size" << size
                    << ", data" << data
                    << "}"
                    << std::endl;
            }
                break;
            case CommandKey::BeginDebug: {
                std::string label;
                mCommandBuffer.read(label);
                out << "    {"
                    << "debugLabel: " << label
                    << "}"
                    << std::endl;
            }
                break;
            case CommandKey::EndDebug: {
            }
                break;
            case CommandKey::ResetQuery: {
                GfxIdxTy queryIdx;
                uint32_t firstQuery;
                uint32_t queryCount;

                mCommandBuffer.read(queryIdx);
                mCommandBuffer.read(firstQuery);
                mCommandBuffer.read(queryCount);

                out << "    {"
                    << "query: " << queryIdx
                    << ", firstQuery: " << firstQuery
                    << ", queryCount: " << queryCount
                    << "}"
                    << std::endl;
            }
                break;
            case CommandKey::BeginQuery: {
                GfxIdxTy queryIdx;
                uint32_t queryIndex;
                bool precise;

                mCommandBuffer.read(queryIdx);
                mCommandBuffer.read(queryIndex);
                mCommandBuffer.read(precise);

                out << "    {"
                    << "query: " << queryIdx
                    << ", queryIndex: " << queryIndex
                    << ", precise: " << precise
                    << "}"
                    << std::endl;
            }
                break;
            case CommandKey::EndQuery: {
                GfxIdxTy queryIdx;
                uint32_t queryIndex;

                mCommandBuffer.read(queryIdx);
                mCommandBuffer.read(queryIndex);

                out << "    {"
                    << "query: " << queryIdx
                    << ", queryIndex: " << queryIndex
                    << "}"
                    << std::endl;
            }
                break;
            case CommandKey::WriteTimestamp: {
                uint32_t pipelineStage;
                GfxIdxTy queryIdx;
                uint32_t queryIndex;

                mCommandBuffer.read(pipelineStage);
                mCommandBuffer.read(queryIdx);
                mCommandBuffer.read(queryIndex);

                out << "    {"
                    << "pipelineStage: " << pipelineStage
                    << ", query: " << queryIdx
                    << ", queryIndex: " << queryIndex
                    << "}"
                    << std::endl;
            }
                break;
            case CommandKey::CopyQueryResults: {
                GfxIdxTy queryIdx;
                uint32_t firstQuery;
                uint32_t queryCount;
                GfxIdxTy dstBufferIdx;
                uint64_t dstOffset;
                QueryResultFlags resultFlags;

                mCommandBuffer.read(queryIdx);
                mCommandBuffer.read(firstQuery);
                mCommandBuffer.read(queryCount);
                mCommandBuffer.read(dstBufferIdx);
                mCommandBuffer.read(dstOffset);
                mCommandBuffer.read(resultFlags);

                out << "    {"
                    << "query: " << queryIdx
                    << ", firstQuery: " << firstQuery
                    << ", queryCount: " << queryCount
                    << ", dstBuffer: " << dstBufferIdx
                    << ", dstOffset: " << dstOffset
                    << ", resultFlags: " << std::hex << (uint32_t)resultFlags << std::dec
                    << "}"
                    << std::endl;
            }
                break;
        }
    } while (cmdKey != CommandKey::End);

    return out.str();
}


CommandBuffer CommandBufferVk::begin()
{
    GX_ASSERT_S(!mIsBegun, "Do not call begin repeatedly");

    resetCommandBuffer();

    uint8_t cmdKey = CommandKey::Begin;
    mCommandBuffer.write(cmdKey);

    mIsBegun = true;

    return this;
}

void CommandBufferVk::end()
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::End;
    mCommandBuffer.write(cmdKey);

    mIsBegun = false;
}

CommandBuffer CommandBufferVk::setClearColor(const ClearColor &clearColor)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetClearColor;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(clearColor);

    return this;
}

CommandBuffer CommandBufferVk::setClearDepthStencil(float depth, uint32_t stencil)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetClearDepSte;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(depth);
    mCommandBuffer.write(stencil);

    return this;
}

CommandBuffer CommandBufferVk::bindRenderTarget(RenderTarget renderTarget)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(renderTarget);
    auto *targetP = dynamic_cast<RenderTarget_P *>(renderTarget);
    uint8_t cmdKey = CommandKey::BindRenderTarget;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(targetP->idx());

    return this;
}

CommandBuffer CommandBufferVk::bindRenderTarget(Frame frame)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(frame);
    GX_ASSERT(frame->renderTarget());
    auto *targetP = dynamic_cast<RenderTarget_P *>(frame->renderTarget());
    uint8_t cmdKey = CommandKey::BindRenderTarget;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(targetP->idx());

    return this;
}

CommandBuffer CommandBufferVk::beginRenderPass(const Rect2D &renderArea, const RenderPassInfo &rpInfo)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::BeginRenderPass;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(renderArea);
    writeRenderPassInfo(mCommandBuffer, rpInfo);

    return this;
}

CommandBuffer CommandBufferVk::endRenderPass()
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::EndRenderPass;
    mCommandBuffer.write(cmdKey);

    return this;
}

CommandBuffer CommandBufferVk::setGraphicsPipelineState(const GraphicsPipelineStateInfo &pipelineState)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetGraphPipelineState;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(pipelineState);

    return this;
}

CommandBuffer CommandBufferVk::setVertexLayout(const VertexLayout &vertexLayout)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetVertexLayout;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(vertexLayout);

    return this;
}

CommandBuffer CommandBufferVk::setShaders(const std::vector<Shader> &shaders)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetShaders;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write((uint32_t) shaders.size());
    for (auto &s : shaders) {
        mCommandBuffer.write(dynamic_cast<Shader_P *>(s)->idx());
    }

    return this;
}

CommandBuffer CommandBufferVk::nextSubpass()
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::NextSubpass;
    mCommandBuffer.write(cmdKey);

    return this;
}

CommandBuffer CommandBufferVk::setViewport(const Rect2D &viewport, float minDepth, float maxDepth)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetViewport;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(viewport);
    mCommandBuffer.write(minDepth);
    mCommandBuffer.write(maxDepth);

    return this;
}

CommandBuffer CommandBufferVk::setScissor(const Rect2D &scissor)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetScissor;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(scissor);

    return this;
}

CommandBuffer CommandBufferVk::setLineWidth(float lineWidth)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetLineWidth;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(lineWidth);

    return this;
}

CommandBuffer CommandBufferVk::setStencilCompareMask(StencilFace::Enum face, uint32_t mask)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetStencilCompMask;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write((uint8_t) face);
    mCommandBuffer.write(mask);

    return this;
}

CommandBuffer CommandBufferVk::setStencilWriteMask(StencilFace::Enum face, uint32_t mask)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetStencilWriteMask;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write((uint8_t) face);
    mCommandBuffer.write(mask);

    return this;
}

CommandBuffer CommandBufferVk::setStencilReference(StencilFace::Enum face, uint32_t reference)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::SetStencilReference;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write((uint8_t) face);
    mCommandBuffer.write(reference);

    return this;
}

CommandBuffer CommandBufferVk::bindResources(ResourceBindPoint::Enum bindPoint,
                                             const std::vector<ResourceBinder> &binders,
                                             const std::vector<uint32_t> &dynamicOffsets)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::BindDescSet;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write((uint8_t) bindPoint);
    mCommandBuffer.write((uint32_t)binders.size());
    for (auto i : binders) {
        mCommandBuffer.write(dynamic_cast<ResourceBinder_P *>(i)->idx());
    }
    mCommandBuffer.write((uint32_t)dynamicOffsets.size());
    mCommandBuffer.write(dynamicOffsets.data(), sizeof(uint32_t) * dynamicOffsets.size());

    return this;
}

CommandBuffer CommandBufferVk::bindVertexBuffer(uint32_t firstBinding,
                                                const std::vector<Buffer> &buffers,
                                                const std::vector<uint64_t> &offsets)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(!buffers.empty());
    GX_ASSERT(buffers.size() == offsets.size());

    uint8_t size = buffers.size();

    uint8_t cmdKey = CommandKey::BindVertexBuf;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(firstBinding);
    mCommandBuffer.write(size);
    for (auto &i : buffers) {
        mCommandBuffer.write(dynamic_cast<Buffer_P *>(i)->idx());
    }
    for (uint64_t i : offsets) {
        mCommandBuffer.write(i);
    }

    return this;
}

CommandBuffer CommandBufferVk::bindIndexBuffer(Buffer buffer, uint32_t offset, IndexType::Enum indexType)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(buffer);
    auto *objP = dynamic_cast<Buffer_P *>(buffer);
    uint8_t cmdKey = CommandKey::BindIndexBuf;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(objP->idx());
    mCommandBuffer.write(offset);
    mCommandBuffer.write(indexType);

    return this;
}

CommandBuffer CommandBufferVk::draw(uint32_t vertexCount, uint32_t instanceCount,
                                    uint32_t firstVertex, uint32_t firstInstance)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::Draw;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(vertexCount);
    mCommandBuffer.write(instanceCount);
    mCommandBuffer.write(firstVertex);
    mCommandBuffer.write(firstInstance);

    return this;
}

CommandBuffer CommandBufferVk::drawIndexed(uint32_t indexCount, uint32_t instanceCount,
                                           uint32_t firstIndex, int32_t vertexOffset,
                                           uint32_t firstInstance)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::DrawIndexed;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(indexCount);
    mCommandBuffer.write(instanceCount);
    mCommandBuffer.write(firstIndex);
    mCommandBuffer.write(vertexOffset);
    mCommandBuffer.write(firstInstance);

    return this;
}

CommandBuffer CommandBufferVk::drawIndirect(Buffer buffer, uint32_t offset, uint32_t drawCount, uint32_t stride)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(buffer);
    auto *objP = dynamic_cast<Buffer_P *>(buffer);

    uint8_t cmdKey = CommandKey::DrawIndirect;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(objP->idx());
    mCommandBuffer.write(offset);
    mCommandBuffer.write(drawCount);
    mCommandBuffer.write(stride);

    return this;
}

CommandBuffer CommandBufferVk::drawIndexedIndirect(Buffer buffer, uint32_t offset, uint32_t drawCount, uint32_t stride)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(buffer);
    auto *objP = dynamic_cast<Buffer_P *>(buffer);

    uint8_t cmdKey = CommandKey::DrawIndexedIndirect;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(objP->idx());
    mCommandBuffer.write(offset);
    mCommandBuffer.write(drawCount);
    mCommandBuffer.write(stride);

    return this;
}

CommandBuffer CommandBufferVk::dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::Dispatch;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(groupCountX);
    mCommandBuffer.write(groupCountY);
    mCommandBuffer.write(groupCountZ);

    return this;
}

CommandBuffer CommandBufferVk::dispatchIndirect(Buffer buffer, uint32_t offset)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(buffer);
    auto *objP = dynamic_cast<Buffer_P *>(buffer);

    uint8_t cmdKey = CommandKey::DispatchIndirect;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(objP->idx());
    mCommandBuffer.write(offset);

    return this;
}

CommandBuffer CommandBufferVk::executeCommands(const std::vector<CommandBuffer> &secondaryCmdBuffers)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    if (secondaryCmdBuffers.empty()) {
        Log("CommandBufferVk::executeCommands secondaryCmdBuffers is empty");
        return this;
    }

    for (auto subBuffer : secondaryCmdBuffers) {
        auto *subBufferVk = dynamic_cast<CommandBufferVk *>(subBuffer);
        if (subBufferVk->mCommandBuffer.writePos() == 0) {
            continue;
        }
        if (subBufferVk->mIsBegun) {
            Log("CommandBufferVk::executeCommands secondary command buffer check end failure");
            continue;
        }
        uint8_t cmdKey;
        GByteArray &cmdBuffer = subBufferVk->mCommandBuffer;
        cmdBuffer.seekReadPos(SEEK_SET, 0);
        cmdBuffer.read(cmdKey);
        if (cmdKey != CommandKey::Begin) {
            Log("CommandBufferVk::executeCommands secondaryCmdBuffer head is not Begin command");
            continue;
        }
        mCommandBuffer.write(cmdBuffer.data() + cmdBuffer.readPos(),
                             cmdBuffer.writePos() - sizeof(uint8_t) - cmdBuffer.readPos());
        cmdBuffer.seekReadPos(SEEK_SET, cmdBuffer.writePos() - sizeof(uint8_t));
        cmdBuffer.read(cmdKey);
        GX_ASSERT_S(cmdKey == CommandKey::End, "secondary command buffer check end failure");
    }

    return this;
}

CommandBuffer CommandBufferVk::pipelineBarrier(PipelineStageMask srcStage, PipelineStageMask dstStage)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::PipelineBarrier;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcStage);
    mCommandBuffer.write(dstStage);

    return this;
}

CommandBuffer CommandBufferVk::bufferMemoryBarrier(Buffer buffer, const BufferBarrierInfo &barrierInfo)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(buffer);
    auto *bufferP = dynamic_cast<BufferVk *>(buffer);

    uint8_t cmdKey = CommandKey::BufferBarrier;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(bufferP->idx());
    mCommandBuffer.write(barrierInfo);

    return this;
}

CommandBuffer CommandBufferVk::imageMemoryBarrier(Texture texture,
                                                  ImageLayout::Enum srcLayout,
                                                  ImageLayout::Enum dstLayout,
                                                  const ImageSubResourceRange &range)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(texture);
    auto *textureP = dynamic_cast<TextureVk *>(texture);

    uint8_t cmdKey = CommandKey::ImageBarrier;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(textureP->idx());
    mCommandBuffer.write((uint8_t) srcLayout);
    mCommandBuffer.write((uint8_t) dstLayout);
    mCommandBuffer.write(range);

    return this;
}

CommandBuffer CommandBufferVk::copyBuffer(Buffer src, Buffer dst, uint64_t srcOffset, uint64_t dstOffset, uint64_t size)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(src);
    GX_ASSERT(dst);
    auto *srcP = dynamic_cast<BufferVk *>(src);
    auto *dstP = dynamic_cast<BufferVk *>(dst);

    uint8_t cmdKey = CommandKey::CopyBuffer;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcP->idx());
    mCommandBuffer.write(dstP->idx());
    mCommandBuffer.write(srcOffset);
    mCommandBuffer.write(dstOffset);
    mCommandBuffer.write(size);

    return this;
}

CommandBuffer CommandBufferVk::copyImage(Texture src, Texture dst, const std::vector<ImageCopyInfo> &copyInfos)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(src);
    GX_ASSERT(dst);
    auto *srcP = dynamic_cast<TextureVk *>(src);
    auto *dstP = dynamic_cast<TextureVk *>(dst);

    uint8_t cmdKey = CommandKey::CopyImage;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcP->idx());
    mCommandBuffer.write(dstP->idx());
    mCommandBuffer.write((uint32_t) copyInfos.size());
    if (!copyInfos.empty()) {
        for (auto &info : copyInfos) {
            mCommandBuffer.write(info);
        }
    }

    return this;
}

CommandBuffer CommandBufferVk::copyBufferToImage(Buffer src, Texture dst,
                                                 const std::vector<BufferImageCopyInfo> &copyInfos)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(src);
    GX_ASSERT(dst);
    auto *srcP = dynamic_cast<BufferVk *>(src);
    auto *dstP = dynamic_cast<TextureVk *>(dst);

    uint8_t cmdKey = CommandKey::CopyBufferToImage;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcP->idx());
    mCommandBuffer.write(dstP->idx());
    mCommandBuffer.write((uint32_t) copyInfos.size());
    if (!copyInfos.empty()) {
        for (auto &info : copyInfos) {
            mCommandBuffer.write(info);
        }
    }

    return this;
}

CommandBuffer CommandBufferVk::copyImageToBuffer(Texture src, Buffer dst,
                                                 const std::vector<BufferImageCopyInfo> &copyInfos)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(src);
    GX_ASSERT(dst);
    auto *srcP = dynamic_cast<TextureVk *>(src);
    auto *dstP = dynamic_cast<BufferVk *>(dst);

    uint8_t cmdKey = CommandKey::CopyImageToBuffer;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcP->idx());
    mCommandBuffer.write(dstP->idx());
    mCommandBuffer.write((uint32_t) copyInfos.size());
    if (!copyInfos.empty()) {
        for (auto &info : copyInfos) {
            mCommandBuffer.write(info);
        }
    }

    return this;
}

CommandBuffer CommandBufferVk::blitImage(Texture src, Texture dst, const std::vector<ImageBlitInfo> &blitInfos,
                                         BlitFilter::Enum filter)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(src);
    GX_ASSERT(dst);
    auto *srcP = dynamic_cast<TextureVk *>(src);
    auto *dstP = dynamic_cast<TextureVk *>(dst);

    uint8_t cmdKey = CommandKey::BlitImage;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcP->idx());
    mCommandBuffer.write(dstP->idx());
    mCommandBuffer.write((uint8_t) filter);
    mCommandBuffer.write((uint32_t) blitInfos.size());
    if (!blitInfos.empty()) {
        for (auto &info : blitInfos) {
            mCommandBuffer.write(info);
        }
    }

    return this;
}

CommandBuffer CommandBufferVk::copyRenderTarget(RenderTarget src, RenderTarget dst,
                                                const std::vector<ImageCopyInfo> &copyInfos,
                                                uint8_t frameIndex)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(src);
    GX_ASSERT(dst);
    auto *srcP = dynamic_cast<RenderTargetVk *>(src);
    auto *dstP = dynamic_cast<RenderTargetVk *>(dst);

    uint8_t cmdKey = CommandKey::CopyRT;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcP->idx());
    mCommandBuffer.write(dstP->idx());
    mCommandBuffer.write(frameIndex);
    mCommandBuffer.write((uint32_t) copyInfos.size());
    if (!copyInfos.empty()) {
        for (auto &info : copyInfos) {
            mCommandBuffer.write(info);
        }
    }

    return this;
}

CommandBuffer CommandBufferVk::blitRenderTarget(RenderTarget src, RenderTarget dst,
                                                const std::vector<ImageBlitInfo> &blitInfos,
                                                BlitFilter::Enum filter,
                                                uint8_t frameIndex)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(src);
    GX_ASSERT(dst);
    auto *srcP = dynamic_cast<RenderTargetVk *>(src);
    auto *dstP = dynamic_cast<RenderTargetVk *>(dst);

    uint8_t cmdKey = CommandKey::BlitRT;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcP->idx());
    mCommandBuffer.write(dstP->idx());
    mCommandBuffer.write((uint8_t) filter);
    mCommandBuffer.write(frameIndex);
    mCommandBuffer.write((uint32_t) blitInfos.size());
    if (!blitInfos.empty()) {
        for (auto &info : blitInfos) {
            mCommandBuffer.write(info);
        }
    }

    return this;
}

CommandBuffer CommandBufferVk::blitRenderTargetToImage(RenderTarget src, Texture dst,
                                                       const std::vector<ImageBlitInfo> &blitInfos,
                                                       BlitFilter::Enum filter,
                                                       uint8_t attachIndex,
                                                       uint8_t frameIndex)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(src);
    GX_ASSERT(dst);

    auto *srcP = dynamic_cast<RenderTargetVk *>(src);
    auto *dstP = dynamic_cast<TextureVk *>(dst);

    uint8_t cmdKey = CommandKey::BlitRTToImage;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcP->idx());
    mCommandBuffer.write(dstP->idx());
    mCommandBuffer.write((uint8_t) filter);
    mCommandBuffer.write(attachIndex);
    mCommandBuffer.write(frameIndex);
    mCommandBuffer.write((uint32_t) blitInfos.size());
    if (!blitInfos.empty()) {
        for (auto &info : blitInfos) {
            mCommandBuffer.write(info);
        }
    }

    return this;
}

CommandBuffer CommandBufferVk::copyRenderTargetToImage(RenderTarget src, Texture dst,
                                                       const std::vector<ImageCopyInfo> &copyInfos,
                                                       uint8_t attachIndex,
                                                       uint8_t frameIndex)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(src);
    GX_ASSERT(dst);

    auto *srcP = dynamic_cast<RenderTargetVk *>(src);
    auto *dstP = dynamic_cast<TextureVk *>(dst);

    uint8_t cmdKey = CommandKey::CopyRTToImage;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcP->idx());
    mCommandBuffer.write(dstP->idx());
    mCommandBuffer.write(attachIndex);
    mCommandBuffer.write(frameIndex);
    mCommandBuffer.write((uint32_t) copyInfos.size());
    if (!copyInfos.empty()) {
        for (auto &info : copyInfos) {
            mCommandBuffer.write(info);
        }
    }

    return this;
}

CommandBuffer CommandBufferVk::copyRenderTargetToBuffer(RenderTarget src, Buffer dst,
                                                        const std::vector<BufferImageCopyInfo> &copyInfos,
                                                        uint8_t attachIndex,
                                                        uint8_t frameIndex)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    GX_ASSERT(src);
    GX_ASSERT(dst);

    auto *srcP = dynamic_cast<RenderTargetVk *>(dst);
    auto *dstP = dynamic_cast<BufferVk *>(dst);

    uint8_t cmdKey = CommandKey::CopyRTToBuffer;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(srcP->idx());
    mCommandBuffer.write(dstP->idx());
    mCommandBuffer.write(attachIndex);
    mCommandBuffer.write(frameIndex);
    mCommandBuffer.write((uint32_t) copyInfos.size());
    if (!copyInfos.empty()) {
        for (auto &info : copyInfos) {
            mCommandBuffer.write(info);
        }
    }

    return this;
}

CommandBuffer CommandBufferVk::fillBuffer(Buffer buffer, uint64_t offset, uint64_t size, uint32_t data)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");
    GX_ASSERT(buffer);

    auto *bufferP = dynamic_cast<BufferVk *>(buffer);

    uint8_t cmdKey = CommandKey::FillBuffer;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(bufferP->idx());
    mCommandBuffer.write(offset);
    mCommandBuffer.write(size);
    mCommandBuffer.write(data);

    return this;
}

CommandBuffer CommandBufferVk::beginDebugLabel(const std::string &label)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::BeginDebug;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(label);

    return this;
}

CommandBuffer CommandBufferVk::endDebugLabel()
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");

    uint8_t cmdKey = CommandKey::EndDebug;
    mCommandBuffer.write(cmdKey);

    return this;
}

CommandBuffer CommandBufferVk::resetQuery(Query query, uint32_t firstQuery, uint32_t queryCount)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");
    GX_ASSERT(query);

    auto *queryP = dynamic_cast<QueryVk *>(query);

    uint8_t cmdKey = CommandKey::ResetQuery;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(queryP->idx());
    mCommandBuffer.write(firstQuery);
    mCommandBuffer.write(queryCount);

    return this;
}

CommandBuffer CommandBufferVk::beginQuery(Query query, uint32_t queryIndex, bool precise)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");
    GX_ASSERT(query);

    auto *queryP = dynamic_cast<QueryVk *>(query);

    uint8_t cmdKey = CommandKey::BeginQuery;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(queryP->idx());
    mCommandBuffer.write(queryIndex);
    mCommandBuffer.write(precise);

    return this;
}

CommandBuffer CommandBufferVk::endQuery(Query query, uint32_t queryIndex)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");
    GX_ASSERT(query);

    auto *queryP = dynamic_cast<QueryVk *>(query);

    uint8_t cmdKey = CommandKey::EndQuery;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(queryP->idx());
    mCommandBuffer.write(queryIndex);

    return this;
}

CommandBuffer CommandBufferVk::writeTimestamp(PipelineStage::Enum pipelineStage, Query query, uint32_t queryIndex)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");
    GX_ASSERT(query);

    auto *queryP = dynamic_cast<QueryVk *>(query);

    uint8_t cmdKey = CommandKey::WriteTimestamp;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write((uint32_t)pipelineStage);
    mCommandBuffer.write(queryP->idx());
    mCommandBuffer.write(queryIndex);

    return this;
}

CommandBuffer CommandBufferVk::copyQueryResults(Query query,
                                                uint32_t firstQuery,
                                                uint32_t queryCount,
                                                Buffer dstBuffer,
                                                uint64_t dstOffset,
                                                QueryResultFlags resultFlags)
{
    GX_ASSERT_S(mIsBegun, "Please call begin first");
    GX_ASSERT(query);
    GX_ASSERT(dstBuffer);

    auto *queryP = dynamic_cast<QueryVk *>(query);
    auto *bufferP = dynamic_cast<BufferVk *>(dstBuffer);

    uint8_t cmdKey = CommandKey::CopyQueryResults;
    mCommandBuffer.write(cmdKey);
    mCommandBuffer.write(queryP->idx());
    mCommandBuffer.write(firstQuery);
    mCommandBuffer.write(queryCount);
    mCommandBuffer.write(bufferP->idx());
    mCommandBuffer.write(dstOffset);
    mCommandBuffer.write(resultFlags);

    return this;
}

void CommandBufferVk::resetCommandBuffer()
{
    mCommandBuffer.reset();
    mIsCompiled = false;
}

void CommandBufferVk::compileCommand(FrameVk *frame)
{
//    Log("CommandBufferVk::compileCommand");
    if (mCommandBuffer.writePos() == 0) {
        return;
    }

    GX_ASSERT_S(!mIsBegun, "Call end first to finish writing to the command buffer");

    if (mIsBegun) {
        return;
    }

    ContextVk *contextVk = dynamic_cast<ContextVk *>(mContextT->contextP());

    uint8_t cmdKey;
    for (uint32_t i = 0; i < mVkCommandBuffers.size(); i++) {
        auto vkCmdBuf = mVkCommandBuffers[i];
        VkClearValue clearColor{};
        VkClearValue depthStencil{};

        uint32_t frameIndex =
                mVkCommandBuffers.size() == 1
                ? (frame == GFX_NULL_HANDLE
                   ? i : frame->currentFrameIndex()) : i;

        std::string debugLabel;

        RenderTargetVk *renderTargetVk = nullptr;
        PipelineLayoutVk *pipelineLayout = nullptr;
        PipelineVk *graphPipeline = nullptr;
        PipelineVk *computePipeline = nullptr;

        CreateGraphicsPipelineStateInfo createGraphPipelineInfo{};
        CreateComputePipelineStateInfo createComputePipelineInfo{};

        mCommandBuffer.seekReadPos(SEEK_SET, 0);
        do {
            mCommandBuffer.read(cmdKey);

            switch (cmdKey) {
                case CommandKey::Begin: {
                    VkCommandBufferBeginInfo cmdBufferBeginInfo{};
                    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

                    vkBeginCommandBuffer(vkCmdBuf, &cmdBufferBeginInfo);
                }
                    break;
                case CommandKey::End: {
                    vkEndCommandBuffer(vkCmdBuf);
                }
                    break;
                case CommandKey::SetClearColor: {
                    ClearColor cc{};
                    mCommandBuffer.read(cc);
                    clearColor.color.float32[0] = cc.r;
                    clearColor.color.float32[1] = cc.g;
                    clearColor.color.float32[2] = cc.b;
                    clearColor.color.float32[3] = cc.a;
                }
                    break;
                case CommandKey::SetClearDepSte: {
                    float depth;
                    uint32_t stencil;
                    mCommandBuffer.read(depth);
                    mCommandBuffer.read(stencil);
                    depthStencil.depthStencil.depth = depth;
                    depthStencil.depthStencil.stencil = stencil;
                }
                    break;
                case CommandKey::BindRenderTarget: {
                    GfxIdxTy idx;
                    mCommandBuffer.read(idx);
                    auto *obj = contextVk->findRenderTargetP(idx);
                    GX_ASSERT_S(obj, "CommandBufferVk::compileCommand can not find RenderTarget from idx = %d", idx);
                    renderTargetVk = dynamic_cast<RenderTargetVk *>(obj);
                }
                    break;
                case CommandKey::BeginRenderPass: {
                    GX_ASSERT_S(renderTargetVk != nullptr, "CommandBufferVk::compileCommand we need to bind render target first");

                    Rect2D renderArea{};
                    RenderPassInfo renderPassInfo{};

                    mCommandBuffer.read(renderArea);
                    readRenderPassInfo(mCommandBuffer, renderPassInfo);

                    uint32_t rtWidth = renderTargetVk->width();
                    uint32_t rtHeight = renderTargetVk->height();

                    RenderPassVk *renderPass = renderTargetVk->getRenderPass(renderPassInfo);

                    renderArea.x = std::clamp(renderArea.x, 0, (int32_t)rtWidth);
                    renderArea.y = std::clamp(renderArea.y, 0, (int32_t)rtHeight);
                    renderArea.width = std::clamp(renderArea.width, 0u, (uint32_t)(rtWidth - renderArea.x));
                    renderArea.height = std::clamp(renderArea.height, 0u, (uint32_t)(rtHeight - renderArea.y));

                    std::vector<VkClearValue> clearValues;
                    clearValues.reserve(renderPass->colorAttachmentCount()
                                        + renderTargetVk->hasDepthStencilAttachment());
                    for (uint32_t x = 0; x < renderPass->colorAttachmentCount(); x++) {
                        clearValues.push_back(clearColor);
                    }
                    if (renderTargetVk->hasDepthStencilAttachment()) {
                        clearValues.push_back(depthStencil);
                    }

                    VkRenderPassBeginInfo renderPassBeginInfo{};
                    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                    renderPassBeginInfo.renderPass = *renderPass->vkRenderPass();
                    renderPassBeginInfo.renderArea = {{renderArea.x,     renderArea.y},
                                                      {renderArea.width, renderArea.height}};
                    renderPassBeginInfo.clearValueCount = clearValues.size();
                    renderPassBeginInfo.pClearValues = clearValues.data();
                    renderPassBeginInfo.framebuffer = *(renderTargetVk->getVkFrameBuffer(renderPass, frameIndex));

                    vkCmdBeginRenderPass(vkCmdBuf, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

                    createGraphPipelineInfo.subpassIndex = 0;
                    createGraphPipelineInfo.renderPass = renderPass->idx();
                    graphPipeline = nullptr;
                }
                    break;
                case CommandKey::EndRenderPass: {
                    vkCmdEndRenderPass(vkCmdBuf);

                    createGraphPipelineInfo.renderPass = 0;
                    graphPipeline = nullptr;
                }
                    break;
                case CommandKey::SetGraphPipelineState: {
                    mCommandBuffer.read(createGraphPipelineInfo.stateInfo);
                    graphPipeline = nullptr;
                }
                    break;
                case CommandKey::SetVertexLayout: {
                    mCommandBuffer.read(createGraphPipelineInfo.vertexLayout);
                    graphPipeline = nullptr;
                }
                    break;
                case CommandKey::SetShaders: {
                    uint32_t size;
                    GfxIdxTy idx;

                    mCommandBuffer.read(size);
                    createGraphPipelineInfo.shaderPrograms.resize(size);
                    createComputePipelineInfo.shaderPrograms.resize(size);
                    for (uint32_t k = 0; k < size; k++) {
                        mCommandBuffer.read(idx);
                        auto *shaderP = contextVk->findShaderP(idx);
                        createGraphPipelineInfo.shaderPrograms[k] = shaderP;
                        createComputePipelineInfo.shaderPrograms[k] = shaderP;
                    }

                    graphPipeline = nullptr;
                    computePipeline = nullptr;
                }
                    break;
                case CommandKey::NextSubpass: {
                    createGraphPipelineInfo.subpassIndex++;
                    graphPipeline = nullptr;

                    vkCmdNextSubpass(vkCmdBuf, VK_SUBPASS_CONTENTS_INLINE);
                }
                    break;
                case CommandKey::SetViewport: {
                    Rect2D viewport{};
                    float minDepth;
                    float maxDepth;

                    mCommandBuffer.read(viewport);
                    mCommandBuffer.read(minDepth);
                    mCommandBuffer.read(maxDepth);

                    VkViewport vkViewport = {(float) viewport.x, (float) viewport.y,
                                             (float) viewport.width, (float) viewport.height,
                                             minDepth, maxDepth};
                    vkCmdSetViewport(vkCmdBuf, 0, 1, &vkViewport);
                }
                    break;
                case CommandKey::SetScissor: {
                    Rect2D scissor{};
                    mCommandBuffer.read(scissor);
                    VkRect2D vkScissor = {{scissor.x,     scissor.y},
                                          {scissor.width, scissor.height}};
                    vkCmdSetScissor(vkCmdBuf, 0, 1, &vkScissor);
                }
                    break;
                case CommandKey::SetLineWidth: {
                    float lineWidth;
                    mCommandBuffer.read(lineWidth);
                    vkCmdSetLineWidth(vkCmdBuf, lineWidth);
                }
                    break;
                case CommandKey::SetStencilCompMask: {
                    uint8_t face;
                    uint32_t mask;
                    mCommandBuffer.read(face);
                    mCommandBuffer.read(mask);

                    auto faceEnum = (StencilFace::Enum) face;

                    if (faceEnum == StencilFace::Front || faceEnum == StencilFace::FrontAndBack) {
                        createGraphPipelineInfo.frontMR.compareMask = mask;
                    }
                    if (faceEnum == StencilFace::Back || faceEnum == StencilFace::FrontAndBack) {
                        createGraphPipelineInfo.backMR.compareMask = mask;
                    }
                    graphPipeline = nullptr;
                }
                    break;
                case CommandKey::SetStencilWriteMask: {
                    uint8_t face;
                    uint32_t mask;
                    mCommandBuffer.read(face);
                    mCommandBuffer.read(mask);

                    auto faceEnum = (StencilFace::Enum) face;

                    if (faceEnum == StencilFace::Front || faceEnum == StencilFace::FrontAndBack) {
                        createGraphPipelineInfo.frontMR.writeMask = mask;
                    }
                    if (faceEnum == StencilFace::Back || faceEnum == StencilFace::FrontAndBack) {
                        createGraphPipelineInfo.backMR.writeMask = mask;
                    }
                    graphPipeline = nullptr;
                }
                    break;
                case CommandKey::SetStencilReference: {
                    uint8_t face;
                    uint32_t reference;
                    mCommandBuffer.read(face);
                    mCommandBuffer.read(reference);

                    auto faceEnum = (StencilFace::Enum) face;

                    if (faceEnum == StencilFace::Front || faceEnum == StencilFace::FrontAndBack) {
                        createGraphPipelineInfo.frontMR.reference = reference;
                    }
                    if (faceEnum == StencilFace::Back || faceEnum == StencilFace::FrontAndBack) {
                        createGraphPipelineInfo.backMR.reference = reference;
                    }
                    graphPipeline = nullptr;
                }
                    break;
                case CommandKey::BindDescSet: {
                    uint8_t bindPoint;
                    uint32_t binderSize;
                    std::vector<VkDescriptorSet> vkDescSets;
                    PipelineLayoutInfo pipelineLayoutInfo;
                    uint32_t dynamicOffsetSize;
                    std::vector<uint32_t> dynamicOffsets;

                    mCommandBuffer.read(bindPoint);
                    mCommandBuffer.read(binderSize);
                    if (binderSize > 0) {
                        vkDescSets.resize(binderSize);
                        pipelineLayoutInfo.layoutInfos.resize(binderSize);
                        for (uint32_t x = 0; x < binderSize; x++) {
                            GfxIdxTy idx;
                            mCommandBuffer.read(idx);
                            ResourceBinder obj = contextVk->findResourceBinderP(idx);
                            GX_ASSERT_S(obj, "CommandBufferVk::compileCommand can not find ResourceBinder from idx = %d",
                                        idx);
                            auto *objP = dynamic_cast<ResourceBinderVk *>(obj);
                            objP->bindResources();

                            vkDescSets[x] = objP->getVkDescriptorSet();
                            pipelineLayoutInfo.layoutInfos[x] = objP->getDescriptorLayoutInfo();
                        }
                    }
                    mCommandBuffer.read(dynamicOffsetSize);
                    if (dynamicOffsetSize > 0) {
                        dynamicOffsets.resize(dynamicOffsetSize);
                        for (uint32_t x = 0; x < dynamicOffsetSize; x++) {
                            mCommandBuffer.read(dynamicOffsets[x]);
                        }
                    }

                    pipelineLayout = contextVk->getPipelineLayout(pipelineLayoutInfo);

                    GX_ASSERT_S(pipelineLayout != nullptr, "Get pipeline layout failure");
                    if (createGraphPipelineInfo.pipelineLayout != pipelineLayout->idx()) {
                        createGraphPipelineInfo.pipelineLayout = pipelineLayout->idx();
                        graphPipeline = nullptr;
                    }
                    if (createComputePipelineInfo.pipelineLayout != pipelineLayout->idx()) {
                        createComputePipelineInfo.pipelineLayout = pipelineLayout->idx();
                        computePipeline = nullptr;
                    }

                    vkCmdBindDescriptorSets(
                            vkCmdBuf,
                            toVkPipelineBindPoint((ResourceBindPoint::Enum) bindPoint),
                            pipelineLayout->getVkPipelineLayout(),
                            0, vkDescSets.size(), vkDescSets.data(),
                            dynamicOffsets.size(), dynamicOffsets.data());
                }
                    break;
                case CommandKey::BindVertexBuf: {
                    uint32_t firstBinding;
                    uint8_t size;
                    std::vector<VkBuffer> buffers;
                    std::vector<VkDeviceSize> offsets;

                    mCommandBuffer.read(firstBinding);
                    mCommandBuffer.read(size);
                    GX_ASSERT(size > 0);
                    buffers.resize(size);
                    offsets.resize(size);
                    for (uint32_t x = 0; x < size; x++) {
                        GfxIdxTy idx;
                        mCommandBuffer.read(idx);
                        Buffer obj = contextVk->findBufferP(idx);
                        GX_ASSERT_S(obj, "CommandBufferVk::compileCommand can not find VertexBuffer from idx = %d",
                                    idx);
                        auto *objP = dynamic_cast<BufferVk *>(obj);
                        buffers[x] = objP->vkBuffer();
                    }
                    for (uint32_t x = 0; x < size; x++) {
                        uint64_t offset;
                        mCommandBuffer.read(offset);
                        offsets[x] = (VkDeviceSize) offset;
                    }

                    vkCmdBindVertexBuffers(vkCmdBuf, firstBinding, buffers.size(), buffers.data(), offsets.data());
                }
                    break;
                case CommandKey::BindIndexBuf: {
                    GfxIdxTy idx;
                    uint32_t offset;
                    IndexType::Enum indexType;
                    mCommandBuffer.read(idx);
                    mCommandBuffer.read(offset);
                    mCommandBuffer.read(indexType);
                    Buffer obj = contextVk->findBufferP(idx);
                    GX_ASSERT_S(obj, "CommandBufferVk::compileCommand can not find IndexBuffer from idx = %d", idx);
                    auto *objP = dynamic_cast<BufferVk *>(obj);
                    VkBuffer vkBuffer = objP->vkBuffer();
                    VkDeviceSize vkOffset = offset;

                    vkCmdBindIndexBuffer(vkCmdBuf, vkBuffer, vkOffset, toVkIndexType(indexType));
                }
                    break;
                case CommandKey::Draw: {
                    uint32_t vertexCount;
                    uint32_t instanceCount;
                    uint32_t firstVertex;
                    uint32_t firstInstance;
                    mCommandBuffer.read(vertexCount);
                    mCommandBuffer.read(instanceCount);
                    mCommandBuffer.read(firstVertex);
                    mCommandBuffer.read(firstInstance);

                    computePipeline = nullptr;
                    graphPipeline = bindGraphPipeline(contextVk, vkCmdBuf, createGraphPipelineInfo, graphPipeline);
                    GX_ASSERT_S(graphPipeline != nullptr, "bind graphics pipeline failure");

                    vkCmdDraw(vkCmdBuf, vertexCount, instanceCount, firstVertex, firstInstance);
                }
                    break;
                case CommandKey::DrawIndexed: {
                    uint32_t indexCount;
                    uint32_t instanceCount;
                    uint32_t firstIndex;
                    int32_t vertexOffset;
                    uint32_t firstInstance;
                    mCommandBuffer.read(indexCount);
                    mCommandBuffer.read(instanceCount);
                    mCommandBuffer.read(firstIndex);
                    mCommandBuffer.read(vertexOffset);
                    mCommandBuffer.read(firstInstance);

                    computePipeline = nullptr;
                    graphPipeline = bindGraphPipeline(contextVk, vkCmdBuf, createGraphPipelineInfo, graphPipeline);
                    GX_ASSERT_S(graphPipeline != nullptr, "bind graphics pipeline failure");

                    vkCmdDrawIndexed(vkCmdBuf, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
                }
                    break;
                case CommandKey::DrawIndirect: {
                    GfxIdxTy idx;
                    uint32_t offset;
                    uint32_t drawCount;
                    uint32_t stride;

                    mCommandBuffer.read(idx);
                    mCommandBuffer.read(offset);
                    mCommandBuffer.read(drawCount);
                    mCommandBuffer.read(stride);

                    Buffer obj = contextVk->findBufferP(idx);
                    GX_ASSERT_S(obj, "CommandBufferVk::compileCommand can not find buffer from idx = %d", idx);
                    auto *objP = dynamic_cast<BufferVk *>(obj);
                    VkBuffer vkBuffer = objP->vkBuffer();
                    VkDeviceSize vkOffset = offset;

                    computePipeline = nullptr;
                    graphPipeline = bindGraphPipeline(contextVk, vkCmdBuf, createGraphPipelineInfo, graphPipeline);
                    GX_ASSERT_S(graphPipeline != nullptr, "bind graphics pipeline failure");

                    vkCmdDrawIndirect(vkCmdBuf, vkBuffer, vkOffset, drawCount, stride);
                }
                    break;
                case CommandKey::DrawIndexedIndirect: {
                    GfxIdxTy idx;
                    uint32_t offset;
                    uint32_t drawCount;
                    uint32_t stride;

                    mCommandBuffer.read(idx);
                    mCommandBuffer.read(offset);
                    mCommandBuffer.read(drawCount);
                    mCommandBuffer.read(stride);

                    Buffer obj = contextVk->findBufferP(idx);
                    GX_ASSERT_S(obj, "CommandBufferVk::compileCommand can not find buffer from idx = %d", idx);
                    auto *objP = dynamic_cast<BufferVk *>(obj);
                    VkBuffer vkBuffer = objP->vkBuffer();
                    VkDeviceSize vkOffset = offset;

                    computePipeline = nullptr;
                    graphPipeline = bindGraphPipeline(contextVk, vkCmdBuf, createGraphPipelineInfo, graphPipeline);
                    GX_ASSERT_S(graphPipeline != nullptr, "bind graphics pipeline failure");

                    vkCmdDrawIndexedIndirect(vkCmdBuf, vkBuffer, vkOffset, drawCount, stride);
                }
                    break;
                case CommandKey::Dispatch: {
                    uint32_t groupCountX;
                    uint32_t groupCountY;
                    uint32_t groupCountZ;
                    mCommandBuffer.read(groupCountX);
                    mCommandBuffer.read(groupCountY);
                    mCommandBuffer.read(groupCountZ);

                    graphPipeline = nullptr;
                    computePipeline = bindComputePipeline(contextVk, vkCmdBuf, createComputePipelineInfo,
                                                          computePipeline);
                    GX_ASSERT_S(computePipeline != nullptr, "bind compute pipeline failure");

                    vkCmdDispatch(vkCmdBuf, groupCountX, groupCountY, groupCountZ);
                }
                    break;
                case CommandKey::DispatchIndirect: {
                    GfxIdxTy idx;
                    uint32_t offset;

                    mCommandBuffer.read(idx);
                    mCommandBuffer.read(offset);

                    Buffer obj = contextVk->findBufferP(idx);
                    GX_ASSERT_S(obj, "CommandBufferVk::compileCommand can not find buffer from idx = %d", idx);
                    auto *objP = dynamic_cast<BufferVk *>(obj);
                    VkBuffer vkBuffer = objP->vkBuffer();
                    VkDeviceSize vkOffset = offset;

                    graphPipeline = nullptr;
                    computePipeline = bindComputePipeline(contextVk, vkCmdBuf, createComputePipelineInfo,
                                                          computePipeline);
                    GX_ASSERT_S(computePipeline != nullptr, "bind compute pipeline failure");

                    vkCmdDispatchIndirect(vkCmdBuf, vkBuffer, vkOffset);
                }
                    break;
                case CommandKey::PipelineBarrier: {
                    PipelineStageMask srcStage;
                    PipelineStageMask dstStage;

                    mCommandBuffer.read(srcStage);
                    mCommandBuffer.read(dstStage);

                    vkCmdPipelineBarrier(
                            vkCmdBuf,
                            toVkPipelineStageFlags(srcStage),
                            toVkPipelineStageFlags(dstStage),
                            0,
                            0, nullptr,
                            0, nullptr,
                            0, nullptr);
                }
                    break;
                case CommandKey::BufferBarrier: {
                    GfxIdxTy idx;
                    BufferBarrierInfo barrierInfo{};

                    mCommandBuffer.read(idx);
                    mCommandBuffer.read(barrierInfo);

                    Buffer buffer = contextVk->findBufferP(idx);
                    GX_ASSERT_S(buffer, "CommandBufferVk::compileCommand can not find buffer from idx = %d", idx);
                    auto *bufferP = dynamic_cast<BufferVk *>(buffer);

                    VkBufferMemoryBarrier bufferBarrier{};
                    bufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
                    bufferBarrier.buffer = bufferP->vkBuffer();
                    bufferBarrier.size = bufferP->size();
                    bufferBarrier.srcAccessMask = toVkAccessFlags(barrierInfo.srcAccess);
                    bufferBarrier.dstAccessMask = toVkAccessFlags(barrierInfo.dstAccess);
                    bufferBarrier.srcQueueFamilyIndex = contextVk->getQueueIndex(barrierInfo.srcQueue);
                    bufferBarrier.dstQueueFamilyIndex = contextVk->getQueueIndex(barrierInfo.dstQueue);

                    vkCmdPipelineBarrier(
                            vkCmdBuf,
                            toVkPipelineStageFlags(barrierInfo.srcStage),
                            toVkPipelineStageFlags(barrierInfo.dstStage),
                            0,
                            0, nullptr,
                            1, &bufferBarrier,
                            0, nullptr);
                }
                    break;
                case CommandKey::ImageBarrier: {
                    GfxIdxTy idx;
                    uint8_t srcLayout;
                    uint8_t dstLayout;
                    ImageSubResourceRange subResRange{};

                    mCommandBuffer.read(idx);
                    mCommandBuffer.read(srcLayout);
                    mCommandBuffer.read(dstLayout);
                    mCommandBuffer.read(subResRange);

                    Texture texture = contextVk->findTextureP(idx);
                    GX_ASSERT_S(texture, "CommandBufferVk::compileCommand can not find src texture from idx = %lld",
                                idx);
                    auto *textureP = dynamic_cast<TextureVk *>(texture);

                    textureP->imageMemoryBarrier(vkCmdBuf, (ImageLayout::Enum) srcLayout, (ImageLayout::Enum) dstLayout,
                                                 subResRange);
                }
                    break;
                case CommandKey::CopyBuffer: {
                    GfxIdxTy srcIdx;
                    GfxIdxTy dstIdx;
                    uint64_t srcOffset;
                    uint64_t dstOffset;
                    uint64_t size;

                    mCommandBuffer.read(srcIdx);
                    mCommandBuffer.read(dstIdx);
                    mCommandBuffer.read(srcOffset);
                    mCommandBuffer.read(dstOffset);
                    mCommandBuffer.read(size);

                    Buffer srcBuffer = contextVk->findBufferP(srcIdx);
                    GX_ASSERT_S(srcBuffer, "CommandBufferVk::compileCommand can not find src buffer from idx = %lld",
                                srcIdx);
                    auto *srcBufP = dynamic_cast<BufferVk *>(srcBuffer);

                    Buffer dstBuffer = contextVk->findBufferP(dstIdx);
                    GX_ASSERT_S(dstBuffer, "CommandBufferVk::compileCommand can not find dst buffer from idx = %lld",
                                dstIdx);
                    auto *dstBufP = dynamic_cast<BufferVk *>(dstBuffer);

                    VkBufferCopy copyRegion{};
                    copyRegion.srcOffset = srcOffset;
                    copyRegion.dstOffset = dstOffset;

                    if (size == GFX_WHOLE_SIZE)
                        copyRegion.size = std::min(dstBufP->size() - dstOffset, srcBufP->size() - srcOffset);
                    else
                        copyRegion.size = size;
                    vkCmdCopyBuffer(vkCmdBuf, srcBufP->vkBuffer(), dstBufP->vkBuffer(), 1, &copyRegion);
                }
                    break;
                case CommandKey::CopyImage: {
                    GfxIdxTy srcIdx;
                    GfxIdxTy dstIdx;
                    uint32_t copyInfoSize;
                    std::vector<ImageCopyInfo> copyInfos;
                    mCommandBuffer.read(srcIdx);
                    mCommandBuffer.read(dstIdx);
                    mCommandBuffer.read(copyInfoSize);

                    auto *src = contextVk->findTextureP(srcIdx);
                    GX_ASSERT_S(src, "CommandBufferVk::compileCommand can not find src texture from idx = %lld",
                                srcIdx);
                    auto *srcP = dynamic_cast<TextureVk *>(src);

                    auto *dst = contextVk->findTextureP(dstIdx);
                    GX_ASSERT_S(dst, "CommandBufferVk::compileCommand can not find dst texture from idx = %lld",
                                dstIdx);
                    auto *dstP = dynamic_cast<TextureVk *>(dst);

                    if (copyInfoSize > 0) {
                        copyInfos.resize(copyInfoSize);
                        for (uint32_t x = 0; x < copyInfoSize; x++) {
                            mCommandBuffer.read(copyInfos[x]);
                        }
                    }

                    doCopyImage(vkCmdBuf, srcP->vkImage(), dstP->vkImage(), copyInfos);
                }
                    break;
                case CommandKey::CopyBufferToImage: {
                    GfxIdxTy srcIdx;
                    GfxIdxTy dstIdx;
                    uint32_t copyInfoSize;
                    std::vector<VkBufferImageCopy> vkCopyInfos;
                    mCommandBuffer.read(srcIdx);
                    mCommandBuffer.read(dstIdx);
                    mCommandBuffer.read(copyInfoSize);

                    auto *src = contextVk->findBufferP(srcIdx);
                    GX_ASSERT_S(src, "CommandBufferVk::compileCommand can not find src buffer from idx = %d", srcIdx);
                    auto *srcP = dynamic_cast<BufferVk *>(src);

                    auto *dst = contextVk->findTextureP(dstIdx);
                    GX_ASSERT_S(dst, "CommandBufferVk::compileCommand can not find dst texture from idx = %d", dstIdx);
                    auto *dstP = dynamic_cast<TextureVk *>(dst);

                    if (copyInfoSize > 0) {
                        vkCopyInfos.resize(copyInfoSize);
                        BufferImageCopyInfo tempInfo{};
                        for (uint32_t x = 0; x < copyInfoSize; x++) {
                            mCommandBuffer.read(tempInfo);
                            vkCopyInfos[x].bufferOffset = tempInfo.bufferOffset;
                            vkCopyInfos[x].bufferRowLength = tempInfo.bufferRowLength;
                            vkCopyInfos[x].bufferImageHeight = tempInfo.bufferImageHeight;
                            vkCopyInfos[x].imageSubresource.aspectMask =
                                    tempInfo.aspectMask == 0 ? toVkAspectFlags(dstP->aspect())
                                                             : toVkAspectFlags(tempInfo.aspectMask);
                            vkCopyInfos[x].imageSubresource.mipLevel = tempInfo.mipLevel;
                            vkCopyInfos[x].imageSubresource.baseArrayLayer = tempInfo.baseArrayLayer;
                            vkCopyInfos[x].imageSubresource.layerCount =
                                    tempInfo.layerCount == 0 ? 1 : tempInfo.layerCount;
                            vkCopyInfos[x].imageOffset.x = tempInfo.imageOffsetX;
                            vkCopyInfos[x].imageOffset.y = tempInfo.imageOffsetY;
                            vkCopyInfos[x].imageOffset.z = tempInfo.imageOffsetZ;
                            vkCopyInfos[x].imageExtent.width = tempInfo.imageWidth;
                            vkCopyInfos[x].imageExtent.height = tempInfo.imageHeight;
                            vkCopyInfos[x].imageExtent.depth = tempInfo.imageDepth;
                        }
                    }

                    vkCmdCopyBufferToImage(
                            vkCmdBuf,
                            srcP->vkBuffer(),
                            *(dstP->vkImage()),
                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                            vkCopyInfos.size(),
                            vkCopyInfos.data());
                }
                    break;
                case CommandKey::CopyImageToBuffer: {
                    GfxIdxTy srcIdx;
                    GfxIdxTy dstIdx;
                    uint32_t copyInfoSize;
                    std::vector<VkBufferImageCopy> vkCopyInfos;
                    mCommandBuffer.read(srcIdx);
                    mCommandBuffer.read(dstIdx);
                    mCommandBuffer.read(copyInfoSize);

                    auto *src = contextVk->findTextureP(srcIdx);
                    GX_ASSERT_S(src, "CommandBufferVk::compileCommand can not find src texture from idx = %d", srcIdx);
                    auto *srcP = dynamic_cast<TextureVk *>(src);

                    auto *dst = contextVk->findBufferP(dstIdx);
                    GX_ASSERT_S(dst, "CommandBufferVk::compileCommand can not find dst buffer from idx = %d", dstIdx);
                    auto *dstP = dynamic_cast<BufferVk *>(dst);

                    if (copyInfoSize > 0) {
                        vkCopyInfos.resize(copyInfoSize);
                        BufferImageCopyInfo tempInfo{};
                        for (uint32_t x = 0; x < copyInfoSize; x++) {
                            mCommandBuffer.read(tempInfo);
                            vkCopyInfos[x].bufferOffset = tempInfo.bufferOffset;
                            vkCopyInfos[x].bufferRowLength = tempInfo.bufferRowLength;
                            vkCopyInfos[x].bufferImageHeight = tempInfo.bufferImageHeight;
                            vkCopyInfos[x].imageSubresource.aspectMask =
                                    tempInfo.aspectMask == 0 ? toVkAspectFlags(srcP->aspect())
                                                             : toVkAspectFlags(tempInfo.aspectMask);
                            vkCopyInfos[x].imageSubresource.mipLevel = tempInfo.mipLevel;
                            vkCopyInfos[x].imageSubresource.baseArrayLayer = tempInfo.baseArrayLayer;
                            vkCopyInfos[x].imageSubresource.layerCount =
                                    tempInfo.layerCount == 0 ? 1 : tempInfo.layerCount;
                            vkCopyInfos[x].imageOffset.x = tempInfo.imageOffsetX;
                            vkCopyInfos[x].imageOffset.y = tempInfo.imageOffsetY;
                            vkCopyInfos[x].imageOffset.z = tempInfo.imageOffsetZ;
                            vkCopyInfos[x].imageExtent.width = tempInfo.imageWidth;
                            vkCopyInfos[x].imageExtent.height = tempInfo.imageHeight;
                            vkCopyInfos[x].imageExtent.depth = tempInfo.imageDepth;
                        }
                    }

                    vkCmdCopyImageToBuffer(
                            vkCmdBuf,
                            *(srcP->vkImage()),
                            srcP->vkImage()->layout(),
                            dstP->vkBuffer(),
                            vkCopyInfos.size(),
                            vkCopyInfos.data());
                }
                    break;
                case CommandKey::BlitImage: {
                    GfxIdxTy srcIdx;
                    GfxIdxTy dstIdx;
                    uint8_t filter;
                    uint32_t blitInfoSize;
                    std::vector<ImageBlitInfo> blitInfos;
                    mCommandBuffer.read(srcIdx);
                    mCommandBuffer.read(dstIdx);
                    mCommandBuffer.read(filter);
                    mCommandBuffer.read(blitInfoSize);

                    auto *src = contextVk->findTextureP(srcIdx);
                    GX_ASSERT_S(src, "CommandBufferVk::compileCommand can not find src texture from idx = %d", srcIdx);
                    auto *srcP = dynamic_cast<TextureVk *>(src);

                    auto *dst = contextVk->findTextureP(dstIdx);
                    GX_ASSERT_S(dst, "CommandBufferVk::compileCommand can not find dst texture from idx = %d", dstIdx);
                    auto *dstP = dynamic_cast<TextureVk *>(dst);

                    if (blitInfoSize > 0) {
                        blitInfos.resize(blitInfoSize);
                        for (uint32_t x = 0; x < blitInfoSize; x++) {
                            mCommandBuffer.read(blitInfos[x]);
                        }
                    }

                    doBlitImage(vkCmdBuf, srcP->vkImage(), dstP->vkImage(),
                                blitInfos, (BlitFilter::Enum) filter);
                }
                    break;
                case CommandKey::CopyRT: {
                    GfxIdxTy srcIdx;
                    GfxIdxTy dstIdx;
                    uint8_t vFrameIndex;
                    uint32_t copyInfoSize;
                    std::vector<ImageCopyInfo> copyInfos;
                    mCommandBuffer.read(srcIdx);
                    mCommandBuffer.read(dstIdx);
                    mCommandBuffer.read(vFrameIndex);
                    mCommandBuffer.read(copyInfoSize);

                    auto *src = contextVk->findRenderTargetP(srcIdx);
                    GX_ASSERT_S(src, "CommandBufferVk::compileCommand can not find src render target from idx = %d",
                                srcIdx);
                    auto *srcP = dynamic_cast<RenderTargetVk *>(src);

                    auto *dst = contextVk->findRenderTargetP(dstIdx);
                    GX_ASSERT_S(dst, "CommandBufferVk::compileCommand can not find dst render target from idx = %d",
                                dstIdx);
                    auto *dstP = dynamic_cast<RenderTargetVk *>(dst);

                    if (copyInfoSize > 0) {
                        copyInfos.resize(copyInfoSize);
                        for (uint32_t x = 0; x < copyInfoSize; x++) {
                            mCommandBuffer.read(copyInfos[x]);
                        }
                    }

                    GX_ASSERT(srcP->colorAttachmentCount() == dstP->colorAttachmentCount());
                    GX_ASSERT(srcP->hasDepthStencilAttachment() == dstP->hasDepthStencilAttachment());

                    for (uint32_t x = 0; x < srcP->colorAttachmentCount(); x++) {
                        GVkImage *srcImage = srcP->getAttachmentImage(vFrameIndex, x);
                        GVkImage *dstImage = dstP->getAttachmentImage(vFrameIndex, x);
                        VkImageLayout oldSrcImageLayout = srcImage->layout();
                        VkImageLayout oldDstImageLayout = dstImage->layout();

                        VkImageSubresourceRange srcRange {};
                        srcRange.aspectMask = srcImage->aspectMask();
                        srcRange.baseMipLevel = 0;
                        srcRange.levelCount = srcImage->mipLevels();
                        srcRange.baseArrayLayer = 0;
                        srcRange.layerCount = srcImage->arrayLayers();

                        VkImageSubresourceRange dstRange {};
                        dstRange.aspectMask = dstImage->aspectMask();
                        dstRange.baseMipLevel = 0;
                        dstRange.levelCount = dstImage->mipLevels();
                        dstRange.baseArrayLayer = 0;
                        dstRange.layerCount = dstImage->arrayLayers();

                        srcImage->imageMemoryBarrier(vkCmdBuf, oldSrcImageLayout,
                                                     VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, srcRange, false);
                        dstImage->imageMemoryBarrier(vkCmdBuf, oldDstImageLayout,
                                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, dstRange, false);

                        doCopyImage(vkCmdBuf, srcImage, dstImage, copyInfos);

                        srcImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                     oldSrcImageLayout, srcRange, false);
                        dstImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                     oldDstImageLayout, dstRange, false);
                    }
                    if (srcP->hasDepthStencilAttachment()) {
                        GVkImage *srcImage = srcP->getAttachmentImage(vFrameIndex, srcP->colorAttachmentCount());
                        GVkImage *dstImage = dstP->getAttachmentImage(vFrameIndex, dstP->colorAttachmentCount());
                        VkImageLayout oldSrcImageLayout = srcImage->layout();
                        VkImageLayout oldDstImageLayout = dstImage->layout();

                        VkImageSubresourceRange srcRange {};
                        srcRange.aspectMask = srcImage->aspectMask();
                        srcRange.baseMipLevel = 0;
                        srcRange.levelCount = srcImage->mipLevels();
                        srcRange.baseArrayLayer = 0;
                        srcRange.layerCount = srcImage->arrayLayers();

                        VkImageSubresourceRange dstRange {};
                        dstRange.aspectMask = dstImage->aspectMask();
                        dstRange.baseMipLevel = 0;
                        dstRange.levelCount = dstImage->mipLevels();
                        dstRange.baseArrayLayer = 0;
                        dstRange.layerCount = dstImage->arrayLayers();

                        srcImage->imageMemoryBarrier(vkCmdBuf, oldSrcImageLayout,
                                                     VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, srcRange, false);
                        dstImage->imageMemoryBarrier(vkCmdBuf, oldDstImageLayout,
                                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, dstRange, false);

                        doCopyImage(vkCmdBuf, srcImage, dstImage, copyInfos);

                        srcImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                     oldSrcImageLayout, srcRange, false);
                        dstImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                     oldDstImageLayout, dstRange, false);
                    }
                }
                    break;
                case CommandKey::BlitRT: {
                    GfxIdxTy srcIdx;
                    GfxIdxTy dstIdx;
                    uint8_t filter;
                    uint8_t vFrameIndex;
                    uint32_t blitInfoSize;
                    std::vector<ImageBlitInfo> blitInfos;
                    mCommandBuffer.read(srcIdx);
                    mCommandBuffer.read(dstIdx);
                    mCommandBuffer.read(filter);
                    mCommandBuffer.read(vFrameIndex);
                    mCommandBuffer.read(blitInfoSize);

                    auto *src = contextVk->findRenderTargetP(srcIdx);
                    GX_ASSERT_S(src, "CommandBufferVk::compileCommand can not find src render target from idx = %d",
                                srcIdx);
                    auto *srcP = dynamic_cast<RenderTargetVk *>(src);

                    auto *dst = contextVk->findRenderTargetP(dstIdx);
                    GX_ASSERT_S(dst, "CommandBufferVk::compileCommand can not find dst render target from idx = %d",
                                dstIdx);
                    auto *dstP = dynamic_cast<RenderTargetVk *>(dst);

                    if (blitInfoSize > 0) {
                        blitInfos.resize(blitInfoSize);
                        for (uint32_t x = 0; x < blitInfoSize; x++) {
                            mCommandBuffer.read(blitInfos[x]);
                        }
                    }

                    GX_ASSERT(srcP->colorAttachmentCount() == dstP->colorAttachmentCount());
                    GX_ASSERT(srcP->hasDepthStencilAttachment() == dstP->hasDepthStencilAttachment());

                    for (uint32_t x = 0; x < srcP->colorAttachmentCount(); x++) {
                        GVkImage *srcImage = srcP->getAttachmentImage(vFrameIndex, x);
                        GVkImage *dstImage = dstP->getAttachmentImage(vFrameIndex, x);
                        VkImageLayout oldSrcImageLayout = srcImage->layout();
                        VkImageLayout oldDstImageLayout = dstImage->layout();

                        VkImageSubresourceRange srcRange {};
                        srcRange.aspectMask = srcImage->aspectMask();
                        srcRange.baseMipLevel = 0;
                        srcRange.levelCount = srcImage->mipLevels();
                        srcRange.baseArrayLayer = 0;
                        srcRange.layerCount = srcImage->arrayLayers();

                        VkImageSubresourceRange dstRange {};
                        dstRange.aspectMask = dstImage->aspectMask();
                        dstRange.baseMipLevel = 0;
                        dstRange.levelCount = dstImage->mipLevels();
                        dstRange.baseArrayLayer = 0;
                        dstRange.layerCount = dstImage->arrayLayers();

                        srcImage->imageMemoryBarrier(vkCmdBuf, oldSrcImageLayout,
                                                     VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, srcRange, false);
                        dstImage->imageMemoryBarrier(vkCmdBuf, oldDstImageLayout,
                                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, dstRange, false);

                        doBlitImage(vkCmdBuf, srcImage, dstImage, blitInfos, (BlitFilter::Enum) filter);

                        srcImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                     oldSrcImageLayout, srcRange, false);
                        dstImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                     oldDstImageLayout, dstRange, false);
                    }
                    if (srcP->hasDepthStencilAttachment()) {
                        GVkImage *srcImage = srcP->getAttachmentImage(vFrameIndex, srcP->colorAttachmentCount());
                        GVkImage *dstImage = dstP->getAttachmentImage(vFrameIndex, dstP->colorAttachmentCount());
                        VkImageLayout oldSrcImageLayout = srcImage->layout();
                        VkImageLayout oldDstImageLayout = dstImage->layout();

                        VkImageSubresourceRange srcRange {};
                        srcRange.aspectMask = srcImage->aspectMask();
                        srcRange.baseMipLevel = 0;
                        srcRange.levelCount = srcImage->mipLevels();
                        srcRange.baseArrayLayer = 0;
                        srcRange.layerCount = srcImage->arrayLayers();

                        VkImageSubresourceRange dstRange {};
                        dstRange.aspectMask = dstImage->aspectMask();
                        dstRange.baseMipLevel = 0;
                        dstRange.levelCount = dstImage->mipLevels();
                        dstRange.baseArrayLayer = 0;
                        dstRange.layerCount = dstImage->arrayLayers();

                        srcImage->imageMemoryBarrier(vkCmdBuf, oldSrcImageLayout,
                                                     VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, srcRange, false);
                        dstImage->imageMemoryBarrier(vkCmdBuf, oldDstImageLayout,
                                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, dstRange, false);

                        doBlitImage(vkCmdBuf, srcImage, dstImage, blitInfos, (BlitFilter::Enum) filter);

                        srcImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                     oldSrcImageLayout, srcRange, false);
                        dstImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                     oldDstImageLayout, dstRange, false);
                    }
                }
                    break;
                case CommandKey::BlitRTToImage: {
                    GfxIdxTy srcIdx;
                    GfxIdxTy dstIdx;
                    uint8_t filter;
                    uint8_t attachIndex;
                    uint8_t vFrameIndex;
                    uint32_t blitInfoSize;
                    std::vector<ImageBlitInfo> blitInfos;
                    mCommandBuffer.read(srcIdx);
                    mCommandBuffer.read(dstIdx);
                    mCommandBuffer.read(filter);
                    mCommandBuffer.read(attachIndex);
                    mCommandBuffer.read(vFrameIndex);
                    mCommandBuffer.read(blitInfoSize);

                    auto *src = contextVk->findRenderTargetP(srcIdx);
                    GX_ASSERT_S(src, "CommandBufferVk::compileCommand can not find src render target from idx = %d",
                                srcIdx);
                    auto *srcP = dynamic_cast<RenderTargetVk *>(src);

                    auto *dst = contextVk->findTextureP(dstIdx);
                    GX_ASSERT_S(dst, "CommandBufferVk::compileCommand can not find dst texture from idx = %d", dstIdx);
                    auto *dstP = dynamic_cast<TextureVk *>(dst);

                    if (blitInfoSize > 0) {
                        blitInfos.resize(blitInfoSize);
                        for (uint32_t x = 0; x < blitInfoSize; x++) {
                            mCommandBuffer.read(blitInfos[x]);
                        }
                    }

                    GVkImage *srcImage = srcP->getAttachmentImage(vFrameIndex, attachIndex);
                    GX_ASSERT_S(srcImage != nullptr, "srcImage is null object");

                    VkImageLayout oldSrcImageLayout = srcImage->layout();

                    VkImageSubresourceRange srcRange {};
                    srcRange.aspectMask = srcImage->aspectMask();
                    srcRange.baseMipLevel = 0;
                    srcRange.levelCount = srcImage->mipLevels();
                    srcRange.baseArrayLayer = 0;
                    srcRange.layerCount = srcImage->arrayLayers();

                    srcImage->imageMemoryBarrier(vkCmdBuf, oldSrcImageLayout,
                                                 VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, srcRange, false);

                    doBlitImage(vkCmdBuf, srcImage, dstP->vkImage(),
                                blitInfos, (BlitFilter::Enum) filter);

                    srcImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                 oldSrcImageLayout, srcRange, false);
                }
                    break;
                case CommandKey::CopyRTToImage: {
                    GfxIdxTy srcIdx;
                    GfxIdxTy dstIdx;
                    uint8_t attachIndex;
                    uint8_t vFrameIndex;
                    uint32_t copyInfoSize;
                    std::vector<ImageCopyInfo> copyInfos;
                    mCommandBuffer.read(srcIdx);
                    mCommandBuffer.read(dstIdx);
                    mCommandBuffer.read(attachIndex);
                    mCommandBuffer.read(vFrameIndex);
                    mCommandBuffer.read(copyInfoSize);

                    auto *src = contextVk->findRenderTargetP(srcIdx);
                    GX_ASSERT_S(src, "CommandBufferVk::compileCommand can not find render target from idx = %d",
                                srcIdx);
                    auto *srcP = dynamic_cast<RenderTargetVk *>(src);

                    auto *dst = contextVk->findTextureP(dstIdx);
                    GX_ASSERT_S(dst, "CommandBufferVk::compileCommand can not find Texture from idx = %d", dstIdx);
                    auto *dstP = dynamic_cast<TextureVk *>(dst);

                    if (copyInfoSize > 0) {
                        copyInfos.resize(copyInfoSize);
                        for (uint32_t x = 0; x < copyInfoSize; x++) {
                            mCommandBuffer.read(copyInfos[x]);
                        }
                    }

                    GVkImage *srcImage = srcP->getAttachmentImage(vFrameIndex, attachIndex);
                    GX_ASSERT_S(srcImage != nullptr, "srcImage is null object");

                    VkImageLayout oldSrcImageLayout = srcImage->layout();

                    VkImageSubresourceRange srcRange {};
                    srcRange.aspectMask = srcImage->aspectMask();
                    srcRange.baseMipLevel = 0;
                    srcRange.levelCount = srcImage->mipLevels();
                    srcRange.baseArrayLayer = 0;
                    srcRange.layerCount = srcImage->arrayLayers();

                    srcImage->imageMemoryBarrier(vkCmdBuf, oldSrcImageLayout,
                                                 VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, srcRange,
                                                 false);

                    doCopyImage(vkCmdBuf, srcImage, dstP->vkImage(), copyInfos);

                    srcImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                 oldSrcImageLayout, srcRange, false);
                }
                    break;
                case CommandKey::CopyRTToBuffer: {
                    GfxIdxTy srcIdx;
                    GfxIdxTy dstIdx;
                    uint8_t attachIndex;
                    uint8_t vFrameIndex;
                    uint32_t copyInfoSize;
                    std::vector<VkBufferImageCopy> vkCopyInfos;
                    mCommandBuffer.read(srcIdx);
                    mCommandBuffer.read(dstIdx);
                    mCommandBuffer.read(attachIndex);
                    mCommandBuffer.read(vFrameIndex);
                    mCommandBuffer.read(copyInfoSize);

                    auto *src = contextVk->findRenderTargetP(srcIdx);
                    GX_ASSERT_S(src, "CommandBufferVk::compileCommand can not find src buffer from idx = %d", srcIdx);
                    auto *srcP = dynamic_cast<RenderTargetVk *>(src);

                    auto *dst = contextVk->findBufferP(dstIdx);
                    GX_ASSERT_S(dst, "CommandBufferVk::compileCommand can not find dst buffer from idx = %d", dstIdx);
                    auto *dstP = dynamic_cast<BufferVk *>(dst);

                    GVkImage *srcImage = srcP->getAttachmentImage(vFrameIndex, attachIndex);
                    GX_ASSERT_S(srcImage != nullptr, "srcImage is null object");

                    if (copyInfoSize > 0) {
                        vkCopyInfos.resize(copyInfoSize);
                        BufferImageCopyInfo tempInfo{};
                        for (uint32_t x = 0; x < copyInfoSize; x++) {
                            mCommandBuffer.read(tempInfo);
                            vkCopyInfos[x].bufferOffset = tempInfo.bufferOffset;
                            vkCopyInfos[x].bufferRowLength = tempInfo.bufferRowLength;
                            vkCopyInfos[x].bufferImageHeight = tempInfo.bufferImageHeight;
                            vkCopyInfos[x].imageSubresource.aspectMask =
                                    tempInfo.aspectMask == 0 ? srcImage->aspectMask()
                                                             : toVkAspectFlags(tempInfo.aspectMask);
                            vkCopyInfos[x].imageSubresource.mipLevel = tempInfo.mipLevel;
                            vkCopyInfos[x].imageSubresource.baseArrayLayer = tempInfo.baseArrayLayer;
                            vkCopyInfos[x].imageSubresource.layerCount = 1;
                            vkCopyInfos[x].imageOffset.x = tempInfo.imageOffsetX;
                            vkCopyInfos[x].imageOffset.y = tempInfo.imageOffsetY;
                            vkCopyInfos[x].imageOffset.z = tempInfo.imageOffsetZ;
                            vkCopyInfos[x].imageExtent.width = tempInfo.imageWidth;
                            vkCopyInfos[x].imageExtent.height = tempInfo.imageHeight;
                            vkCopyInfos[x].imageExtent.depth = tempInfo.imageDepth;
                        }
                    }

                    VkImageLayout oldSrcImageLayout = srcImage->layout();

                    VkImageSubresourceRange srcRange {};
                    srcRange.aspectMask = srcImage->aspectMask();
                    srcRange.baseMipLevel = 0;
                    srcRange.levelCount = srcImage->mipLevels();
                    srcRange.baseArrayLayer = 0;
                    srcRange.layerCount = srcImage->arrayLayers();

                    srcImage->imageMemoryBarrier(vkCmdBuf, oldSrcImageLayout,
                                                 VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, srcRange, false);

                    vkCmdCopyImageToBuffer(
                            vkCmdBuf,
                            *srcImage,
                            srcImage->layout(),
                            dstP->vkBuffer(),
                            vkCopyInfos.size(),
                            vkCopyInfos.data());

                    srcImage->imageMemoryBarrier(vkCmdBuf, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                 oldSrcImageLayout, srcRange, false);
                }
                    break;
                case CommandKey::FillBuffer: {
                    GfxIdxTy idx;
                    uint64_t offset;
                    uint64_t size;
                    uint32_t data;

                    mCommandBuffer.read(idx);
                    mCommandBuffer.read(offset);
                    mCommandBuffer.read(size);
                    mCommandBuffer.read(data);

                    auto *buffer = contextVk->findBufferP(idx);
                    GX_ASSERT_S(buffer, "CommandBufferVk::compileCommand can not find buffer from idx = %d", idx);
                    auto *bufferP = dynamic_cast<BufferVk *>(buffer);

                    vkCmdFillBuffer(vkCmdBuf, bufferP->vkBuffer(), offset, size, data);
                }
                    break;
                case CommandKey::BeginDebug: {
                    mCommandBuffer.read(debugLabel);

                    VkDebugUtilsLabelEXT labelInfo = {
                            VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
                            nullptr,
                            debugLabel.c_str(),
                            {0, 1, 0, 1},
                    };
                    vkd::vkCmdBeginDebugUtilsLabel(vkCmdBuf, &labelInfo);
                }
                    break;
                case CommandKey::EndDebug: {
                    debugLabel.clear();
                    vkd::vkCmdEndDebugUtilsLabel(vkCmdBuf);
                }
                    break;
                case CommandKey::ResetQuery: {
                    GfxIdxTy queryIdx;
                    uint32_t firstQuery;
                    uint32_t queryCount;

                    mCommandBuffer.read(queryIdx);
                    mCommandBuffer.read(firstQuery);
                    mCommandBuffer.read(queryCount);

                    auto *query = contextVk->findQueryP(queryIdx);
                    GX_ASSERT_S(query, "CommandBufferVk::compileCommand can not find Query from idx = %d", queryIdx);
                    auto *queryP = dynamic_cast<QueryVk *>(query);

                    vkCmdResetQueryPool(vkCmdBuf, queryP->getVkQueryPool(), firstQuery, queryCount);
                }
                    break;
                case CommandKey::BeginQuery: {
                    GfxIdxTy queryIdx;
                    uint32_t queryIndex;
                    bool precise;

                    mCommandBuffer.read(queryIdx);
                    mCommandBuffer.read(queryIndex);
                    mCommandBuffer.read(precise);

                    auto *query = contextVk->findQueryP(queryIdx);
                    GX_ASSERT_S(query, "CommandBufferVk::compileCommand can not find Query from idx = %d", queryIdx);
                    auto *queryP = dynamic_cast<QueryVk *>(query);

                    vkCmdBeginQuery(vkCmdBuf, queryP->getVkQueryPool(), queryIndex,
                                    precise ? VK_QUERY_CONTROL_PRECISE_BIT : 0);
                }
                    break;
                case CommandKey::EndQuery: {
                    GfxIdxTy queryIdx;
                    uint32_t queryIndex;

                    mCommandBuffer.read(queryIdx);
                    mCommandBuffer.read(queryIndex);

                    auto *query = contextVk->findQueryP(queryIdx);
                    GX_ASSERT_S(query, "CommandBufferVk::compileCommand can not find Query from idx = %d", queryIdx);
                    auto *queryP = dynamic_cast<QueryVk *>(query);

                    vkCmdEndQuery(vkCmdBuf, queryP->getVkQueryPool(), queryIndex);
                }
                    break;
                case CommandKey::WriteTimestamp: {
                    uint32_t pipelineStage;
                    GfxIdxTy queryIdx;
                    uint32_t queryIndex;

                    mCommandBuffer.read(pipelineStage);
                    mCommandBuffer.read(queryIdx);
                    mCommandBuffer.read(queryIndex);

                    if (contextVk->isSupportQueryTimestamp()) {
                        auto *query = contextVk->findQueryP(queryIdx);
                        GX_ASSERT_S(query, "CommandBufferVk::compileCommand can not find Query from idx = %d",
                                    queryIdx);
                        auto *queryP = dynamic_cast<QueryVk *>(query);

                        vkCmdWriteTimestamp(vkCmdBuf, toVkPipelineStageFlagBits((PipelineStage::Enum) pipelineStage),
                                            queryP->getVkQueryPool(), queryIndex);
                    }
                }
                    break;
                case CommandKey::CopyQueryResults: {
                    GfxIdxTy queryIdx;
                    uint32_t firstQuery;
                    uint32_t queryCount;
                    GfxIdxTy dstBufferIdx;
                    uint64_t dstOffset;
                    QueryResultFlags resultFlags;

                    mCommandBuffer.read(queryIdx);
                    mCommandBuffer.read(firstQuery);
                    mCommandBuffer.read(queryCount);
                    mCommandBuffer.read(dstBufferIdx);
                    mCommandBuffer.read(dstOffset);
                    mCommandBuffer.read(resultFlags);

                    auto *query = contextVk->findQueryP(queryIdx);
                    GX_ASSERT_S(query, "CommandBufferVk::compileCommand can not find Query from idx = %d", queryIdx);
                    auto *queryP = dynamic_cast<QueryVk *>(query);

                    auto *dstBuffer = contextVk->findBufferP(dstBufferIdx);
                    GX_ASSERT_S(query, "CommandBufferVk::compileCommand can not find Buffer from idx = %d",
                                dstBufferIdx);
                    auto *dstBufferP = dynamic_cast<BufferVk *>(dstBuffer);

                    vkCmdCopyQueryPoolResults(vkCmdBuf, queryP->getVkQueryPool(), firstQuery, queryCount,
                                              dstBufferP->vkBuffer(), dstOffset, sizeof(uint64_t),
                                              toVkQueryResultFlags(resultFlags));
                }
                    break;
                default:
                    GX_ASSERT_S(cmdKey > CommandKey::None && cmdKey < CommandKey::Count,
                                "CommandBufferVk::compileCommand unknown command(%d)", cmdKey);
            }
        } while (cmdKey != CommandKey::End);
    }
    mIsCompiled = true;
}

void CommandBufferVk::doCopyImage(VkCommandBuffer cmdBuffer, GVkImage *src, GVkImage *dst,
                                  const std::vector<ImageCopyInfo> &copyInfos)
{
    std::vector<VkImageCopy> vkCopyInfos;
    if (!copyInfos.empty()) {
        vkCopyInfos.resize(copyInfos.size());
        for (uint32_t x = 0; x < copyInfos.size(); x++) {
            auto &tempInfo = copyInfos[x];
            vkCopyInfos[x].srcSubresource.aspectMask =
                    tempInfo.srcAspectMask == 0 ? src->aspectMask() : toVkAspectFlags(tempInfo.srcAspectMask);
            vkCopyInfos[x].srcSubresource.mipLevel = tempInfo.srcMipLevel;
            vkCopyInfos[x].srcSubresource.baseArrayLayer = tempInfo.srcBaseArrayLayer;
            vkCopyInfos[x].srcSubresource.layerCount = src->arrayLayers();
            vkCopyInfos[x].dstSubresource.aspectMask =
                    tempInfo.dstAspectMask == 0 ? dst->aspectMask() : toVkAspectFlags(tempInfo.dstAspectMask);
            vkCopyInfos[x].dstSubresource.mipLevel = tempInfo.dstMipLevel;
            vkCopyInfos[x].dstSubresource.baseArrayLayer = tempInfo.dstBaseArrayLayer;
            vkCopyInfos[x].dstSubresource.layerCount = dst->arrayLayers();
            vkCopyInfos[x].srcOffset.x = tempInfo.srcOffsetX;
            vkCopyInfos[x].srcOffset.y = tempInfo.srcOffsetY;
            vkCopyInfos[x].srcOffset.z = tempInfo.srcOffsetZ;
            vkCopyInfos[x].dstOffset.x = tempInfo.dstOffsetX;
            vkCopyInfos[x].dstOffset.y = tempInfo.dstOffsetY;
            vkCopyInfos[x].dstOffset.z = tempInfo.dstOffsetZ;
            vkCopyInfos[x].extent.width = tempInfo.imageWidth;
            vkCopyInfos[x].extent.height = tempInfo.imageHeight;
            vkCopyInfos[x].extent.depth = tempInfo.imageDepth;
        }
    }

    vkCmdCopyImage(
            cmdBuffer,
            *src,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            *dst,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            vkCopyInfos.size(),
            vkCopyInfos.data());
}

void CommandBufferVk::doBlitImage(VkCommandBuffer cmdBuffer, GVkImage *src, GVkImage *dst,
                                  const std::vector<ImageBlitInfo> &blitInfos, BlitFilter::Enum filter)
{
    std::vector<VkImageBlit> vkBlitInfos;
    if (!blitInfos.empty()) {
        vkBlitInfos.resize(blitInfos.size());
        for (uint32_t x = 0; x < blitInfos.size(); x++) {
            auto &tempInfo = blitInfos[x];
            vkBlitInfos[x].srcSubresource.aspectMask =
                    tempInfo.srcAspectMask == 0 ? src->aspectMask() : toVkAspectFlags(tempInfo.srcAspectMask);
            vkBlitInfos[x].srcSubresource.mipLevel = tempInfo.srcMipLevel;
            vkBlitInfos[x].srcSubresource.baseArrayLayer = tempInfo.srcBaseArrayLayer;
            vkBlitInfos[x].srcSubresource.layerCount = tempInfo.srcLayerCount == 0 ? 1 : tempInfo.srcLayerCount;
            vkBlitInfos[x].srcOffsets[0] = {tempInfo.srcOffsetX, tempInfo.srcOffsetY, tempInfo.srcOffsetZ};
            vkBlitInfos[x].srcOffsets[1] = {(int32_t) tempInfo.srcWidth, (int32_t) tempInfo.srcHeight,
                                            (int32_t) tempInfo.srcDepth};

            vkBlitInfos[x].dstSubresource.aspectMask =
                    tempInfo.dstAspectMask == 0 ? dst->aspectMask() : toVkAspectFlags(tempInfo.dstAspectMask);
            vkBlitInfos[x].dstSubresource.mipLevel = tempInfo.dstMipLevel;
            vkBlitInfos[x].dstSubresource.baseArrayLayer = tempInfo.dstBaseArrayLayer;
            vkBlitInfos[x].dstSubresource.layerCount = tempInfo.dstLayerCount == 0 ? 1 : tempInfo.dstLayerCount;
            vkBlitInfos[x].dstOffsets[0] = {tempInfo.dstOffsetX, tempInfo.dstOffsetY, tempInfo.dstOffsetZ};
            vkBlitInfos[x].dstOffsets[1] = {(int32_t) tempInfo.dstWidth, (int32_t) tempInfo.dstHeight,
                                            (int32_t) tempInfo.dstDepth};
        }
    }

    vkCmdBlitImage(cmdBuffer,
                   src->vkImage(),
                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   dst->vkImage(),
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   vkBlitInfos.size(),
                   vkBlitInfos.data(),
                   toVkFilter(filter));
}

PipelineVk *CommandBufferVk::bindGraphPipeline(ContextVk *context, VkCommandBuffer cmdBuffer,
                                               const CreateGraphicsPipelineStateInfo &createInfo,
                                               PipelineVk *pipeline)
{
    if (pipeline != nullptr) {
        return pipeline;
    }
    pipeline = context->getGraphicsPipeline(createInfo);
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->vkPipeline());
    return pipeline;
}

PipelineVk *CommandBufferVk::bindComputePipeline(ContextVk *context, VkCommandBuffer cmdBuffer,
                                                 const CreateComputePipelineStateInfo &createInfo,
                                                 PipelineVk *pipeline)
{
    if (pipeline != nullptr) {
        return pipeline;
    }
    pipeline = context->getComputePipeline(createInfo);
    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline->vkPipeline());
    return pipeline;
}

/// ============ QueryVk ============ ///

bool QueryVk::init(Context_T *context, const CreateQueryInfo &createInfo)
{
    mContextT = context;
    mHash = hashOf(createInfo);
    mQueryType = createInfo.queryType;
    mQueryCount = createInfo.queryCount;

    VkQueryPoolCreateInfo queryPoolCreateInfo{};
    queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    queryPoolCreateInfo.flags = 0;
    queryPoolCreateInfo.queryType = toVkQueryType(createInfo.queryType);
    queryPoolCreateInfo.queryCount = createInfo.queryCount;
    queryPoolCreateInfo.pipelineStatistics = toVkQueryPipelineStatisticFlags(createInfo.pipelineStatistics);

    VK_CHECK_RESULT(vkCreateQueryPool(getGVkContext(context)->vkDevice(), &queryPoolCreateInfo, nullptr, &mVkQuery));

    return mVkQuery != VK_NULL_HANDLE;
}

void QueryVk::destroy()
{
    VkDevice vkDevice = getGVkContext(mContextT)->vkDevice();

    if (mVkQuery != VK_NULL_HANDLE) {
        vkDestroyQueryPool(vkDevice, mVkQuery, nullptr);
    }

    mContextT = nullptr;
}

Context_T *QueryVk::context()
{
    return mContextT;
}

size_t QueryVk::hash()
{
    return mHash;
}

void QueryVk::getQueryResults(std::vector<uint64_t> &results, QueryResultFlags flags)
{
    results.resize(mQueryCount);
    auto *contextVk = dynamic_cast<ContextVk *>(mContextT->contextP());
    auto *gvkContext = contextVk->vkContext();

    if (mQueryType == QueryType::Occlusion) {
        vkGetQueryPoolResults(
                gvkContext->vkDevice(),
                mVkQuery,
                0, mQueryCount, sizeof(uint64_t) * mQueryCount, results.data(), sizeof(uint64_t),
                toVkQueryResultFlags(flags)
        );
    } else if (mQueryType == QueryType::PipelineStatistics) {
        vkGetQueryPoolResults(
                gvkContext->vkDevice(),
                mVkQuery,
                0, 1, sizeof(uint64_t) * mQueryCount, results.data(), sizeof(uint64_t),
                toVkQueryResultFlags(flags)
        );
    } else if (mQueryType == QueryType::Timestamp) {
        if (contextVk->isSupportQueryTimestamp()) {
            vkGetQueryPoolResults(
                    gvkContext->vkDevice(),
                    mVkQuery,
                    0, mQueryCount, sizeof(uint64_t) * mQueryCount, results.data(), sizeof(uint64_t),
                    toVkQueryResultFlags(flags)
            );
            for (auto &i : results) {
                i *= contextVk->getTimestampPeriod();
            }
        }
    }
}

VkQueryPool QueryVk::getVkQueryPool() const
{
    return mVkQuery;
}

#endif //USE_GP_API_VULKAN

}
}
