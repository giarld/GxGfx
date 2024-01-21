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

#ifndef GX_GFX_STL_TEMPLATE_H
#define GX_GFX_STL_TEMPLATE_H

#include <gfx/gfx.h>
#include <gx/common.h>


namespace gfx
{

inline bool operator==(const Attachment &a, const Attachment &b)
{
    return a.texture == b.texture
           && a.mipLevel == b.mipLevel
           && a.layer == b.layer;
}

inline bool operator==(const CreateRenderTargetInfo &a, const CreateRenderTargetInfo &b)
{
    return a.colorAttachments == b.colorAttachments
           && a.depthStencilAttachment == b.depthStencilAttachment
           && a.sample == b.sample;
}

inline bool operator==(const SubPassDesc &a, const SubPassDesc &b)
{
    return a.useDepthStencil == b.useDepthStencil
           && a.colorAttachments == b.colorAttachments
           && a.inputAttachments == b.inputAttachments;
}

inline bool operator==(const SubPassDependency &a, const SubPassDependency &b)
{
    return a.srcSubPass == b.srcSubPass
           && a.dstSubPass == b.dstSubPass;
}

inline bool operator==(const SubPassInfo &a, const SubPassInfo &b)
{
    return a.subPassDescs == b.subPassDescs
           && a.subPassDepends == b.subPassDepends;
}

inline bool operator==(const VertexInputBindingInfo &a, const VertexInputBindingInfo &b)
{
    return gx::bitwiseEqual(a, b);
}

inline bool operator==(const VertexInputAttributeDescInfo &a, const VertexInputAttributeDescInfo &b)
{
    return gx::bitwiseEqual(a, b);
}

inline bool operator==(const PipelineRasterStateInfo &a, const PipelineRasterStateInfo &b)
{
    return a.t == b.t;
}

inline bool operator==(const PipelineParamValueInfo &a, const PipelineParamValueInfo &b)
{
    return gx::bitwiseEqual(a, b);
}

inline bool operator==(const GraphicsPipelineStateInfo &a, const GraphicsPipelineStateInfo &b)
{
    return a.rasterStateInfo == b.rasterStateInfo
           && a.paramValueInfo == b.paramValueInfo;
}

inline bool operator==(const VertexLayout &a, const VertexLayout &b)
{
    return gx::bitwiseEqual(a, b);
}

inline bool operator==(const ResourceLayoutBindingInfo &a, const ResourceLayoutBindingInfo &b)
{
    return gx::bitwiseEqual(a, b);
}

inline bool operator==(const ResourceLayoutInfo &a, const ResourceLayoutInfo &b)
{
    return a.bindingInfos == b.bindingInfos;
}

inline bool operator==(const CreateTextureInfo &a, const CreateTextureInfo &b)
{
    return gx::bitwiseEqual(a, b);
}

inline bool operator==(const CreateSamplerInfo &a, const CreateSamplerInfo &b)
{
    return a.t == b.t;
}

inline bool operator==(const TextureBindRange &a, const TextureBindRange &b)
{
    return gx::bitwiseEqual(a, b);
}

}


namespace std
{
//! hash functions

template<>
struct hash<gfx::Attachment>
{
    size_t operator()(const gfx::Attachment &type) const
    {
        size_t hash = 0;
        hash = gx::hashMerge(hash, gfx::getObjectHash(type.texture));
        hash = gx::hashOf(hash, type.mipLevel);
        hash = gx::hashOf(hash, type.layer);
        return hash;
    }
};

template<>
struct hash<gfx::CreateRenderTargetInfo>
{
    size_t operator()(const gfx::CreateRenderTargetInfo &type) const
    {
        size_t hash = 0;

        std::hash<gfx::Attachment> hashAttFunc;

        for (auto &i: type.colorAttachments) {
            for (auto &j: i) {
                hash = gx::hashMerge(hash, hashAttFunc(j));
            }
        }
        hash = type.depthStencilAttachment.texture ? gx::hashMerge(hash, hashAttFunc(type.depthStencilAttachment))
                                                   : hash;
        hash = gx::hashOf(hash, type.sample);
        return hash;
    }
};

template<>
struct hash<gfx::SubPassDesc>
{
    size_t operator()(const gfx::SubPassDesc &type) const
    {
        size_t hash = gx::hashOf(type.useDepthStencil);
        for (auto i : type.colorAttachments) {
            hash = gx::hashOf(hash, i);
        }
        for (auto i : type.inputAttachments) {
            hash = gx::hashOf(hash, i);
        }
        return hash;
    }
};

template<>
struct hash<gfx::SubPassDependency>
{
    size_t operator()(const gfx::SubPassDependency &type) const
    {
        size_t hash = gx::hashOf(type);
        return hash;
    }
};

template<>
struct hash<gfx::SubPassInfo>
{
    size_t operator()(const gfx::SubPassInfo &type) const
    {
        std::hash<gfx::SubPassDesc> descHashFunc;
        std::hash<gfx::SubPassDependency> dependHashFunc;

        size_t hash = 0;
        for (auto &i : type.subPassDescs) {
            hash = gx::hashMerge(hash, descHashFunc(i));
        }
        for (auto &i : type.subPassDepends) {
            hash = gx::hashMerge(hash, dependHashFunc(i));
        }

        return hash;
    }
};

template<>
struct hash<gfx::GraphicsPipelineStateInfo>
{
    size_t operator()(const gfx::GraphicsPipelineStateInfo &type) const
    {
        size_t hash = 0;
        hash = gx::hashOf(hash, type.rasterStateInfo.t);
        hash = gx::hashOf(hash, type.paramValueInfo);
        return hash;
    }
};

template<>
struct hash<gfx::VertexLayout>
{
    size_t operator()(const gfx::VertexLayout &type) const
    {
        size_t hash = gx::hashOf(type);
        return hash;
    }
};

template<>
struct hash<gfx::ResourceLayoutInfo>
{
    size_t operator()(const gfx::ResourceLayoutInfo &type) const
    {
        size_t hash = 0;

        for (auto &i: type.bindingInfos) {
            hash = gx::hashOf(hash, i);
        }

        return hash;
    }
};

template<>
struct hash<gfx::CreateTextureInfo>
{
    size_t operator()(const gfx::CreateTextureInfo &type) const
    {
        return gx::hashOf(type);
    }
};

template<>
struct hash<gfx::CreateSamplerInfo>
{
    size_t operator()(const gfx::CreateSamplerInfo &type) const
    {
        return gx::hashOf(type.t);
    }
};

template<>
struct hash<gfx::TextureBindRange>
{
    size_t operator()(const gfx::TextureBindRange &type) const
    {
        return gx::hashOf(type);
    }
};

}

#endif //GX_GFX_STL_TEMPLATE_H
