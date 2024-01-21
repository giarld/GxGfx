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

#include <gfx/gfx_core.h>
#include "gfx_instance.h"
#include "gfx_context.h"

#include <gx/debug.h>


namespace gfx
{

Instance createInstance(const CreateInstanceInfo &createInfo)
{
    auto *obj = GX_NEW(Instance_T);
    if (obj == GFX_NULL_HANDLE) {
        return GFX_NULL_HANDLE;
    }
    if (obj->init(createInfo)) {
        return obj;
    }
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void destroyInstance(Instance obj)
{
    auto *objT = dynamic_cast<Instance_T *>(obj);
    GX_ASSERT(objT);
    objT->destroy();
    GX_DELETE(objT);
}

Context createContext(Instance instance, const CreateContextInfo &createInfo)
{
    auto *obj = GX_NEW(Context_T);
    if (obj == GFX_NULL_HANDLE) {
        return GFX_NULL_HANDLE;
    }
    if (obj->init(instance, createInfo)) {
        return obj;
    }
    GX_DELETE(obj);
    return GFX_NULL_HANDLE;
}

void destroyContext(Context obj)
{
    auto *objT = dynamic_cast<Context_T *>(obj);
    GX_ASSERT(objT);
    objT->destroy();
    GX_DELETE(objT);
}

Fence createFence(Context context, bool signaled)
{
    auto *contextT = dynamic_cast<Context_T *>(context);
    GX_ASSERT(contextT);
    return contextT->createFence(signaled);
}

void destroyFence(Fence fence)
{
    auto *objP = dynamic_cast<ElementHandle*>(fence);
    GX_ASSERT(objP != nullptr);
    auto *contextT = dynamic_cast<Context_T *>(objP->context());
    GX_ASSERT(contextT);
    contextT->destroyFence(fence);
}

Frame createFrame(Context context, const CreateFrameInfo &createInfo)
{
    auto *contextT = dynamic_cast<Context_T *>(context);
    GX_ASSERT(contextT);
    return contextT->createFrame(createInfo);
}

void destroyFrame(Frame obj)
{
    auto *objP = dynamic_cast<ElementHandle*>(obj);
    GX_ASSERT(objP != nullptr);
    auto *contextT = dynamic_cast<Context_T *>(objP->context());
    GX_ASSERT(contextT);
    contextT->destroyFrame(obj);
}

RenderTarget createRenderTarget(Context context, const CreateRenderTargetInfo &createInfo)
{
    auto *contextT = dynamic_cast<Context_T *>(context);
    GX_ASSERT(contextT);
    return contextT->createRenderTarget(createInfo);
}

void destroyRenderTarget(RenderTarget obj)
{
    auto *objP = dynamic_cast<ElementHandle*>(obj);
    GX_ASSERT(objP != nullptr);
    auto *contextT = dynamic_cast<Context_T *>(objP->context());
    GX_ASSERT(contextT);
    contextT->destroyRenderTarget(obj);
}

Buffer createBuffer(Context context, const CreateBufferInfo &createInfo)
{
    auto *contextT = dynamic_cast<Context_T *>(context);
    GX_ASSERT(contextT);
    return contextT->createBuffer(createInfo);
}

void destroyBuffer(Buffer buffer)
{
    auto *objP = dynamic_cast<ElementHandle*>(buffer);
    GX_ASSERT(objP != nullptr);
    auto *contextT = dynamic_cast<Context_T *>(objP->context());
    GX_ASSERT(contextT);
    contextT->destroyBuffer(buffer);
}

Texture createTexture(Context context, const CreateTextureInfo &createInfo)
{
    auto *contextT = dynamic_cast<Context_T *>(context);
    GX_ASSERT(contextT);
    return contextT->createTexture(createInfo);
}

void destroyTexture(Texture texture)
{
    auto *objP = dynamic_cast<ElementHandle*>(texture);
    GX_ASSERT(objP != nullptr);
    auto *contextT = dynamic_cast<Context_T *>(objP->context());
    GX_ASSERT(contextT);
    contextT->destroyTexture(texture);
}

Sampler createSampler(Context context, const CreateSamplerInfo &createInfo)
{
    auto *contextT = dynamic_cast<Context_T *>(context);
    GX_ASSERT(contextT);
    return contextT->createSampler(createInfo);
}

void destroySampler(Sampler sampler)
{
    auto *objP = dynamic_cast<ElementHandle*>(sampler);
    GX_ASSERT(objP != nullptr);
    auto *contextT = dynamic_cast<Context_T *>(objP->context());
    GX_ASSERT(contextT);
    contextT->destroySampler(sampler);
}

Shader createShader(Context context, const CreateShaderInfo &createInfo)
{
    auto *contextT = dynamic_cast<Context_T *>(context);
    GX_ASSERT(contextT);
    return contextT->createShader(createInfo);
}

void destroyShader(Shader shader)
{
    auto *objP = dynamic_cast<ElementHandle*>(shader);
    GX_ASSERT(objP != nullptr);
    auto *contextT = dynamic_cast<Context_T *>(objP->context());
    GX_ASSERT(contextT);
    contextT->destroyShader(shader);
}

ResourceBinder createResourceBinder(Context context, const ResourceLayoutInfo &layoutInfo)
{
    auto *contextT = dynamic_cast<Context_T *>(context);
    GX_ASSERT(contextT);
    return contextT->createResourceBinder(layoutInfo);
}

void destroyResourceBinder(ResourceBinder binder)
{
    auto *objP = dynamic_cast<ElementHandle*>(binder);
    GX_ASSERT(objP != nullptr);
    auto *contextT = dynamic_cast<Context_T *>(objP->context());
    GX_ASSERT(contextT);
    contextT->destroyResourceBinder(binder);
}

CommandBuffer createCommandBuffer(Context context, const CreateCommandBufferInfo &createInfo)
{
    auto *contextT = dynamic_cast<Context_T *>(context);
    GX_ASSERT(contextT);
    return contextT->createCommandBuffer(createInfo);
}

void destroyCommandBuffer(CommandBuffer obj)
{
    auto *objP = dynamic_cast<ElementHandle*>(obj);
    GX_ASSERT(objP != nullptr);
    auto *contextT = dynamic_cast<Context_T *>(objP->context());
    GX_ASSERT(contextT);
    contextT->destroyCommandBuffer(obj);
}

Query createQuery(Context context, const CreateQueryInfo &createInfo)
{
    auto *contextT = dynamic_cast<Context_T *>(context);
    GX_ASSERT(contextT);
    return contextT->createQuery(createInfo);
}

void destroyQuery(Query obj)
{
    auto *objP = dynamic_cast<ElementHandle*>(obj);
    GX_ASSERT(objP != nullptr);
    auto *contextT = dynamic_cast<Context_T *>(objP->context());
    GX_ASSERT(contextT);
    contextT->destroyQuery(obj);
}

#include "gfx_private.h"

size_t getObjectHash(GfxObject *obj)
{
    auto *objP = dynamic_cast<ElementHandle*>(obj);
    GX_ASSERT(objP != nullptr);
    return objP->hash();
}

uint32_t getObjectIdx(GfxObject *obj)
{
    auto *objP = dynamic_cast<ElementHandle*>(obj);
    GX_ASSERT(objP != nullptr);
    return objP->idx();
}

bool objectEquals(GfxObject *a, GfxObject *b)
{
    auto *pA = dynamic_cast<ElementHandle*>(a);
    auto *pB = dynamic_cast<ElementHandle*>(b);

    if (pA == nullptr || pB == nullptr) {
        return false;
    }
    if (pA->context() != pB->context()) {
        return false;
    }
    return pA->equals(pB);
}

}
