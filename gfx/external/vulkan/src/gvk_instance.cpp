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

#include "gfx/gvk_instance.h"

#include "gfx/gvk_tools.h"
#include "gfx/gvk_debug.h"
#include <gx/debug.h>

#include <cstring>

#if GX_PLATFORM_WINDOWS
#include <windows.h>
#endif


using namespace gx;

namespace gfx
{

bool GVkInstance::create(const std::string &applicationName, uint32_t apiVersion,
                         const std::vector<const char *> &enabledInstanceExtensions, bool enableValidation)
{
    static bool loadVkLib = false;
    if (!loadVkLib) {
        volkInitialize();
        loadVkLib = true;
    }
    mEnableValidation = enableValidation;
    return createVkInstance(applicationName, "GxGfx", apiVersion, enabledInstanceExtensions)
           && initPhysicalDevice();
}

void GVkInstance::destroy()
{
    vkd::freeVkDebugging(this);

    if (mVkInstance != VK_NULL_HANDLE) {
        vkDestroyInstance(mVkInstance, nullptr);
    }
    mVkInstance = VK_NULL_HANDLE;
}

GVkInstance::operator VkInstance()
{
    return mVkInstance;
}

VkInstance GVkInstance::vkInstance()
{
    return mVkInstance;
}

uint32_t GVkInstance::physicalDeviceCount() const
{
    return mPhysicalDevices.size();
}

std::vector<VkPhysicalDevice> GVkInstance::physicalDevices() const
{
    return mPhysicalDevices;
}

VkPhysicalDevice GVkInstance::getPhysicalDevice(uint32_t gpuIndex) const
{
    if (gpuIndex >= mPhysicalDevices.size()) {
        return VK_NULL_HANDLE;
    }
    return mPhysicalDevices.at(gpuIndex);
}

void GVkInstance::showGpuInfos()
{
    Log("GPU count = %d", mPhysicalDevices.size());

    Log("Available Vulkan devices");
    for (uint32_t i = 0; i < mPhysicalDevices.size(); i++) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(mPhysicalDevices[i], &deviceProperties);
        Log("Device [%d] : %s", i, deviceProperties.deviceName);
        Log(" Type: %s", vks::tools::physicalDeviceTypeString(deviceProperties.deviceType).c_str());
        Log(" API: %d.%d.%d\n", (deviceProperties.apiVersion >> 22), ((deviceProperties.apiVersion >> 12) & 0x3ff),
            (deviceProperties.apiVersion & 0xfff));
    }
}

#if !GX_PLATFORM_OSX && !GX_PLATFORM_IOS

VkSurfaceKHR GVkInstance::createSurfaceForWindow(const GVkPlatformData &platformData) const
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    VkResult err = VK_SUCCESS;

    void *platformWindow;

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    platformWindow = platformData.nativeWindowHandle;
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    platformWindow = platformData.nativeWindowHandle;
#elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
    platformWindow = platformData.nativeWindowHandle;
#elif defined(VK_USE_PLATFORM_XLIB_KHR) || defined(VK_USE_PLATFORM_XCB_KHR)
    platformWindow = platformData.nativeWindowHandle;
#endif

    // Create the os-specific surface
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance = (HINSTANCE) GetModuleHandle(nullptr);
    surfaceCreateInfo.hwnd = (HWND) platformWindow;
    err = vkCreateWin32SurfaceKHR(mVkInstance, &surfaceCreateInfo, nullptr, &surface);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo{};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.window = (ANativeWindow *) platformWindow;
    err = vkCreateAndroidSurfaceKHR(mVkInstance, &surfaceCreateInfo, NULL, &surface);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
        VkIOSSurfaceCreateInfoMVK surfaceCreateInfo{};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
        surfaceCreateInfo.pNext = NULL;
        surfaceCreateInfo.flags = 0;
        surfaceCreateInfo.pView = view;
        err = vkCreateIOSSurfaceMVK(mVkInstance, &surfaceCreateInfo, nullptr, &surface);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
        NSWindow* window = (NSWindow*)(platformWindow);
        NSView* contentView = (NSView*)window.contentView;
        CAMetalLayer* layer = [CAMetalLayer layer];
        [contentView setWantsLayer : YES] ;
        [contentView setLayer : layer] ;

        VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo{};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
        surfaceCreateInfo.pNext = NULL;
        surfaceCreateInfo.flags = 0;
        surfaceCreateInfo.pView = (void*)layer;
        err = vkCreateMacOSSurfaceMVK(mVkInstance, &surfaceCreateInfo, NULL, &surface);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
        VkWaylandSurfaceCreateInfoKHR surfaceCreateInfo{};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.display = display;
        surfaceCreateInfo.surface = window;
        err = vkCreateWaylandSurfaceKHR(mVkInstance, &surfaceCreateInfo, nullptr, &surface);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    VkXlibSurfaceCreateInfoKHR surfaceCreateInfo{};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.dpy = (Display*) platformData.nativeDisplayType;
    surfaceCreateInfo.window = (Window)platformWindow;

    err = vkCreateXlibSurfaceKHR(mVkInstance, &surfaceCreateInfo, nullptr, &surface);
#endif
    GX_ASSERT_S(err == VK_SUCCESS, "Could not create surface! %s", vks::tools::errorString(err).c_str());
    if (err != VK_SUCCESS) {
        Log("Could not create surface! %s", vks::tools::errorString(err).c_str());
    }
    return surface;
}

#endif

bool GVkInstance::isEnableValidation() const
{
    return mEnableValidation;
}

bool GVkInstance::createVkInstance(const std::string &applicationName, const std::string &engineName, uint32_t apiVersion,
                                   const std::vector<const char *> &enabledInstanceExtensions)
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName.c_str();
    appInfo.pEngineName = engineName.c_str();
    appInfo.apiVersion = apiVersion;

    std::vector<const char *> instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME};

    // Enable surface extensions depending on os
#if GX_PLATFORM_WINDOWS
    instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
    instanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#endif
#if defined(_DIRECT2DISPLAY)
    instanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    instanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    instanceExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
    instanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
#if defined(VK_USE_PLATFORM_IOS_MVK)
    instanceExtensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#endif
#if defined(VK_USE_PLATFORM_MACOS_MVK)
    instanceExtensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
    instanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

#ifndef NDEBUG
    instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    if (!enabledInstanceExtensions.empty()) {
        for (auto enabledExtension : enabledInstanceExtensions) {
            instanceExtensions.push_back(enabledExtension);
        }
    }

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.pApplicationInfo = &appInfo;

#ifdef VK_USE_PLATFORM_MACOS_MVK
    instanceCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    if (!instanceExtensions.empty()) {
        instanceCreateInfo.enabledExtensionCount = (uint32_t) instanceExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
    }
    if (mEnableValidation) {
        // The VK_LAYER_KHRONOS_validation contains all current validation functionality.
        // Note that on Android this layer requires at least NDK r20
        const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
        // Check if this layer is available at instance level
        uint32_t instanceLayerCount;
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
        std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
        bool validationLayerPresent = false;
        for (VkLayerProperties layer : instanceLayerProperties) {
            if (strcmp(layer.layerName, validationLayerName) == 0) {
                validationLayerPresent = true;
                break;
            }
        }
        if (validationLayerPresent) {
            instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
            instanceCreateInfo.enabledLayerCount = 1;
        } else {
            Log("Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled");
        }
    }
    VkResult res;
    if ((res = vkCreateInstance(&instanceCreateInfo, nullptr, &mVkInstance)) != VK_SUCCESS) {
        Log("Could not create Vulkan instance : \n %s", vks::tools::errorString(res).c_str());
        return false;
    }
    if (mVkInstance != VK_NULL_HANDLE) {
        volkLoadInstance(mVkInstance);
        return true;
    }
    return false;
}

bool GVkInstance::initPhysicalDevice()
{
    VkResult res;

    // 初始化DebugUtils
    GX_ASSERT(vkd::setupVkDebugging(this, mEnableValidation) == VK_SUCCESS);

    // Physical device
    uint32_t gpuCount = 0;
    // Get number of available physical devices
    res = vkEnumeratePhysicalDevices(mVkInstance, &gpuCount, nullptr);
    if (res != VK_SUCCESS) {
        Log("Could not enumerate physical devices count : \n %s", vks::tools::errorString(res).c_str());
        return false;
    }
    // Enumerate devices
    mPhysicalDevices.resize(gpuCount);

    res = vkEnumeratePhysicalDevices(mVkInstance, &gpuCount, mPhysicalDevices.data());
    if (res) {
        Log("Could not enumerate physical devices : \n %s", vks::tools::errorString(res).c_str());
        return false;
    }

    return true;
}

}


