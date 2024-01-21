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

#include "gfx/gvk_fence.h"

#include <gfx/gvk_device.h>
#include <gfx/gvk_tools.h>


namespace gfx
{

void GVkFence::create(GVkDevice *device, VkFlags flags)
{
    mDevice = device;
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = flags;
    VK_CHECK_RESULT(vkCreateFence(*device, &fenceCreateInfo, nullptr, &mHandle));
}

void GVkFence::destroy()
{
    if (mHandle != VK_NULL_HANDLE) {
        vkDestroyFence(*mDevice, mHandle, nullptr);
    }
    mHandle = VK_NULL_HANDLE;
}

GVkFence::operator VkFence()
{
    return mHandle;
}

VkFence GVkFence::vkFence()
{
    return mHandle;
}

bool GVkFence::isSignaled()
{
    return vkGetFenceStatus(*mDevice, mHandle) == VK_SUCCESS;
}

VkResult GVkFence::wait(uint64_t timeout)
{
    return vkWaitForFences(*mDevice, 1, &mHandle, VK_TRUE, timeout);
}

void GVkFence::reset()
{
    VK_CHECK_RESULT(vkResetFences(*mDevice, 1, &mHandle));
}

}