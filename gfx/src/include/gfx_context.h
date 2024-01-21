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

#ifndef GX_GFX_CONTEXT_H
#define GX_GFX_CONTEXT_H

#include <gfx/gfx_core.h>
#include "gfx_private.h"
#include "gfx_instance.h"
#include "gfx_p.h"

#include <unordered_map>


namespace gfx
{

GFX_API_IMPL(Context)
{
public:
    bool init(Instance instance, const CreateContextInfo &createInfo);

    void destroy();

public:
    uint32_t deviceIndex() override;

    DeviceInfo deviceInfo() override;

    Format::Enum getSupportedDepthFormat() override;

    std::vector<Format::Enum> formatSupported(const std::vector<Format::Enum> &formats,
                                              FormatFeatureFlags featureFlags) override;

    size_t uniformBufferOffsetAlignment(size_t dataSize) override;

    size_t storageBufferOffsetAlignment(size_t dataSize) override;

    size_t texelBufferOffsetAlignment(size_t dataSize) override;

    size_t formatSize(Format::Enum format) override;

    size_t vertexAttribTypeSize(VertexAttribType::Enum type) override;

    SampleCountFlag::Enum maxRenderTargetSampleCount() override;

    uint32_t maxTextureDimension1D() override;

    uint32_t maxTextureDimension2D() override;

    uint32_t maxTextureDimension3D() override;

    uint32_t maxTextureDimensionCube() override;

    uint32_t maxTextureArrayLayers() override;

    bool isSupportTextureCubeArray() override;

    uint32_t maxPerStageShaderSamplersCount() override;

    uint32_t maxPerStageShaderUniformBuffersCount() override;

    uint32_t maxPerStageShaderStorageBuffersCount() override;

    uint32_t maxPerStageShaderSampledImagesCount() override;

    uint32_t maxPerStageShaderStorageImagesCount() override;

    void waitIdle() override;

    void submitCommandBlock(CommandBuffer cmdBuffer, uint32_t bufferIndex) override;

    void submitCommand(CommandBuffer cmdBuffer, uint32_t bufferIndex, Fence fence) override;

    void queueWaitIdle(QueueType::Enum queueType) override;

    std::string dumpCommandBuffer(CommandBuffer commandBuffer) override;

public:
    Context_P *contextP();

    Instance_T *instanceT();

    Fence createFence(bool signaled);

    void destroyFence(Fence obj);

    Frame createFrame(const CreateFrameInfo &createInfo);

    void destroyFrame(Frame obj);

    RenderTarget createRenderTarget(const CreateRenderTargetInfo &createInfo);

    void destroyRenderTarget(RenderTarget obj);

    Buffer createBuffer(const CreateBufferInfo &createInfo);

    void destroyBuffer(Buffer obj);

    Texture createTexture(const CreateTextureInfo &createInfo);

    void destroyTexture(Texture obj);

    Sampler createSampler(const CreateSamplerInfo &createInfo);

    void destroySampler(Sampler obj);

    Shader createShader(const CreateShaderInfo &createInfo);

    void destroyShader(Shader obj);

    ResourceBinder createResourceBinder(const ResourceLayoutInfo &layoutInfo);

    void destroyResourceBinder(ResourceBinder obj);

    CommandBuffer createCommandBuffer(const CreateCommandBufferInfo &createInfo);

    void destroyCommandBuffer(CommandBuffer obj);

    Query createQuery(const CreateQueryInfo &createInfo);

    void destroyQuery(Query obj);

private:
    Context_P *mHandleP = GFX_NULL_HANDLE;
    Instance_T *mInstanceT = GFX_NULL_HANDLE;

    uint32_t mDeviceIndex = 0;
};

}

#endif //GX_GFX_CONTEXT_H
