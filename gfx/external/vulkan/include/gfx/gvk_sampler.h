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

#ifndef GFX_GVK_SAMPLER_H
#define GFX_GVK_SAMPLER_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>


namespace gfx
{

class GVkDevice;

class GX_API GVkSampler
{
CLASS_DEF(GVkSampler)

public:
    void create(GVkDevice *device, bool toCreate = true);

    void destroy();

    bool isCreated() const;

public:
    operator VkSampler() const;

    VkSampler vkSampler() const;

    void beginResetSampler();

    void endResetSampler();

    void setSamplerFilter(VkFilter magFilter, VkFilter minFilter);

    void setSamplerMipmapMode(VkSamplerMipmapMode mipmapMode);

    void setSamplerAddressMode(VkSamplerAddressMode addressModeU,
                               VkSamplerAddressMode addressModeV,
                               VkSamplerAddressMode addressModeW);

    void setSamplerMipLodBias(float mipLodBias);

    void setSamplerAnisotropyEnable(bool anisotropyEnable);

    void setSamplerMaxAnisotropy(float maxAnisotropy);

    void setSamplerCompareEnable(bool compareEnable);

    void setSamplerCompareOp(VkCompareOp compareOp);

    void setSamplerMinLod(float minLod);

    void setSamplerMaxLod(float maxLod);

    void setSamplerBorderColor(VkBorderColor borderColor);

    void setSamplerBorderColorFloatV(float r, float g, float b, float a);

    void setSamplerUnnormalizedCoordinates(bool unnormalizedCoordinates);

private:
    void reset();

private:
    GVkDevice *mDevice = nullptr;
    VkSampler mHandle = VK_NULL_HANDLE;

    // sampler infos
    bool mRecordSamplerMode = false;
    VkFilter mMagFilter = VK_FILTER_LINEAR;
    VkFilter mMinFilter = VK_FILTER_LINEAR;
    VkSamplerMipmapMode mMipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    VkSamplerAddressMode mAddressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode mAddressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode mAddressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    float mMipLodBias = 0.0f;
    VkBool32 mAnisotropyEnable = VK_FALSE;
    float mMaxAnisotropy = 1.0f;
    VkBool32 mCompareEnable = VK_FALSE;
    VkCompareOp mCompareOp = VK_COMPARE_OP_NEVER;
    float mMinLod = 0.0f;
    float mMaxLod = VK_LOD_CLAMP_NONE;
    VkBorderColor mBorderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    VkBool32 mUnnormalizedCoordinates = VK_FALSE;
    struct {
        float r;
        float g;
        float b;
        float a;
    } mBorderColorFloatV;
};

}

#endif //GFX_GVK_SAMPLER_H
