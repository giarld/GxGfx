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

#ifndef GX_GFX_CORE_H
#define GX_GFX_CORE_H

#include <gfx/gfx_base.h>
#include <gfx/gfx_def.h>

#include <gx/gstring.h>
#include <vector>
#include <variant>
#include <functional>


namespace gfx
{

/**
 * 实例
 */
GFX_API_DEFINE(Instance);

/**
 * 上下文
 */
GFX_API_DEFINE(Context);

/**
 * CPU-GPU同步用的栅栏
 */
GFX_API_DEFINE(Fence);

/**
 * 目标帧控制器
 * 管理帧输出目标，帧指令缓冲，提交绘制指令，完成帧输出同步
 */
GFX_API_DEFINE(Frame);

/**
 * 渲染目标
 * 可以理解为帧缓冲区集合
 */
GFX_API_DEFINE(RenderTarget);

/**
 * 缓冲区对象
 */
GFX_API_DEFINE(Buffer);

/**
 * 纹理
 */
GFX_API_DEFINE(Texture);

/**
 * 纹理采样器
 */
GFX_API_DEFINE(Sampler);

/**
 * 着色器
 */
GFX_API_DEFINE(Shader);

/**
 * 资源绑定器
 */
GFX_API_DEFINE(ResourceBinder);

/**
 * 指令缓冲区
 * 进行管线状态录制和绘制、计算、转移指令的录制
 */
GFX_API_DEFINE(CommandBuffer);

/**
 * 查询
 */
GFX_API_DEFINE(Query);


/**
 * Gfx 实例
 */
GFX_API(Instance)
{
    GFX_API_FUNC(uint32_t deviceCount());

    GFX_API_FUNC(DeviceInfo deviceInfo(uint32_t deviceIndex));

    GFX_API_FUNC(TargetApiType::Enum targetApiType());
};

struct CreateInstanceInfo
{
    const char *appName;
    TargetApiType::Enum targetApiType;
    std::vector<GfxEXT> exts;
    bool enableValidation;
};

GX_API Instance createInstance(const CreateInstanceInfo &createInfo);

GX_API void destroyInstance(Instance);


/**
 * Gfx 上下文
 */
GFX_API(Context)
{
    /**
     * 获取设备编号
     * @return
     */
    GFX_API_FUNC(uint32_t deviceIndex());

    /**
     * 获取设备信息
     *
     * @return
     */
    GFX_API_FUNC(DeviceInfo deviceInfo());

    /**
     * 获取支持的最佳深度裁剪缓冲区格式
     *
     * @return
     */
    GFX_API_FUNC(Format::Enum getSupportedDepthFormat());

    /**
     * 判断并返回硬件支持的图像格式
     *
     * @param formats
     * @param featureFlags
     * @return
     */
    GFX_API_FUNC(std::vector<Format::Enum> formatSupported(const std::vector<Format::Enum> &formats,
                                                           FormatFeatureFlags featureFlags));

    /**
     * 计算UniformBuffer对齐后的dataSize
     * 不同的GPU这个值可能会不同
     *
     * @param dataSize
     * @return
     */
    GFX_API_FUNC(size_t uniformBufferOffsetAlignment(size_t dataSize));

    /**
     * 计算StorageBuffer对齐后的dataSize
     * 不同的GPU这个可能值会不同
     *
     * @param dataSize
     * @return
     */
    GFX_API_FUNC(size_t storageBufferOffsetAlignment(size_t dataSize));

    /**
     * 计算TexelBuffer对齐后的dataSize
     * 不同的GPU这个可能值会不同
     *
     * @param dataSize
     * @return
     */
    GFX_API_FUNC(size_t texelBufferOffsetAlignment(size_t dataSize));

    /**
     * 获取Format的单位大小 (byte)
     *
     * @param format
     * @return
     */
    GFX_API_FUNC(size_t formatSize(Format::Enum format));

    /**
     * 获取VertexAttribType所占字节大小
     *
     * @param type
     * @return
     */
    GFX_API_FUNC(size_t vertexAttribTypeSize(VertexAttribType::Enum type));

    /**
     * 获取RenderTarget硬件多重采样能支持的最大采样
     * 即MSAA最大采样精度
     *
     * @return
     */
    GFX_API_FUNC(SampleCountFlag::Enum maxRenderTargetSampleCount());

    /**
     * 获取最大1D纹理尺寸
     *
     * @return
     */
    GFX_API_FUNC(uint32_t maxTextureDimension1D());

    /**
     * 获取最大2D纹理尺寸
     *
     * @return
     */
    GFX_API_FUNC(uint32_t maxTextureDimension2D());

    /**
     * 获取最大3D纹理尺寸
     *
     * @return
     */
    GFX_API_FUNC(uint32_t maxTextureDimension3D());

    /**
     * 获取最大Cube纹理尺寸
     *
     * @return
     */
    GFX_API_FUNC(uint32_t maxTextureDimensionCube());

    /**
     * 获取最大纹理数组最大层数
     *
     * @return
     */
    GFX_API_FUNC(uint32_t maxTextureArrayLayers());

    /**
     * 获取是否支持 CubeArray
     *
     * @return
     */
    GFX_API_FUNC(bool isSupportTextureCubeArray());

    /**
     * 获取着色器支持的最大每阶段采样器数量
     *
     * @return
     */
    GFX_API_FUNC(uint32_t maxPerStageShaderSamplersCount());

    /**
     * 获取着色器支持的最大每阶段 Uniform buffer 数量
     *
     * @return
     */
    GFX_API_FUNC(uint32_t maxPerStageShaderUniformBuffersCount());

    /**
     * 获取着色器支持的最大每阶段 Storage buffer 数量
     *
     * @return
     */
    GFX_API_FUNC(uint32_t maxPerStageShaderStorageBuffersCount());

    /**
     * 获取着色器支持的最大每阶段 Sampled image 数量
     *
     * @return
     */
    GFX_API_FUNC(uint32_t maxPerStageShaderSampledImagesCount());

    /**
     * 获取着色器支持的最大每阶段 Storage image 数量
     *
     * @return
     */
    GFX_API_FUNC(uint32_t maxPerStageShaderStorageImagesCount());

    /**
     * 等待设备结束
     */
    GFX_API_FUNC(void waitIdle());

    /**
     * 提交指令缓冲，并阻塞等待指令执行完成
     *
     * @param cmdBuffer     要提交的指令缓冲区
     * @param bufferIndex   要被提交的缓冲区编号
     */
    GFX_API_FUNC(void submitCommandBlock(CommandBuffer cmdBuffer, uint32_t bufferIndex = 0));

    /**
     * 提交指令缓冲
     * 可以依托提供的Fence实现CPU和GPU指令执行之间的同步
     *
     * @param cmdBuffer     要提交的指令缓冲区
     * @param bufferIndex   要被提交的缓冲区编号
     * @param fence         同步栅栏
     */
    GFX_API_FUNC(void submitCommand(CommandBuffer cmdBuffer, uint32_t bufferIndex = 0, Fence fence = GFX_NULL_HANDLE));

    /**
     * 等待对应类型的Command queue执行完成
     *
     * @param queueType
     */
    GFX_API_FUNC(void queueWaitIdle(QueueType::Enum queueType));

    /**
     * 将CommandBuffer的指令流输出为调试字符串
     *
     * @return
     */
    GFX_API_FUNC(std::string dumpCommandBuffer(CommandBuffer commandBuffer));
};

struct CreateContextInfo
{
    uint32_t deviceIndex;
    std::vector<DeviceEXT> exts;
};

GX_API Context createContext(Instance instance, const CreateContextInfo &createInfo);

GX_API void destroyContext(Context);


GFX_API(Fence)
{
    /**
     * 发起等待
     *
     * @param timeout 单位(纳秒)，默认
     * @return
     */
    GFX_API_FUNC(FenceWaitRet::Enum wait(uint64_t timeout = UINT64_MAX));

    GFX_API_FUNC(void reset());

    GFX_API_FUNC(bool isSignaled());
};

GX_API Fence createFence(Context context, bool signaled = false);

GX_API void destroyFence(Fence fence);


/**
 * 帧交换链发生错误的回调
 */
using FrameSwapChainErrorCallback = std::function<void()>;

/**
 * Gfx 目标帧控制器
 * 管理帧输出目标，帧指令缓冲，提交绘制指令，完成帧输出同步
 */
GFX_API(Frame)
{
    /**
     * 重设帧管理器
     * 在屏幕大小改变、垂直同步改变处调用
     *
     * @param width
     * @param height
     * @param vSync
     * @param enforce   是否强制重设，如果是在前三个参数不变的情况下也会触发重设Frame
     * @return 返回Frame是否有变更，如果有变更需要执行重新录制指令缓冲等操作
     */
    GFX_API_FUNC(bool reset(uint32_t width, uint32_t height, bool vSync, bool enforce = false));

    /**
     * 开始当前帧
     *
     * @return 返回开始新帧是否成功，仅当成功时可以去继续执行submit()和endFrame()函数
     */
    GFX_API_FUNC(bool beginFrame());

    /**
     * 提交当前帧
     * 在prepare()之后调用
     */
    GFX_API_FUNC(void submit(CommandBuffer commandBuffer));

    /**
     * 结束当前帧
     *
     * @param waitQueue 等待队列处理完成，可用于控制(Frames in flight)
     */
    GFX_API_FUNC(void endFrame(bool waitQueue = true));

    /**
     * 主动等待图形命令队列处理结束
     * 当endFrame()的可选参数 waitQueue 为false时，需要在下一帧beginFrame()前
     * 的某个时机调用本函数
     */
    GFX_API_FUNC(void waitGraphicsQueueIdle());

    /**
     * 设置帧交换链发生错误的回调
     *
     * @param callback
     */
    GFX_API_FUNC(void setSwapChainErrorCallback(const FrameSwapChainErrorCallback &callback));

    /**
     * 获取帧缓冲区数量
     *
     * @return 返回帧缓冲区数量
     */
    GFX_API_FUNC(uint32_t frameBufferCount());

    /**
     * 获取当前的帧缓冲区编号
     *
     * @return 返回当前的帧缓冲区编号
     */
    GFX_API_FUNC(uint32_t currentFrameIndex());

    /**
     * 获取帧控制器的渲染目标
     * 返回的RenderPass不能进行destroy操作，否则将出错
     *
     * @return
     */
    GFX_API_FUNC(RenderTarget renderTarget());

    /**
     * 获取帧目标宽度
     *
     * @return
     */
    GFX_API_FUNC(uint32_t width());

    /**
     * 获取帧目标高度
     *
     * @return
     */
    GFX_API_FUNC(uint32_t height());

    /**
     * 获取当前交换链匹配的颜色格式
     *
     * @return
     */
    GFX_API_FUNC(Format::Enum getSwapChainColorFormat());

    /**
     * 获取当前交换链匹配的深度缓冲区格式
     *
     * @return
     */
    GFX_API_FUNC(Format::Enum getSwapChainDepthFormat());

    /**
     * 获取最近一帧的帧时间
     * 单位: 微秒(us)
     *
     * @return
     */
    GFX_API_FUNC(uint64_t getFrameTime());
};

/**
 * 创建Frame的结构
 */
struct CreateFrameInfo
{
    /// 渲染目标类型
    FrameTargetType::Enum targetType;

    /**
     * 目标对象指针，如果targetType为SwapChain，传递PlatformData的指针，
     * 如果targetType为RenderTarget，传递RenderTarget对象指针,
     * 此时的RenderTarget必须是至少一个有效颜色附件+一个有效深度/模板附件的组合
     */
    void *pTarget;

    /// 帧缓冲区宽度
    uint32_t frameWidth;

    /// 帧缓冲区高度
    uint32_t frameHeight;

    /// 是否启用垂直同步
    bool vSync;
};


GX_API Frame createFrame(Context context, const CreateFrameInfo &createInfo);

GX_API void destroyFrame(Frame);


struct Attachment
{
    Texture texture;            // 目标纹理
    uint32_t mipLevel = 0;      // 目标mipLevel
    uint32_t layer = 0;         // 目标layer
};

/**
 * 渲染目标
 */
GFX_API(RenderTarget)
{
    GFX_API_FUNC(uint32_t width());

    GFX_API_FUNC(uint32_t height());

    GFX_API_FUNC(uint8_t frameBufferCount());

    GFX_API_FUNC(uint8_t colorAttachmentCount());

    GFX_API_FUNC(bool hasDepthStencil());

    GFX_API_FUNC(Attachment getColorAttachment(uint8_t frameIndex, uint8_t attachIndex));

    GFX_API_FUNC(Attachment getDepthStencilAttachment());
};

struct CreateRenderTargetInfo
{
    std::vector<std::vector<Attachment>> colorAttachments;     // 颜色纹理附件信息
    Attachment depthStencilAttachment;                        // 深度纹理附件信息

    SampleCountFlag::Enum sample = SampleCountFlag::SampleCount_1;
};

/**
 * 创建渲染目标
 * 通过输入的颜色纹理集和深度模板纹理创建渲染目标
 * 颜色纹理的大小和格式要保持一致，深度模板纹理大小要和颜色纹理大小保持一致
 *
 * @param context
 * @param createInfo
 * @return
 */
GX_API RenderTarget createRenderTarget(Context, const CreateRenderTargetInfo &);

GX_API void destroyRenderTarget(RenderTarget);


/**
 * 缓冲区对象
 */
GFX_API(Buffer)
{
    /**
     * 获取Buffer大小（字节Byte）
     * @return
     */
    GFX_API_FUNC(uint64_t size());

    /**
     * 映射Buffer到内存
     * 如果当前Buffer无法被映射将返回nullptr
     *
     * @return
     */
    GFX_API_FUNC(void *map());

    /**
     * 显式的同步内存块数据到指定存储区域
     */
    GFX_API_FUNC(void flush());

    /**
     * 解除映射
     */
    GFX_API_FUNC(void unmap());
};

struct CreateBufferInfo
{
    BufferTypeFlags type;
    BufferMemoryUsage::Enum memoryUsage;
    uint64_t size;
};

GX_API Buffer createBuffer(Context context, const CreateBufferInfo &createInfo);

GX_API void destroyBuffer(Buffer buffer);

/**
 * 纹理对象
 */
GFX_API(Texture)
{
    GFX_API_FUNC(TextureType::Enum type());

    GFX_API_FUNC(Format::Enum format());

    GFX_API_FUNC(TextureUsageFlags usage());

    GFX_API_FUNC(TextureAspectFlags aspect());

    GFX_API_FUNC(uint32_t width());

    GFX_API_FUNC(uint32_t height());

    GFX_API_FUNC(uint32_t depth());

    GFX_API_FUNC(uint32_t layerCount());

    GFX_API_FUNC(uint32_t mipLevels());

    /**
     * 返回纹理所占内存大小
     *
     * @return
     */
    GFX_API_FUNC(uint64_t size());

    /**
     * 填充数据
     * 数据的像素格式，大小需匹配
     *
     * @param data
     * @param size
     * @param fence 当用户控制同步时，传用户创建的值
     */
    GFX_API_FUNC(void setData(const void *data, uint64_t size, Fence fence = GFX_NULL_HANDLE));

    /**
     * 创建Mipmap
     * 在创建Texture时填写正确的mipLevels, 根据mipLevels创建Mipmap
     *
     * @param fence 当用户控制同步时，传用户创建的值
     */
    GFX_API_FUNC(void genMipmap(Fence fence = GFX_NULL_HANDLE));
};

/**
 * 创建纹理的参数结构体
 */
struct CreateTextureInfo
{
    //! 纹理类型
    TextureType::Enum type;
    //! 纹理颜色格式
    Format::Enum format;
    //! 纹理用途(如：纹理贴图一般用途是Sampled, 离屏渲染的帧缓冲区颜色纹理用途是Attachment|Sampled)
    TextureUsageFlags usage;
    //! 纹理样式(颜色、深度、模板、深度模板)
    TextureAspectFlags aspect;
    //! 宽
    uint32_t width;
    //! 高
    uint32_t height;
    //! 深度，当type为Texture3D时，和宽高一起表示尺度
    uint32_t depth;
    //! mipMap等级
    uint32_t mipLevels;
    //! 如果是纹理数组，表示数组大小；如果是Cube，则表示Cube的面数；默认情况下填写1
    uint32_t arrayLayers;
    //! 颜色通道调换配置
    TextureSwizzleMapping swizzle;
};

/**
 * 创建纹理
 * 根据使用场景不同，需要传递正确的参数
 *
 * @param context
 * @param createInfo
 * @return
 */
GX_API Texture createTexture(Context context, const CreateTextureInfo &createInfo);

GX_API void destroyTexture(Texture texture);


GFX_API(Sampler)
{
};

struct CreateSamplerInfo
{
    union
    {
        struct
        {
            TextureSamplerFilter::Enum magFilter: 1;            // 1    放大过滤器
            TextureSamplerFilter::Enum minFilter: 1;            // 2    缩小过滤器
            TextureSamplerAddressMode::Enum addressModeU: 2;    // 4    指定U在[0,1]范围外的寻址方式
            TextureSamplerAddressMode::Enum addressModeV: 2;    // 6    指定V在[0,1]范围外的寻址方式
            TextureSamplerAddressMode::Enum addressModeW: 2;    // 8    指定W在[0,1]范围外的寻址方式
            TextureSamplerMipmapMode::Enum mipmapMode: 2;       // 10   mipmap过滤器
            bool hasMips: 1;                                    // 11   是否启用mipmap
            bool compareEnable: 1;                              // 12   是否启用比较器
            CompareOp::Enum compareOp: 4;                       // 16   比较器模式
            uint16_t anisotropyLog2: 8;                         // 32   各向异性值的级别(0-7)
        };
        uint32_t t = 0;
    };

    CreateSamplerInfo()
    {
        magFilter = TextureSamplerFilter::Linear;
        minFilter = TextureSamplerFilter::Linear;
        addressModeU = TextureSamplerAddressMode::Repeat;
        addressModeV = TextureSamplerAddressMode::Repeat;
        addressModeW = TextureSamplerAddressMode::Repeat;
        mipmapMode = TextureSamplerMipmapMode::Linear;
        hasMips = true;
        compareEnable = false;
        compareOp = CompareOp::Never;
        anisotropyLog2 = 0;
    }
    /**
     * @note
     * 如果想达到OpenGL中minFilter为GL_LINEAR或GL_NEAREST的效果，则按以下值设置，可以达到相同的效果：
     * mipmapMode = TextureSamplerMipmapMode::Nearest
     * minFilter = TextureSamplerFilter::Linear(GL_LINEAR时) or TextureSamplerFilter::Nearest(GL_NEAREST时)
     * hasMips = false
     */
};

GX_API Sampler createSampler(Context context, const CreateSamplerInfo &createInfo = {});

GX_API void destroySampler(Sampler sampler);


/**
 * 着色器对象
 */
GFX_API(Shader)
{

};

struct CreateShaderInfo
{
    ShaderType::Enum type;
    const void *pCode;
    size_t codeSize;
    std::string tag;
};

GX_API Shader createShader(Context context, const CreateShaderInfo &createInfo);

GX_API void destroyShader(Shader shader);


struct ResourceLayoutInfo
{
    std::vector<ResourceLayoutBindingInfo> bindingInfos;   // 描述符资源绑定信息
};

GFX_API(ResourceBinder)
{
    GFX_API_FUNC(void bindBuffer(uint32_t binding, Buffer buffer));

    GFX_API_FUNC(void bindBufferRange(uint32_t binding, Buffer buffer,
                                            uint64_t offset, uint64_t range));

    GFX_API_FUNC(void bindTexelBuffer(uint32_t binding, Buffer buffer,
                                            Format::Enum format, uint64_t offset, uint64_t range));

    GFX_API_FUNC(void bindTexture(uint32_t binding, Texture texture, Sampler sampler,
                                        const TextureBindRange &range = {}));

    GFX_API_FUNC(void bindInputAttachment(uint32_t binding, Texture texture, const TextureBindRange &range = {}));
};

GX_API ResourceBinder createResourceBinder(Context context, const ResourceLayoutInfo &layoutInfo);

GX_API void destroyResourceBinder(ResourceBinder binder);


GFX_API(CommandBuffer)
{
    /**
     * 开始录制指令
     * 正常模式
     * 开始一次新的指令录制，将清空上一次录制的缓冲区
     */
    GFX_API_FUNC(CommandBuffer begin());

    /**
     * 结束指令录制
     * 如果不调用结束，录制的指令不会生效
     */
    GFX_API_FUNC(void end());

    //! =============== Graphics commands =============== !//

    /**
     * 设置渲染目标帧缓冲区清屏颜色
     * 仅当在beginRenderPass前设置生效
     *
     * @param r     0-1
     * @param g     0-1
     * @param b     0-1
     * @param a     0-1
     */
    GFX_API_FUNC(CommandBuffer setClearColor(const ClearColor &clearColor));

    /**
     * 设置渲染目标的深度模板测试清理参数
     * 仅当在beginRenderPass前设置生效
     *
     * @param depth     深度值
     * @param stencil   模板值
     */
    GFX_API_FUNC(CommandBuffer setClearDepthStencil(float depth, uint32_t stencil));

    /**
     * 绑定渲染目标
     * 仅当在beginRenderPass前设置生效
     *
     * @param renderTarget  一个目标纹理数量与CommandBuffer创建时传入的bufferCount相同的RenderTarget
     */
    GFX_API_FUNC(CommandBuffer bindRenderTarget(RenderTarget renderTarget));

    /**
     * 绑定渲染目标
     * 通过帧控制器来实现绑定渲染目标
     *
     * @param frame
     */
    GFX_API_FUNC(CommandBuffer bindRenderTarget(Frame frame));

    /**
     * 开始RenderPass
     *
     * @param renderArea    渲染范围
     * @param rpInfo        RenderPass信息
     * @return
     */
    GFX_API_FUNC(CommandBuffer beginRenderPass(const Rect2D &renderArea, const RenderPassInfo &rpInfo));

    /**
     * 结束RenderPass
     */
    GFX_API_FUNC(CommandBuffer endRenderPass());

    GFX_API_FUNC(CommandBuffer setGraphicsPipelineState(const GraphicsPipelineStateInfo &pipelineState));

    GFX_API_FUNC(CommandBuffer setVertexLayout(const VertexLayout &vertexLayout));

    GFX_API_FUNC(CommandBuffer setShaders(const std::vector<Shader> &shaders));

    /**
     * 切换Subpass
     * 仅当RenderPass为Subpass布局时有效
     *
     * @return
     */
    GFX_API_FUNC(CommandBuffer nextSubpass());

    /**
     * 设置Viewport
     *
     * @param viewport
     */
    GFX_API_FUNC(CommandBuffer setViewport(const Rect2D &viewport, float minDepth = 0.0f, float maxDepth = 1.0f));

    /**
     * 设置裁剪范围
     *
     * @param scissor
     */
    GFX_API_FUNC(CommandBuffer setScissor(const Rect2D &scissor));

    /**
     * 设置线宽
     *
     * @param lineWidth
     */
    GFX_API_FUNC(CommandBuffer setLineWidth(float lineWidth));

    GFX_API_FUNC(CommandBuffer setStencilCompareMask(StencilFace::Enum face, uint32_t mask));

    GFX_API_FUNC(CommandBuffer setStencilWriteMask(StencilFace::Enum face, uint32_t mask));

    GFX_API_FUNC(CommandBuffer setStencilReference(StencilFace::Enum face, uint32_t reference));

    /**
     * 绑定资源
     *
     * @param bindPoint
     * @param binder
     * @param dynamicOffsets
     * @return
     */
    GFX_API_FUNC(CommandBuffer bindResources(ResourceBindPoint::Enum bindPoint,
                                             const std::vector<ResourceBinder> &binders,
                                             const std::vector<uint32_t> &dynamicOffsets));

    /**
     * 绑定顶点Buffer
     *
     * @param firstBinding
     * @param buffers
     * @param offsets
     * @return
     */
    GFX_API_FUNC(CommandBuffer bindVertexBuffer(uint32_t firstBinding,
                                                const std::vector<Buffer> &buffers,
                                                const std::vector<uint64_t> &offsets));

    /**
     * 绑定索引缓冲区
     *
     * @param buffer    顶点缓冲区对象
     * @param offset    缓冲区内地址偏移
     * @param indexType 顶点索引的类型
     */
    GFX_API_FUNC(CommandBuffer bindIndexBuffer(Buffer buffer, uint32_t offset, IndexType::Enum indexType));

    /**
     * 根据顶点绘制
     *
     * @param vertexCount       顶点数量
     * @param instanceCount     实例数量
     * @param firstVertex       第一个顶点的偏移
     * @param firstInstance     第一个实例的偏移
     */
    GFX_API_FUNC(CommandBuffer draw(uint32_t vertexCount, uint32_t instanceCount,
                                    uint32_t firstVertex, uint32_t firstInstance));

    /**
     * 根据索引绘制
     *
     * @param indexCount        索引数量
     * @param instanceCount     实例数量
     * @param firstIndex        第一个索引的偏移
     * @param vertexOffset      顶点偏移
     * @param firstInstance     第一个实例的偏移
     */
    GFX_API_FUNC(CommandBuffer drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                         int32_t vertexOffset, uint32_t firstInstance));

    /**
     * 使用buffer中的绘制指令缓冲进行间接绘制
     * (draw的间接版本)
     *
     * @param buffer    间接绘制指令缓冲区, 用于绘制的索引结构必须是 DrawIndirectCommand
     * @param offset    缓冲区起始偏移(Byte)
     * @param drawCount 绘制指令数量
     * @param stride    buffer中两个间接缓冲区数据的间隔(Byte), 如果drawCount为1, stride传值无意义
     * @return
     */
    GFX_API_FUNC(CommandBuffer drawIndirect(Buffer buffer, uint32_t offset, uint32_t drawCount, uint32_t stride));

    /**
     * 使用buffer中的绘制指令缓冲进行间接绘制
     * (drawIndexed的间接版本)
     *
     * @param buffer    间接绘制指令缓冲区, 用于绘制的索引结构必须是 DrawIndexedIndirectCommand
     * @param offset    缓冲区起始偏移(Byte)
     * @param drawCount 绘制指令数量
     * @param stride    buffer中两个间接缓冲区数据的间隔(Byte), 如果drawCount为1, stride传值无意义
     * @return
     */
    GFX_API_FUNC(CommandBuffer drawIndexedIndirect(Buffer buffer, uint32_t offset,
                                                   uint32_t drawCount, uint32_t stride));

    /**
     * 调度计算任务
     * (使用GPU并行Computer功能)
     *
     * @param groupCountX   X 维度上调度的本地工作组的数量
     * @param groupCountY   Y 维度上调度的本地工作组的数量
     * @param groupCountZ   Z 维度上调度的本地工作组的数量
     * @return
     */
    GFX_API_FUNC(CommandBuffer dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ));

    /**
     * dispatch的间接批量版本
     *
     * @param buffer    间dispatch指令缓冲区, 数据单元必须是DispatchIndirectCommand结构
     * @param offset    缓冲区起始偏移(Byte)
     * @return
     */
    GFX_API_FUNC(CommandBuffer dispatchIndirect(Buffer buffer, uint32_t offset));

    /**
     * 提交次级指令
     * 将会被提交的次级指令按提交顺序插入到当前指令缓冲区中
     *
     * @param secondaryCmdBuffers
     * @return
     */
    GFX_API_FUNC(CommandBuffer executeCommands(const std::vector<CommandBuffer> &secondaryCmdBuffers));

    //! =============== Transfer commands =============== !//
    GFX_API_FUNC(CommandBuffer pipelineBarrier(PipelineStageMask srcStage, PipelineStageMask dstStage));

    GFX_API_FUNC(CommandBuffer bufferMemoryBarrier(Buffer buffer, const BufferBarrierInfo &barrierInfo));

    GFX_API_FUNC(CommandBuffer imageMemoryBarrier(Texture texture,
                                                  ImageLayout::Enum srcLayout,
                                                  ImageLayout::Enum dstLayout,
                                                  const ImageSubResourceRange &range));

    /**
     * 复制Buffer到Buffer
     *
     * @param src
     * @param dst
     * @param srcOffset
     * @param dstOffset
     * @param size
     * @return
     */
    GFX_API_FUNC(CommandBuffer copyBuffer(Buffer src, Buffer dst, uint64_t srcOffset, uint64_t dstOffset,
                                          uint64_t size));

    /**
     * 复制Image到Image
     *
     * @param src
     * @param dst
     * @param copyInfos
     * @return
     */
    GFX_API_FUNC(CommandBuffer copyImage(Texture src, Texture dst, const std::vector<ImageCopyInfo> &copyInfos));

    /**
     * 复制Buffer到Image
     *
     * @param src
     * @param dst
     * @param copyInfos
     * @return
     */
    GFX_API_FUNC(CommandBuffer copyBufferToImage(Buffer src, Texture dst,
                                                 const std::vector<BufferImageCopyInfo> &copyInfos));

    /**
     * 复制Image到Buffer
     *
     * @param src
     * @param dst
     * @param copyInfos
     * @return
     */
    GFX_API_FUNC(CommandBuffer copyImageToBuffer(Texture src, Buffer dst,
                                                 const std::vector<BufferImageCopyInfo> &copyInfos));

    /**
     * 复制图像变按照目标格式进行转换
     * 注：必须使用QueueType::Graphics的队列来完成操作
     *
     * @param src
     * @param dst
     * @param blitInfos
     * @param filter        转换时像素的过滤方式
     * @return
     */
    GFX_API_FUNC(CommandBuffer blitImage(Texture src, Texture dst, const std::vector<ImageBlitInfo> &blitInfos,
                                                BlitFilter::Enum filter));

    /**
     * 复制src的内容到dst
     * 要求src与dst副本数量一致，副本纹理格式、大小、层次等信息一致
     *
     * @param src
     * @param dst
     * @param copyInfos
     * @param frameIndex    指定帧序号
     * @return
     */
    GFX_API_FUNC(CommandBuffer copyRenderTarget(RenderTarget src, RenderTarget dst,
                                                const std::vector<ImageCopyInfo> &copyInfos,
                                                uint8_t frameIndex));

    /**
     * 转换src的内容到dst
     * 要求src与dst副本数量一致
     *
     * @param src
     * @param dst
     * @param blitInfos
     * @param filter        转换时像素的过滤方式
     * @param frameIndex    指定帧序号
     * @return
     */
    GFX_API_FUNC(CommandBuffer blitRenderTarget(RenderTarget src, RenderTarget dst,
                                                const std::vector<ImageBlitInfo> &blitInfos,
                                                BlitFilter::Enum filter,
                                                uint8_t frameIndex));

    /**
     * 转换RenderTarget指定内容到图像
     * 注：必须使用QueueType::Graphics的队列来完成操作
     *
     * @param src
     * @param dst
     * @param blitInfos
     * @param filter        转换时像素的过滤方式
     * @param attachIndex   副本的编号，深度副本的编号为颜色副本编号最大值加一
     * @param frameIndex    指定帧序号
     * @return
     */
    GFX_API_FUNC(CommandBuffer blitRenderTargetToImage(RenderTarget src, Texture dst,
                                                       const std::vector<ImageBlitInfo> &blitInfos,
                                                       BlitFilter::Enum filter,
                                                       uint8_t attachIndex,
                                                       uint8_t frameIndex));

    /**
     * 复制RenderTarget指定内容到图像
     *
     * @param src
     * @param dst
     * @param copyInfos
     * @param attachIndex   副本的编号，深度副本的编号为颜色副本编号最大值加一
     * @param frameIndex    指定帧序号
     * @return
     */
    GFX_API_FUNC(CommandBuffer copyRenderTargetToImage(RenderTarget src, Texture dst,
                                                       const std::vector<ImageCopyInfo> &copyInfos,
                                                       uint8_t attachIndex,
                                                       uint8_t frameIndex));

    /**
     * 复制RenderTarget指定内容到Buffer
     *
     * @param src
     * @param dst
     * @param copyInfos
     * @param attachIndex   副本的编号，深度副本的编号为颜色副本编号最大值加一
     * @param frameIndex    指定帧序号
     * @return
     */
    GFX_API_FUNC(CommandBuffer copyRenderTargetToBuffer(RenderTarget src, Buffer dst,
                                                        const std::vector<BufferImageCopyInfo> &copyInfos,
                                                        uint8_t attachIndex,
                                                        uint8_t frameIndex));

    /**
     * 使用data值填充Buffer
     *
     * @param buffer
     * @param offset    Buffer填充起点便宜
     * @param size      要被填充的数据大小，接受GFX_WHOLE_SIZE
     * @param data      填充的模板数据
     * @return
     */
    GFX_API_FUNC(CommandBuffer fillBuffer(Buffer buffer, uint64_t offset, uint64_t size, uint32_t data));

    /**
     * 设置Debug label的开始点
     *
     * @param label
     * @return
     */
    GFX_API_FUNC(CommandBuffer beginDebugLabel(const std::string &label));

    /**
     * 设置debug label的结束点
     *
     * @note
     * 与beginDebugLabel成对出现
     *
     * @return
     */
    GFX_API_FUNC(CommandBuffer endDebugLabel());

    /**
     * 重设查询
     *
     * @param query
     * @param firstQuery
     * @param queryCount
     * @return
     */
    GFX_API_FUNC(CommandBuffer resetQuery(Query query, uint32_t firstQuery, uint32_t queryCount));

    /**
     * 设置查询开始点
     *
     * @note
     * 当QueryType为Occlusion或PipelineStatistics时有效
     *
     * @param query
     * @param queryIndex    PipelineStatistics时一定为0
     * @param precise       对Occlusion有效
     * @return
     */
    GFX_API_FUNC(CommandBuffer beginQuery(Query query, uint32_t queryIndex, bool precise));

    /**
     * 设置查询结束点
     * @param query
     * @param queryIndex
     * @return
     */
    GFX_API_FUNC(CommandBuffer endQuery(Query query, uint32_t queryIndex));

    /**
     * 指定在某个RenderPass内的某个PipelineStage点插入时间戳查询
     *
     * @param pipelineStage
     * @param query
     * @param queryIndex
     * @return
     */
    GFX_API_FUNC(CommandBuffer writeTimestamp(PipelineStage::Enum pipelineStage, Query query, uint32_t queryIndex));

    /**
     * 将查询结果拷贝到Buffer
     *
     * @note
     * Buffer的单元数据类型只能是uint64_t
     *
     * @param query
     * @param firstQuery
     * @param queryCount    当QueryType为PipelineStatistics时，数值为pipelineStatistics的枚举量相关
     *                      详见CreateQueryInfo的queryCount注释
     * @param dstBuffer
     * @param dstOffset     以uint64_t为单位大小的字节偏移
     * @param resultFlags
     * @return
     */
    GFX_API_FUNC(CommandBuffer copyQueryResults(Query query, uint32_t firstQuery, uint32_t queryCount,
                                                      Buffer dstBuffer, uint64_t dstOffset,
                                                      QueryResultFlags resultFlags = 0));
};

struct CreateCommandBufferInfo
{
    QueueType::Enum queueType;
    uint32_t bufferCount;
};

GX_API CommandBuffer createCommandBuffer(Context context, const CreateCommandBufferInfo &createInfo);

GX_API void destroyCommandBuffer(CommandBuffer obj);


GFX_API(Query)
{
    /**
     * 获取查询结果，
     * 要获取到正确的查询结果，推荐等待指令执行结束后再调用getQueryResults。
     *
     * @param results   结果通过该容器返回，容器原本的数据会被覆盖
     * @param flags     更多获取结果时的控制要求
     */
    GFX_API_FUNC(void getQueryResults(std::vector<uint64_t> &results, QueryResultFlags flags = 0));
};

struct CreateQueryInfo
{
    /// 查询类型
    QueryType::Enum queryType;

    ///当queryType为PipelineStatistics时，数值为pipelineStatistics的枚举数量；
    ///当queryType为Occlusion时，数值为可插入的beginQuery和endQuery的数量；
    ///当queryType为Timestamp时，数值为writeTimestamp的query数量
    uint32_t queryCount;

    ///仅当queryType为PipelineStatistics时时，值有效
    QueryPipelineStatisticsFlags pipelineStatistics;
};

GX_API Query createQuery(Context context, const CreateQueryInfo &createInfo);

GX_API void destroyQuery(Query obj);

/// ========== Other ========== ///
GX_API size_t getObjectHash(GfxObject *obj);

GX_API uint32_t getObjectIdx(GfxObject *obj);

GX_API bool objectEquals(GfxObject *a, GfxObject *b);

}

#endif //GX_GFX_CORE_H
