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

#include "gfx/gvk_instance.h"

#include "gfx/gvk_tools.h"
#include "gfx/gvk_debug.h"
#include <gx/debug.h>

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <Metal/Metal.h>


using namespace gx;

namespace gfx
{

VkSurfaceKHR GVkInstance::createSurfaceForWindow(const GVkPlatformData &platformData) const
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    VkResult err = VK_SUCCESS;

    void *platformWindow = platformData.nativeWindowHandle;

    NSWindow* window = (NSWindow*)(platformWindow);
    NSView* contentView = (NSView*)window.contentView;
    CAMetalLayer* layer = [CAMetalLayer layer];
    [contentView setWantsLayer : YES] ;
    [contentView setLayer : layer] ;

    VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo{};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    surfaceCreateInfo.pNext = NULL;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.pView = (void*)layer;
    err = vkCreateMacOSSurfaceMVK(mVkInstance, &surfaceCreateInfo, NULL, &surface);

    GX_ASSERT_S(err == VK_SUCCESS, "Could not create surface! %s", vks::tools::errorString(err).c_str());
    if (err != VK_SUCCESS) {
        Log("Could not create surface! %s", vks::tools::errorString(err).c_str());
    }
    return surface;
}

}


