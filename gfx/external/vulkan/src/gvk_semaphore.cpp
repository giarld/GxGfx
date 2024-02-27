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

#include "gfx/gvk_semaphore.h"

#include <gfx/gvk_tools.h>


namespace gfx
{

void GVkSemaphore::create(VkDevice device)
{
    if (isCreated()) {
        return;
    }

    mDevice = device;
    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VK_CHECK_RESULT(vkCreateSemaphore(device, &info, nullptr, &mHandle));
}

void GVkSemaphore::destroy()
{
    if (mHandle != VK_NULL_HANDLE) {
        vkDestroySemaphore(mDevice, mHandle, nullptr);
    }
    mHandle = VK_NULL_HANDLE;
}

bool GVkSemaphore::isCreated() const
{
    return mHandle != VK_NULL_HANDLE;
}

GVkSemaphore::operator VkSemaphore()
{
    return mHandle;
}

VkSemaphore GVkSemaphore::vkSemaphore()
{
    return mHandle;
}

}