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

#include <gfx/gvk_tools.h>

#include <gx/gfile.h>

#include <vector>


namespace vks
{
namespace tools
{

std::string errorString(VkResult errorCode)
{
    switch (errorCode) {
#define STR(r) case VK_ ##r: return #r
        STR(NOT_READY);
        STR(TIMEOUT);
        STR(EVENT_SET);
        STR(EVENT_RESET);
        STR(INCOMPLETE);
        STR(ERROR_OUT_OF_HOST_MEMORY);
        STR(ERROR_OUT_OF_DEVICE_MEMORY);
        STR(ERROR_INITIALIZATION_FAILED);
        STR(ERROR_DEVICE_LOST);
        STR(ERROR_MEMORY_MAP_FAILED);
        STR(ERROR_LAYER_NOT_PRESENT);
        STR(ERROR_EXTENSION_NOT_PRESENT);
        STR(ERROR_FEATURE_NOT_PRESENT);
        STR(ERROR_INCOMPATIBLE_DRIVER);
        STR(ERROR_TOO_MANY_OBJECTS);
        STR(ERROR_FORMAT_NOT_SUPPORTED);
        STR(ERROR_SURFACE_LOST_KHR);
        STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
        STR(SUBOPTIMAL_KHR);
        STR(ERROR_OUT_OF_DATE_KHR);
        STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
        STR(ERROR_VALIDATION_FAILED_EXT);
        STR(ERROR_INVALID_SHADER_NV);
#undef STR
        default:
            return "UNKNOWN_ERROR";
    }
}

std::string physicalDeviceTypeString(VkPhysicalDeviceType type)
{
    switch (type) {
#define STR(r) case VK_PHYSICAL_DEVICE_TYPE_ ##r: return #r
        STR(OTHER);
        STR(INTEGRATED_GPU);
        STR(DISCRETE_GPU);
        STR(VIRTUAL_GPU);
#undef STR
        default:
            return "UNKNOWN_DEVICE_TYPE";
    }
}

VkBool32 getSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat *depthFormat)
{
    // Since all depth formats may be optional, we need to find a suitable depth format to use
    // Start with the highest precision packed format
    std::vector<VkFormat> depthFormats = {
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D24_UNORM_S8_UINT,
            VK_FORMAT_D16_UNORM_S8_UINT,
            VK_FORMAT_D16_UNORM
    };

    for (auto &format : depthFormats) {
        VkFormatProperties formatProps;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
        // Format must support depth stencil attachment for optimal tiling
        if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            *depthFormat = format;
            return true;
        }
    }

    return false;
}

void showPhysicalDeviceInfo(VkPhysicalDevice physicalDevice, uint32_t gpuIndex)
{
    VkPhysicalDeviceProperties deviceProperties{};
    VkPhysicalDeviceFeatures deviceFeatures{};
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties{};

    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

    Log("\nSelect Device %d: %s", gpuIndex, deviceProperties.deviceName);
    Log("\tType: %s", vks::tools::physicalDeviceTypeString(deviceProperties.deviceType).c_str());
    Log("\tAPI: %d.%d.%d", (deviceProperties.apiVersion >> 22),
        ((deviceProperties.apiVersion >> 12) & 0x3ff), (deviceProperties.apiVersion & 0xfff));
    Log("\tmaxImageDimension1D: %d", deviceProperties.limits.maxImageDimension1D);
    Log("\tmaxImageDimension2D: %d", deviceProperties.limits.maxImageDimension2D);
    Log("\tmaxImageDimension3D: %d", deviceProperties.limits.maxImageDimension3D);
    Log("\tmaxImageDimensionCube: %d", deviceProperties.limits.maxImageDimensionCube);
    Log("\tmaxImageArrayLayers: %d", deviceProperties.limits.maxImageArrayLayers);
    Log("\tmaxTexelBufferElements: %d", deviceProperties.limits.maxTexelBufferElements);
    Log("\tmaxVertexInputAttributes: %d", deviceProperties.limits.maxVertexInputAttributes);
    Log("\tmaxFragmentOutputAttachments: %d", deviceProperties.limits.maxFragmentOutputAttachments);
    Log("\tmaxFramebufferWidth: %d", deviceProperties.limits.maxFramebufferWidth);
    Log("\tmaxFramebufferHeight: %d", deviceProperties.limits.maxFramebufferHeight);
    Log("\tmaxFramebufferLayers: %d", deviceProperties.limits.maxFramebufferLayers);
    Log("\tframebufferColorSampleCounts: %d", deviceProperties.limits.framebufferColorSampleCounts);
    Log("\tframebufferDepthSampleCounts: %d", deviceProperties.limits.framebufferDepthSampleCounts);
    Log("\tframebufferStencilSampleCounts: %d", deviceProperties.limits.framebufferStencilSampleCounts);
    Log("\tmaxColorAttachments: %d", deviceProperties.limits.maxColorAttachments);

    Log("\n\tgeometryShader: %d", deviceFeatures.geometryShader);
    Log("\ttessellationShader: %d", deviceFeatures.tessellationShader);
    Log("\timageCubeArray: %d", deviceFeatures.imageCubeArray);
    Log("\tindependentBlend: %d", deviceFeatures.independentBlend);
    Log("\tmultiViewport: %d", deviceFeatures.multiViewport);
    Log("\tsamplerAnisotropy: %d", deviceFeatures.samplerAnisotropy);
    Log("\ttextureCompressionETC2: %d", deviceFeatures.textureCompressionETC2);
    Log("\ttextureCompressionASTC_LDR: %d", deviceFeatures.textureCompressionASTC_LDR);
    Log("\ttextureCompressionBC: %d\n", deviceFeatures.textureCompressionBC);
}

#if GX_PLATFORM_ANDROID

// Android shaders are stored as assets in the apk
// So they need to be loaded via the asset manager
VkShaderModule loadShader(const char *fileName, VkDevice device)
{
    // Load shader from compressed asset
    AAssetManager *assetManager = androidApp->activity->assetManager;
    AAsset *asset = AAssetManager_open(assetManager, fileName, AASSET_MODE_STREAMING);
    GX_ASSERT(asset);
    size_t size = AAsset_getLength(asset);
    GX_ASSERT(size > 0);

    char *shaderCode = new char[size];
    AAsset_read(asset, shaderCode, size);
    AAsset_close(asset);

    VkShaderModule shaderModule;
    VkShaderModuleCreateInfo moduleCreateInfo;
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.codeSize = size;
    moduleCreateInfo.pCode = (uint32_t *) shaderCode;
    moduleCreateInfo.flags = 0;

    VK_CHECK_RESULT(vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderModule));

    delete[] shaderCode;

    return shaderModule;
}

#else

VkShaderModule loadShader(const char *fileName, VkDevice device)
{
    using namespace gx;

    GFile file(fileName);

    if (file.open(GFile::OpenMode::ReadOnly | GFile::OpenMode::Binary)) {
        int64_t size = file.fileSize();
        char *shaderCode = new char[size];
        file.read(shaderCode, size);
        file.close();

        VkShaderModule shaderModule;
        VkShaderModuleCreateInfo moduleCreateInfo{};
        moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCreateInfo.codeSize = size;
        moduleCreateInfo.pCode = (uint32_t *) shaderCode;

        VK_CHECK_RESULT(vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &shaderModule));

        delete[] shaderCode;
        return shaderModule;
    } else {
        return VK_NULL_HANDLE;
    }
}

#endif

int32_t findQueueFamilyIndex(VkPhysicalDevice device, VkQueueFlags flags)
{
    uint32_t n;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &n, NULL);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(n);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &n, queueFamilyProperties.data());

    int32_t idx = -1;
    for (uint32_t i = 0; i < n; i++) {
        if (queueFamilyProperties[i].queueFlags == flags) {
            /*
             * Assume that a queue family that matches exactly is more efficient.
             * For instance, a queue family that support only transfer queues, might be
             * more efficient at transfer than a graphics- or compute queue family
             * (which also support transfer).
             */
            idx = i;
            break;
        } else if ((flags & VK_QUEUE_GRAPHICS_BIT) && !(flags & VK_QUEUE_COMPUTE_BIT) &&
                   !(queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                   (queueFamilyProperties[i].queueFlags & flags) == flags) {
            /*
             * Assume that a graphics capable queue family that does not support compute
             * is more efficient at graphics than a queue family that also support
             * compute.
             */
            idx = i;
        } else if ((flags & VK_QUEUE_COMPUTE_BIT) && !(flags & VK_QUEUE_GRAPHICS_BIT) &&
                   !(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                   (queueFamilyProperties[i].queueFlags & flags) == flags) {
            /*
             * Assume that a compute capable queue family that does not support graphics
             * is more efficient at compute than a queue family that also support
             * graphics.
             */
            idx = i;
        } else if (!(flags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) &&
                   !(queueFamilyProperties[i].queueFlags &
                     (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) &&
                   (queueFamilyProperties[i].queueFlags & flags) == flags) {
            /*
             * If graphics or compute is not required, prefer queue families that does
             * not support graphics or compute.
             */
            idx = i;
        } else if (idx == -1 && (queueFamilyProperties[i].queueFlags & flags) == flags) {
            idx = i;
        }
    }

    return idx;
}

int32_t findSurfaceQueueFamilyIndex(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t n;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &n, NULL);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(n);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &n, queueFamilyProperties.data());

    int32_t idx = -1;
    for (uint32_t i = 0; i < n; i++)
    {
        if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            VkBool32 surfaceSupported;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &surfaceSupported);
            if (surfaceSupported)
            {
                idx = i;
                break;
            }
        }
    }
    return idx;
}

}
}
