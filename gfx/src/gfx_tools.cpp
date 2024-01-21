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

#include <gfx/gfx_tools.h>

#include <cmath>
#include <gx/debug.h>


namespace gfx
{

std::string deviceTypeString(DeviceType::Enum type)
{
    switch (type) {
        case DeviceType::Other:
            return "Other";
        case DeviceType::IntegratedGPU:
            return "IntegratedGPU";
        case DeviceType::DiscreteGPU:
            return "DiscreteGPU";
        case DeviceType::VirtualGPU:
            return "VirtualGPU";
        case DeviceType::CPU:
            return "CPU";
        case DeviceType::Count:
        default:
            return "Unknown";
    }
}

void printDeviceInfo(const DeviceInfo &deviceInfo)
{
    Log("Device(%d): %s (%d)", deviceInfo.deviceIndex, deviceInfo.deviceName, deviceInfo.deviceID);
    Log("\tType: %s", deviceTypeString(deviceInfo.deviceType).c_str());
    Log("\tAPI: %s", getAPIVersion2String(deviceInfo.apiVersion).c_str());
    Log("\tDevice Version: %s", getAPIVersion2String(deviceInfo.driverVersion).c_str());
}

std::string getAPIVersion2String(uint32_t version)
{
    char temp[32];
    sprintf(temp, "%d.%d.%d", (version >> 22), ((version >> 12) & 0x3ff), (version & 0xfff));
    return temp;
}

void *alignedAlloc(size_t size, size_t alignment)
{
    void *data;
#if defined(_MSC_VER) || defined(__MINGW32__)
    data = _aligned_malloc(size, alignment);
#else
    int res = posix_memalign(&data, alignment, size);
    if (res != 0)
        data = nullptr;
#endif
    return data;
}

void alignedFree(void *data)
{
#if defined(_MSC_VER) || defined(__MINGW32__)
    _aligned_free(data);
#else
    free(data);
#endif
}

uint32_t calcMipLevels(uint32_t width, uint32_t height, uint32_t depth)
{
    return std::max(1, std::ilogbf((float)std::max(width, std::max(height, depth))) + 1);
}

ImageLayout::Enum getImageLayoutFromUsage(TextureUsageFlags usageFlags,
                                          TextureAspectFlags aspectFlags,
                                          bool computeUsage)
{
    if ((usageFlags & TextureUsage::Attachment) == TextureUsage::Attachment) {
        if (((aspectFlags & TextureAspect::AspectDepth) == TextureAspect::AspectDepth)
            || ((aspectFlags & TextureAspect::AspectStencil) == TextureAspect::AspectStencil)) {
            return ImageLayout::DepthStencilAttachment;
        } else {
            return ImageLayout::ColorAttachment;
        }
    } else if ((usageFlags & TextureUsage::Storage) == TextureUsage::Storage) {
        return computeUsage ? ImageLayout::ComputeGeneral : ImageLayout::GraphicsGeneral;
    } else {
        return ImageLayout::ShaderReadOnly;
    }
}

}
