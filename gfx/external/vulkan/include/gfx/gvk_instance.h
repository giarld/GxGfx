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

#ifndef GFX_GVK_INSTANCE_H
#define GFX_GVK_INSTANCE_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>

#include <gx/gstring.h>


namespace gfx
{

struct GVkPlatformData
{
    void *nativeDisplayType = nullptr;       //!< Native display type.
    void *nativeWindowHandle = nullptr;      //!< Native window handle.
    void *context = nullptr;                 //!< GL context, or D3D device.
    void *backBuffer = nullptr;              //!< GL backbuffer, or D3D render target view.
    void *backBufferDS = nullptr;            //!< Backbuffer depth/stencil.
};

class GX_API GVkInstance
{
CLASS_DEF(GVkInstance)

public:
    bool create(const std::string &applicationName, uint32_t apiVersion,
            const std::vector<const char *> &enabledInstanceExtensions, bool enableValidation = false);

    void destroy();

public:
    operator VkInstance();

    VkInstance vkInstance();

    uint32_t physicalDeviceCount() const;

    std::vector<VkPhysicalDevice> physicalDevices() const;

    VkPhysicalDevice getPhysicalDevice(uint32_t gpuIndex) const;

    void showGpuInfos();

    VkSurfaceKHR createSurfaceForWindow(const GVkPlatformData &platformData) const;

    bool isEnableValidation() const;

private:
    bool createVkInstance(const std::string &applicationName, const std::string &engineName, uint32_t apiVersion,
                          const std::vector<const char *> &enabledInstanceExtensions);

    bool initPhysicalDevice();

private:
    VkInstance mVkInstance = VK_NULL_HANDLE;
    std::vector<VkPhysicalDevice> mPhysicalDevices;
    bool mEnableValidation = false;
};

}

#endif //GFX_GVK_INSTANCE_H
