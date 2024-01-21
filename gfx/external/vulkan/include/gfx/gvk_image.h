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

#ifndef GFX_GVK_IMAGE_H
#define GFX_GVK_IMAGE_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>

#include <cassert>

namespace gfx
{

class GVkDevice;

class GVkBuffer;

class GVkQueue;

class GVkCommandPool;

class GX_API GVkImage
{
CLASS_DEF(GVkImage)

public:
    void create(GVkDevice *device,
                VkImageType imageType,
                VkFormat format,
                uint32_t width,
                uint32_t height,
                uint32_t depth,
                VkSampleCountFlagBits samples,
                VkImageTiling tiling,
                VkImageUsageFlags usage,
                VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED,
                uint32_t mipLevels = 1,
                uint32_t arrayLayers = 1,
                VkFlags imageFlag = 0);

    /**
     * 使用托管模式创建
     * 传入的vkImage，不会被分配Memory，也不会在destroy时被销毁
     *
     * @param device
     * @param vkImage
     * @param format
     * @param width
     * @param height
     * @param depth
     * @param aspectMask
     */
    void create(GVkDevice *device,
                VkImage vkImage,
                VkFormat format,
                uint32_t width,
                uint32_t height,
                uint32_t depth = 1,
                VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED);

    void destroy();

public:
    bool isCreated() const;

    operator VkImage() const;

    VkImage vkImage() const;

    void imageMemoryBarrier(VkCommandBuffer cmdBuffer,
                            VkImageLayout srcLayout,
                            VkImageLayout dstLayout,
                            const VkImageSubresourceRange &subResRange,
                            bool computeUsage);

public:
    uint32_t width() const;

    uint32_t height() const;

    uint32_t depth() const;

    uint64_t size() const;

    uint32_t mipLevels() const;

    uint32_t arrayLayers() const;

    VkImageLayout layout() const;

    void setLayout(VkImageLayout layout);

    VkImageAspectFlags aspectMask() const;

private:
    void setImageBarrierInfo(VkImageLayout src, VkImageLayout dst, VkImageMemoryBarrier &barrier,
                             VkPipelineStageFlags &srcStage, VkPipelineStageFlags &dstStage,
                             bool computeUsage);

    VkPipelineStageFlags getImageBarrierFlags(VkImageLayout target,
                                              VkAccessFlags &accessFlags,
                                              VkImageLayout &layout,
                                              bool computeUsage);

private:
    GVkDevice *mDevice = nullptr;
    VkImage mHandle = VK_NULL_HANDLE;

    bool mIsHostedImage = false;
    VkImageType mImageType = VK_IMAGE_TYPE_2D;
    VkFormat mFormat = VK_FORMAT_R8G8B8A8_UNORM;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    uint32_t mDepth = 1;
    uint32_t mMipLevels = 1;
    uint32_t mArrayLayers = 1;
    VkSampleCountFlagBits mSamples = VK_SAMPLE_COUNT_1_BIT;
    VkImageTiling mTiling = VK_IMAGE_TILING_OPTIMAL;
    VkImageUsageFlags mUsage = 0;
    VkSharingMode mSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkImageLayout mLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageAspectFlags mAspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    VkMemoryRequirements mMemReqs{};
    VkDeviceMemory mMemory = VK_NULL_HANDLE;
};

}


#endif //GFX_GVK_IMAGE_H
