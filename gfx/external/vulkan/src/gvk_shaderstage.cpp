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

#include "gfx/gvk_shaderstage.h"

#include <gfx/gvk_tools.h>
#include <gfx/gvk_device.h>


using namespace gx;

namespace gfx
{

void GVkShaderStage::create(GVkDevice *device, VkShaderStageFlagBits shaderStage, const std::string &path)
{
    mDevice = device;

    mHandle = vks::tools::loadShader(path.c_str(), *mDevice);
    GX_ASSERT(mHandle != VK_NULL_HANDLE);

    mPipelineShaderStageCreateInfo = {};
    mPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    mPipelineShaderStageCreateInfo.stage = shaderStage;
    mPipelineShaderStageCreateInfo.module = mHandle;
    mPipelineShaderStageCreateInfo.pName = "main";
}

void GVkShaderStage::create(GVkDevice *device, VkShaderStageFlagBits shaderStage, const void *code, size_t codeSize)
{
    mDevice = device;

    VkShaderModuleCreateInfo moduleCreateInfo{};
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.codeSize = codeSize;
    moduleCreateInfo.pCode = (uint32_t*)code;

    VK_CHECK_RESULT(vkCreateShaderModule(*mDevice, &moduleCreateInfo, nullptr, &mHandle));

    mPipelineShaderStageCreateInfo = {};
    mPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    mPipelineShaderStageCreateInfo.stage = shaderStage;
    mPipelineShaderStageCreateInfo.module = mHandle;
    mPipelineShaderStageCreateInfo.pName = "main";
}

void GVkShaderStage::destroy()
{
    if (mHandle != VK_NULL_HANDLE) {
        vkDestroyShaderModule(*mDevice, mHandle, nullptr);
    }
    mHandle = VK_NULL_HANDLE;
}

VkPipelineShaderStageCreateInfo &GVkShaderStage::vkPipelineShaderStageCreateInfo()
{
    return mPipelineShaderStageCreateInfo;
}

VkShaderModule &GVkShaderStage::vkShaderModule()
{
    return mHandle;
}

VkShaderStageFlagBits GVkShaderStage::shaderStage()
{
    return mPipelineShaderStageCreateInfo.stage;
}

// ============== GVkShaderStageBind ============== //

void GVkShaderStageBind::addShaderStage(GVkShaderStage &shaderStage)
{
    mShaderStages.push_back(shaderStage.vkPipelineShaderStageCreateInfo());
}

std::vector<VkPipelineShaderStageCreateInfo> &GVkShaderStageBind::ShaderStageCreateInfos()
{
    return mShaderStages;
}

void GVkShaderStageBind::bindToGraphicsPipelineCreateInfo(VkGraphicsPipelineCreateInfo &info)
{
    info.stageCount = static_cast<uint32_t>(mShaderStages.size());
    info.pStages = mShaderStages.data();
}

void GVkShaderStageBind::clear()
{
    mShaderStages.clear();
}

}
