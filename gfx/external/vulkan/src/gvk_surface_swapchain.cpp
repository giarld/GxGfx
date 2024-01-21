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

#include "gfx/gvk_surface_swapchain.h"

#include <gfx/gvk_tools.h>
#include <gfx/gvk_device.h>
#include <gfx/gvk_semaphore.h>
#include <gfx/gvk_image.h>


namespace gfx
{

void GVkSurfaceSwapChain::create(GVkDevice *device, VkSurfaceKHR surface, uint32_t width, uint32_t height,
                                 bool vsync)
{
    mDevice = device;
    mSurface = surface;

    mWidth = width;
    mHeight = height;
    mVsync = vsync;

    init();
}

void GVkSurfaceSwapChain::destroy()
{
    if (mHandle != VK_NULL_HANDLE) {
        for (uint32_t i = 0; i < mImageCount; i++) {
            if (mBuffers[i].gvkImage) {
                mBuffers[i].gvkImage->destroy();
                GX_DELETE(mBuffers[i].gvkImage);
            }
            vkDestroyImageView(*mDevice, mBuffers[i].view, nullptr);
        }
    }
    if (mSurface != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(*mDevice, mHandle, nullptr);
    }
    mImageAvailableSemaphore.destroy();

    mSurface = VK_NULL_HANDLE;
    mHandle = VK_NULL_HANDLE;
    mImageCount = 0;
}

bool GVkSurfaceSwapChain::isCreated() const
{
    return mHandle != VK_NULL_HANDLE;
}

VkResult GVkSurfaceSwapChain::acquireNextImage(uint32_t *bufferIndex)
{
    VkResult result = vkAcquireNextImageKHR(*mDevice, mHandle, UINT64_MAX, mImageAvailableSemaphore,
                                            (VkFence)nullptr, bufferIndex);
    return result;
}

VkResult GVkSurfaceSwapChain::queuePresent(GVkQueue *queue, uint32_t bufferIndex, VkSemaphore waitSemaphore)
{
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &mHandle;
    presentInfo.pImageIndices = &bufferIndex;
    // Check if a wait semaphore has been specified to wait for before presenting the image
    if (waitSemaphore != VK_NULL_HANDLE)
    {
        presentInfo.pWaitSemaphores = &waitSemaphore;
        presentInfo.waitSemaphoreCount = 1;
    }
    return vkQueuePresentKHR(queue->vkQueue(), &presentInfo);
}

uint32_t GVkSurfaceSwapChain::bufferCount()
{
    return mImageCount;
}

GVkSwapChainBuffer *GVkSurfaceSwapChain::getSwapChainBuffer(uint32_t index)
{
    return &mBuffers[index];
}

VkFormat GVkSurfaceSwapChain::colorFormat()
{
    return mColorFormat;
}

VkSemaphore GVkSurfaceSwapChain::getImageAvailableSemaphore()
{
    return mImageAvailableSemaphore.vkSemaphore();
}

uint32_t GVkSurfaceSwapChain::width()
{
    return mWidth;
}

uint32_t GVkSurfaceSwapChain::height()
{
    return mHeight;
}

void GVkSurfaceSwapChain::init()
{
    mColorFormat = VK_FORMAT_B8G8R8_UNORM;

    mImageAvailableSemaphore.destroy();
    mImageAvailableSemaphore.create(*mDevice);

    uint32_t formatCount;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(*mDevice, mSurface, &formatCount, nullptr));
    GX_ASSERT(formatCount > 0);

    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(*mDevice, mSurface, &formatCount, surfaceFormats.data()));

    if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED)) {
        mColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
        mColorSpace = surfaceFormats[0].colorSpace;
    } else {
        bool found_B8G8R8A8_UNORM = false;
        for (auto &&surfaceFormat : surfaceFormats) {
            if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM) {
                mColorFormat = surfaceFormat.format;
                mColorSpace = surfaceFormat.colorSpace;
                found_B8G8R8A8_UNORM = true;
                break;
            }
        }

        if (!found_B8G8R8A8_UNORM) {
            mColorFormat = surfaceFormats[0].format;
            mColorSpace = surfaceFormats[0].colorSpace;
        }
    }

    VkBool32 supported = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(mDevice->physicalDevice(),
                                         mDevice->getGraphicsQueue().queueFamilyIndex(),
                                         mSurface, &supported);
    GX_ASSERT_S(supported, "Graphics Queue does not support swapchain creation");

    // =========================================
    VkSwapchainKHR oldSwapchain = mHandle;

    // Get physical device surface properties and formats
    VkSurfaceCapabilitiesKHR surfCaps;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*mDevice, mSurface, &surfCaps));

    // Get available present modes
    uint32_t presentModeCount;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(*mDevice, mSurface, &presentModeCount, nullptr));
    GX_ASSERT(presentModeCount > 0);

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    VK_CHECK_RESULT(
            vkGetPhysicalDeviceSurfacePresentModesKHR(*mDevice, mSurface, &presentModeCount, presentModes.data()));

    VkExtent2D swapchainExtent = {};
    // If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
    if (surfCaps.currentExtent.width == (uint32_t) -1) {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = mWidth;
        swapchainExtent.height = mHeight;
    } else {
        // If the surface size is defined, the swap chain size must match
        swapchainExtent = surfCaps.currentExtent;
        mWidth = surfCaps.currentExtent.width;
        mHeight = surfCaps.currentExtent.height;
    }


    // Select a present mode for the swapchain

    // The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
    // This mode waits for the vertical blank ("v-sync")
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    // If v-sync is not requested, try to find a mailbox mode
    // It's the lowest latency non-tearing present mode available
    if (!mVsync) {
        for (size_t i = 0; i < presentModeCount; i++) {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            }
            if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
                (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
                swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            }
        }
    }

    // Determine the number of images
    uint32_t desiredNumberOfSwapchainImages = surfCaps.minImageCount + 1;
    if ((surfCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfCaps.maxImageCount)) {
        desiredNumberOfSwapchainImages = surfCaps.maxImageCount;
    }

    // Find the transformation of the surface
    VkSurfaceTransformFlagsKHR preTransform;
    if (surfCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        // We prefer a non-rotated transform
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
        preTransform = surfCaps.currentTransform;
    }

    // Find a supported composite alpha format (not all devices support alpha opaque)
    VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    // Simply select the first composite alpha format available
    std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };
    for (auto &compositeAlphaFlag : compositeAlphaFlags) {
        if (surfCaps.supportedCompositeAlpha & compositeAlphaFlag) {
            compositeAlpha = compositeAlphaFlag;
            break;
        };
    }

    VkSwapchainCreateInfoKHR swapchainCI = {};
    swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCI.pNext = nullptr;
    swapchainCI.surface = mSurface;
    swapchainCI.minImageCount = desiredNumberOfSwapchainImages;
    swapchainCI.imageFormat = mColorFormat;
    swapchainCI.imageColorSpace = mColorSpace;
    swapchainCI.imageExtent = {swapchainExtent.width, swapchainExtent.height};
    swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCI.preTransform = (VkSurfaceTransformFlagBitsKHR) preTransform;
    swapchainCI.imageArrayLayers = 1;
    swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCI.queueFamilyIndexCount = 0;
    swapchainCI.pQueueFamilyIndices = nullptr;
    swapchainCI.presentMode = swapchainPresentMode;
    swapchainCI.oldSwapchain = oldSwapchain;
    // Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area
    swapchainCI.clipped = VK_TRUE;
    swapchainCI.compositeAlpha = compositeAlpha;

    // Enable transfer source on swap chain images if supported
    if (surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
        swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }

    // Enable transfer destination on swap chain images if supported
    if (surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
        swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }

    VK_CHECK_RESULT(vkCreateSwapchainKHR(*mDevice, &swapchainCI, nullptr, &mHandle));

    // If an existing swap chain is re-created, destroy the old swap chain
    // This also cleans up all the presentable images
    if (oldSwapchain != VK_NULL_HANDLE) {
        for (uint32_t i = 0; i < mImageCount; i++) {
            if (mBuffers[i].gvkImage) {
                mBuffers[i].gvkImage->destroy();
                GX_DELETE(mBuffers[i].gvkImage);
            }
            vkDestroyImageView(*mDevice, mBuffers[i].view, nullptr);
        }
        vkDestroySwapchainKHR(*mDevice, oldSwapchain, nullptr);
    }
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(*mDevice, mHandle, &mImageCount, nullptr));

    // Get the swap chain images
    std::vector<VkImage> images;
    images.resize(mImageCount);
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(*mDevice, mHandle, &mImageCount, images.data()));

    // Get the swap chain buffers containing the image and imageview
    mBuffers.resize(mImageCount);

    VkImageViewCreateInfo colorAttachmentView = {};
    colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    colorAttachmentView.pNext = nullptr;
    colorAttachmentView.flags = 0;
    colorAttachmentView.format = mColorFormat;
    colorAttachmentView.components = {
            VK_COMPONENT_SWIZZLE_R,
            VK_COMPONENT_SWIZZLE_G,
            VK_COMPONENT_SWIZZLE_B,
            VK_COMPONENT_SWIZZLE_A
    };
    colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    colorAttachmentView.subresourceRange.baseMipLevel = 0;
    colorAttachmentView.subresourceRange.levelCount = 1;
    colorAttachmentView.subresourceRange.baseArrayLayer = 0;
    colorAttachmentView.subresourceRange.layerCount = 1;
    colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;

    for (uint32_t i = 0; i < mImageCount; i++) {
        GVkImage *gvkImage = GX_NEW(GVkImage);
        gvkImage->create(mDevice, images[i], mColorFormat,
                         mWidth, mHeight, 1,
                         VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT,
                         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        mBuffers[i].gvkImage = gvkImage;

        colorAttachmentView.image = images[i];

        VK_CHECK_RESULT(vkCreateImageView(*mDevice, &colorAttachmentView, nullptr, &mBuffers[i].view));
    }
}

}