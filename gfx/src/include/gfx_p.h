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

#ifndef GX_GFX_P_H
#define GX_GFX_P_H

#include <gfx/gfx_core.h>

#include "gfx_private.h"


namespace gfx
{

class Context_T;

GFX_P_API_DEFINE(Instance);
GFX_P_API_DEFINE(Context);
GFX_P_API_DEFINE(Fence);
GFX_P_API_DEFINE(Frame);
GFX_P_API_DEFINE(RenderTarget);
GFX_P_API_DEFINE(Buffer);
GFX_P_API_DEFINE(Texture);
GFX_P_API_DEFINE(Sampler);
GFX_P_API_DEFINE(Shader);
GFX_P_API_DEFINE(ResourceBinder);
GFX_P_API_DEFINE(CommandBuffer);
GFX_P_API_DEFINE(Query);


GFX_P_API(Instance)
{
    GFX_API_FUNC(bool init(const CreateInstanceInfo &createInfo));

    GFX_API_FUNC(uint32_t deviceCount());

    GFX_API_FUNC(DeviceInfo deviceInfo(uint32_t deviceIndex));

    GFX_API_FUNC(bool isCreated());

    GFX_API_FUNC(Context_P *createContextP());

    GFX_API_FUNC(void destroyContextP(Context_P *obj));
};


GFX_P_API(Context)
{
    GFX_API_FUNC(bool init(Instance_P *instance, Context_T *context, const CreateContextInfo &createInfo));

    GFX_API_FUNC(Format::Enum getSupportedDepthFormat());

    GFX_API_FUNC(std::vector<Format::Enum> formatSupported(const std::vector<Format::Enum> &formats,
                                                           FormatFeatureFlags featureFlags));

    GFX_API_FUNC(size_t uniformBufferOffsetAlignment(size_t dataSize));

    GFX_API_FUNC(size_t storageBufferOffsetAlignment(size_t dataSize));

    GFX_API_FUNC(size_t texelBufferOffsetAlignment(size_t dataSize));

    GFX_API_FUNC(SampleCountFlag::Enum maxRenderTargetSampleCount());

    GFX_API_FUNC(uint32_t maxTextureDimension1D());

    GFX_API_FUNC(uint32_t maxTextureDimension2D());

    GFX_API_FUNC(uint32_t maxTextureDimension3D());

    GFX_API_FUNC(uint32_t maxTextureDimensionCube());

    GFX_API_FUNC(uint32_t maxTextureArrayLayers());

    GFX_API_FUNC(bool isSupportTextureCubeArray());

    GFX_API_FUNC(uint32_t maxPerStageShaderSamplersCount());

    GFX_API_FUNC(uint32_t maxPerStageShaderUniformBuffersCount());

    GFX_API_FUNC(uint32_t maxPerStageShaderStorageBuffersCount());

    GFX_API_FUNC(uint32_t maxPerStageShaderSampledImagesCount());

    GFX_API_FUNC(uint32_t maxPerStageShaderStorageImagesCount());

    GFX_API_FUNC(void waitIdle());

    GFX_API_FUNC(void submitCommandBlock(CommandBuffer cmdBuffer, uint32_t bufferIndex));

    GFX_API_FUNC(void submitCommand(CommandBuffer cmdBuffer, uint32_t bufferIndex, Fence fence));

    GFX_API_FUNC(void queueWaitIdle(QueueType::Enum queueType));

    GFX_API_FUNC(std::string dumpCommandBuffer(CommandBuffer commandBuffer));

    GFX_API_FUNC(Fence_P *createFenceP(bool signaled));

    GFX_API_FUNC(void destroyFenceP(Fence obj));

    GFX_API_FUNC(Fence_P *findFenceP(GfxIdxTy idx));

    GFX_API_FUNC(Frame_P *createFrameP(const CreateFrameInfo &createInfo));

    GFX_API_FUNC(void destroyFrameP(Frame obj));

    GFX_API_FUNC(Frame_P *findFrameP(GfxIdxTy idx));

    GFX_API_FUNC(RenderTarget_P *createRenderTargetP(const CreateRenderTargetInfo &createInfo));

    GFX_API_FUNC(void destroyRenderTargetP(RenderTarget obj));

    GFX_API_FUNC(RenderTarget_P *findRenderTargetP(GfxIdxTy idx));

    GFX_API_FUNC(Texture_P *createTextureP(const CreateTextureInfo &createInfo,
                                           SampleCountFlag::Enum sample = SampleCountFlag::SampleCount_1));

    GFX_API_FUNC(void destroyTextureP(Texture obj));

    GFX_API_FUNC(Texture_P *findTextureP(GfxIdxTy idx));

    GFX_API_FUNC(Sampler_P *createSamplerP(const CreateSamplerInfo &createInfo));

    GFX_API_FUNC(void destroySamplerP(Sampler obj));

    GFX_API_FUNC(Sampler_P *findSamplerP(GfxIdxTy idx));

    GFX_API_FUNC(Buffer_P *createBufferP(const CreateBufferInfo &createInfo));

    GFX_API_FUNC(void destroyBufferP(Buffer obj));

    GFX_API_FUNC(Buffer_P *findBufferP(GfxIdxTy idx));

    GFX_API_FUNC(Shader_P *createShaderP(const CreateShaderInfo &createInfo));

    GFX_API_FUNC(void destroyShaderP(Shader obj));

    GFX_API_FUNC(Shader_P *findShaderP(GfxIdxTy idx));

    GFX_API_FUNC(ResourceBinder_P *createResourceBinderP(const ResourceLayoutInfo &layoutInfo));

    GFX_API_FUNC(void destroyResourceBinderP(ResourceBinder obj));

    GFX_API_FUNC(ResourceBinder_P *findResourceBinderP(GfxIdxTy idx));

    GFX_API_FUNC(CommandBuffer_P *createCommandBufferP(const CreateCommandBufferInfo &createInfo));

    GFX_API_FUNC(void destroyCommandBufferP(CommandBuffer obj));

    GFX_API_FUNC(CommandBuffer_P *findCommandBufferP(GfxIdxTy idx));

    GFX_API_FUNC(Query_P *createQueryP(const CreateQueryInfo &createInfo));

    GFX_API_FUNC(void destroyQueryP(Query obj));

    GFX_API_FUNC(Query_P *findQueryP(GfxIdxTy idx));
};


typedef Instance_P *(*createInstanceP_Fn)();

typedef void (*destroyInstanceP_Fn)(Instance_P *);

#define GFX_P_INTERFACE(_namespace) \
namespace _namespace        \
{                           \
extern Instance_P *createInstanceP();            \
extern void destroyInstanceP(Instance_P *);      \
}

//GFX_P_INTERFACE(none)
GFX_P_INTERFACE(vk)

#undef GFX_P_INTERFACE

struct PInstanceCDFuncs
{
    createInstanceP_Fn createInstancePFn;
    destroyInstanceP_Fn destroyInstancePFn;
};

/**
 * Instance的P层创建/销毁函数
 */
static PInstanceCDFuncs gfxPInstanceCDFuncs[] =
        {
//                {none::createInstanceP, none::destroyInstanceP},
                {vk::createInstanceP,   vk::destroyInstanceP}
        };
static_assert(ARRAY_LEN(gfxPInstanceCDFuncs) == TargetApiType::Count, "gfxPInstanceCDFuncs array size error");

}

#endif //GX_GFX_P_H
