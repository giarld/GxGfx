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

#ifndef GX_GFX_INSTANCE_H
#define GX_GFX_INSTANCE_H

#include <gfx/gfx_core.h>
#include "gfx_private.h"
#include "gfx_p.h"


namespace gfx
{

GFX_API_IMPL(Instance)
{
public:
    bool init(const CreateInstanceInfo &createInfo);

    void destroy();

public:
    uint32_t deviceCount() override;

    DeviceInfo deviceInfo(uint32_t deviceIndex) override;

    TargetApiType::Enum targetApiType() override;

public:
    Instance_P *instanceP();

private:
    static bool checkInstanceCreateInfo(const char *appName, TargetApiType::Enum targetApiType);

private:
    TargetApiType::Enum mTargetApiType = TargetApiType::Count;

    Instance_P *mHandleP = GFX_NULL_HANDLE;
};

}

#endif //GX_GFX_INSTANCE_H
