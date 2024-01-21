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

#include "gfx/gvk_image.h"

#include <gfx/gvk_device.h>
#include <gfx/gvk_buffer.h>
#include <gfx/gvk_tools.h>
#include <gfx/gvk_fence.h>

#include <memory.h>


namespace gfx
{

void GVkImage::create(GVkDevice *device, VkImageType imageType, VkFormat format,
                      uint32_t width, uint32_t height, uint32_t depth,
                      VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage,
                      VkImageAspectFlags aspectMask,
                      VkSharingMode sharingMode, VkImageLayout layout,
                      uint32_t mipLevels, uint32_t arrayLayers, VkFlags imageFlag)
{
    GX_ASSERT_S(!isCreated(), "GVkImage::create is created");
    if (isCreated()) {
        return;
    }
    this->mIsHostedImage = false;
    this->mDevice = device;
    this->mImageType = imageType;
    this->mFormat = format;
    this->mWidth = width;
    this->mHeight = height;
    this->mDepth = depth;
    this->mMipLevels = mipLevels;
    this->mArrayLayers = arrayLayers;
    this->mSamples = samples;
    this->mTiling = tiling;
    this->mUsage = usage;
    this->mSharingMode = sharingMode;
    this->mLayout = layout;
    this->mAspectMask = aspectMask;

    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.flags = imageFlag;
    imageCreateInfo.imageType = imageType;
    imageCreateInfo.format = format;
    imageCreateInfo.extent = {width, height, depth};
    imageCreateInfo.samples = samples;
    imageCreateInfo.tiling = tiling;
    imageCreateInfo.usage = usage;
    imageCreateInfo.sharingMode = sharingMode;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.mipLevels = mipLevels;
    imageCreateInfo.arrayLayers = arrayLayers;

    VK_CHECK_RESULT(vkCreateImage(*device, &imageCreateInfo, nullptr, &mHandle));

    VkMemoryAllocateInfo memAllocInfo{};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

    vkGetImageMemoryRequirements(*mDevice, mHandle, &mMemReqs);
    memAllocInfo.allocationSize = mMemReqs.size;
    if (tiling == VK_IMAGE_TILING_LINEAR) {
        memAllocInfo.memoryTypeIndex = mDevice->getMemoryType(mMemReqs.memoryTypeBits,
                                                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    } else {
        memAllocInfo.memoryTypeIndex = mDevice->getMemoryType(mMemReqs.memoryTypeBits,
                                                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    }
    VK_CHECK_RESULT(vkAllocateMemory(*mDevice, &memAllocInfo, nullptr, &mMemory));
    VK_CHECK_RESULT(vkBindImageMemory(*mDevice, mHandle, mMemory, 0));
}

void GVkImage::create(GVkDevice *device, VkImage vkImage, VkFormat format,
                      uint32_t width, uint32_t height, uint32_t depth,
                      VkImageAspectFlags aspectMask, VkImageLayout layout)
{
    GX_ASSERT_S(!isCreated(), "GVkImage::create is created");
    if (isCreated()) {
        return;
    }
    this->mIsHostedImage = true;
    this->mDevice = device;
    this->mHandle = vkImage;
    this->mFormat = format;
    this->mWidth = width;
    this->mHeight = height;
    this->mDepth = depth;
    this->mAspectMask = aspectMask;
    this->mLayout = layout;

    vkGetImageMemoryRequirements(*mDevice, mHandle, &mMemReqs);
}

void GVkImage::destroy()
{
    if (mDevice && !mIsHostedImage && mHandle != VK_NULL_HANDLE) {
        vkDestroyImage(*mDevice, mHandle, nullptr);
        vkFreeMemory(*mDevice, mMemory, nullptr);
    }
    mHandle = VK_NULL_HANDLE;
    mMemory = VK_NULL_HANDLE;
    mDevice = nullptr;
}

bool GVkImage::isCreated() const
{
    return mHandle != VK_NULL_HANDLE;
}

GVkImage::operator VkImage() const
{
    return mHandle;
}

VkImage GVkImage::vkImage() const
{
    return mHandle;
}

void GVkImage::imageMemoryBarrier(VkCommandBuffer cmdBuffer, VkImageLayout srcLayout, VkImageLayout dstLayout,
                                  const VkImageSubresourceRange &subResRange, bool computeUsage)
{
    VkImageMemoryBarrier imageBarrier{};
    imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageBarrier.image = mHandle;
    imageBarrier.subresourceRange = subResRange;
    imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    VkPipelineStageFlags srcStage = 0;
    VkPipelineStageFlags dstStage = 0;
    setImageBarrierInfo(srcLayout, dstLayout, imageBarrier, srcStage, dstStage, computeUsage);

    if (srcLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
        srcStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    } else if (dstLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
        dstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }

    vkCmdPipelineBarrier(cmdBuffer, srcStage, dstStage, 0,
                         0, nullptr,
                         0, nullptr,
                         1, &imageBarrier);

    setLayout(imageBarrier.newLayout);
}

uint32_t GVkImage::width() const
{
    return mWidth;
}

uint32_t GVkImage::height() const
{
    return mHeight;
}

uint32_t GVkImage::depth() const
{
    return mDepth;
}

uint64_t GVkImage::size() const
{
    return mMemReqs.size;
}

uint32_t GVkImage::mipLevels() const
{
    return mMipLevels;
}

uint32_t GVkImage::arrayLayers() const
{
    return mArrayLayers;
}

VkImageLayout GVkImage::layout() const
{
    return mLayout;
}

void GVkImage::setLayout(VkImageLayout layout)
{
    mLayout = layout;
}

VkImageAspectFlags GVkImage::aspectMask() const
{
    return mAspectMask;
}

void GVkImage::setImageBarrierInfo(VkImageLayout src, VkImageLayout dst, VkImageMemoryBarrier &barrier,
                                   VkPipelineStageFlags &srcStage, VkPipelineStageFlags &dstStage,
                                   bool computeUsage)
{
    srcStage |= getImageBarrierFlags(src, barrier.srcAccessMask, barrier.oldLayout, computeUsage);
    dstStage |= getImageBarrierFlags(dst, barrier.dstAccessMask, barrier.newLayout, computeUsage);
}

VkPipelineStageFlags GVkImage::getImageBarrierFlags(VkImageLayout target,
                                                    VkAccessFlags &accessFlags,
                                                    VkImageLayout &layout,
                                                    bool computeUsage)
{
    VkPipelineStageFlags stageFlags = 0;
    switch (target) {
        case VK_IMAGE_LAYOUT_UNDEFINED: {
            accessFlags = 0;
            stageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            layout = VK_IMAGE_LAYOUT_UNDEFINED;
        }
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: {
            accessFlags = VK_ACCESS_TRANSFER_WRITE_BIT;
            stageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
            layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        }
            break;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: {
            accessFlags = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            stageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
            break;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: {
            accessFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            stageFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: {
            accessFlags = VK_ACCESS_TRANSFER_READ_BIT;
            stageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
            layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        }
            break;
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: {
            accessFlags = 0;
            stageFlags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        }
            break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: {
            accessFlags = VK_ACCESS_SHADER_READ_BIT;
            stageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
            break;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL: {
            accessFlags = VK_ACCESS_SHADER_READ_BIT;
            stageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        }
            break;
        case VK_IMAGE_LAYOUT_GENERAL: {
            accessFlags = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;
            stageFlags = computeUsage ? VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT : VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            layout = VK_IMAGE_LAYOUT_GENERAL;
        }
            break;
        default: {
            Log("Unknown ImageLayout %d", (int32_t) target);
        }
            break;
    }

    return stageFlags;
}

}