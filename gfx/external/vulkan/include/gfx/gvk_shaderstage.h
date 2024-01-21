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

#ifndef GFX_GVK_SHADERSTAGE_H
#define GFX_GVK_SHADERSTAGE_H

#include <gfx/compiler.h>
#include <gfx/vulkan.h>

#include <gx/gstring.h>


namespace gfx
{

class GVkDevice;

class GX_API GVkShaderStage
{
CLASS_DEF(GVkShaderStage)

public:
    void create(GVkDevice *device, VkShaderStageFlagBits shaderStage, const std::string &path);

    void create(GVkDevice *device, VkShaderStageFlagBits shaderStage, const void *code, size_t codeSize);

    void destroy();

public:
    VkPipelineShaderStageCreateInfo &vkPipelineShaderStageCreateInfo();

    VkShaderModule &vkShaderModule();

    VkShaderStageFlagBits shaderStage();

private:
    GVkDevice *mDevice = nullptr;

    VkPipelineShaderStageCreateInfo mPipelineShaderStageCreateInfo {};
    VkShaderModule mHandle = VK_NULL_HANDLE;
};

class GX_API GVkShaderStageBind
{
CLASS_DEF(GVkShaderStageBind)

public:
    void addShaderStage(GVkShaderStage &shaderStage);

    std::vector<VkPipelineShaderStageCreateInfo> &ShaderStageCreateInfos();

    void bindToGraphicsPipelineCreateInfo(VkGraphicsPipelineCreateInfo &info);

    void clear();

private:
    std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;
};

}

#endif //GFX_GVK_SHADERSTAGE_H
