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

#ifndef GX_GFX_DEF_VK_H
#define GX_GFX_DEF_VK_H

#include <gfx/gfx_stl_template.h>


namespace gfx
{

struct AttachmentDesc
{
    union
    {
        struct
        {
            Format::Enum format: 8;
            bool clear: 4;
            bool discard: 4;
            ImageLayout::Enum initLayout: 4;
            ImageLayout::Enum finalLayout: 4;
            ImageLayout::Enum subPassLayout: 8;
        };
        uint32_t t = 0;     // 32 bit
    };
};

struct GetRenderPassInfo
{
    std::vector<AttachmentDesc> colorAttachmentDescs;
    AttachmentDesc depthAttachmentDesc;
    std::vector<SubPassDesc> subPassDescs;                          // subPass结构信息
    std::vector<SubPassDependency> subPassDepends;
    SampleCountFlag::Enum sample = SampleCountFlag::SampleCount_1;  // 采样率，非SampleCount_1则表示启用MSAA
    bool isSwapChain;
};

struct CreateBufferViewInfo
{
    Format::Enum format;
    uint64_t offset;
    uint64_t size;
};

struct CreateImageViewInfo
{
    TextureBindRange range{};
    bool isAttachment = false;
};

struct PipelineLayoutInfo
{
    std::vector<ResourceLayoutInfo> layoutInfos;
};

struct StencilMaskReferenceInfo
{
    uint32_t compareMask = UINT32_MAX;
    uint32_t writeMask = UINT32_MAX;
    uint32_t reference = 0;
};

struct CreateGraphicsPipelineStateInfo
{
    GraphicsPipelineStateInfo stateInfo;
    VertexLayout vertexLayout;
    uint64_t renderPass;
    std::vector<Shader> shaderPrograms;
    uint64_t pipelineLayout;
    uint32_t subpassIndex;
    StencilMaskReferenceInfo frontMR;
    StencilMaskReferenceInfo backMR;
};

struct QueryGraphicsPipelineStateInfo
{
    GraphicsPipelineStateInfo stateInfo;
    VertexLayout vertexLayout;
    uint64_t renderPass{};
    std::vector<uint64_t> shaderPrograms;
    uint64_t pipelineLayout{};
    uint32_t subpassIndex{};
    StencilMaskReferenceInfo frontMR;
    StencilMaskReferenceInfo backMR;
};

struct CreateComputePipelineStateInfo
{
    std::vector<Shader> shaderPrograms;
    uint64_t pipelineLayout;
};

struct QueryComputePipelineStateInfo
{
    std::vector<uint64_t> shaderPrograms;
    uint64_t pipelineLayout{};
};

static QueryGraphicsPipelineStateInfo createQueryGraphicsPipelineStateInfo(
        const CreateGraphicsPipelineStateInfo &createInfo)
{
    QueryGraphicsPipelineStateInfo queryInfo;
    queryInfo.stateInfo = createInfo.stateInfo;
    queryInfo.vertexLayout = createInfo.vertexLayout;
    queryInfo.renderPass = createInfo.renderPass;
    queryInfo.shaderPrograms.reserve(createInfo.shaderPrograms.size());
    for (auto s : createInfo.shaderPrograms) {
        queryInfo.shaderPrograms.push_back(getObjectIdx(s));
    }
    queryInfo.pipelineLayout = createInfo.pipelineLayout;
    queryInfo.subpassIndex = createInfo.subpassIndex;
    queryInfo.frontMR = createInfo.frontMR;
    queryInfo.backMR = createInfo.backMR;
    return queryInfo;
}

static QueryComputePipelineStateInfo createQueryComputePipelineStateInfo(
        const CreateComputePipelineStateInfo &createInfo)
{
    QueryComputePipelineStateInfo queryInfo;
    queryInfo.pipelineLayout = createInfo.pipelineLayout;
    queryInfo.shaderPrograms.reserve(createInfo.shaderPrograms.size());
    for (auto s : createInfo.shaderPrograms) {
        queryInfo.shaderPrograms.push_back(getObjectIdx(s));
    }
    return queryInfo;
}


inline bool operator==(const AttachmentDesc &a, const AttachmentDesc &b)
{
    return a.t == b.t;
}

inline bool operator==(const GetRenderPassInfo &a, const GetRenderPassInfo &b)
{
    return a.colorAttachmentDescs == b.colorAttachmentDescs
           && a.depthAttachmentDesc == b.depthAttachmentDesc
           && a.subPassDescs == b.subPassDescs
           && a.subPassDepends == b.subPassDepends
           && a.sample == b.sample;
}

inline bool operator==(const CreateBufferViewInfo &a, const CreateBufferViewInfo &b)
{
    return a.format == b.format
           && a.offset == b.format
           && a.size == b.size;
}

inline bool operator==(const CreateImageViewInfo &a, const CreateImageViewInfo &b)
{
    return a.range == b.range && a.isAttachment == b.isAttachment;
}

inline bool operator==(const StencilMaskReferenceInfo &a, const StencilMaskReferenceInfo &b)
{
    return gx::bitwiseEqual(a, b);
}

inline bool operator==(const PipelineLayoutInfo &a, const PipelineLayoutInfo &b)
{
    return a.layoutInfos == b.layoutInfos;
}

inline bool operator==(const QueryGraphicsPipelineStateInfo &a, const QueryGraphicsPipelineStateInfo &b)
{
    return a.stateInfo == b.stateInfo
           && a.vertexLayout == b.vertexLayout
           && a.shaderPrograms == b.shaderPrograms
           && a.renderPass == b.renderPass
           && a.pipelineLayout == b.pipelineLayout
           && a.subpassIndex == b.subpassIndex
           && a.frontMR == b.frontMR
           && a.backMR == b.backMR;
}

inline bool operator==(const QueryComputePipelineStateInfo &a, const QueryComputePipelineStateInfo &b)
{
    return a.shaderPrograms == b.shaderPrograms
           && a.pipelineLayout == b.pipelineLayout;
}

}


namespace std
{
template<>
struct hash<gfx::GetRenderPassInfo>
{
    size_t operator()(const gfx::GetRenderPassInfo &type) const
    {
        size_t hash = 0;
        for (auto &ci : type.colorAttachmentDescs) {
            hash = gx::hashOf(hash, ci.t);
        }
        hash = gx::hashOf(hash, type.depthAttachmentDesc.t);
        for (auto &si : type.subPassDescs) {
            hash = gx::hashOf(hash, si.useDepthStencil);
            for (auto &i : si.colorAttachments) {
                hash = gx::hashOf(hash, i);
            }
            for (auto &i : si.inputAttachments) {
                hash = gx::hashOf(hash, i);
            }
        }
        for (auto &si : type.subPassDepends) {
            hash = gx::hashOf(hash, si);
        }
        hash = gx::hashOf(hash, type.sample);
        return hash;
    }
};

template<>
struct hash<gfx::CreateBufferViewInfo>
{
    size_t operator()(const gfx::CreateBufferViewInfo &type) const
    {
        return gx::hashOf(type);
    }
};

template<>
struct hash<gfx::PipelineLayoutInfo>
{
    size_t operator()(const gfx::PipelineLayoutInfo &type) const
    {
        size_t hash = 0;

        std::hash<gfx::ResourceLayoutInfo> hashV;

        for (auto &i : type.layoutInfos) {
            hash = gx::hashMerge(hash, hashV(i));
        }

        return hash;
    }
};

template<>
struct hash<gfx::QueryGraphicsPipelineStateInfo>
{
    size_t operator()(const gfx::QueryGraphicsPipelineStateInfo &type) const
    {
        std::hash<gfx::GraphicsPipelineStateInfo> hashFS;
        std::hash<gfx::VertexLayout> hashFV;
        size_t hash = gx::hashOf(0x0A);
        hash = gx::hashMerge(hash, hashFS(type.stateInfo));
        hash = gx::hashMerge(hash, hashFV(type.vertexLayout));
        for (auto &i : type.shaderPrograms) {
            hash = gx::hashOf(hash, i);
        }
        hash = gx::hashOf(hash, type.renderPass);
        hash = gx::hashOf(hash, type.pipelineLayout);
        hash = gx::hashOf(hash, type.subpassIndex);
        hash = gx::hashOf(hash, type.frontMR);
        hash = gx::hashOf(hash, type.backMR);

        return hash;
    }
};

template<>
struct hash<gfx::QueryComputePipelineStateInfo>
{
    size_t operator()(const gfx::QueryComputePipelineStateInfo &type) const
    {
        size_t hash = gx::hashOf(0x0B);
        for (auto &i : type.shaderPrograms) {
            hash = gx::hashOf(hash, i);
        }
        hash = gx::hashOf(hash, type.pipelineLayout);
        return hash;
    }
};

template<>
struct hash<gfx::CreateImageViewInfo>
{
    size_t operator()(const gfx::CreateImageViewInfo &type) const
    {
        size_t hash = gx::hashOf(type.range);
        hash = gx::hashOf(hash, type.isAttachment);
        return hash;
    }
};

}

#endif //GX_GFX_DEF_VK_H
