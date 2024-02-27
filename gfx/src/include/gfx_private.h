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

#ifndef GX_GFX_PRIVATE_H
#define GX_GFX_PRIVATE_H

#include <gfx/gfx_base.h>
#include <gx/gbytearray.h>

#include <gfx/gfx_stl_template.h>
#include <atomic>


using namespace gx;

namespace gfx
{

// 接口实现类
#define GFX_API_IMPL(CLASS_NAME) class CLASS_NAME##_T : public CLASS_NAME##_I

#define GFX_API_ELEMENT_CLASS(CLASS_NAME) class CLASS_NAME##_P : public CLASS_NAME##_I, public ElementHandle

// 元素接口包装
#define GFX_API_ELEMENT_PACK(CLASS_NAME)  GFX_API_ELEMENT_CLASS(CLASS_NAME) {   \
public:         \
    explicit CLASS_NAME##_P(GfxIdxTy idx) : ElementHandle(idx) {}   \
}

#define GFX_P_API_DEFINE(CLASS_NAME) struct CLASS_NAME##_P

#define GFX_P_API(CLASS_NAME) struct CLASS_NAME##_P : public GfxObject

#define GFX_P_API_IMPL(CLASS_NAME, P_TYPE) class CLASS_NAME##P_TYPE : public CLASS_NAME##_P


#define CMD_BUFFER_SIZE 65536

struct ElementType
{
    enum Enum : uint8_t
    {
        Fence = 0,
        Frame,
        RenderTarget,
        Buffer,
        Texture,
        Sampler,
        RenderPass,
        Shader,
        DescLayout,
        ResourceBinder,
        PipelineLayout,
        PipelineState,
        CommandBuffer,
        Query,

        Count
    };
};

constexpr const char *const ElementTypeNames[] = {
        "Fence",
        "Frame",
        "RenderTarget",
        "Buffer",
        "Texture",
        "Sampler",
        "RenderPass",
        "Shader",
        "DescLayout",
        "DescSetBinder",
        "PipelineLayout",
        "PipelineState",
        "CommandBuffer",
        "Query"
};

static_assert(ARRAY_LEN(ElementTypeNames) == ElementType::Count,
              "The number of ElementTypeNames does not match ElementType");

/**
 * 指令Key
 */
struct CommandKey
{
    enum Enum : uint8_t
    {
        None = 0,
        Begin,              // begin
        End,                // end
        SetClearColor,      // setClearColor
        SetClearDepSte,     // setClearDepthStencil
        BindRenderTarget,
        BeginRenderPass,
        EndRenderPass,
        SetGraphPipelineState,
        SetVertexLayout,
        SetShaders,
        NextSubpass,
        SetViewport,
        SetScissor,
        SetLineWidth,
        SetStencilCompMask,
        SetStencilWriteMask,
        SetStencilReference,
        BindDescSet,
        BindVertexBuf,
        BindIndexBuf,
        Draw,
        DrawIndexed,
        DrawIndirect,
        DrawIndexedIndirect,
        Dispatch,
        DispatchIndirect,

        PipelineBarrier,
        BufferBarrier,
        ImageBarrier,
        CopyBuffer,
        CopyImage,
        CopyBufferToImage,
        CopyImageToBuffer,
        BlitImage,
        CopyRT,
        BlitRT,
        BlitRTToImage,
        CopyRTToImage,
        CopyRTToBuffer,
        FillBuffer,

        BeginDebug,
        EndDebug,
        ResetQuery,
        BeginQuery,
        EndQuery,
        WriteTimestamp,
        CopyQueryResults,

        Count
    };
};

constexpr const char *const CommandKeyStr[] = {
        "None",
        "Begin",
        "End",
        "SetClearColor",
        "SetClearDepSte",
        "BindRenderTarget",
        "BeginRenderPass",
        "EndRenderPass",
        "SetGraphPipelineState",
        "SetVertexLayout",
        "SetShaders",
        "NextSubpass",
        "SetViewport",
        "SetScissor",
        "SetLineWidth",
        "SetStencilCompMask",
        "SetStencilWriteMask",
        "SetStencilReference",
        "BindDescSet",
        "BindVertexBuf",
        "BindIndexBuf",
        "Draw",
        "DrawIndexed",
        "DrawIndirect",
        "DrawIndexedIndirect",
        "Dispatch",
        "DispatchIndirect",

        "PipelineBarrier",
        "BufferBarrier",
        "ImageBarrier",
        "CopyBuffer",
        "CopyImage",
        "CopyBufferToImage",
        "CopyImageToBuffer",
        "BlitImage",
        "CopyRT",
        "BlitRT",
        "BlitRTToImage",
        "CopyRTToImage",
        "CopyRTToBuffer",
        "FillBuffer",

        "BeginDebug",
        "EndDebug",
        "ResetQuery",
        "BeginQuery",
        "EndQuery",
        "WriteTimestamp",
        "CopyQueryResults",
};

static_assert(ARRAY_LEN(CommandKeyStr) == CommandKey::Count,
              "The number of CommandKeyStr does not match CommandKey");


class Context_T;

typedef uint32_t GfxIdxTy;

class ElementHandle
{
public:
    explicit ElementHandle(GfxIdxTy idx) : mIdx(idx)
    {};

    virtual ~ElementHandle() = default;

    GFX_API_FUNC(void destroy());

    GFX_API_FUNC(Context_T *context());

    GfxIdxTy idx() const
    { return mIdx; }

    virtual size_t hash()
    { return mIdx; }

    virtual bool equals(ElementHandle *b)
    {
        return this == b;
    }

private:
    const GfxIdxTy mIdx = 0;
};


#define GFX_MAX_FENCES 256
#define GFX_MAX_FRAMES 256
#define GFX_MAX_RENDER_TARGETS 8192
#define GFX_MAX_BUFFERS 32768
#define GFX_MAX_TEXTURES 32768
#define GFX_MAX_SAMPLERS 32768
#define GFX_MAX_RENDER_PASSES 8192
#define GFX_MAX_SHADERS 32768
#define GFX_MAX_DESC_LAYOUTS 32768
#define GFX_MAX_DESC_SET_BINDERS 32768
#define GFX_MAX_PIPELINE_LAYOUTS 32768
#define GFX_MAX_PIPELINE_STATES 32768
#define GFX_MAX_COMMAND_BUFFERS 32768
#define GFX_MAX_QUERY 256

#define GFX_OBJ_ID_MASK 0x0000ffffu
#define GFX_CTX_ID_MASK 0x00ff0000u
#define GFX_TYPE_ID_MASK 0xff000000u

#define GFX_CTX_ID_SHIFT 16
#define GFX_TYPE_ID_SHIFT 28

inline uint32_t genElementIdx(uint16_t cIdx, uint16_t oIdx, uint8_t tIdx)
{
    return 0 | (tIdx << GFX_TYPE_ID_SHIFT) | (cIdx << GFX_CTX_ID_SHIFT) | oIdx;
}

inline uint16_t getElementObjectIdx(uint32_t elementIdx)
{
    return (uint16_t)(elementIdx & GFX_OBJ_ID_MASK);
}

inline uint16_t getElementContextIdx(uint32_t elementIdx)
{
    return (uint16_t)((elementIdx & GFX_CTX_ID_MASK) >> GFX_CTX_ID_SHIFT);
}

inline uint8_t getElementTypeIdx(uint32_t elementIdx)
{
    return (uint8_t)((elementIdx & GFX_TYPE_ID_MASK) >> GFX_TYPE_ID_SHIFT);
}


/// ============ Serialization and deserialization ============ ///

static void writeSubPassInfo(GByteArray &buffer, const SubPassInfo &info)
{
    // subPassDescs
    uint32_t vSize = info.subPassDescs.size();
    buffer.write(vSize);
    if (vSize > 0) {
        for (auto &desc : info.subPassDescs) {
            buffer.write(desc.useDepthStencil);
            uint32_t size = desc.colorAttachments.size();
            buffer.write(size);
            if (size > 0) {
                buffer.write(desc.colorAttachments.data(), desc.colorAttachments.size() * sizeof(uint32_t));
            }
            size = desc.inputAttachments.size();
            buffer.write(size);
            if (size > 0) {
                buffer.write(desc.inputAttachments.data(), desc.inputAttachments.size() * sizeof(uint32_t));
            }
        }
    }

    // subPassDepends
    vSize = info.subPassDepends.size();
    buffer.write(vSize);
    if (vSize > 0) {
        buffer.write(info.subPassDepends.data(), vSize * sizeof(SubPassDependency));
    }
}

static void readSubPassInfo(GByteArray &buffer, SubPassInfo &info)
{
    // subPassDescs
    uint32_t vSize;
    buffer.read(vSize);
    info.subPassDescs.clear();
    if (vSize > 0) {
        info.subPassDescs.resize(vSize);
        for (auto &desc : info.subPassDescs) {
            buffer.read(desc.useDepthStencil);
            uint32_t size;
            buffer.read(size);
            if (size > 0) {
                desc.colorAttachments.resize(size);
                buffer.read(desc.colorAttachments.data(), desc.colorAttachments.size() * sizeof(uint32_t));
            }

            buffer.read(size);
            if (size > 0) {
                desc.inputAttachments.resize(size);
                buffer.read(desc.inputAttachments.data(), desc.inputAttachments.size() * sizeof(uint32_t));
            }
        }
    }

    // subPassDepends
    buffer.read(vSize);
    info.subPassDepends.clear();
    if (vSize > 0) {
        info.subPassDepends.resize(vSize);
        buffer.read(info.subPassDepends.data(), vSize * sizeof(SubPassDependency));
    }
}

static void writeRenderPassInfo(GByteArray &buffer, const RenderPassInfo &info)
{
    buffer.write(info.clear);
    buffer.write(info.discard);
    writeSubPassInfo(buffer, info.subPassInfo);
}

static void readRenderPassInfo(GByteArray &buffer, RenderPassInfo &info)
{
    buffer.read(info.clear);
    buffer.read(info.discard);
    readSubPassInfo(buffer, info.subPassInfo);
}

}

#endif //GX_GFX_PRIVATE_H
