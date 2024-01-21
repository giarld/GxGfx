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

#include "gfx/gvk_sampler.h"

#include <gfx/gvk_device.h>
#include <gfx/gvk_tools.h>


namespace gfx
{

void GVkSampler::create(GVkDevice *device)
{
    mDevice = device;

    mMaxAnisotropy = device->deviceFeatures().samplerAnisotropy ? device->deviceProperties().limits.maxSamplerAnisotropy : 1.0f;
    mAnisotropyEnable = device->deviceFeatures().samplerAnisotropy;
    reset();
}

void GVkSampler::destroy()
{
    vkDestroySampler(*mDevice, mHandle, nullptr);

    mHandle = VK_NULL_HANDLE;
    mDevice = nullptr;
}

bool GVkSampler::isCreated() const
{
    return mHandle != VK_NULL_HANDLE;
}

GVkSampler::operator VkSampler() const
{
    return mHandle;
}

VkSampler GVkSampler::vkSampler() const
{
    return mHandle;
}

void GVkSampler::beginResetSampler()
{
    mRecordSamplerMode = true;
}

void GVkSampler::endResetSampler()
{
    mRecordSamplerMode = false;
    reset();
}

void GVkSampler::setSamplerFilter(VkFilter magFilter, VkFilter minFilter)
{
    this->mMagFilter = magFilter;
    this->mMinFilter = minFilter;
    reset();
}

void GVkSampler::setSamplerMipmapMode(VkSamplerMipmapMode mipmapMode)
{
    this->mMipmapMode = mipmapMode;
    reset();
}

void GVkSampler::setSamplerAddressMode(VkSamplerAddressMode addressModeU, VkSamplerAddressMode addressModeV,
                                       VkSamplerAddressMode addressModeW)
{
    this->mAddressModeU = addressModeU;
    this->mAddressModeV = addressModeV;
    this->mAddressModeW = addressModeW;
    reset();
}

void GVkSampler::setSamplerMipLodBias(float mipLodBias)
{
    this->mMipLodBias = mipLodBias;
    reset();
}

void GVkSampler::setSamplerAnisotropyEnable(bool anisotropyEnable)
{
    this->mAnisotropyEnable = anisotropyEnable;
    reset();
}

void GVkSampler::setSamplerMaxAnisotropy(float maxAnisotropy)
{
    this->mMaxAnisotropy = maxAnisotropy;
    reset();
}

void GVkSampler::setSamplerCompareEnable(bool compareEnable)
{
    this->mCompareEnable = compareEnable;
    reset();
}

void GVkSampler::setSamplerCompareOp(VkCompareOp compareOp)
{
    this->mCompareOp = compareOp;
    reset();
}

void GVkSampler::setSamplerMinLod(float minLod)
{
    this->mMinLod = minLod;
    reset();
}

void GVkSampler::setSamplerMaxLod(float maxLod)
{
    this->mMaxLod = maxLod;
    reset();
}

void GVkSampler::setSamplerBorderColor(VkBorderColor borderColor)
{
    this->mBorderColor = borderColor;
    reset();
}

void GVkSampler::setSamplerBorderColorFloatV(float r, float g, float b, float a)
{
    this->mBorderColorFloatV.r = r;
    this->mBorderColorFloatV.g = g;
    this->mBorderColorFloatV.b = b;
    this->mBorderColorFloatV.a = a;
    reset();
}

void GVkSampler::setSamplerUnnormalizedCoordinates(bool unnormalizedCoordinates)
{
    this->mUnnormalizedCoordinates = unnormalizedCoordinates;
    reset();
}

void GVkSampler::reset()
{
    if (mRecordSamplerMode) {
        return;
    }

    if (mDevice == nullptr) {
        return;
    }

    if (mHandle != VK_NULL_HANDLE) {
        vkDestroySampler(*mDevice, mHandle, nullptr);
    }

    VkSamplerCreateInfo samplerCreateInfo{};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter = mMagFilter;
    samplerCreateInfo.minFilter = mMinFilter;
    samplerCreateInfo.mipmapMode = mMipmapMode;
    samplerCreateInfo.addressModeU = mAddressModeU;
    samplerCreateInfo.addressModeV = mAddressModeV;
    samplerCreateInfo.addressModeW = mAddressModeW;
    samplerCreateInfo.mipLodBias = mMipLodBias;
    samplerCreateInfo.anisotropyEnable = mAnisotropyEnable;
    samplerCreateInfo.maxAnisotropy = mMaxAnisotropy;
    samplerCreateInfo.compareEnable = mCompareEnable;
    samplerCreateInfo.compareOp = mCompareOp;
    samplerCreateInfo.minLod = mMinLod;
    samplerCreateInfo.maxLod = mMaxLod;
    samplerCreateInfo.borderColor = mBorderColor;
    samplerCreateInfo.unnormalizedCoordinates = mUnnormalizedCoordinates;

#if defined(VK_API_VERSION_1_2)
    VkSamplerCustomBorderColorCreateInfoEXT borderColor;
    if (mAddressModeU == VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
        || mAddressModeV == VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
        || mAddressModeW == VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER) {
        borderColor.sType = VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT;
        borderColor.format = VK_FORMAT_R8G8B8A8_UNORM;
        borderColor.customBorderColor.float32[0] = mBorderColorFloatV.r;
        borderColor.customBorderColor.float32[1] = mBorderColorFloatV.g;
        borderColor.customBorderColor.float32[2] = mBorderColorFloatV.b;
        borderColor.customBorderColor.float32[3] = mBorderColorFloatV.a;
        samplerCreateInfo.pNext = &borderColor;
    } else {
#endif
        samplerCreateInfo.pNext = nullptr;
#if defined(VK_API_VERSION_1_2)
    }
#endif

    VK_CHECK_RESULT(vkCreateSampler(*mDevice, &samplerCreateInfo, nullptr, &mHandle));
}

}
