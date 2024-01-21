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

#include "gfx_context.h"

#include "gfx_element.h"

#include <gx/debug.h>


namespace gfx
{

bool Context_T::init(Instance instance, const CreateContextInfo &createInfo)
{
    mInstanceT = dynamic_cast<Instance_T *>(instance);
    GX_ASSERT(mInstanceT);

    mDeviceIndex = createInfo.deviceIndex;

    mHandleP = mInstanceT->instanceP()->createContextP();
    if (!mHandleP->init(mInstanceT->instanceP(), this, createInfo)) {
        Log("Can not init contextP");
        return false;
    }
    return true;
}

void Context_T::destroy()
{
    GX_ASSERT_S(mHandleP, "Handle is null object");
    GX_ASSERT_S(mInstanceT, "Instance is null object can not do destroy");
    mInstanceT->instanceP()->destroyContextP(mHandleP);
    mHandleP = GFX_NULL_HANDLE;
    mInstanceT = GFX_NULL_HANDLE;
}

uint32_t Context_T::deviceIndex()
{
    return mDeviceIndex;
}

DeviceInfo Context_T::deviceInfo()
{
    GX_ASSERT(mInstanceT);

    return mInstanceT->deviceInfo(mDeviceIndex);
}

Format::Enum Context_T::getSupportedDepthFormat()
{
    return mHandleP->getSupportedDepthFormat();
}

std::vector<Format::Enum>
Context_T::formatSupported(const std::vector<Format::Enum> &formats, FormatFeatureFlags featureFlags)
{
    return mHandleP->formatSupported(formats, featureFlags);
}

size_t Context_T::uniformBufferOffsetAlignment(size_t dataSize)
{
    return mHandleP->uniformBufferOffsetAlignment(dataSize);
}

size_t Context_T::storageBufferOffsetAlignment(size_t dataSize)
{
    return mHandleP->storageBufferOffsetAlignment(dataSize);
}

size_t Context_T::texelBufferOffsetAlignment(size_t dataSize)
{
    return mHandleP->texelBufferOffsetAlignment(dataSize);
}

size_t Context_T::formatSize(Format::Enum format)
{
    switch (format) {
        case Format::Undefined:
            return 0;
        case Format::R8_UNorm:
        case Format::R8_SNorm:
        case Format::R8_UInt:
        case Format::R8_SInt:
        case Format::R8_SRgb:
        case Format::S8_UInt:
            return 1;
        case Format::R8G8_UNorm:
        case Format::R8G8_SNorm:
        case Format::R8G8_UInt:
        case Format::R8G8_SInt:
        case Format::R8G8_SRgb:
        case Format::R5G6B5_UNorm:
        case Format::B5G6R5_UNorm:
        case Format::R5G5B5A1_UNorm:
        case Format::B5G5R5A1_UNorm:
        case Format::A1R5G5B5_UNorm:
        case Format::R4G4B4A4_UNorm:
        case Format::D16_UNorm:
        case Format::R16_UInt:
        case Format::R16_SInt:
        case Format::R16_UNorm:
        case Format::R16_SNorm:
        case Format::R16_SFloat:
            return 2;
        case Format::R8G8B8_UNorm:
        case Format::R8G8B8_SNorm:
        case Format::R8G8B8_UInt:
        case Format::R8G8B8_SInt:
        case Format::R8G8B8_SRgb:
        case Format::B8G8R8_UNorm:
        case Format::B8G8R8_SNorm:
        case Format::B8G8R8_UInt:
        case Format::B8G8R8_SInt:
        case Format::B8G8R8_SRgb:
        case Format::D16_UNorm_S8_UInt:
            return 3;
        case Format::R8G8B8A8_UNorm:
        case Format::R8G8B8A8_SNorm:
        case Format::R8G8B8A8_UInt:
        case Format::R8G8B8A8_SInt:
        case Format::R8G8B8A8_SRgb:
        case Format::B8G8R8A8_UNorm:
        case Format::B8G8R8A8_SNorm:
        case Format::B8G8R8A8_UInt:
        case Format::B8G8R8A8_SInt:
        case Format::B8G8R8A8_SRgb:
        case Format::A8B8G8R8_UNorm:
        case Format::A8B8G8R8_SNorm:
        case Format::A8B8G8R8_UInt:
        case Format::A8B8G8R8_SInt:
        case Format::A8B8G8R8_SRgb:
        case Format::A2R10G10B10_UNorm:
        case Format::A2R10G10B10_SNorm:
        case Format::A2R10G10B10_UInt:
        case Format::A2R10G10B10_SInt:
        case Format::A2B10G10R10_UNorm:
        case Format::A2B10G10R10_SNorm:
        case Format::A2B10G10R10_UInt:
        case Format::A2B10G10R10_SInt:
        case Format::X8_D24_UNorm:
        case Format::D32_SFloat:
        case Format::D24_UNorm_S8_UInt:
        case Format::R16G16_UInt:
        case Format::R16G16_SInt:
        case Format::R16G16_UNorm:
        case Format::R16G16_SNorm:
        case Format::R16G16_SFloat:
        case Format::R32_UInt:
        case Format::R32_SInt:
        case Format::R32_SFloat:
            return 4;
        case Format::D32_SFloat_S8_UInt:
            return 8;
        case Format::R16G16B16_UInt:
        case Format::R16G16B16_SInt:
        case Format::R16G16B16_UNorm:
        case Format::R16G16B16_SNorm:
        case Format::R16G16B16_SFloat:
            return 6;
        case Format::R16G16B16A16_UInt:
        case Format::R16G16B16A16_SInt:
        case Format::R16G16B16A16_UNorm:
        case Format::R16G16B16A16_SNorm:
        case Format::R16G16B16A16_SFloat:
        case Format::R32G32_UInt:
        case Format::R32G32_SInt:
        case Format::R32G32_SFloat:
        case Format::R64_UInt:
        case Format::R64_SInt:
        case Format::R64_SFloat:
            return 8;
        case Format::R32G32B32_UInt:
        case Format::R32G32B32_SInt:
        case Format::R32G32B32_SFloat:
            return 12;
        case Format::R32G32B32A32_UInt:
        case Format::R32G32B32A32_SInt:
        case Format::R32G32B32A32_SFloat:
        case Format::R64G64_UInt:
        case Format::R64G64_SInt:
        case Format::R64G64_SFloat:
            return 16;
        case Format::R64G64B64_UInt:
        case Format::R64G64B64_SInt:
        case Format::R64G64B64_SFloat:
            return 24;
        case Format::R64G64B64A64_UInt:
        case Format::R64G64B64A64_SInt:
        case Format::R64G64B64A64_SFloat:
            return 32;
        case Format::B10G11R11_UFloat:
        case Format::E5B9G9R9_UFloat:
            return 4;
        case Format::BC1_RGB_UNorm:
        case Format::BC1_RGB_SRgb:
        case Format::BC1_RGBA_UNorm:
        case Format::BC1_RGBA_SRgb:
            return 8;
        case Format::BC2_UNorm:
        case Format::BC2_SRgb:
        case Format::BC3_UNorm:
        case Format::BC3_SRgb:
            return 16;
        case Format::BC4_UNorm:
        case Format::BC4_SNorm:
            return 8;
        case Format::BC5_UNorm:
        case Format::BC5_SNorm:
            return 16;
        case Format::BC6H_UFloat:
        case Format::BC6H_SFloat:
        case Format::BC7_UNorm:
        case Format::BC7_SRgb:
            return 16;
        case Format::ETC2_R8G8B8_UNorm:
        case Format::ETC2_R8G8B8_SRgb:
        case Format::ETC2_R8G8B8A1_UNorm:
        case Format::ETC2_R8G8B8A1_SRgb:
            return 8;
        case Format::ETC2_R8G8B8A8_UNorm:
        case Format::ETC2_R8G8B8A8_SRgb:
            return 16;
        case Format::EAC_R11_UNorm:
        case Format::EAC_R11_SNorm:
            return 8;
        case Format::EAC_R11G11_UNorm:
        case Format::EAC_R11G11_SNorm:
            return 16;
        case Format::ASTC_4x4_UNorm:
        case Format::ASTC_4x4_SRgb:
        case Format::ASTC_5x4_UNorm:
        case Format::ASTC_5x4_SRgb:
        case Format::ASTC_5x5_UNorm:
        case Format::ASTC_5x5_SRgb:
        case Format::ASTC_6x5_UNorm:
        case Format::ASTC_6x5_SRgb:
        case Format::ASTC_6x6_UNorm:
        case Format::ASTC_6x6_SRgb:
        case Format::ASTC_8x5_UNorm:
        case Format::ASTC_8x5_SRgb:
        case Format::ASTC_8x6_UNorm:
        case Format::ASTC_8x6_SRgb:
        case Format::ASTC_8x8_UNorm:
        case Format::ASTC_8x8_SRgb:
        case Format::ASTC_10x5_UNorm:
        case Format::ASTC_10x5_SRgb:
        case Format::ASTC_10x6_UNorm:
        case Format::ASTC_10x6_SRgb:
        case Format::ASTC_10x8_UNorm:
        case Format::ASTC_10x8_SRgb:
        case Format::ASTC_10x10_UNorm:
        case Format::ASTC_10x10_SRgb:
        case Format::ASTC_12x10_UNorm:
        case Format::ASTC_12x10_SRgb:
        case Format::ASTC_12x12_UNorm:
        case Format::ASTC_12x12_SRgb:
            return 16;
        case Format::Count:
            return 0;
    }
    return 0;
}

size_t Context_T::vertexAttribTypeSize(VertexAttribType::Enum type)
{
    switch (type) {
        case VertexAttribType::Byte:
        case VertexAttribType::UByte:
            return 1;
        case VertexAttribType::Byte2:
        case VertexAttribType::UByte2:
        case VertexAttribType::Short:
        case VertexAttribType::UShort:
        case VertexAttribType::Half:
            return 2;
        case VertexAttribType::Byte3:
        case VertexAttribType::UByte3:
            return 3;
        case VertexAttribType::Byte4:
        case VertexAttribType::UByte4:
        case VertexAttribType::Short2:
        case VertexAttribType::UShort2:
        case VertexAttribType::Int:
        case VertexAttribType::UInt:
        case VertexAttribType::Float:
        case VertexAttribType::Half2:
            return 4;
        case VertexAttribType::Short3:
        case VertexAttribType::UShort3:
        case VertexAttribType::Half3:
            return 6;
        case VertexAttribType::Short4:
        case VertexAttribType::UShort4:
        case VertexAttribType::Float2:
        case VertexAttribType::Half4:
            return 8;
        case VertexAttribType::Float3:
            return 12;
        case VertexAttribType::Float4:
            return 16;
        case VertexAttribType::Count:
            return 0;
    }
    return 0;
}

SampleCountFlag::Enum Context_T::maxRenderTargetSampleCount()
{
    return mHandleP->maxRenderTargetSampleCount();
}

uint32_t Context_T::maxTextureDimension1D()
{
    return mHandleP->maxTextureDimension1D();
}

uint32_t Context_T::maxTextureDimension2D()
{
    return mHandleP->maxTextureDimension2D();
}

uint32_t Context_T::maxTextureDimension3D()
{
    return mHandleP->maxTextureDimension3D();
}

uint32_t Context_T::maxTextureDimensionCube()
{
    return mHandleP->maxTextureDimensionCube();
}

uint32_t Context_T::maxTextureArrayLayers()
{
    return mHandleP->maxTextureArrayLayers();
}

bool Context_T::isSupportTextureCubeArray()
{
    return mHandleP->isSupportTextureCubeArray();
}

uint32_t Context_T::maxPerStageShaderSamplersCount()
{
    return mHandleP->maxPerStageShaderSamplersCount();
}

uint32_t Context_T::maxPerStageShaderUniformBuffersCount()
{
    return mHandleP->maxPerStageShaderUniformBuffersCount();
}

uint32_t Context_T::maxPerStageShaderStorageBuffersCount()
{
    return mHandleP->maxPerStageShaderStorageBuffersCount();
}

uint32_t Context_T::maxPerStageShaderSampledImagesCount()
{
    return mHandleP->maxPerStageShaderSampledImagesCount();
}

uint32_t Context_T::maxPerStageShaderStorageImagesCount()
{
    return mHandleP->maxPerStageShaderStorageImagesCount();
}

void Context_T::waitIdle()
{
    mHandleP->waitIdle();
}

void Context_T::submitCommandBlock(CommandBuffer cmdBuffer, uint32_t bufferIndex)
{
    mHandleP->submitCommandBlock(cmdBuffer, bufferIndex);
}

void Context_T::submitCommand(CommandBuffer cmdBuffer, uint32_t bufferIndex, Fence fence)
{
    mHandleP->submitCommand(cmdBuffer, bufferIndex, fence);
}

void Context_T::queueWaitIdle(QueueType::Enum queueType)
{
    mHandleP->queueWaitIdle(queueType);
}

std::string Context_T::dumpCommandBuffer(CommandBuffer commandBuffer)
{
    return mHandleP->dumpCommandBuffer(commandBuffer);
}

Context_P *Context_T::contextP()
{
    return mHandleP;
}

Instance_T *Context_T::instanceT()
{
    return mInstanceT;
}

Fence Context_T::createFence(bool signaled)
{
    return mHandleP->createFenceP(signaled);
}

void Context_T::destroyFence(Fence obj)
{
    mHandleP->destroyFenceP(obj);
}

Frame Context_T::createFrame(const CreateFrameInfo &createInfo)
{
    return mHandleP->createFrameP(createInfo);
}

void Context_T::destroyFrame(Frame obj)
{
    mHandleP->destroyFrameP(obj);
}

RenderTarget Context_T::createRenderTarget(const CreateRenderTargetInfo &createInfo)
{
    return mHandleP->createRenderTargetP(createInfo);
}

void Context_T::destroyRenderTarget(RenderTarget obj)
{
    mHandleP->destroyRenderTargetP(obj);
}

Buffer Context_T::createBuffer(const CreateBufferInfo &createInfo)
{
    return mHandleP->createBufferP(createInfo);
}

void Context_T::destroyBuffer(Buffer obj)
{
    mHandleP->destroyBufferP(obj);
}

Texture Context_T::createTexture(const CreateTextureInfo &createInfo)
{
    return mHandleP->createTextureP(createInfo);
}

void Context_T::destroyTexture(Texture obj)
{
    mHandleP->destroyTextureP(obj);
}

Sampler Context_T::createSampler(const CreateSamplerInfo &createInfo)
{
    return mHandleP->createSamplerP(createInfo);
}

void Context_T::destroySampler(Sampler obj)
{
    mHandleP->destroySamplerP(obj);
}

Shader Context_T::createShader(const CreateShaderInfo &createInfo)
{
    return mHandleP->createShaderP(createInfo);
}

void Context_T::destroyShader(Shader obj)
{
    mHandleP->destroyShaderP(obj);
}

ResourceBinder Context_T::createResourceBinder(const ResourceLayoutInfo &layoutInfo)
{
    return mHandleP->createResourceBinderP(layoutInfo);
}

void Context_T::destroyResourceBinder(ResourceBinder obj)
{
    mHandleP->destroyResourceBinderP(obj);
}

CommandBuffer Context_T::createCommandBuffer(const CreateCommandBufferInfo &createInfo)
{
    return mHandleP->createCommandBufferP(createInfo);
}

void Context_T::destroyCommandBuffer(CommandBuffer obj)
{
    mHandleP->destroyCommandBufferP(obj);
}

Query Context_T::createQuery(const CreateQueryInfo &createInfo)
{
    return mHandleP->createQueryP(createInfo);
}

void Context_T::destroyQuery(Query obj)
{
    mHandleP->destroyQueryP(obj);
}

}
