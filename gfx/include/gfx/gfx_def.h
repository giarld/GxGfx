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

#ifndef GX_GFX_DEF_H
#define GX_GFX_DEF_H

#include <gfx/gfx_base.h>
#include <gx/gglobal.h>
#include <vector>

#include <memory.h>

#ifdef Always
#undef Always
#endif

#ifdef None
#undef None
#endif

#ifdef Success
#undef Success
#endif


namespace gfx
{

#define SUBPASS_EXTERNAL (~0U)

#define GFX_WHOLE_SIZE   (~0ULL)

#define GFX_MAX_VERTEX_ATTRIBUTE_COUNT 16

/**
 * 目标API类型枚举
 */
struct TargetApiType
{
    enum Enum : uint8_t
    {
        Vulkan = 0,

        Count
    };
};

/**
 * 设备类型
 */
struct DeviceType
{
    enum Enum : uint8_t
    {
        Other = 0,
        IntegratedGPU,  // 集成显卡
        DiscreteGPU,    // 独立显卡
        VirtualGPU,     // 虚拟显卡
        CPU,            // cpu

        Count
    };
};

/**
 * Gfx可选扩展
 */
struct GfxEXT
{
    enum Enum
    {

    };
};

/**
 * 设备可选扩展
 */
struct DeviceEXT
{
    enum Enum
    {

    };
};

/**
 * 帧渲染目标枚举
 */
struct FrameTargetType
{
    enum Enum : uint8_t
    {
        SwapChain = 0,      // 屏幕
        RenderTarget = 1,   // 目标纹理

        Count
    };
};

/**
 * 颜色和深度模板格式
 */
struct Format
{
    enum Enum : uint8_t
    {
        Undefined = 0,
        R8_UNorm,
        R8_SNorm,
        R8_UInt,
        R8_SInt,
        R8_SRgb,

        R8G8_UNorm,
        R8G8_SNorm,
        R8G8_UInt,
        R8G8_SInt,
        R8G8_SRgb,

        R8G8B8_UNorm,
        R8G8B8_SNorm,
        R8G8B8_UInt,
        R8G8B8_SInt,
        R8G8B8_SRgb,

        B8G8R8_UNorm,
        B8G8R8_SNorm,
        B8G8R8_UInt,
        B8G8R8_SInt,
        B8G8R8_SRgb,

        R8G8B8A8_UNorm,
        R8G8B8A8_SNorm,
        R8G8B8A8_UInt,
        R8G8B8A8_SInt,
        R8G8B8A8_SRgb,

        B8G8R8A8_UNorm,
        B8G8R8A8_SNorm,
        B8G8R8A8_UInt,
        B8G8R8A8_SInt,
        B8G8R8A8_SRgb,

        A8B8G8R8_UNorm,
        A8B8G8R8_SNorm,
        A8B8G8R8_UInt,
        A8B8G8R8_SInt,
        A8B8G8R8_SRgb,

        R5G6B5_UNorm,
        B5G6R5_UNorm,

        R5G5B5A1_UNorm,
        B5G5R5A1_UNorm,
        A1R5G5B5_UNorm,
        R4G4B4A4_UNorm,

        A2R10G10B10_UNorm,
        A2R10G10B10_SNorm,
        A2R10G10B10_UInt,
        A2R10G10B10_SInt,

        A2B10G10R10_UNorm,
        A2B10G10R10_SNorm,
        A2B10G10R10_UInt,
        A2B10G10R10_SInt,

        D16_UNorm,
        X8_D24_UNorm,
        D32_SFloat,
        S8_UInt,
        D16_UNorm_S8_UInt,
        D24_UNorm_S8_UInt,
        D32_SFloat_S8_UInt,

        R16_UInt,
        R16_SInt,
        R16_UNorm,
        R16_SNorm,
        R16_SFloat,
        R16G16_UInt,
        R16G16_SInt,
        R16G16_UNorm,
        R16G16_SNorm,
        R16G16_SFloat,
        R16G16B16_UInt,
        R16G16B16_SInt,
        R16G16B16_UNorm,
        R16G16B16_SNorm,
        R16G16B16_SFloat,
        R16G16B16A16_UInt,
        R16G16B16A16_SInt,
        R16G16B16A16_UNorm,
        R16G16B16A16_SNorm,
        R16G16B16A16_SFloat,

        R32_UInt,
        R32_SInt,
        R32_SFloat,
        R32G32_UInt,
        R32G32_SInt,
        R32G32_SFloat,
        R32G32B32_UInt,
        R32G32B32_SInt,
        R32G32B32_SFloat,
        R32G32B32A32_UInt,
        R32G32B32A32_SInt,
        R32G32B32A32_SFloat,

        R64_UInt,
        R64_SInt,
        R64_SFloat,
        R64G64_UInt,
        R64G64_SInt,
        R64G64_SFloat,
        R64G64B64_UInt,
        R64G64B64_SInt,
        R64G64B64_SFloat,
        R64G64B64A64_UInt,
        R64G64B64A64_SInt,
        R64G64B64A64_SFloat,

        B10G11R11_UFloat,
        E5B9G9R9_UFloat,

        BC1_RGB_UNorm,
        BC1_RGB_SRgb,
        BC1_RGBA_UNorm,
        BC1_RGBA_SRgb,
        BC2_UNorm,
        BC2_SRgb,
        BC3_UNorm,
        BC3_SRgb,
        BC4_UNorm,
        BC4_SNorm,
        BC5_UNorm,
        BC5_SNorm,
        BC6H_UFloat,
        BC6H_SFloat,
        BC7_UNorm,
        BC7_SRgb,

        ETC2_R8G8B8_UNorm,
        ETC2_R8G8B8_SRgb,
        ETC2_R8G8B8A1_UNorm,
        ETC2_R8G8B8A1_SRgb,
        ETC2_R8G8B8A8_UNorm,
        ETC2_R8G8B8A8_SRgb,
        EAC_R11_UNorm,
        EAC_R11_SNorm,
        EAC_R11G11_UNorm,
        EAC_R11G11_SNorm,
        ASTC_4x4_UNorm,
        ASTC_4x4_SRgb,
        ASTC_5x4_UNorm,
        ASTC_5x4_SRgb,
        ASTC_5x5_UNorm,
        ASTC_5x5_SRgb,
        ASTC_6x5_UNorm,
        ASTC_6x5_SRgb,
        ASTC_6x6_UNorm,
        ASTC_6x6_SRgb,
        ASTC_8x5_UNorm,
        ASTC_8x5_SRgb,
        ASTC_8x6_UNorm,
        ASTC_8x6_SRgb,
        ASTC_8x8_UNorm,
        ASTC_8x8_SRgb,
        ASTC_10x5_UNorm,
        ASTC_10x5_SRgb,
        ASTC_10x6_UNorm,
        ASTC_10x6_SRgb,
        ASTC_10x8_UNorm,
        ASTC_10x8_SRgb,
        ASTC_10x10_UNorm,
        ASTC_10x10_SRgb,
        ASTC_12x10_UNorm,
        ASTC_12x10_SRgb,
        ASTC_12x12_UNorm,
        ASTC_12x12_SRgb,

        Count
    };
};

struct FormatFeatureFlag
{
    enum Enum
    {
        SampledImage = 0x00000001,
        StorageImage = 0x00000002,
        StorageImageAtomic = 0x00000004,
        UniformTexelBuffer = 0x00000008,
        StorageTexelBuffer = 0x00000010,
        StorageTexelBufferAtomic = 0x00000020,
        ColorAttachment = 0x00000040,
        ColorAttachmentBlend = 0x00000080,
        DepthStencilAttachment = 0x00000100,
        BlitSrc = 0x00000200,
        BlitDst = 0x00000400,
        TransferSrc = 0x00000800,
        TransferDst = 0x00001000,
    };
};

typedef uint32_t FormatFeatureFlags;

/**
 * 顶点属性类型
 */
struct VertexAttribType
{
    enum Enum : uint8_t
    {
        Byte = 0,
        Byte2,
        Byte3,
        Byte4,
        UByte,
        UByte2,
        UByte3,
        UByte4,
        Short,
        Short2,
        Short3,
        Short4,
        UShort,
        UShort2,
        UShort3,
        UShort4,
        Int,
        UInt,
        Float,
        Float2,
        Float3,
        Float4,
        Half,
        Half2,
        Half3,
        Half4,

        Count
    };
};

/**
 * 索引格式
 */
struct IndexType
{
    enum Enum : uint8_t
    {
        UInt16 = 0,
        UInt32
    };
};

/**
 * 采样次数枚举
 */
struct SampleCountFlag
{
    enum Enum : uint8_t
    {
        SampleCount_1 = 0x01,
        SampleCount_2 = 0x02,
        SampleCount_4 = 0x04,
        SampleCount_8 = 0x08,
        SampleCount_16 = 0x10,
        SampleCount_32 = 0x20,
        SampleCount_64 = 0x40,
    };
};

/**
 * 图像布局格式
 */
struct ImageLayout
{
    enum Enum : uint8_t
    {
        Undefined = 0,
        GraphicsGeneral,             // 可被内存映射(图形管线)
        ComputeGeneral,              // 可被内存映射(计算管线)
        ColorAttachment,             // 存储颜色内容
        DepthStencilAttachment,      // 存储深度模板内容
        DepthStencilReadOnly,        // 只读深度模板
        ShaderReadOnly,
        TransferSrc,
        TransferDst,
        PresentSrc,

        Count
    };
};

/**
 * 纹理类型
 */
struct TextureType
{
    enum Enum : uint8_t
    {
        Texture1D = 0,
        Texture2D,
        Texture3D,
        Texture1DArray,
        Texture2DArray,
        TextureCube,

        Count
    };
};

/**
 * 纹理用途
 */
struct TextureUsage
{
    enum Enum : uint8_t
    {
        Sampled = 0x01,            // 用于纹理采样器，如输入纹理
        Storage = 0x02,            // 用于着色器可读写资源
        Attachment = 0x04,         // 资源附件，用于渲染目标（帧缓冲区，渲染结果输出）
        InputAttachment = 0x08,    // 输入附件，用于subpassInput
    };
};

typedef uint8_t TextureUsageFlags;

/**
 * 纹理的样式
 */
struct TextureAspect
{
    enum Enum : uint8_t
    {
        AspectNone = 0,         // 仅用于资源绑定描述
        AspectColor = 0x01,
        AspectDepth = 0x02,
        AspectStencil = 0x04,
    };
};

typedef uint8_t TextureAspectFlags;

/**
 * 纹理颜色调换枚举
 */
struct TextureSwizzle
{
    enum Enum : uint8_t
    {
        Identity = 0,       // Default, 表示保持原通道映射
        Zero,
        One,
        R,
        G,
        B,
        A
    };
};

/**
 * 纹理采样器采样方式
 */
struct TextureSamplerFilter
{
    enum Enum : uint8_t
    {
        Nearest = 0,    // 邻近采样
        Linear,         // 线性采样
    };
};

/**
 * 纹理采样器Mipmap的采样模式
 */
struct TextureSamplerMipmapMode
{
    enum Enum : uint8_t
    {
        Nearest = 0,    // 邻近采样
        Linear,     // 线性采样
    };
};

/**
 * 采样uv坐标以外像素的采环绕方式
 */
struct TextureSamplerAddressMode
{
    enum Enum : uint8_t
    {
        Repeat = 0,                 // 重复纹理图像
        MirroredRepeat,             // 和Repeat一样，但每次重复图片是镜像放置的
        ClampToEdge                 // 纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果
    };
};

/**
 * 纹理过滤获取数据的比较函数或模板缓冲的比较函数
 */
struct CompareOp
{
    enum Enum : uint8_t
    {
        Never = 0,          // always False
        Less,               // A < B
        Equal,              // A = B
        LessOrEqual,        // A <= B
        Greater,            // A > B
        NotEqual,           // A != B
        GreaterOrEqual,     // A >= B
        Always              // always True
    };
};

/**
 * Buffer类型
 */
struct BufferType
{
    enum Enum : uint8_t
    {
        Staging = 0x01,       // 暂存(可用于转换)
        Uniform = 0x02,       // Uniform
        Storage = 0x04,       // 存储缓冲(用于Shader中可读写的部分)
        Index = 0x08,         // 索引缓冲
        Vertex = 0x10,        // 顶点缓冲
        Indirect = 0x20,      // 间接渲染缓冲
        Texel = 0x40,         // 纹素, 有效的前提是BufferType同时为Uniform或Storage
    };
};

typedef uint8_t BufferTypeFlags;

struct BufferMemoryUsage
{
    enum Enum : uint8_t
    {
        GpuOnly = 0,    // 分配在专用GPU内存中，不可直接map
        CpuOnly,        // 分配在共享GPU内存中，可直接map，但gpu读写慢
        CpuToGpu,       // 主机端可见并且GPU端可快速访问，主要用于主机端频繁写入，由设备端读取的场景
        GpuToCpu,       // 用于GPU端写入资源，主机端读取资源的情况，如屏幕捕获等场景

        Count
    };
};

/**
 * 渲染目标附件枚举，
 * 目前主流GPU能接受的最大颜色附件数量为8，日常使用也不会超出这个数量
 */
struct RenderTargetAttachmentFlag
{
    enum Enum : uint32_t
    {
        None = 0x0u,                        //!< No Attachment selected.
        Color0 = 0x00000001u,               //!< Attachment Color0
        Color1 = 0x00000002u,               //!< Attachment Color1
        Color2 = 0x00000004u,               //!< Attachment Color2
        Color3 = 0x00000008u,               //!< Attachment Color3
        Color4 = 0x00000010u,               //!< Attachment Color4
        Color5 = 0x00000020u,               //!< Attachment Color5
        Color6 = 0x00000040u,               //!< Attachment Color6
        Color7 = 0x00000080u,               //!< Attachment Color7

        ColorAll = Color0 | Color1 | Color2 | Color3 | Color4 | Color5 | Color6 | Color7,
        Depth = 0x10000000u,                //!< Depth And Stencil Attachment.
        All = ColorAll | Depth,        //!< Color, depth and stencil Attachment selected.

        ColorCount = 8
    };
};

typedef uint32_t RenderTargetAttachmentFlags;

/**
 * 指令队列的类型
 */
struct QueueType
{
    enum Enum : uint8_t
    {
        Graphics = 0,
        Compute,
        Transfer
    };
};

struct ShaderType
{
    enum Enum : uint8_t
    {
        Vertex = 0x01,
        TessellationControl = 0x02,
        TessellationEvaluation = 0x04,
        Geometry = 0x08,
        Fragment = 0x10,
        Compute = 0x20,
        AllGraphics = 0x1f,
        All = 0xff
    };
};

typedef uint32_t ShaderTypeFlags;

struct ResourceType
{
    enum Enum : uint8_t
    {
        Sampler = 0,
        CombinedImageSampler,
        SamplerImage,
        StorageImage,
        UniformTexelBuffer,
        StorageTexelBuffer,
        UniformBuffer,
        StorageBuffer,
        UniformBufferDynamic,
        StorageBufferDynamic,
        InputAttachment,
    };
};

struct ResourceBindPoint
{
    enum Enum : uint8_t
    {
        Graphics = 0,
        Compute = 1
    };
};

struct VertexInputRate
{
    enum Enum : uint8_t
    {
        Vertex = 0,
        Instance,
    };
};

/**
 * 多边形生成方式
 */
struct PolygonMode
{
    enum Enum : uint8_t
    {
        Fill = 0,       // 填充（面）
        Line,       // 线
        Point,      // 点

        Count
    };
};

/**
 * 剔除方式
 */
struct CullMode
{
    enum Enum : uint8_t
    {
        None = 0,
        Front,
        Back,
        FrontAndBack,

        Count
    };
};

/**
 * 正面的绘制方向
 */
struct FrontFace
{
    enum Enum : uint8_t
    {
        CounterClockwise = 0,       // 逆时针
        Clockwise,              // 顺时针

        Count
    };
};

/**
 * 顶点的原始组装方式
 */
struct PrimitiveTopology
{
    enum Enum : uint8_t
    {
        PointList = 0,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        TriangleFan,
        LineListWithAdjacency,
        LineStripWithAdjacency,
        TriangleListWithAdjacency,
        TriangleStripWithAdjacency,
        PatchList,

        Count
    };
};

/**
 * 混合因子
 */
struct BlendFactor
{
    enum Enum : uint8_t
    {
        Zero = 0,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        SrcAlphaSaturate,
        Src1Color,
        OneMinusSrc1Color,
        Src1Alpha,
        OneMinusSrc1Alpha,

        Count
    };
};

/**
 * 混合比较函数
 */
struct BlendOp
{
    enum Enum : uint8_t
    {
        Add = 0,
        Subtract,
        ReverseSubtract,
        Min,
        Max,

        Count
    };
};

struct ColorComponentFlag
{
    enum Enum : uint8_t
    {
        ComponentR = 0x01,
        ComponentG = 0x02,
        ComponentB = 0x04,
        ComponentA = 0x08,

        ComponentRGB = ComponentR | ComponentG | ComponentB,
        ComponentAll = ComponentRGB | ComponentA
    };
};

typedef uint8_t ColorComponentFlags;

/**
 * 逻辑比较函数
 */
struct LogicOp
{
    enum Enum : uint8_t
    {
        Clear = 0,
        And,
        AndReverse,
        Copy,
        AndInverted,
        NoOp,
        Xor,
        Or,
        NOr,
        Equivalent,
        Invert,
        OrReverse,
        CopyInverted,
        OrInverted,
        NAnd,
        Set,

        Count
    };
};

struct StencilOp
{
    enum Enum : uint8_t
    {
        Keep = 0,
        Zero,
        Replace,
        IncrementAndClamp,
        DecrementAndClamp,
        Invert,
        IncrementAndWrap,
        DecrementAndWrap,

        Count
    };
};

struct StencilFace
{
    enum Enum : uint8_t
    {
        Front = 0,
        Back,
        FrontAndBack,

        Count
    };
};

/**
 * Blit操作指令的采样方式
 */
struct BlitFilter
{
    enum Enum : uint8_t
    {
        Nearest = 0,    // 邻近采样
        Linear,         // 线性采样

        Count
    };
};

struct FenceWaitRet
{
    enum Enum : uint8_t
    {
        Success = 0,
        Timeout
    };
};

struct PipelineStage
{
    enum Enum
    {
        TopOfPipe = 0x00000001,
        DrawIndirect = 0x00000002,
        VertexInput = 0x00000004,
        VertexShader = 0x00000008,
        TessellationControlShader = 0x00000010,
        TessellationEvaluationShader = 0x00000020,
        GeometryShader = 0x00000040,
        FragmentShader = 0x00000080,
        EarlyFragmentTests = 0x00000100,
        LateFragmentTests = 0x00000200,
        ColorAttachmentOutput = 0x00000400,
        ComputeShader = 0x00000800,
        Transfer = 0x00001000,
        BottomOfPipe = 0x00002000,
        Host = 0x00004000,
        AllGraphics = 0x00008000,
        AllCommands = 0x00010000,
    };
};

typedef uint32_t PipelineStageMask;

struct BufferAccess
{
    enum Enum
    {
        IndirectCommandRead = 0x00000001,
        IndexRead = 0x00000002,
        VertexAttributeRead = 0x00000004,
        UniformRead = 0x00000008,
        InputAttachmentRead = 0x00000010,
        ShaderRead = 0x00000020,
        ShaderWrite = 0x00000040,
        ColorAttachmentRead = 0x00000080,
        ColorAttachmentWrite = 0x00000100,
        DepthStencilAttachmentRead = 0x00000200,
        DepthStencilAttachmentWrite = 0x00000400,
        TransferRead = 0x00000800,
        TransferWrite = 0x00001000,
        HostRead = 0x00002000,
        HostWrite = 0x00004000,
        MemoryRead = 0x00008000,
        MemoryWrite = 0x00010000,
    };
};

typedef uint32_t BufferAccessMask;

/**
 * 查询类型
 */
struct QueryType
{
    enum Enum : uint8_t
    {
        Occlusion = 0,          //!< 遮挡查询
        PipelineStatistics,     //!< 流水线统计数据查询
        Timestamp               //!< 时间戳查询
    };
};

/**
 * 流水线统计数据类型
 */
struct QueryPipelineStatistics
{
    enum Enum : uint16_t
    {
        InputAssemblyVertices = 0x0001,                     //!< 顶点组装阶段顶点数
        InputAssemblyPrimitives = 0x0002,                   //!< 顶点组装阶段图元数
        VertexShaderInvocations = 0x0004,                   //!< 顶点着色器调用数
        GeometryShaderInvocations = 0x0008,                 //!< 几何着色器调用数
        GeometryShaderPrimitives = 0x0010,                  //!< 几何着色器图元数
        ClippingInvocations = 0x0020,                       //!< 图元裁剪阶段处理的图元数
        ClippingPrimitives = 0x0040,                        //!< 图元裁剪阶后输出图元数
        FragmentShaderInvocations = 0x0080,                 //!< 片元着色器调用数
        TessellationControlShaderPatches = 0x0100,          //!< 细分控制着色器 patch 数量
        TessellationEvaluationShaderInvocations = 0x0200,   //!< 细分计算着色器调用数
        ComputeShaderInvocations = 0x0400                   //!< 计算着色器调用数
    };
};

typedef uint16_t QueryPipelineStatisticsFlags;

struct QueryResultFlag
{
    enum Enum : uint8_t
    {
        Wait = 0x01,                //!< 指定将等待每个查询的状态变为可用，然后再检索其结果
        WithAvailability = 0x02,    //!< 指定结果附带可用性状态
        Partial = 0x04              //!< 指定返回部分结果是可接受的
    };
};

typedef uint8_t QueryResultFlags;


struct ClearColor
{
    float r;
    float g;
    float b;
    float a;
};

struct Rect2D
{
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;
};

struct Rect2DF
{
    float x;
    float y;
    float width;
    float height;
};

// data infos

/**
 * 平台窗口的数据
 */
struct SwapChainInfo
{
    void *nativeDisplayType;       //!< Native display type.
    void *nativeWindowHandle;      //!< Native window handle.
    void *context;                 //!< GL context, or D3D device.
    void *backBuffer;              //!< GL backbuffer, or D3D render target view.
    void *backBufferDS;            //!< Backbuffer depth/stencil.
};

/**
 * 额外扩展特性
 */
struct ExtFeatures
{
    bool robustBufferAccess;
    bool fullDrawIndexUint32;
    bool imageCubeArray;
    bool independentBlend;
    bool geometryShader;
    bool tessellationShader;
    bool sampleRateShading;
    bool dualSrcBlend;
    bool logicOp;
    bool multiDrawIndirect;
    bool drawIndirectFirstInstance;
    bool depthClamp;
    bool depthBiasClamp;
    bool fillModeNonSolid;
    bool depthBounds;
    bool wideLines;
    bool largePoints;
    bool alphaToOne;
    bool multiViewport;
    bool samplerAnisotropy;
    bool textureCompressionETC2;
    bool textureCompressionASTC_LDR;
    bool textureCompressionBC;
    bool occlusionQueryPrecise;
    bool pipelineStatisticsQuery;
    bool vertexPipelineStoresAndAtomics;
    bool fragmentStoresAndAtomics;
    bool shaderTessellationAndGeometryPointSize;
    bool shaderImageGatherExtended;
    bool shaderStorageImageExtendedFormats;
    bool shaderStorageImageMultisample;
    bool shaderStorageImageReadWithoutFormat;
    bool shaderStorageImageWriteWithoutFormat;
    bool shaderUniformBufferArrayDynamicIndexing;
    bool shaderSampledImageArrayDynamicIndexing;
    bool shaderStorageBufferArrayDynamicIndexing;
    bool shaderStorageImageArrayDynamicIndexing;
    bool shaderClipDistance;
    bool shaderCullDistance;
    bool shaderFloat64;
    bool shaderInt64;
    bool shaderInt16;
    bool shaderResourceResidency;
    bool shaderResourceMinLod;
    bool sparseBinding;
    bool sparseResidencyBuffer;
    bool sparseResidencyImage2D;
    bool sparseResidencyImage3D;
    bool sparseResidency2Samples;
    bool sparseResidency4Samples;
    bool sparseResidency8Samples;
    bool sparseResidency16Samples;
    bool sparseResidencyAliased;
    bool variableMultisampleRate;
    bool inheritedQueries;
};

/**
 * 设备信息
 */
struct DeviceInfo
{
    uint32_t deviceIndex;
    uint32_t deviceID;
    uint32_t apiVersion;
    uint32_t driverVersion;
    DeviceType::Enum deviceType;
    char deviceName[256];
    ExtFeatures features;
};

struct SubPassDesc
{
    bool useDepthStencil;
    std::vector<uint32_t> colorAttachments;
    std::vector<uint32_t> inputAttachments;
};

struct SubPassDependency
{
    uint32_t srcSubPass;
    uint32_t dstSubPass;
};

struct SubPassInfo
{
    std::vector<SubPassDesc> subPassDescs;
    std::vector<SubPassDependency> subPassDepends;
};

struct RenderPassInfo
{
    RenderTargetAttachmentFlags clear = RenderTargetAttachmentFlag::None;
    RenderTargetAttachmentFlags discard = RenderTargetAttachmentFlag::None;

    SubPassInfo subPassInfo{};
};

/**
 * 顶点绑定参数
 * 32 bit
 */
struct VertexInputBindingInfo
{
    uint8_t binding;                    // binding id
    VertexInputRate::Enum inputRate;    // 顶点输入的方式（顶点、实例）
    uint16_t stride;                     // 表示一个顶点数据结构的大小
};

/**
 * 顶点输入属性的描述 (VAO desc)
 * 64 bit
 */
struct VertexInputAttributeDescInfo
{
    uint8_t binding: 8;                // binding id
    uint8_t location: 8;               // 对应Vertex shader的location
    VertexAttribType::Enum attrib: 8;  // 顶点属性类型
    bool normalized: 4;                // 是否归一化
    bool use: 4;                       // 属性被使用的标记 (用户无关)
    uint32_t offset;                    // 当前单元的地址偏移
};

struct TextureSwizzleMapping
{
    TextureSwizzle::Enum r;
    TextureSwizzle::Enum g;
    TextureSwizzle::Enum b;
    TextureSwizzle::Enum a;
};

struct TextureBindRange
{
    TextureAspectFlags aspect = TextureAspect::AspectNone;
    uint32_t baseMipLevel = 0;
    uint32_t levelCount = UINT32_MAX;
    uint32_t baseArrayLayer = 0;
    uint32_t layerCount = UINT32_MAX;
};

struct StencilOpState
{
    StencilOpState()
    {
        static_assert(sizeof(StencilOpState) == sizeof(uint16_t));

        failOp = StencilOp::Keep;
        passOp = StencilOp::Keep;
        depthFailOp = StencilOp::Keep;
        compareOp = CompareOp::Always;
    }

    struct
    {
        StencilOp::Enum failOp: 4;
        StencilOp::Enum passOp: 4;
        StencilOp::Enum depthFailOp: 4;
        CompareOp::Enum compareOp: 4;
    };
};

struct PipelineRasterStateInfo
{
    PipelineRasterStateInfo()
    {
        static_assert(sizeof(PipelineRasterStateInfo) == sizeof(uint64_t));

        polygonMode = PolygonMode::Fill;
        primitive = PrimitiveTopology::TriangleList;
        cullMode = CullMode::None;
        frontFace = FrontFace::CounterClockwise;
        primitiveRestartEnable = false;
        logicOpEnable = false;
        logicOp = LogicOp::Clear;

        colorBlendOp = BlendOp::Add;
        alphaBlendOp = BlendOp::Add;
        srcColorBlendFactor = BlendFactor::One;
        srcAlphaBlendFactor = BlendFactor::One;
        dstColorBlendFactor = BlendFactor::Zero;
        dstAlphaBlendFactor = BlendFactor::Zero;
        colorWriteMask = ColorComponentFlag::ComponentAll;

        depthTestEnable = true;
        depthWriteEnable = true;
        depthCompareOp = CompareOp::LessOrEqual;

        stencilTestEnable = false;

        sampleShadingEnable = false;
        conservativeEnable = false;
        alphaToCoverageEnable = false;
        alphaToOneEnable = false;
    }

    union
    {
        struct
        {
            PolygonMode::Enum polygonMode: 2;          // 2
            PrimitiveTopology::Enum primitive: 4;      // 6
            CullMode::Enum cullMode: 2;                // 8

            FrontFace::Enum frontFace: 1;              // 9
            BlendOp::Enum colorBlendOp: 3;             // 12

            bool primitiveRestartEnable: 1;            // 13
            BlendOp::Enum alphaBlendOp: 3;             // 16

            LogicOp::Enum logicOp: 4;                  // 20
            BlendFactor::Enum srcColorBlendFactor: 4;  // 24
            BlendFactor::Enum srcAlphaBlendFactor: 4;  // 28
            BlendFactor::Enum dstColorBlendFactor: 4;  // 32
            BlendFactor::Enum dstAlphaBlendFactor: 4;  // 36
            ColorComponentFlags colorWriteMask: 4;     // 40

            bool logicOpEnable: 1;                     //
            bool depthTestEnable: 1;                   //
            bool depthWriteEnable: 1;                  //
            bool stencilTestEnable: 1;                 // 44

            CompareOp::Enum depthCompareOp: 4;         // 48

            bool sampleShadingEnable: 1;               //
            bool conservativeEnable: 1;                //
            bool alphaToCoverageEnable: 1;             //
            bool alphaToOneEnable: 1;                  // 52
        };
        uint64_t t = 0;
    };
};

/**
 * 渲染管线参数值
 */
struct PipelineParamValueInfo
{
    float depthBiasConstantFactor = 0;              // 32
    float depthBiasClamp = 0;                       // 32
    float depthBiasSlopeFactor = 0;                 // 32

    StencilOpState frontStencilOp{};                // 16
    StencilOpState backStencilOp{};                 // 16

    uint32_t tessellationPatchControlPoints = 0;    // 32
};


struct ResourceLayoutBindingInfo
{
    ResourceType::Enum descriptorType;
    ShaderTypeFlags shaderStage;
};

struct GraphicsPipelineStateInfo
{
    PipelineRasterStateInfo rasterStateInfo{};
    PipelineParamValueInfo paramValueInfo{};

    GraphicsPipelineStateInfo &setPolygonMode(PolygonMode::Enum polygonMode)
    {
        rasterStateInfo.polygonMode = polygonMode;
        return *this;
    }

    GraphicsPipelineStateInfo &setCullMode(CullMode::Enum cullMode)
    {
        rasterStateInfo.cullMode = cullMode;
        return *this;
    }

    GraphicsPipelineStateInfo &setFrontFace(FrontFace::Enum frontFace)
    {
        rasterStateInfo.frontFace = frontFace;
        return *this;
    }

    GraphicsPipelineStateInfo &setDepthBias(float depthBiasConstantFactor,
                                            float depthBiasClamp,
                                            float depthBiasSlopeFactor)
    {
        paramValueInfo.depthBiasConstantFactor = depthBiasConstantFactor;
        paramValueInfo.depthBiasClamp = depthBiasClamp;
        paramValueInfo.depthBiasSlopeFactor = depthBiasSlopeFactor;
        return *this;
    }

    GraphicsPipelineStateInfo &setPrimitiveRestartEnable(bool enable)
    {
        rasterStateInfo.primitiveRestartEnable = enable;
        return *this;
    }

    GraphicsPipelineStateInfo &setPrimitiveTopology(PrimitiveTopology::Enum topology)
    {
        rasterStateInfo.primitive = topology;
        return *this;
    }

    GraphicsPipelineStateInfo &setTessellationPatchControlPoints(uint32_t patchControlPoints)
    {
        paramValueInfo.tessellationPatchControlPoints = patchControlPoints;
        return *this;
    }

    GraphicsPipelineStateInfo &setBlendLogicOpEnable(bool enable)
    {
        rasterStateInfo.logicOpEnable = enable;
        return *this;
    }

    GraphicsPipelineStateInfo &setColorBlendLogicOp(LogicOp::Enum logicOp)
    {
        rasterStateInfo.logicOp = logicOp;
        return *this;
    }

    GraphicsPipelineStateInfo &setBlendSrcColorBlendFactor(BlendFactor::Enum factor)
    {
        rasterStateInfo.srcColorBlendFactor = factor;
        return *this;
    }

    GraphicsPipelineStateInfo &setBlendDstColorBlendFactor(BlendFactor::Enum factor)
    {
        rasterStateInfo.dstColorBlendFactor = factor;
        return *this;
    }

    GraphicsPipelineStateInfo &setBlendColorBlendOp(BlendOp::Enum op)
    {
        rasterStateInfo.colorBlendOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setBlendSrcAlphaBlendFactor(BlendFactor::Enum factor)
    {
        rasterStateInfo.srcAlphaBlendFactor = factor;
        return *this;
    }

    GraphicsPipelineStateInfo &setBlendDstAlphaBlendFactor(BlendFactor::Enum factor)
    {
        rasterStateInfo.dstAlphaBlendFactor = factor;
        return *this;
    }

    GraphicsPipelineStateInfo &setBlendAlphaBlendOp(BlendOp::Enum op)
    {
        rasterStateInfo.alphaBlendOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setColorWriteMask(ColorComponentFlags mask)
    {
        rasterStateInfo.colorWriteMask = mask;
        return *this;
    }

    GraphicsPipelineStateInfo &setDepthTestEnable(bool enable)
    {
        rasterStateInfo.depthTestEnable = enable;
        return *this;
    }

    GraphicsPipelineStateInfo &setDepthWriteEnable(bool enable)
    {
        rasterStateInfo.depthWriteEnable = enable;
        return *this;
    }

    GraphicsPipelineStateInfo &setDepthCompareOp(CompareOp::Enum op)
    {
        rasterStateInfo.depthCompareOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setStencilTestEnable(bool enable)
    {
        rasterStateInfo.stencilTestEnable = enable;
        return *this;
    }

    GraphicsPipelineStateInfo &setStencilFailOpFront(StencilOp::Enum op)
    {
        paramValueInfo.frontStencilOp.failOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setStencilPassOpFront(StencilOp::Enum op)
    {
        paramValueInfo.frontStencilOp.passOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setStencilDepthFailOpFront(StencilOp::Enum op)
    {
        paramValueInfo.frontStencilOp.depthFailOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setStencilCompareOpFront(CompareOp::Enum op)
    {
        paramValueInfo.frontStencilOp.compareOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setStencilFailOpBack(StencilOp::Enum op)
    {
        paramValueInfo.backStencilOp.failOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setStencilPassOpBack(StencilOp::Enum op)
    {
        paramValueInfo.backStencilOp.passOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setStencilDepthFailOpBack(StencilOp::Enum op)
    {
        paramValueInfo.backStencilOp.depthFailOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setStencilCompareOpBack(CompareOp::Enum op)
    {
        paramValueInfo.backStencilOp.compareOp = op;
        return *this;
    }

    GraphicsPipelineStateInfo &setSampleShadingEnable(bool enable)
    {
        rasterStateInfo.sampleShadingEnable = enable;
        return *this;
    }

    GraphicsPipelineStateInfo &setConservativeEnable(bool enable)
    {
        rasterStateInfo.conservativeEnable = enable;
        return *this;
    }

    GraphicsPipelineStateInfo &setSampleAlphaToCoverageEnable(bool enable)
    {
        rasterStateInfo.alphaToCoverageEnable = enable;
        return *this;
    }

    GraphicsPipelineStateInfo &setSampleAlphaToOneEnable(bool enable)
    {
        rasterStateInfo.alphaToOneEnable = enable;
        return *this;
    }
};


struct VertexLayout
{
    VertexInputBindingInfo vertexInputBindingInfos[GFX_MAX_VERTEX_ATTRIBUTE_COUNT]{};
    VertexInputAttributeDescInfo vertexInputAttributeDescInfos[GFX_MAX_VERTEX_ATTRIBUTE_COUNT]{};

    VertexLayout &addVertexInputBinding(uint8_t binding,
                                        uint16_t stride,
                                        VertexInputRate::Enum inputRate)
    {
        vertexInputBindingInfos[binding].binding = binding;
        vertexInputBindingInfos[binding].stride = stride;
        vertexInputBindingInfos[binding].inputRate = inputRate;
        return *this;
    }

    VertexLayout &addVertexInputAttributeDesc(uint8_t binding,
                                              uint8_t location,
                                              VertexAttribType::Enum attrib,
                                              uint32_t offset,
                                              bool normalized = false)
    {
        vertexInputAttributeDescInfos[location].binding = binding;
        vertexInputAttributeDescInfos[location].location = location;
        vertexInputAttributeDescInfos[location].attrib = attrib;
        vertexInputAttributeDescInfos[location].normalized = normalized;
        vertexInputAttributeDescInfos[location].use = true;
        vertexInputAttributeDescInfos[location].offset = offset;
        return *this;
    }
};


/**
 * Buffer与Texture(Image)间拷贝的参数
 */
struct BufferImageCopyInfo
{
    uint64_t bufferOffset;
    uint32_t bufferRowLength;
    uint32_t bufferImageHeight;
    uint32_t mipLevel;
    uint32_t baseArrayLayer;
    uint32_t layerCount;
    int32_t imageOffsetX;
    int32_t imageOffsetY;
    int32_t imageOffsetZ;
    uint32_t imageWidth;
    uint32_t imageHeight;
    uint32_t imageDepth;
    TextureAspectFlags aspectMask;
};

/**
 * Texture(Image)间拷贝的参数
 */
struct ImageCopyInfo
{
    uint32_t srcMipLevel;
    uint32_t srcBaseArrayLayer;
    uint32_t srcLayerCount;
    int32_t srcOffsetX;
    int32_t srcOffsetY;
    int32_t srcOffsetZ;

    uint32_t dstMipLevel;
    uint32_t dstBaseArrayLayer;
    uint32_t dstLayerCount;
    int32_t dstOffsetX;
    int32_t dstOffsetY;
    int32_t dstOffsetZ;

    uint32_t imageWidth;
    uint32_t imageHeight;
    uint32_t imageDepth;

    TextureAspectFlags srcAspectMask;
    TextureAspectFlags dstAspectMask;
};

/**
 * 图像转换的参数
 */
struct ImageBlitInfo
{
    uint32_t srcMipLevel;
    uint32_t srcBaseArrayLayer;
    uint32_t srcLayerCount;
    int32_t srcOffsetX;
    int32_t srcOffsetY;
    int32_t srcOffsetZ;
    uint32_t srcWidth;
    uint32_t srcHeight;
    uint32_t srcDepth;

    uint32_t dstMipLevel;
    uint32_t dstBaseArrayLayer;
    uint32_t dstLayerCount;
    int32_t dstOffsetX;
    int32_t dstOffsetY;
    int32_t dstOffsetZ;
    uint32_t dstWidth;
    uint32_t dstHeight;
    uint32_t dstDepth;

    TextureAspectFlags srcAspectMask;
    TextureAspectFlags dstAspectMask;
};

/**
 * Buffer的Barrier同步配置
 */
struct BufferBarrierInfo
{
    PipelineStageMask srcStage;
    PipelineStageMask dstStage;

    BufferAccessMask srcAccess;
    BufferAccessMask dstAccess;

    QueueType::Enum srcQueue;
    QueueType::Enum dstQueue;
};

struct ImageSubResourceRange
{
    uint32_t baseMipLevel;
    uint32_t baseArrayLayer;
    uint32_t levelCount;
    uint32_t layerCount;
};

struct DrawIndirectCommand
{
    uint32_t vertexCount;
    uint32_t instanceCount;
    uint32_t firstVertex;
    uint32_t firstInstance;
};

struct DrawIndexedIndirectCommand
{
    uint32_t indexCount;
    uint32_t instanceCount;
    uint32_t firstIndex;
    int32_t vertexOffset;
    uint32_t firstInstance;
};

struct DispatchIndirectCommand
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
};

}

#endif //GX_GFX_DEF_H
