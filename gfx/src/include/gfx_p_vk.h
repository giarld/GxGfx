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

#ifndef GX_GFX_P_VK_H
#define GX_GFX_P_VK_H

#include "gfx_p.h"

#include "gfx_element.h"
#include "gfx_private.h"
#include "gfx_def_vk.h"

#include <gfx/vulkan.h>
#include <gfx/gvk_instance.h>
#include <gfx/gvk_context.h>
#include <gfx/gvk_base_swapchain.h>
#include <gfx/gvk_renderpass.h>
#include <gfx/gvk_framebuffer.h>
#include <gfx/gvk_image.h>
#include <gfx/gvk_buffer.h>
#include <gfx/gvk_sampler.h>
#include <gfx/gvk_fence.h>
#include <gfx/gvk_semaphore.h>
#include <gfx/gvk_queue.h>

#include <gx/gbytearray.h>
#include <gx/gid_allocator.h>
#include <gx/gtime.h>

#include <functional>
#include <unordered_set>

#include <gx/gmutex.h>
#include <memory>

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
#ifndef VMA_IMPLEMENTATION
#define VMA_IMPLEMENTATION

#ifndef VMA_STATIC_VULKAN_FUNCTIONS
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#endif

#ifndef VMA_DYNAMIC_VULKAN_FUNCTIONS
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#endif

#include <gfx/vk_mem_alloc.h>

#endif //VMA_IMPLEMENTATION
#endif


namespace gfx
{
namespace vk
{

#if defined(VK_API_VERSION_1_2)
#define USE_VK_API_VER VK_API_VERSION_1_2
#elif defined(VK_API_VERSION_1_1)
#define USE_VK_API_VER VK_API_VERSION_1_1
#elif defined(VK_API_VERSION_1_0)
#define USE_VK_API_VER VK_API_VERSION_1_0
#else
#error "No Vulkan version is available"
#endif


#define MAX_DESC_POOL_SIZE 2048

#define MAX_DESC_SETS 1024

/// ============ TransFuncs ============ ///

extern VkFormat toVkFormat(Format::Enum format);

extern Format::Enum fromVkFormat(VkFormat format);

extern VkFormatFeatureFlags toVkFormatFeatureFlags(FormatFeatureFlags flags);

extern VkSampleCountFlagBits toVkSampleCount(SampleCountFlag::Enum sample);

extern VkImageLayout toVkImageLayout(ImageLayout::Enum layout);

extern VkImageAspectFlagBits toVkImageAspectFlagBits(TextureAspect::Enum aspect);

extern VkImageAspectFlags toVkAspectFlags(TextureAspectFlags aspect);

extern VkFilter toVkFilter(TextureSamplerFilter::Enum filter);

extern VkFilter toVkFilter(BlitFilter::Enum filter);

extern VkSamplerMipmapMode toVkSamplerMipmapMode(TextureSamplerMipmapMode::Enum mode);

extern VkSamplerAddressMode toVkSamplerAddressMode(TextureSamplerAddressMode::Enum mode);

extern VkCompareOp toVkCompareOp(CompareOp::Enum op);

extern VkVertexInputRate toVkVertexInputRate(VertexInputRate::Enum rate);

extern VkDescriptorType toVkDescriptorType(ResourceType::Enum type);

extern VkPipelineBindPoint toVkPipelineBindPoint(ResourceBindPoint::Enum type);

extern VkShaderStageFlagBits toVkShaderStageFlagBits(ShaderType::Enum type);

extern VkShaderStageFlags toVkShaderStageFlags(ShaderTypeFlags flags);

extern VkPolygonMode toVkPolygonMode(PolygonMode::Enum mode);

extern VkCullModeFlags toVkCullModeFlags(CullMode::Enum mode);

extern VkFrontFace toVkFrontFace(FrontFace::Enum frontFace);

extern VkPrimitiveTopology toVkPrimitiveTopology(PrimitiveTopology::Enum topology);

extern VkBlendFactor toVkBlendFactor(BlendFactor::Enum factor);

extern VkBlendOp toVkBlendOp(BlendOp::Enum op);

extern VkColorComponentFlags toVkColorComponentFlags(ColorComponentFlags flags);

extern VkLogicOp toVkLogicOp(LogicOp::Enum op);

extern VkStencilOp toVkStencilOp(StencilOp::Enum op);

extern VkStencilOpState toVkStencilOpState(const StencilOpState &opState,
                                           uint32_t compareMask, uint32_t writeMask, uint32_t reference);

extern VkIndexType toVkIndexType(IndexType::Enum type);

extern VkStencilFaceFlags toVkStencilFaceFlags(StencilFace::Enum face);

extern VkFormat toVkVertexAttribType(VertexAttribType::Enum attrib, bool normalized);

extern VkPipelineStageFlagBits toVkPipelineStageFlagBits(PipelineStage::Enum stage);

extern VkPipelineStageFlags toVkPipelineStageFlags(PipelineStageMask mask);

extern VkAccessFlags toVkAccessFlags(BufferAccessMask mask);

extern VkComponentSwizzle toVkSwizzle(TextureSwizzle::Enum swizzle);

extern VkComponentMapping toVkComponentMapping(TextureSwizzleMapping sm);

extern VkQueryType toVkQueryType(QueryType::Enum type);

extern VkQueryPipelineStatisticFlags toVkQueryPipelineStatisticFlags(QueryPipelineStatisticsFlags flags);

extern VkQueryResultFlags toVkQueryResultFlags(QueryResultFlags flags);


/// ============ Class ============ ///

template<uint16_t MaxNum>
using IDAllocator = gx::GIDAllocator<uint16_t, MaxNum, std::mutex>;

class CommandBufferVk;

class RenderTargetVk;

class RenderPassVk;

class TextureVk;

class DescriptorLayoutVk;

class PipelineLayoutVk;

class PipelineVk;

/**
 * Instance的Vulkan实现
 */
GFX_P_API_IMPL(Instance, Vk)
{
public:
    bool init(const CreateInstanceInfo &createInfo) override;

    void destroy();

public:
    uint32_t deviceCount() override;

    DeviceInfo deviceInfo(uint32_t deviceIndex) override;

    bool isCreated() override;

    Context_P *createContextP() override;

    void destroyContextP(Context_P *obj) override;

public:
    GVkInstance *vkInstance();

private:
    /**
     * 转换Instance扩展描述
     *
     * @param exts
     * @return
     */
    static std::vector<const char *> transInstanceExt(const std::vector<GfxEXT> &exts);

    DeviceInfo tranVkDeviceInfo(uint32_t index);

private:
    GVkInstance mVkInstance;

    IDAllocator<0xfffu> mCtxIDAllocator;
};


/**
 * Context的Vulkan实现
 */
GFX_P_API_IMPL(Context, Vk)
{
public:
    explicit ContextVk(uint16_t idx)
        : mIdx(idx)
    {}

    bool init(Instance_P *instance, Context_T *context, const CreateContextInfo &createInfo) override;

    void destroy();

    uint16_t idx()
    {
        return mIdx;
    }

public:
    Format::Enum getSupportedDepthFormat() override;

    std::vector<Format::Enum> formatSupported(const std::vector<Format::Enum> &formats,
                                              FormatFeatureFlags featureFlags) override;

    size_t uniformBufferOffsetAlignment(size_t dataSize) override;

    size_t storageBufferOffsetAlignment(size_t dataSize) override;

    size_t texelBufferOffsetAlignment(size_t dataSize) override;

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

    Fence_P *createFenceP(bool signaled) override;

    void destroyFenceP(Fence obj) override;

    Fence_P *findFenceP(GfxIdxTy idx) override;

    RenderPassVk *getRenderPass(const GetRenderPassInfo &getInfo);

    RenderPassVk *findRenderPass(GfxIdxTy idx);

    Frame_P *createFrameP(const CreateFrameInfo &createInfo) override;

    void destroyFrameP(Frame obj) override;

    Frame_P *findFrameP(GfxIdxTy idx) override;

    RenderTarget_P *createRenderTargetP(const CreateRenderTargetInfo &createInfo) override;

    RenderTarget_P *createRenderTargetP(const CreateRenderTargetInfo &createInfo, bool isSwapChain);

    void destroyRenderTargetP(RenderTarget obj) override;

    RenderTarget_P *findRenderTargetP(GfxIdxTy idx) override;

    Texture_P *createTextureP(const CreateTextureInfo &createInfo, SampleCountFlag::Enum sample, GVkImage *image);

    Texture_P *createTextureP(const CreateTextureInfo &createInfo, SampleCountFlag::Enum sample) override;

    void destroyTextureP(Texture obj) override;

    Texture_P *findTextureP(GfxIdxTy idx) override;

    Sampler_P *createSamplerP(const CreateSamplerInfo &createInfo) override;

    void destroySamplerP(Sampler obj) override;

    Sampler_P *findSamplerP(GfxIdxTy idx) override;

    Buffer_P *createBufferP(const CreateBufferInfo &createInfo) override;

    void destroyBufferP(Buffer obj) override;

    Buffer_P *findBufferP(GfxIdxTy idx) override;

    Shader_P *createShaderP(const CreateShaderInfo &createInfo) override;

    void destroyShaderP(Shader obj) override;

    Shader_P *findShaderP(GfxIdxTy idx) override;

    DescriptorLayoutVk *getDescriptorLayout(const ResourceLayoutInfo &createInfo);

    DescriptorLayoutVk *findDescriptorLayout(GfxIdxTy idx);

    ResourceBinder_P *createResourceBinderP(const ResourceLayoutInfo &layoutInfo) override;

    void destroyResourceBinderP(ResourceBinder obj) override;

    ResourceBinder_P *findResourceBinderP(GfxIdxTy idx) override;

    PipelineLayoutVk *getPipelineLayout(const PipelineLayoutInfo &info);

    PipelineLayoutVk *findPipelineLayout(GfxIdxTy idx);

    PipelineVk *getGraphicsPipeline(const CreateGraphicsPipelineStateInfo &createInfo);

    PipelineVk *getComputePipeline(const CreateComputePipelineStateInfo &createInfo);

    PipelineVk *findPipeline(GfxIdxTy idx);

    void destroyPipeline(PipelineVk *obj);

    CommandBuffer_P *createCommandBufferP(const CreateCommandBufferInfo &createInfo) override;

    void destroyCommandBufferP(CommandBuffer obj) override;

    CommandBuffer_P *findCommandBufferP(GfxIdxTy idx) override;

    Query_P *createQueryP(const CreateQueryInfo &createInfo) override;

    void destroyQueryP(Query obj) override;

    Query_P *findQueryP(GfxIdxTy idx) override;

public:
    GVkContext *vkContext();

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    VmaAllocator getVmaAllocator();
#endif

    bool samplerAnisotropy();

    float maxSamplerAnisotropy();

    VkDescriptorSet allocVkDescriptorSet(VkDescriptorSetLayout vkLayout,
                                         VkDescriptorPool &pool);

    uint32_t getQueueIndex(QueueType::Enum queueType);

    bool isEnableValidation() const;

    uint64_t isSupportQueryTimestamp() const;

    float getTimestampPeriod() const;

private:
    static VkPhysicalDeviceFeatures getVkDeviceFeatures(uint32_t deviceIndex, InstanceVk *instance);

    static std::vector<const char *> transDeviceExt(const std::vector<DeviceEXT> &exts);

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    void initVma();
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR

    VkDescriptorPool createDescriptorPool();

    std::vector<VkDescriptorPool> &getDescriptorPools();

    RenderPassVk *createRenderPass(const GetRenderPassInfo &createInfo);

    void destroyRenderPass(RenderPassVk *obj);

    DescriptorLayoutVk *createDescriptorLayout(const ResourceLayoutInfo &createInfo);

    void destroyDescriptorLayout(DescriptorLayoutVk *obj);

    PipelineLayoutVk *createPipelineLayout(const PipelineLayoutInfo &info);

    void destroyPipelineLayout(PipelineLayoutVk *obj);

    PipelineVk *createGraphicsPipeline(const CreateGraphicsPipelineStateInfo &createInfo);

    PipelineVk *createComputePipeline(const CreateComputePipelineStateInfo &createInfo);

    /**
     * 判断对应元素Map中是否存在对应idx的元素
     *
     * @param type
     * @param idx
     * @return
     */
    bool containElementMap(GfxIdxTy idx);

    void insertElementMap(ElementHandle *obj);

    void removeElementMap(GfxIdxTy idx, ElementHandle *obj);

    void destroyElement(ElementHandle *obj);

    ElementHandle *findElement(ElementType::Enum type, GfxIdxTy idx);

    void checkLeak();

private:
    uint16_t mIdx;

    Context_T *mParentCtx = GFX_NULL_HANDLE;
    GVkContext mVkContext;

    GMutex mElementMapMutex;
    std::unordered_map<GfxIdxTy, ElementHandle *> mElementMap;

    IDAllocator<GFX_MAX_FENCES> mFenceIDAlloc;
    IDAllocator<GFX_MAX_FRAMES> mFrameIDAlloc;
    IDAllocator<GFX_MAX_RENDER_TARGETS> mRenderTargetIDAlloc;
    IDAllocator<GFX_MAX_BUFFERS> mBufferIDAlloc;
    IDAllocator<GFX_MAX_TEXTURES> mTextureIDAlloc;
    IDAllocator<GFX_MAX_SAMPLERS> mSamplerIDAlloc;
    IDAllocator<GFX_MAX_RENDER_PASSES> mRenderPassIDAlloc;
    IDAllocator<GFX_MAX_SHADERS> mShaderIDAlloc;
    IDAllocator<GFX_MAX_DESC_LAYOUTS> mDescLayoutIDAlloc;
    IDAllocator<GFX_MAX_DESC_SET_BINDERS> mDescSetBinderIDAlloc;
    IDAllocator<GFX_MAX_PIPELINE_LAYOUTS> mPipelineLayoutIDAlloc;
    IDAllocator<GFX_MAX_PIPELINE_STATES> mPipelineStateIDAlloc;
    IDAllocator<GFX_MAX_COMMAND_BUFFERS> mCommandBufferIDAlloc;
    IDAllocator<GFX_MAX_QUERY> mQueryIDAlloc;

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    VmaAllocator mVmaAllocator = nullptr;
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR

    std::vector<VkDescriptorPool> mVkDescriptorPools;

    GMutex mRwRPMapMutex;
    GMutex mRwGPMapMutex;
    GMutex mRwCPMapMutex;
    GMutex mRwDLMapMutex;
    GMutex mRwPLMapMutex;

    std::unordered_map<GetRenderPassInfo, GfxIdxTy> mRenderPassMap;
    std::unordered_map<QueryGraphicsPipelineStateInfo, GfxIdxTy> mGraphPipelineMap;
    std::unordered_map<QueryComputePipelineStateInfo, GfxIdxTy> mCompPipelineMap;
    std::unordered_map<ResourceLayoutInfo, GfxIdxTy> mDescriptorLayoutMap;
    std::unordered_map<PipelineLayoutInfo, GfxIdxTy> mPipelineLayoutMap;

    bool mEnableValidation = false;
    bool mSupportQueryTimestamp = false;
    float mTimestampPeriod = 1;
};


GFX_P_API_IMPL(Fence, Vk)
{
public:
    explicit FenceVk(GfxIdxTy idx)
        : Fence_P(idx)
    {}

    bool init(Context_T *context, bool signaled);

    void destroy() override;

    Context_T *context() override;

    FenceWaitRet::Enum wait(uint64_t timeout) override;

    void reset() override;

    bool isSignaled() override;

public:
    GVkFence *vkFence();

private:
    Context_T *mContextT = GFX_NULL_HANDLE;

    GVkFence mVkFence;
};


class RenderPassVk : public ElementHandle
{
public:
    explicit RenderPassVk(GfxIdxTy idx)
            : ElementHandle(idx)
    {}

    bool init(Context_T *context, const GetRenderPassInfo &createInfo);

    void destroy() override;

public:
    Context_T *context() override;

    size_t hash() override;

public:
    GVkRenderPass *vkRenderPass();

    uint32_t subPassCount() const;

    uint32_t colorAttachmentCount() const;

    SampleCountFlag::Enum sampleCountFlag() const;

private:
    bool initVkRenderPass(const GetRenderPassInfo &createInfo);

private:
    Context_T *mContextT = GFX_NULL_HANDLE;
    GVkRenderPass mVkRenderPass;
    uint32_t mSubPassCount = 0;
    uint32_t mColorAttachmentCount = 0;
    uint8_t mSampleCountFlag = 0;
    size_t mHash = 0;
};


GFX_P_API_IMPL(Frame, Vk)
{
public:
    explicit FrameVk(GfxIdxTy idx)
            : Frame_P(idx)
    {}

    bool init(Context_T *context, const CreateFrameInfo &createInfo);

    void destroy() override;

public:
    bool reset(uint32_t width, uint32_t height, bool vSync, bool enforce) override;

    bool beginFrame() override;

    void submit(CommandBuffer commandBuffer) override;

    void endFrame(bool waitQueue) override;

    void waitGraphicsQueueIdle() override;

    void setSwapChainErrorCallback(const FrameSwapChainErrorCallback &callback) override;

    uint32_t frameBufferCount() override;

    uint32_t currentFrameIndex() override;

    RenderTarget renderTarget() override;

    uint32_t width() override;

    uint32_t height() override;

    Format::Enum getSwapChainDepthFormat() override;

    Format::Enum getSwapChainColorFormat() override;

    uint64_t getFrameTime() override;

    Context_T *context() override;

private:
    bool initSwapChain(const SwapChainInfo *swapChainInfo);

    bool initRenderTarget(bool isSwapChain);

    bool initFence(uint32_t bufferCount);

    void updateFrameState();

private:
    Context_T *mContextT = GFX_NULL_HANDLE;

    VkSurfaceKHR mVkSurface = VK_NULL_HANDLE;
    GVkBaseSwapChain *mVkSwapChain = GFX_NULL_HANDLE;
    std::vector<GVkFence> mFences;
    GVkSemaphore mRenderSemaphore;

    std::vector<TextureVk *> mColorTextures;
    TextureVk *mDepthTexture = nullptr;

    Format::Enum mColorFormat = Format::Undefined;
    Format::Enum mDepthFormat = Format::Undefined;

    RenderTargetVk *mRenderTarget = GFX_NULL_HANDLE;

    uint32_t mCurrentFrameIndex = 0;

    FrameTargetType::Enum mRenderTargetType = FrameTargetType::SwapChain;
    bool mVSync = false;
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;

    struct
    {
        GTime time;
        uint64_t frameTime = 0;
    } mFrameState;

    FrameSwapChainErrorCallback mSwapChainErrorCb = nullptr;

    friend class RenderTargetVk;

    friend class CommandBufferVk;
};


GFX_P_API_IMPL(RenderTarget, Vk)
{
public:
    explicit RenderTargetVk(GfxIdxTy idx)
            : RenderTarget_P(idx)
    {}

    bool init(Context_T *context, const CreateRenderTargetInfo &createInfo, bool isSwapChain);

    void destroy() override;

public:
    uint32_t width() override;

    uint32_t height() override;

    uint8_t frameBufferCount() override;

    uint8_t colorAttachmentCount() override;

    bool hasDepthStencil() override;

    Attachment getColorAttachment(uint8_t frameIndex, uint8_t attachIndex) override;

    Attachment getDepthStencilAttachment() override;

public:
    Context_T *context() override;

    GVkFrameBuffer *getVkFrameBuffer(RenderPassVk *rp, uint32_t index);

    RenderPassVk *getRenderPass(const RenderPassInfo &rpInfo) const;

    uint16_t colorAttachmentCount() const;

    bool hasDepthStencilAttachment() const;

    GVkImage *getAttachmentImage(uint32_t frameIndex, uint32_t attachIndex) const;

private:
    bool initMSAARenderTarget(const CreateRenderTargetInfo &createInfo);

    void initRenderPassBase(const CreateRenderTargetInfo &createInfo, bool isSwapChain);

    std::vector<GVkFrameBuffer *> createFrameBuffer(RenderPassVk *rp);

    static uint32_t valueForLevel(uint8_t level, uint32_t baseLevelValue);

private:
    Context_T *mContextT = GFX_NULL_HANDLE;

    uint32_t mWidth = 0;
    uint32_t mHeight = 0;

    uint8_t mFrameCount = 1;
    uint8_t mColorCount = 0;
    bool mHasDepth = false;

    std::vector<std::vector<GVkImage *>> mAttachColorImages;
    GVkImage *mAttachDepthImage = nullptr;

    std::vector<std::vector<Attachment>> mAttachColors;
    Attachment mAttachDepth{};

    std::vector<std::vector<VkImageView>> mFBViews;
    std::unordered_map<GfxIdxTy, std::vector<GVkFrameBuffer *>> mFBCache;
    GetRenderPassInfo mGetRenderPassBase{};

    Texture mMsaaColorTexture = GFX_NULL_HANDLE;
    Texture mMsaaDepthTexture = GFX_NULL_HANDLE;
};


GFX_P_API_IMPL(Buffer, Vk)
{
public:
    explicit BufferVk(GfxIdxTy idx)
            : Buffer_P(idx)
    {}

    bool init(Context_T *context, const CreateBufferInfo &createInfo);

    void destroy() override;

    Context_T *context() override;

public:
    uint64_t size() override;

    void *map() override;

    void flush() override;

    void unmap() override;

    VkBuffer vkBuffer();

    VkDescriptorBufferInfo getVkDescriptorBufferInfo(uint64_t offset = 0, uint64_t range = VK_WHOLE_SIZE);

    VkBufferView getVkBufferView(Format::Enum format, uint64_t offset, uint64_t size);

private:
    ContextVk *getContextVk();

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    bool initByVMA(const CreateBufferInfo &createInfo);
#else
    bool initByGVkBuffer(const CreateBufferInfo &createInfo);
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR

private:
    Context_T *mContextT = GFX_NULL_HANDLE;

    BufferTypeFlags mType = 0;
    BufferMemoryUsage::Enum mMemoryUsage = BufferMemoryUsage::GpuOnly;
    uint64_t mSize = 0;

#ifdef USE_AMD_VULKAN_MEMORY_ALLOCATOR
    VkBuffer mBuffer = VK_NULL_HANDLE;
    VmaAllocation mAllocation = nullptr;
    VmaAllocationInfo mAllocInfo{};
    void *mMapped = nullptr;
#else
    GVkBuffer mGVkBuffer;
#endif //USE_AMD_VULKAN_MEMORY_ALLOCATOR

    std::unordered_map<CreateBufferViewInfo, VkBufferView> mVkBufferViews;
};


GFX_P_API_IMPL(Texture, Vk)
{
public:
    explicit TextureVk(GfxIdxTy idx)
            : Texture_P(idx)
    {}

    bool init(Context_T *context, const CreateTextureInfo &createInfo,
              SampleCountFlag::Enum sample, GVkImage *image);

    void destroy() override;

    Context_T *context() override;

    size_t hash() override;

public:
    TextureType::Enum type() override;

    Format::Enum format() override;

    TextureUsageFlags usage() override;

    TextureAspectFlags aspect() override;

    uint32_t width() override;

    uint32_t height() override;

    uint32_t depth() override;

    uint32_t layerCount() override;

    uint32_t mipLevels() override;

    uint64_t size() override;

    void setData(const void *data, uint64_t size, Fence fence) override;

    void genMipmap(Fence fence) override;

public:
    GVkContext *vkContext();

    GVkImage *vkImage();

    VkImageView createImageView(const TextureBindRange &range, bool isAttachment);

    VkDescriptorImageInfo getDescriptor(VkImageView imageView, VkSampler sampler) const;

    void imageMemoryBarrier(VkCommandBuffer cmdBuffer,
                            ImageLayout::Enum srcLayout,
                            ImageLayout::Enum dstLayout,
                            const ImageSubResourceRange &subResRange);

    static ImageLayout::Enum getUsageImageLayout(TextureUsageFlags usage, TextureAspectFlags aspect);

private:
    bool initVkTexture(const CreateTextureInfo &createInfo,
                       SampleCountFlag::Enum sample,
                       GVkImage *image);

    bool createImage(uint32_t width, uint32_t height, uint32_t depth,
                     uint32_t mipLevels,
                     uint32_t arrayLayers,
                     VkImageType imageType,
                     VkFormat format,
                     VkSampleCountFlagBits sampleCount,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkImageAspectFlags aspect,
                     VkSharingMode sharingMode,
                     VkImageLayout layout,
                     bool isCube);

private:
    friend class RenderTargetVk;

    Context_T *mContextT = GFX_NULL_HANDLE;
    GVkImage *mVkImage = nullptr;

    bool mIsFromImage = false;

    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    uint32_t mDepth = 0;
    TextureType::Enum mType = TextureType::Texture1D;
    Format::Enum mFormat = Format::R8G8B8A8_UNorm;
    TextureUsageFlags mUsage = 0;
    TextureAspectFlags mAspect = 0;
    uint32_t mMipLevels = 0;
    uint32_t mLayerCount = 0;
    TextureSwizzleMapping mSwizzleMapping{};

    std::unordered_map<CreateImageViewInfo, VkImageView> mImageViewCache;

    size_t mHash = 0;
};


GFX_P_API_IMPL(Sampler, Vk)
{
public:
    explicit SamplerVk(GfxIdxTy idx)
            : Sampler_P(idx)
    {}

    bool init(Context_T *context, const CreateSamplerInfo &createInfo);

    void destroy() override;

    Context_T *context() override;

public:
    GVkSampler *vkSampler();

private:
    Context_T *mContextT = GFX_NULL_HANDLE;

    GVkSampler mVkSampler;
};


GFX_P_API_IMPL(Shader, Vk)
{
public:
    explicit ShaderVk(GfxIdxTy idx)
            : Shader_P(idx)
    {}

    bool init(Context_T *context, const CreateShaderInfo &createInfo);

    void destroy() override;

    Context_T *context() override;

    size_t hash() override;

public:
    VkShaderModule vkShaderModule() const;

    VkShaderStageFlagBits vkShaderStage() const;

    void refPipeline(GfxIdxTy idx);

private:
    bool createVkShaderModule(const CreateShaderInfo &createInfo);

    void releaseRefPipeline();

private:
    Context_T *mContextT = GFX_NULL_HANDLE;
    VkShaderModule mVkShaderModule = VK_NULL_HANDLE;
    VkShaderStageFlagBits mVkShaderStage = VK_SHADER_STAGE_VERTEX_BIT;
    size_t mHash = 0;
    std::string mTag;

    std::unordered_set<GfxIdxTy> mPipelineRef;
};


class DescriptorLayoutVk : public ElementHandle
{
public:
    explicit DescriptorLayoutVk(GfxIdxTy idx)
            : ElementHandle(idx)
    {}

    bool init(Context_T *context, const ResourceLayoutInfo &createInfo);

    void destroy() override;

    Context_T *context() override;

    size_t hash() override;

    bool equals(ElementHandle *b) override;

public:
    VkDescriptorSetLayout vkDescriptorSetLayout() const;

private:
    Context_T *mContextT = GFX_NULL_HANDLE;

    VkDescriptorSetLayout mVkDescSetLayout = VK_NULL_HANDLE;
    std::unordered_map<uint32_t, ResourceLayoutBindingInfo> mBindingInfo;

    size_t mHash = 0;

    friend class ContextVk;

    friend class ResourceBinderVk;
};


GFX_P_API_IMPL(ResourceBinder, Vk)
{
public:
    explicit ResourceBinderVk(GfxIdxTy idx)
            : ResourceBinder_P(idx)
    {}

    bool init(Context_T *context, const ResourceLayoutInfo &layoutInfo);

    void destroy() override;

    Context_T *context() override;

    void bindBuffer(uint32_t binding, Buffer buffer) override;

    void bindBufferRange(uint32_t binding, Buffer buffer, uint64_t offset, uint64_t range) override;

    void bindTexelBuffer(uint32_t binding, Buffer buffer,
                         Format::Enum format, uint64_t offset, uint64_t range) override;

    void bindTexture(uint32_t binding, Texture texture, Sampler sampler, const TextureBindRange &range) override;

    void bindInputAttachment(uint32_t binding, Texture texture, const TextureBindRange &range) override;

public:
    GVkContext *vkContext();

    ResourceLayoutInfo getDescriptorLayoutInfo() const;

    VkDescriptorSet getVkDescriptorSet() const;

    void bindResources();

private:
    bool initBindInfo();

private:
    Context_T *mContextT = GFX_NULL_HANDLE;

    ResourceLayoutInfo mDescLayoutInfo;
    VkDescriptorSet mDescSet = VK_NULL_HANDLE;
    DescriptorLayoutVk *mDescLayout = nullptr;
    VkDescriptorPool mPool = VK_NULL_HANDLE;

    struct BindBufferInfo
    {
        ResourceType::Enum type;
        Buffer buffer;
        uint64_t offset;
        uint64_t range;
        bool updated;
        VkDescriptorBufferInfo writeInfo;
    };

    struct BindSamplerInfo
    {
        ResourceType::Enum type;
        Texture texture;
        Sampler sampler;
        TextureBindRange range;
        bool updated;
        VkDescriptorImageInfo writeInfo;
    };

    struct BindTexelBufferInfo
    {
        ResourceType::Enum type;
        Buffer buffer;
        uint64_t offset;
        uint64_t range;
        Format::Enum format;
        bool updated;
        VkBufferView bufferView;
    };

    using BindDescInfo = std::variant<BindBufferInfo, BindSamplerInfo, BindTexelBufferInfo>;

    bool mAllUpdated = true;                       // 是否所有资源都是更新状态
    std::vector<BindDescInfo> mBindDescInfo;       // 资源绑定关联表，一个数组，表示关系为：[binding]
};


class PipelineLayoutVk : public ElementHandle
{
public:
    explicit PipelineLayoutVk(GfxIdxTy idx)
            : ElementHandle(idx)
    {}

    bool init(Context_T *context, const PipelineLayoutInfo &info);

    void destroy() override;

    Context_T *context() override;

public:
    VkPipelineLayout getVkPipelineLayout() const;

private:
    Context_T *mContextT = GFX_NULL_HANDLE;

    VkPipelineLayout mVkPipelineLayout = VK_NULL_HANDLE;
};


class PipelineVk : public ElementHandle
{
public:
    explicit PipelineVk(GfxIdxTy idx)
            : ElementHandle(idx)
    {}

    bool init(Context_T *context, const CreateGraphicsPipelineStateInfo &createInfo);

    bool init(Context_T *context, const CreateComputePipelineStateInfo &createInfo);

    void destroy() override;

    Context_T *context() override;

    size_t hash() override;

public:
    VkPipeline vkPipeline() const;

private:
    bool createGraphPipeline(const CreateGraphicsPipelineStateInfo &createInfo);

    bool createComputePipeline(const CreateComputePipelineStateInfo &createInfo);

private:
    Context_T *mContextT = GFX_NULL_HANDLE;

    size_t mHash = 0;
    VkPipeline mVkPipeline = VK_NULL_HANDLE;
};


GFX_P_API_IMPL(CommandBuffer, Vk)
{
public:
    explicit CommandBufferVk(GfxIdxTy idx)
            : CommandBuffer_P(idx)
    {}

    bool init(Context_T *context, const CreateCommandBufferInfo &createInfo);

    void destroy() override;

    Context_T *context() override;

public:
    VkCommandBuffer getVkCommandBuffer(uint32_t index);

    void compile(FrameVk *frame = GFX_NULL_HANDLE);

    bool isCompiled() const;

    std::string dump();

public:
    CommandBuffer begin() override;

    void end() override;

    CommandBuffer setClearColor(const ClearColor &clearColor) override;

    CommandBuffer setClearDepthStencil(float depth, uint32_t stencil) override;

    CommandBuffer bindRenderTarget(RenderTarget renderTarget) override;

    CommandBuffer bindRenderTarget(Frame frame) override;

    CommandBuffer beginRenderPass(const Rect2D &renderArea, const RenderPassInfo &rpInfo) override;

    CommandBuffer endRenderPass() override;

    CommandBuffer setGraphicsPipelineState(const GraphicsPipelineStateInfo &pipelineState) override;

    CommandBuffer setVertexLayout(const VertexLayout &vertexLayout) override;

    CommandBuffer setShaders(const std::vector<Shader> &shaders) override;

    CommandBuffer nextSubpass() override;

    CommandBuffer setViewport(const Rect2D &viewport, float minDepth, float maxDepth) override;

    CommandBuffer setScissor(const Rect2D &scissor) override;

    CommandBuffer setLineWidth(float lineWidth) override;

    CommandBuffer setStencilCompareMask(StencilFace::Enum face, uint32_t mask) override;

    CommandBuffer setStencilWriteMask(StencilFace::Enum face, uint32_t mask) override;

    CommandBuffer setStencilReference(StencilFace::Enum face, uint32_t reference) override;

    CommandBuffer bindResources(ResourceBindPoint::Enum bindPoint,
                                const std::vector<ResourceBinder> &binders,
                                const std::vector<uint32_t> &dynamicOffsets) override;

    CommandBuffer bindVertexBuffer(uint32_t firstBinding,
                                   const std::vector<Buffer> &buffers,
                                   const std::vector<uint64_t> &offsets) override;

    CommandBuffer bindIndexBuffer(Buffer buffer, uint32_t offset, IndexType::Enum indexType) override;

    CommandBuffer draw(uint32_t vertexCount, uint32_t instanceCount,
                       uint32_t firstVertex, uint32_t firstInstance) override;

    CommandBuffer drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
                              uint32_t firstInstance) override;

    CommandBuffer drawIndirect(Buffer buffer, uint32_t offset, uint32_t drawCount, uint32_t stride) override;

    CommandBuffer drawIndexedIndirect(Buffer buffer, uint32_t offset, uint32_t drawCount, uint32_t stride) override;

    CommandBuffer dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override;

    CommandBuffer dispatchIndirect(Buffer buffer, uint32_t offset) override;

    CommandBuffer executeCommands(const std::vector<CommandBuffer> &secondaryCmdBuffers) override;

    CommandBuffer pipelineBarrier(PipelineStageMask srcStage, PipelineStageMask dstStage) override;

    CommandBuffer bufferMemoryBarrier(Buffer buffer, const BufferBarrierInfo &barrierInfo) override;

    CommandBuffer imageMemoryBarrier(Texture texture,
                                     ImageLayout::Enum srcLayout,
                                     ImageLayout::Enum dstLayout,
                                     const ImageSubResourceRange &range) override;

    CommandBuffer copyBuffer(Buffer src, Buffer dst, uint64_t srcOffset, uint64_t dstOffset, uint64_t size) override;

    CommandBuffer copyImage(Texture src, Texture dst, const std::vector<ImageCopyInfo> &copyInfos) override;

    CommandBuffer copyBufferToImage(Buffer src, Texture dst,
                                    const std::vector<BufferImageCopyInfo> &copyInfos) override;

    CommandBuffer copyImageToBuffer(Texture src, Buffer dst,
                                    const std::vector<BufferImageCopyInfo> &copyInfos) override;

    CommandBuffer blitImage(Texture src, Texture dst, const std::vector<ImageBlitInfo> &blitInfos,
                            BlitFilter::Enum filter) override;

    CommandBuffer copyRenderTarget(RenderTarget src, RenderTarget dst,
                                   const std::vector<ImageCopyInfo> &copyInfos,
                                   uint8_t frameIndex) override;

    CommandBuffer blitRenderTarget(RenderTarget src, RenderTarget dst,
                                   const std::vector<ImageBlitInfo> &blitInfos,
                                   BlitFilter::Enum filter,
                                   uint8_t frameIndex) override;

    CommandBuffer blitRenderTargetToImage(RenderTarget src, Texture dst,
                                          const std::vector<ImageBlitInfo> &blitInfos,
                                          BlitFilter::Enum filter,
                                          uint8_t attachIndex,
                                          uint8_t frameIndex) override;

    CommandBuffer copyRenderTargetToImage(RenderTarget src, Texture dst,
                                          const std::vector<ImageCopyInfo> &copyInfos,
                                          uint8_t attachIndex,
                                          uint8_t frameIndex) override;

    CommandBuffer copyRenderTargetToBuffer(RenderTarget src, Buffer dst,
                                           const std::vector<BufferImageCopyInfo> &copyInfos,
                                           uint8_t attachIndex,
                                           uint8_t frameIndex) override;

    CommandBuffer fillBuffer(Buffer buffer, uint64_t offset, uint64_t size, uint32_t data) override;

    CommandBuffer beginDebugLabel(const std::string &label) override;

    CommandBuffer endDebugLabel() override;

    CommandBuffer resetQuery(Query query, uint32_t firstQuery, uint32_t queryCount) override;

    CommandBuffer beginQuery(Query query, uint32_t queryIndex, bool precise) override;

    CommandBuffer endQuery(Query query, uint32_t queryIndex) override;

    CommandBuffer writeTimestamp(PipelineStage::Enum pipelineStage, Query query, uint32_t queryIndex) override;

    CommandBuffer copyQueryResults(Query query,
                                   uint32_t firstQuery, uint32_t queryCount,
                                   Buffer dstBuffer, uint64_t dstOffset,
                                   QueryResultFlags resultFlags) override;

private:
    void resetCommandBuffer();

    /**
     * 编译Gfx指令到Vulkan指令
     */
    void compileCommand(FrameVk *frame);

    static void doCopyImage(VkCommandBuffer cmdBuffer, GVkImage *src, GVkImage *dst,
                            const std::vector<ImageCopyInfo> &copyInfos);

    static void doBlitImage(VkCommandBuffer cmdBuffer, GVkImage *src, GVkImage *dst,
                            const std::vector<ImageBlitInfo> &blitInfos, BlitFilter::Enum filter);

    static PipelineVk *bindGraphPipeline(ContextVk *context, VkCommandBuffer cmdBuffer,
                                         const CreateGraphicsPipelineStateInfo &createInfo,
                                         PipelineVk *pipeline);

    static PipelineVk *bindComputePipeline(ContextVk *context, VkCommandBuffer cmdBuffer,
                                           const CreateComputePipelineStateInfo &createInfo,
                                           PipelineVk *pipeline);

private:
    friend class ContextVk;

    Context_T *mContextT = GFX_NULL_HANDLE;

    GVkCommandPool *mVkCommandPool = nullptr;

    std::vector<VkCommandBuffer> mVkCommandBuffers;

    GByteArray mCommandBuffer;

    bool mIsCompiled = false;
    bool mIsBegun = false;
};


GFX_P_API_IMPL(Query, Vk)
{
public:
    explicit QueryVk(GfxIdxTy idx)
            : Query_P(idx)
    {}

    bool init(Context_T *context, const CreateQueryInfo &createInfo);

    void destroy() override;

    Context_T *context() override;

    size_t hash() override;

    void getQueryResults(std::vector<uint64_t> &results, QueryResultFlags flags) override;

public:
    VkQueryPool getVkQueryPool() const;

private:
    Context_T *mContextT = GFX_NULL_HANDLE;

    size_t mHash = 0;

    VkQueryPool mVkQuery = VK_NULL_HANDLE;

    QueryType::Enum mQueryType = QueryType::Occlusion;
    uint32_t mQueryCount = 0;
};

}
}

#endif //GX_GFX_P_VK_H
