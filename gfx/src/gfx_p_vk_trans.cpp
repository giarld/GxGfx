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

#include <gfx/vulkan.h>
#include <gfx/gfx_def.h>

#include <gx/debug.h>


namespace gfx
{
namespace vk
{

VkFormat toVkFormat(Format::Enum format)
{
    switch (format) {
        case Format::Count:
        case Format::Undefined:
            return VK_FORMAT_UNDEFINED;
        case Format::R8_UNorm:
            return VK_FORMAT_R8_UNORM;
        case Format::R8_SNorm:
            return VK_FORMAT_R8_SNORM;
        case Format::R8_UInt:
            return VK_FORMAT_R8_UINT;
        case Format::R8_SInt:
            return VK_FORMAT_R8_SINT;
        case Format::R8_SRgb:
            return VK_FORMAT_R8_SRGB;
        case Format::R8G8_UNorm:
            return VK_FORMAT_R8G8_UNORM;
        case Format::R8G8_SNorm:
            return VK_FORMAT_R8G8_SNORM;
        case Format::R8G8_UInt:
            return VK_FORMAT_R8G8_UINT;
        case Format::R8G8_SInt:
            return VK_FORMAT_R8G8_SINT;
        case Format::R8G8_SRgb:
            return VK_FORMAT_R8G8_SRGB;
        case Format::R8G8B8_UNorm:
            return VK_FORMAT_R8G8B8_UNORM;
        case Format::R8G8B8_SNorm:
            return VK_FORMAT_R8G8B8_SNORM;
        case Format::R8G8B8_UInt:
            return VK_FORMAT_R8G8B8_UINT;
        case Format::R8G8B8_SInt:
            return VK_FORMAT_R8G8B8_SINT;
        case Format::R8G8B8_SRgb:
            return VK_FORMAT_R8G8B8_SRGB;
        case Format::B8G8R8_UNorm:
            return VK_FORMAT_B8G8R8_UNORM;
        case Format::B8G8R8_SNorm:
            return VK_FORMAT_B8G8R8_SNORM;
        case Format::B8G8R8_UInt:
            return VK_FORMAT_B8G8R8_UINT;
        case Format::B8G8R8_SInt:
            return VK_FORMAT_B8G8R8_SINT;
        case Format::B8G8R8_SRgb:
            return VK_FORMAT_B8G8R8_SRGB;
        case Format::R8G8B8A8_UNorm:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case Format::R8G8B8A8_SNorm:
            return VK_FORMAT_R8G8B8A8_SNORM;
        case Format::R8G8B8A8_UInt:
            return VK_FORMAT_R8G8B8A8_UINT;
        case Format::R8G8B8A8_SInt:
            return VK_FORMAT_R8G8B8A8_SINT;
        case Format::R8G8B8A8_SRgb:
            return VK_FORMAT_R8G8B8A8_SRGB;
        case Format::B8G8R8A8_UNorm:
            return VK_FORMAT_B8G8R8A8_UNORM;
        case Format::B8G8R8A8_SNorm:
            return VK_FORMAT_B8G8R8A8_SNORM;
        case Format::B8G8R8A8_UInt:
            return VK_FORMAT_B8G8R8A8_UINT;
        case Format::B8G8R8A8_SInt:
            return VK_FORMAT_B8G8R8A8_SINT;
        case Format::B8G8R8A8_SRgb:
            return VK_FORMAT_B8G8R8A8_SRGB;
        case Format::A8B8G8R8_UNorm:
            return VK_FORMAT_A8B8G8R8_UNORM_PACK32;
        case Format::A8B8G8R8_SNorm:
            return VK_FORMAT_A8B8G8R8_SNORM_PACK32;
        case Format::A8B8G8R8_UInt:
            return VK_FORMAT_A8B8G8R8_UINT_PACK32;
        case Format::A8B8G8R8_SInt:
            return VK_FORMAT_A8B8G8R8_SINT_PACK32;
        case Format::A8B8G8R8_SRgb:
            return VK_FORMAT_A8B8G8R8_SRGB_PACK32;
        case Format::R5G6B5_UNorm:
            return VK_FORMAT_R5G6B5_UNORM_PACK16;
        case Format::B5G6R5_UNorm:
            return VK_FORMAT_B5G6R5_UNORM_PACK16;
        case Format::R5G5B5A1_UNorm:
            return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
        case Format::B5G5R5A1_UNorm:
            return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
        case Format::A1R5G5B5_UNorm:
            return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
        case Format::R4G4B4A4_UNorm:
            return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
        case Format::A2R10G10B10_UNorm:
            return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
        case Format::A2R10G10B10_SNorm:
            return VK_FORMAT_A2R10G10B10_SNORM_PACK32;
        case Format::A2R10G10B10_UInt:
            return VK_FORMAT_A2R10G10B10_UINT_PACK32;
        case Format::A2R10G10B10_SInt:
            return VK_FORMAT_A2R10G10B10_SINT_PACK32;
        case Format::D16_UNorm:
            return VK_FORMAT_D16_UNORM;
        case Format::X8_D24_UNorm:
            return VK_FORMAT_X8_D24_UNORM_PACK32;
        case Format::D32_SFloat:
            return VK_FORMAT_D32_SFLOAT;
        case Format::S8_UInt:
            return VK_FORMAT_S8_UINT;
        case Format::D16_UNorm_S8_UInt:
            return VK_FORMAT_D16_UNORM_S8_UINT;
        case Format::D24_UNorm_S8_UInt:
            return VK_FORMAT_D24_UNORM_S8_UINT;
        case Format::D32_SFloat_S8_UInt:
            return VK_FORMAT_D32_SFLOAT_S8_UINT;
        case Format::R16_UInt:
            return VK_FORMAT_R16_UINT;
        case Format::R16_SInt:
            return VK_FORMAT_R16_SINT;
        case Format::R16_UNorm:
            return VK_FORMAT_R16_UNORM;
        case Format::R16_SNorm:
            return VK_FORMAT_R16_SNORM;
        case Format::R16_SFloat:
            return VK_FORMAT_R16_SFLOAT;
        case Format::R16G16_UInt:
            return VK_FORMAT_R16G16_UINT;
        case Format::R16G16_SInt:
            return VK_FORMAT_R16G16_SINT;
        case Format::R16G16_UNorm:
            return VK_FORMAT_R16G16_UNORM;
        case Format::R16G16_SNorm:
            return VK_FORMAT_R16G16_SNORM;
        case Format::R16G16_SFloat:
            return VK_FORMAT_R16G16_SFLOAT;
        case Format::R16G16B16_UInt:
            return VK_FORMAT_R16G16B16_UINT;
        case Format::R16G16B16_SInt:
            return VK_FORMAT_R16G16B16_SINT;
        case Format::R16G16B16_UNorm:
            return VK_FORMAT_R16G16B16_UNORM;
        case Format::R16G16B16_SNorm:
            return VK_FORMAT_R16G16B16_SNORM;
        case Format::R16G16B16_SFloat:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case Format::R16G16B16A16_UInt:
            return VK_FORMAT_R16G16B16A16_UINT;
        case Format::R16G16B16A16_SInt:
            return VK_FORMAT_R16G16B16A16_SINT;
        case Format::R16G16B16A16_UNorm:
            return VK_FORMAT_R16G16B16A16_UNORM;
        case Format::R16G16B16A16_SNorm:
            return VK_FORMAT_R16G16B16A16_SNORM;
        case Format::R16G16B16A16_SFloat:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        case Format::R32_UInt:
            return VK_FORMAT_R32_UINT;
        case Format::R32_SInt:
            return VK_FORMAT_R32_SINT;
        case Format::R32_SFloat:
            return VK_FORMAT_R32_SFLOAT;
        case Format::R32G32_UInt:
            return VK_FORMAT_R32G32_UINT;
        case Format::R32G32_SInt:
            return VK_FORMAT_R32G32_SINT;
        case Format::R32G32_SFloat:
            return VK_FORMAT_R32G32_SFLOAT;
        case Format::R32G32B32_UInt:
            return VK_FORMAT_R32G32B32_UINT;
        case Format::R32G32B32_SInt:
            return VK_FORMAT_R32G32B32_SINT;
        case Format::R32G32B32_SFloat:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case Format::R32G32B32A32_UInt:
            return VK_FORMAT_R32G32B32A32_UINT;
        case Format::R32G32B32A32_SInt:
            return VK_FORMAT_R32G32B32A32_SINT;
        case Format::R32G32B32A32_SFloat:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case Format::R64_UInt:
            return VK_FORMAT_R64_UINT;
        case Format::R64_SInt:
            return VK_FORMAT_R64_SINT;
        case Format::R64_SFloat:
            return VK_FORMAT_R64_SFLOAT;
        case Format::R64G64_UInt:
            return VK_FORMAT_R64G64_UINT;
        case Format::R64G64_SInt:
            return VK_FORMAT_R64G64_SINT;
        case Format::R64G64_SFloat:
            return VK_FORMAT_R64G64_SFLOAT;
        case Format::R64G64B64_UInt:
            return VK_FORMAT_R64G64B64_UINT;
        case Format::R64G64B64_SInt:
            return VK_FORMAT_R64G64B64_SINT;
        case Format::R64G64B64_SFloat:
            return VK_FORMAT_R64G64B64_SFLOAT;
        case Format::R64G64B64A64_UInt:
            return VK_FORMAT_R64G64B64A64_UINT;
        case Format::R64G64B64A64_SInt:
            return VK_FORMAT_R64G64B64A64_SINT;
        case Format::R64G64B64A64_SFloat:
            return VK_FORMAT_R64G64B64A64_SFLOAT;
        case Format::B10G11R11_UFloat:
            return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
        case Format::E5B9G9R9_UFloat:
            return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
        case Format::A2B10G10R10_UNorm:
            return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        case Format::A2B10G10R10_SNorm:
            return VK_FORMAT_A2B10G10R10_SNORM_PACK32;
        case Format::A2B10G10R10_UInt:
            return VK_FORMAT_A2B10G10R10_UINT_PACK32;
        case Format::A2B10G10R10_SInt:
            return VK_FORMAT_A2B10G10R10_SINT_PACK32;
        case Format::BC1_RGB_UNorm:
            return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
        case Format::BC1_RGB_SRgb:
            return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
        case Format::BC1_RGBA_UNorm:
            return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
        case Format::BC1_RGBA_SRgb:
            return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
        case Format::BC2_UNorm:
            return VK_FORMAT_BC2_UNORM_BLOCK;
        case Format::BC2_SRgb:
            return VK_FORMAT_BC2_SRGB_BLOCK;
        case Format::BC3_UNorm:
            return VK_FORMAT_BC3_UNORM_BLOCK;
        case Format::BC3_SRgb:
            return VK_FORMAT_BC3_SRGB_BLOCK;
        case Format::BC4_UNorm:
            return VK_FORMAT_BC4_UNORM_BLOCK;
        case Format::BC4_SNorm:
            return VK_FORMAT_BC4_SNORM_BLOCK;
        case Format::BC5_UNorm:
            return VK_FORMAT_BC5_UNORM_BLOCK;
        case Format::BC5_SNorm:
            return VK_FORMAT_BC5_SNORM_BLOCK;
        case Format::BC6H_UFloat:
            return VK_FORMAT_BC6H_UFLOAT_BLOCK;
        case Format::BC6H_SFloat:
            return VK_FORMAT_BC6H_SFLOAT_BLOCK;
        case Format::BC7_UNorm:
            return VK_FORMAT_BC7_UNORM_BLOCK;
        case Format::BC7_SRgb:
            return VK_FORMAT_BC7_SRGB_BLOCK;
        case Format::ETC2_R8G8B8_UNorm:
            return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
        case Format::ETC2_R8G8B8_SRgb:
            return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
        case Format::ETC2_R8G8B8A1_UNorm:
            return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
        case Format::ETC2_R8G8B8A1_SRgb:
            return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
        case Format::ETC2_R8G8B8A8_UNorm:
            return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
        case Format::ETC2_R8G8B8A8_SRgb:
            return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
        case Format::EAC_R11_UNorm:
            return VK_FORMAT_EAC_R11_UNORM_BLOCK;
        case Format::EAC_R11_SNorm:
            return VK_FORMAT_EAC_R11_SNORM_BLOCK;
        case Format::EAC_R11G11_UNorm:
            return VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
        case Format::EAC_R11G11_SNorm:
            return VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
        case Format::ASTC_4x4_UNorm:
            return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
        case Format::ASTC_4x4_SRgb:
            return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
        case Format::ASTC_5x4_UNorm:
            return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
        case Format::ASTC_5x4_SRgb:
            return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
        case Format::ASTC_5x5_UNorm:
            return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
        case Format::ASTC_5x5_SRgb:
            return VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
        case Format::ASTC_6x5_UNorm:
            return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
        case Format::ASTC_6x5_SRgb:
            return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
        case Format::ASTC_6x6_UNorm:
            return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
        case Format::ASTC_6x6_SRgb:
            return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
        case Format::ASTC_8x5_UNorm:
            return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
        case Format::ASTC_8x5_SRgb:
            return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
        case Format::ASTC_8x6_UNorm:
            return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
        case Format::ASTC_8x6_SRgb:
            return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
        case Format::ASTC_8x8_UNorm:
            return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
        case Format::ASTC_8x8_SRgb:
            return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
        case Format::ASTC_10x5_UNorm:
            return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
        case Format::ASTC_10x5_SRgb:
            return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
        case Format::ASTC_10x6_UNorm:
            return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
        case Format::ASTC_10x6_SRgb:
            return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
        case Format::ASTC_10x8_UNorm:
            return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
        case Format::ASTC_10x8_SRgb:
            return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
        case Format::ASTC_10x10_UNorm:
            return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
        case Format::ASTC_10x10_SRgb:
            return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
        case Format::ASTC_12x10_UNorm:
            return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
        case Format::ASTC_12x10_SRgb:
            return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
        case Format::ASTC_12x12_UNorm:
            return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
        case Format::ASTC_12x12_SRgb:
            return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
    }
}

Format::Enum fromVkFormat(VkFormat vkFormat)
{
    switch (vkFormat) {
        default:
        case VK_FORMAT_UNDEFINED:
            return Format::Undefined;
        case VK_FORMAT_R8_UNORM:
            return Format::R8_UNorm;
        case VK_FORMAT_R8_SNORM:
            return Format::R8_SNorm;
        case VK_FORMAT_R8_UINT:
            return Format::R8_UInt;
        case VK_FORMAT_R8_SINT:
            return Format::R8_SInt;
        case VK_FORMAT_R8_SRGB:
            return Format::R8_SRgb;
        case VK_FORMAT_R8G8_UNORM:
            return Format::R8G8_UNorm;
        case VK_FORMAT_R8G8_SNORM:
            return Format::R8G8_SNorm;
        case VK_FORMAT_R8G8_UINT:
            return Format::R8G8_UInt;
        case VK_FORMAT_R8G8_SINT:
            return Format::R8G8_SInt;
        case VK_FORMAT_R8G8_SRGB:
            return Format::R8G8_SRgb;
        case VK_FORMAT_R8G8B8_UNORM:
            return Format::R8G8B8_UNorm;
        case VK_FORMAT_R8G8B8_SNORM:
            return Format::R8G8B8_SNorm;
        case VK_FORMAT_R8G8B8_UINT:
            return Format::R8G8B8_UInt;
        case VK_FORMAT_R8G8B8_SINT:
            return Format::R8G8B8_SInt;
        case VK_FORMAT_R8G8B8_SRGB:
            return Format::R8G8B8_SRgb;
        case VK_FORMAT_B8G8R8_UNORM:
            return Format::B8G8R8_UNorm;
        case VK_FORMAT_B8G8R8_SNORM:
            return Format::B8G8R8_SNorm;
        case VK_FORMAT_B8G8R8_UINT:
            return Format::B8G8R8_UInt;
        case VK_FORMAT_B8G8R8_SINT:
            return Format::B8G8R8_SInt;
        case VK_FORMAT_B8G8R8_SRGB:
            return Format::B8G8R8_SRgb;
        case VK_FORMAT_R8G8B8A8_UNORM:
            return Format::R8G8B8A8_UNorm;
        case VK_FORMAT_R8G8B8A8_SNORM:
            return Format::R8G8B8A8_SNorm;
        case VK_FORMAT_R8G8B8A8_UINT:
            return Format::R8G8B8A8_UInt;
        case VK_FORMAT_R8G8B8A8_SINT:
            return Format::R8G8B8A8_SInt;
        case VK_FORMAT_R8G8B8A8_SRGB:
            return Format::R8G8B8A8_SRgb;
        case VK_FORMAT_B8G8R8A8_UNORM:
            return Format::B8G8R8A8_UNorm;
        case VK_FORMAT_B8G8R8A8_SNORM:
            return Format::B8G8R8A8_SNorm;
        case VK_FORMAT_B8G8R8A8_UINT:
            return Format::B8G8R8A8_UInt;
        case VK_FORMAT_B8G8R8A8_SINT:
            return Format::B8G8R8A8_SInt;
        case VK_FORMAT_B8G8R8A8_SRGB:
            return Format::B8G8R8A8_SRgb;
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
            return Format::A8B8G8R8_UNorm;
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
            return Format::A8B8G8R8_SNorm;
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
            return Format::A8B8G8R8_UInt;
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
            return Format::A8B8G8R8_SInt;
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            return Format::A8B8G8R8_SRgb;
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
            return Format::R5G6B5_UNorm;
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
            return Format::B5G6R5_UNorm;
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
            return Format::R5G5B5A1_UNorm;
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
            return Format::B5G5R5A1_UNorm;
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
            return Format::A1R5G5B5_UNorm;
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
            return Format::R4G4B4A4_UNorm;
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            return Format::A2R10G10B10_UNorm;
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
            return Format::A2R10G10B10_SNorm;
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
            return Format::A2R10G10B10_UInt;
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
            return Format::A2R10G10B10_SInt;
        case VK_FORMAT_D16_UNORM:
            return Format::D16_UNorm;
        case VK_FORMAT_X8_D24_UNORM_PACK32:
            return Format::X8_D24_UNorm;
        case VK_FORMAT_D32_SFLOAT:
            return Format::D32_SFloat;
        case VK_FORMAT_S8_UINT:
            return Format::S8_UInt;
        case VK_FORMAT_D16_UNORM_S8_UINT:
            return Format::D16_UNorm_S8_UInt;
        case VK_FORMAT_D24_UNORM_S8_UINT:
            return Format::D24_UNorm_S8_UInt;
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return Format::D32_SFloat_S8_UInt;
        case VK_FORMAT_R16_UINT:
            return Format::R16_UInt;
        case VK_FORMAT_R16_SINT:
            return Format::R16_SInt;
        case VK_FORMAT_R16_UNORM:
            return Format::R16_UNorm;
        case VK_FORMAT_R16_SNORM:
            return Format::R16_SNorm;
        case VK_FORMAT_R16_SFLOAT:
            return Format::R16_SFloat;
        case VK_FORMAT_R16G16_UINT:
            return Format::R16G16_UInt;
        case VK_FORMAT_R16G16_SINT:
            return Format::R16G16_SInt;
        case VK_FORMAT_R16G16_UNORM:
            return Format::R16G16_UNorm;
        case VK_FORMAT_R16G16_SNORM:
            return Format::R16G16_SNorm;
        case VK_FORMAT_R16G16_SFLOAT:
            return Format::R16G16_SFloat;
        case VK_FORMAT_R16G16B16_UINT:
            return Format::R16G16B16_UInt;
        case VK_FORMAT_R16G16B16_SINT:
            return Format::R16G16B16_SInt;
        case VK_FORMAT_R16G16B16_UNORM:
            return Format::R16G16B16_UNorm;
        case VK_FORMAT_R16G16B16_SNORM:
            return Format::R16G16B16_SNorm;
        case VK_FORMAT_R16G16B16_SFLOAT:
            return Format::R16G16B16_SFloat;
        case VK_FORMAT_R16G16B16A16_UINT:
            return Format::R16G16B16A16_UInt;
        case VK_FORMAT_R16G16B16A16_SINT:
            return Format::R16G16B16A16_SInt;
        case VK_FORMAT_R16G16B16A16_UNORM:
            return Format::R16G16B16A16_UNorm;
        case VK_FORMAT_R16G16B16A16_SNORM:
            return Format::R16G16B16A16_SNorm;
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return Format::R16G16B16A16_SFloat;
        case VK_FORMAT_R32_UINT:
            return Format::R32_UInt;
        case VK_FORMAT_R32_SINT:
            return Format::R32_SInt;
        case VK_FORMAT_R32_SFLOAT:
            return Format::R32_SFloat;
        case VK_FORMAT_R32G32_UINT:
            return Format::R32G32_UInt;
        case VK_FORMAT_R32G32_SINT:
            return Format::R32G32_SInt;
        case VK_FORMAT_R32G32_SFLOAT:
            return Format::R32G32_SFloat;
        case VK_FORMAT_R32G32B32_UINT:
            return Format::R32G32B32_UInt;
        case VK_FORMAT_R32G32B32_SINT:
            return Format::R32G32B32_SInt;
        case VK_FORMAT_R32G32B32_SFLOAT:
            return Format::R32G32B32_SFloat;
        case VK_FORMAT_R32G32B32A32_UINT:
            return Format::R32G32B32A32_UInt;
        case VK_FORMAT_R32G32B32A32_SINT:
            return Format::R32G32B32A32_SInt;
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return Format::R32G32B32A32_SFloat;
        case VK_FORMAT_R64_UINT:
            return Format::R64_UInt;
        case VK_FORMAT_R64_SINT:
            return Format::R64_SInt;
        case VK_FORMAT_R64_SFLOAT:
            return Format::R64_SFloat;
        case VK_FORMAT_R64G64_UINT:
            return Format::R64G64_UInt;
        case VK_FORMAT_R64G64_SINT:
            return Format::R64G64_SInt;
        case VK_FORMAT_R64G64_SFLOAT:
            return Format::R64G64_SFloat;
        case VK_FORMAT_R64G64B64_UINT:
            return Format::R64G64B64_UInt;
        case VK_FORMAT_R64G64B64_SINT:
            return Format::R64G64B64_SInt;
        case VK_FORMAT_R64G64B64_SFLOAT:
            return Format::R64G64B64_SFloat;
        case VK_FORMAT_R64G64B64A64_UINT:
            return Format::R64G64B64A64_UInt;
        case VK_FORMAT_R64G64B64A64_SINT:
            return Format::R64G64B64A64_SInt;
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            return Format::R64G64B64A64_SFloat;
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
            return Format::B10G11R11_UFloat;
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
            return Format::E5B9G9R9_UFloat;
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
            return Format::A2B10G10R10_UNorm;
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
            return Format::A2B10G10R10_SNorm;
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
            return Format::A2B10G10R10_UInt;
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
            return Format::A2B10G10R10_SInt;
        case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
            return Format::BC1_RGB_UNorm;
        case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
            return Format::BC1_RGB_SRgb;
        case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
            return Format::BC1_RGBA_UNorm;
        case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
            return Format::BC1_RGBA_SRgb;
        case VK_FORMAT_BC2_UNORM_BLOCK:
            return Format::BC2_UNorm;
        case VK_FORMAT_BC2_SRGB_BLOCK:
            return Format::BC2_SRgb;
        case VK_FORMAT_BC3_UNORM_BLOCK:
            return Format::BC3_UNorm;
        case VK_FORMAT_BC3_SRGB_BLOCK:
            return Format::BC3_SRgb;
        case VK_FORMAT_BC4_UNORM_BLOCK:
            return Format::BC4_UNorm;
        case VK_FORMAT_BC4_SNORM_BLOCK:
            return Format::BC4_SNorm;
        case VK_FORMAT_BC5_UNORM_BLOCK:
            return Format::BC5_UNorm;
        case VK_FORMAT_BC5_SNORM_BLOCK:
            return Format::BC5_SNorm;
        case VK_FORMAT_BC6H_UFLOAT_BLOCK:
            return Format::BC6H_UFloat;
        case VK_FORMAT_BC6H_SFLOAT_BLOCK:
            return Format::BC6H_SFloat;
        case VK_FORMAT_BC7_UNORM_BLOCK:
            return Format::BC7_UNorm;
        case VK_FORMAT_BC7_SRGB_BLOCK:
            return Format::BC7_SRgb;
        case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
            return Format::ETC2_R8G8B8_UNorm;
        case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
            return Format::ETC2_R8G8B8_SRgb;
        case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
            return Format::ETC2_R8G8B8A1_UNorm;
        case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
            return Format::ETC2_R8G8B8A1_SRgb;
        case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
            return Format::ETC2_R8G8B8A8_UNorm;
        case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
            return Format::ETC2_R8G8B8A8_SRgb;
        case VK_FORMAT_EAC_R11_UNORM_BLOCK:
            return Format::EAC_R11_UNorm;
        case VK_FORMAT_EAC_R11_SNORM_BLOCK:
            return Format::EAC_R11_SNorm;
        case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
            return Format::EAC_R11G11_UNorm;
        case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
            return Format::EAC_R11G11_SNorm;
        case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
            return Format::ASTC_4x4_UNorm;
        case VK_FORMAT_ASTC_4x4_SRGB_BLOCK :
            return Format::ASTC_4x4_SRgb;
        case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
            return Format::ASTC_5x4_UNorm;
        case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
            return Format::ASTC_5x4_SRgb;
        case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
            return Format::ASTC_5x5_UNorm;
        case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
            return Format::ASTC_5x5_SRgb;
        case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
            return Format::ASTC_6x5_UNorm;
        case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
            return Format::ASTC_6x5_SRgb;
        case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
            return Format::ASTC_6x6_UNorm;
        case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
            return Format::ASTC_6x6_SRgb;
        case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
            return Format::ASTC_8x5_UNorm;
        case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
            return Format::ASTC_8x5_SRgb;
        case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
            return Format::ASTC_8x6_UNorm;
        case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
            return Format::ASTC_8x6_SRgb;
        case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
            return Format::ASTC_8x8_UNorm;
        case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
            return Format::ASTC_8x8_SRgb;
        case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
            return Format::ASTC_10x5_UNorm;
        case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
            return Format::ASTC_10x5_SRgb;
        case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
            return Format::ASTC_10x6_UNorm;
        case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
            return Format::ASTC_10x6_SRgb;
        case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
            return Format::ASTC_10x8_UNorm;
        case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
            return Format::ASTC_10x8_SRgb;
        case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
            return Format::ASTC_10x10_UNorm;
        case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
            return Format::ASTC_10x10_SRgb;
        case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
            return Format::ASTC_12x10_UNorm;
        case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
            return Format::ASTC_12x10_SRgb;
        case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
            return Format::ASTC_12x12_UNorm;
        case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
            return Format::ASTC_12x12_SRgb;
    }
}

VkFormatFeatureFlags toVkFormatFeatureFlags(FormatFeatureFlags flags)
{
    VkFormatFeatureFlags vkFlags = 0;
    if ((flags & FormatFeatureFlag::SampledImage) == FormatFeatureFlag::SampledImage) {
        vkFlags |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
    }
    if ((flags & FormatFeatureFlag::StorageImage) == FormatFeatureFlag::StorageImage) {
        vkFlags |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
    }
    if ((flags & FormatFeatureFlag::StorageImageAtomic) == FormatFeatureFlag::StorageImageAtomic) {
        vkFlags |= VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT;
    }
    if ((flags & FormatFeatureFlag::UniformTexelBuffer) == FormatFeatureFlag::UniformTexelBuffer) {
        vkFlags |= VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT;
    }
    if ((flags & FormatFeatureFlag::StorageTexelBuffer) == FormatFeatureFlag::StorageTexelBuffer) {
        vkFlags |= VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT;
    }
    if ((flags & FormatFeatureFlag::StorageTexelBufferAtomic) == FormatFeatureFlag::StorageTexelBufferAtomic) {
        vkFlags |= VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT;
    }
    if ((flags & FormatFeatureFlag::ColorAttachment) == FormatFeatureFlag::ColorAttachment) {
        vkFlags |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
    }
    if ((flags & FormatFeatureFlag::ColorAttachmentBlend) == FormatFeatureFlag::ColorAttachmentBlend) {
        vkFlags |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT;
    }
    if ((flags & FormatFeatureFlag::DepthStencilAttachment) == FormatFeatureFlag::DepthStencilAttachment) {
        vkFlags |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    if ((flags & FormatFeatureFlag::BlitSrc) == FormatFeatureFlag::BlitSrc) {
        vkFlags |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
    }
    if ((flags & FormatFeatureFlag::BlitDst) == FormatFeatureFlag::BlitDst) {
        vkFlags |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
    }
    if ((flags & FormatFeatureFlag::TransferSrc) == FormatFeatureFlag::TransferSrc) {
        vkFlags |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
    }
    if ((flags & FormatFeatureFlag::TransferDst) == FormatFeatureFlag::TransferDst) {
        vkFlags |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
    }

    return vkFlags;
}

VkSampleCountFlagBits toVkSampleCount(SampleCountFlag::Enum sample)
{
    switch (sample) {
        case SampleCountFlag::SampleCount_1:
            return VK_SAMPLE_COUNT_1_BIT;
        case SampleCountFlag::SampleCount_2:
            return VK_SAMPLE_COUNT_2_BIT;
        case SampleCountFlag::SampleCount_4:
            return VK_SAMPLE_COUNT_4_BIT;
        case SampleCountFlag::SampleCount_8:
            return VK_SAMPLE_COUNT_8_BIT;
        case SampleCountFlag::SampleCount_16:
            return VK_SAMPLE_COUNT_16_BIT;
        case SampleCountFlag::SampleCount_32:
            return VK_SAMPLE_COUNT_32_BIT;
        case SampleCountFlag::SampleCount_64:
            return VK_SAMPLE_COUNT_64_BIT;
    }
}

VkImageLayout toVkImageLayout(ImageLayout::Enum layout)
{
    switch (layout) {
        case ImageLayout::Undefined:
            return VK_IMAGE_LAYOUT_UNDEFINED;
        case ImageLayout::GraphicsGeneral:
        case ImageLayout::ComputeGeneral:
            return VK_IMAGE_LAYOUT_GENERAL;
        case ImageLayout::ColorAttachment:
            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case ImageLayout::DepthStencilAttachment:
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        case ImageLayout::DepthStencilReadOnly:
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        case ImageLayout::ShaderReadOnly:
            return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        case ImageLayout::TransferSrc:
            return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        case ImageLayout::TransferDst:
            return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        case ImageLayout::PresentSrc:
            return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        case ImageLayout::Count:
            return VK_IMAGE_LAYOUT_MAX_ENUM;
    }
}

VkImageAspectFlagBits toVkImageAspectFlagBits(TextureAspect::Enum aspect)
{
    GX_ASSERT(aspect != TextureAspect::AspectNone);
    switch (aspect) {
        default:
        case TextureAspect::AspectColor:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        case TextureAspect::AspectDepth:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        case TextureAspect::AspectStencil:
            return VK_IMAGE_ASPECT_STENCIL_BIT;
    }
}

VkImageAspectFlags toVkAspectFlags(TextureAspectFlags aspect)
{
    VkImageAspectFlags ret = 0;
    if (aspect & TextureAspect::AspectColor) {
        ret |= VK_IMAGE_ASPECT_COLOR_BIT;
    }
    if (aspect & TextureAspect::AspectDepth) {
        ret |= VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    if (aspect & TextureAspect::AspectStencil) {
        ret |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    return ret;
}

VkFilter toVkFilter(TextureSamplerFilter::Enum filter)
{
    switch (filter) {
        case TextureSamplerFilter::Nearest:
            return VK_FILTER_NEAREST;
        case TextureSamplerFilter::Linear:
            return VK_FILTER_LINEAR;
        default:
            return VK_FILTER_MAX_ENUM;
    }
}

VkFilter toVkFilter(BlitFilter::Enum filter)
{
    switch (filter) {
        case BlitFilter::Nearest:
            return VK_FILTER_NEAREST;
        case BlitFilter::Linear:
            return VK_FILTER_LINEAR;
        default:
            return VK_FILTER_MAX_ENUM;
    }
}

VkSamplerMipmapMode toVkSamplerMipmapMode(TextureSamplerMipmapMode::Enum mipmapMode)
{
    switch (mipmapMode) {
        case TextureSamplerMipmapMode::Nearest:
            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case TextureSamplerMipmapMode::Linear:
            return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        default:
            return VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;
    }
}

VkSamplerAddressMode toVkSamplerAddressMode(TextureSamplerAddressMode::Enum addressMode)
{
    switch (addressMode) {
        case TextureSamplerAddressMode::Repeat:
            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case TextureSamplerAddressMode::MirroredRepeat:
            return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case TextureSamplerAddressMode::ClampToEdge:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//        case TextureSamplerAddressMode::ClampToBorder:
//            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        default:
            return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
    }
}

VkCompareOp toVkCompareOp(CompareOp::Enum op)
{
    switch (op) {
        case CompareOp::Never:
            return VK_COMPARE_OP_NEVER;
        case CompareOp::Less:
            return VK_COMPARE_OP_LESS;
        case CompareOp::Equal:
            return VK_COMPARE_OP_EQUAL;
        case CompareOp::LessOrEqual:
            return VK_COMPARE_OP_LESS_OR_EQUAL;
        case CompareOp::Greater:
            return VK_COMPARE_OP_GREATER;
        case CompareOp::NotEqual:
            return VK_COMPARE_OP_NOT_EQUAL;
        case CompareOp::GreaterOrEqual:
            return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case CompareOp::Always:
            return VK_COMPARE_OP_ALWAYS;
        default:
            return VK_COMPARE_OP_MAX_ENUM;
    }
}

VkVertexInputRate toVkVertexInputRate(VertexInputRate::Enum rate)
{
    switch (rate) {
        case VertexInputRate::Vertex:
            return VK_VERTEX_INPUT_RATE_VERTEX;
        case VertexInputRate::Instance:
            return VK_VERTEX_INPUT_RATE_INSTANCE;
        default:
            return VK_VERTEX_INPUT_RATE_MAX_ENUM;
    }
}

VkDescriptorType toVkDescriptorType(ResourceType::Enum type)
{
    switch (type) {
        case ResourceType::Sampler:
            return VK_DESCRIPTOR_TYPE_SAMPLER;
        case ResourceType::CombinedImageSampler:
            return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        case ResourceType::SamplerImage:
            return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case ResourceType::StorageImage:
            return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case ResourceType::UniformTexelBuffer:
            return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        case ResourceType::StorageTexelBuffer:
            return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        case ResourceType::UniformBuffer:
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case ResourceType::StorageBuffer:
            return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case ResourceType::UniformBufferDynamic:
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        case ResourceType::StorageBufferDynamic:
            return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        case ResourceType::InputAttachment:
            return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        default:
            return VK_DESCRIPTOR_TYPE_MAX_ENUM;
    }
}

VkPipelineBindPoint toVkPipelineBindPoint(ResourceBindPoint::Enum type)
{
    switch (type) {
        case ResourceBindPoint::Graphics:
            return VK_PIPELINE_BIND_POINT_GRAPHICS;
        case ResourceBindPoint::Compute:
            return VK_PIPELINE_BIND_POINT_COMPUTE;
    }
}

VkShaderStageFlagBits toVkShaderStageFlagBits(ShaderType::Enum type)
{
    switch (type) {
        case ShaderType::Vertex:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderType::TessellationControl:
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case ShaderType::TessellationEvaluation:
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case ShaderType::Geometry:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
        case ShaderType::Fragment:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case ShaderType::Compute:
            return VK_SHADER_STAGE_COMPUTE_BIT;
        case ShaderType::AllGraphics:
            return VK_SHADER_STAGE_ALL_GRAPHICS;
        case ShaderType::All:
            return VK_SHADER_STAGE_ALL;
        default:
            return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
}

VkShaderStageFlags toVkShaderStageFlags(ShaderTypeFlags flags)
{
    VkShaderStageFlags vkFlags = 0;
    if ((flags & ShaderType::Vertex) == ShaderType::Vertex) {
        vkFlags |= VK_SHADER_STAGE_VERTEX_BIT;
    }
    if ((flags & ShaderType::TessellationControl) == ShaderType::TessellationControl) {
        vkFlags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    }
    if ((flags & ShaderType::TessellationEvaluation) == ShaderType::TessellationEvaluation) {
        vkFlags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    }
    if ((flags & ShaderType::Geometry) == ShaderType::Geometry) {
        vkFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    if ((flags & ShaderType::Fragment) == ShaderType::Fragment) {
        vkFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    if ((flags & ShaderType::Compute) == ShaderType::Compute) {
        vkFlags |= VK_SHADER_STAGE_COMPUTE_BIT;
    }
    return vkFlags;
}

VkPolygonMode toVkPolygonMode(PolygonMode::Enum mode)
{
    switch (mode) {
        case PolygonMode::Fill:
            return VK_POLYGON_MODE_FILL;
        case PolygonMode::Line:
            return VK_POLYGON_MODE_LINE;
        case PolygonMode::Point:
            return VK_POLYGON_MODE_POINT;
        default:
            return VK_POLYGON_MODE_MAX_ENUM;
    }
}

VkCullModeFlags toVkCullModeFlags(CullMode::Enum mode)
{
    switch (mode) {
        case CullMode::None:
            return VK_CULL_MODE_NONE;
        case CullMode::Front:
            return VK_CULL_MODE_FRONT_BIT;
        case CullMode::Back:
            return VK_CULL_MODE_BACK_BIT;
        case CullMode::FrontAndBack:
            return VK_CULL_MODE_FRONT_AND_BACK;
        default:
            return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
    }
}

VkFrontFace toVkFrontFace(FrontFace::Enum frontFace)
{
    switch (frontFace) {
        case FrontFace::CounterClockwise:
            return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        case FrontFace::Clockwise:
            return VK_FRONT_FACE_CLOCKWISE;
        default:
            return VK_FRONT_FACE_MAX_ENUM;
    }
}

VkPrimitiveTopology toVkPrimitiveTopology(PrimitiveTopology::Enum topology)
{
    switch (topology) {
        case PrimitiveTopology::PointList:
            return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case PrimitiveTopology::LineList:
            return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        case PrimitiveTopology::LineStrip:
            return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        case PrimitiveTopology::TriangleList:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case PrimitiveTopology::TriangleStrip:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        case PrimitiveTopology::TriangleFan:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        case PrimitiveTopology::LineListWithAdjacency:
            return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
        case PrimitiveTopology::LineStripWithAdjacency:
            return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
        case PrimitiveTopology::TriangleListWithAdjacency:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
        case PrimitiveTopology::TriangleStripWithAdjacency:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
        case PrimitiveTopology::PatchList:
            return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
        default:
            return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
    }
}

VkBlendFactor toVkBlendFactor(BlendFactor::Enum factor)
{
    switch (factor) {
        case BlendFactor::Zero:
            return VK_BLEND_FACTOR_ZERO;
        case BlendFactor::One:
            return VK_BLEND_FACTOR_ONE;
        case BlendFactor::SrcColor:
            return VK_BLEND_FACTOR_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DstColor:
            return VK_BLEND_FACTOR_DST_COLOR;
        case BlendFactor::OneMinusDstColor:
            return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case BlendFactor::SrcAlpha:
            return VK_BLEND_FACTOR_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha:
            return VK_BLEND_FACTOR_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha:
            return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        case BlendFactor::SrcAlphaSaturate:
            return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
        case BlendFactor::Src1Color:
            return VK_BLEND_FACTOR_SRC1_COLOR;
        case BlendFactor::OneMinusSrc1Color:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
        case BlendFactor::Src1Alpha:
            return VK_BLEND_FACTOR_SRC1_ALPHA;
        case BlendFactor::OneMinusSrc1Alpha:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
        default:
            return VK_BLEND_FACTOR_MAX_ENUM;
    }
}

VkBlendOp toVkBlendOp(BlendOp::Enum op)
{
    switch (op) {
        case BlendOp::Add:
            return VK_BLEND_OP_ADD;
        case BlendOp::Subtract:
            return VK_BLEND_OP_SUBTRACT;
        case BlendOp::ReverseSubtract:
            return VK_BLEND_OP_REVERSE_SUBTRACT;
        case BlendOp::Min:
            return VK_BLEND_OP_MIN;
        case BlendOp::Max:
            return VK_BLEND_OP_MAX;
        default:
            return VK_BLEND_OP_MAX_ENUM;
    }
}

VkColorComponentFlags toVkColorComponentFlags(ColorComponentFlags flags)
{
    VkColorComponentFlags vkFlags = 0;

    if ((flags & ColorComponentFlag::ComponentR) == ColorComponentFlag::ComponentR) {
        vkFlags |= VK_COLOR_COMPONENT_R_BIT;
    }
    if ((flags & ColorComponentFlag::ComponentG) == ColorComponentFlag::ComponentG) {
        vkFlags |= VK_COLOR_COMPONENT_G_BIT;
    }
    if ((flags & ColorComponentFlag::ComponentB) == ColorComponentFlag::ComponentB) {
        vkFlags |= VK_COLOR_COMPONENT_B_BIT;
    }
    if ((flags & ColorComponentFlag::ComponentA) == ColorComponentFlag::ComponentA) {
        vkFlags |= VK_COLOR_COMPONENT_A_BIT;
    }

    return vkFlags;
}

VkLogicOp toVkLogicOp(LogicOp::Enum op)
{
    switch (op) {
        case LogicOp::Clear:
            return VK_LOGIC_OP_CLEAR;
        case LogicOp::And:
            return VK_LOGIC_OP_AND;
        case LogicOp::AndReverse:
            return VK_LOGIC_OP_AND_REVERSE;
        case LogicOp::Copy:
            return VK_LOGIC_OP_COPY;
        case LogicOp::AndInverted:
            return VK_LOGIC_OP_AND_INVERTED;
        case LogicOp::NoOp:
            return VK_LOGIC_OP_NO_OP;
        case LogicOp::Xor:
            return VK_LOGIC_OP_XOR;
        case LogicOp::Or:
            return VK_LOGIC_OP_OR;
        case LogicOp::NOr:
            return VK_LOGIC_OP_NOR;
        case LogicOp::Equivalent:
            return VK_LOGIC_OP_EQUIVALENT;
        case LogicOp::Invert:
            return VK_LOGIC_OP_INVERT;
        case LogicOp::OrReverse:
            return VK_LOGIC_OP_OR_REVERSE;
        case LogicOp::CopyInverted:
            return VK_LOGIC_OP_COPY_INVERTED;
        case LogicOp::OrInverted:
            return VK_LOGIC_OP_OR_INVERTED;
        case LogicOp::NAnd:
            return VK_LOGIC_OP_NAND;
        case LogicOp::Set:
            return VK_LOGIC_OP_SET;
        default:
            return VK_LOGIC_OP_MAX_ENUM;
    }
}

VkStencilOp toVkStencilOp(StencilOp::Enum op)
{
    switch (op) {
        case StencilOp::Keep:
            return VK_STENCIL_OP_KEEP;
        case StencilOp::Zero:
            return VK_STENCIL_OP_ZERO;
        case StencilOp::Replace:
            return VK_STENCIL_OP_REPLACE;
        case StencilOp::IncrementAndClamp:
            return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
        case StencilOp::DecrementAndClamp:
            return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
        case StencilOp::Invert:
            return VK_STENCIL_OP_INVERT;
        case StencilOp::IncrementAndWrap:
            return VK_STENCIL_OP_INCREMENT_AND_WRAP;
        case StencilOp::DecrementAndWrap:
            return VK_STENCIL_OP_DECREMENT_AND_WRAP;
        default:
            return VK_STENCIL_OP_MAX_ENUM;
    }
}

VkStencilOpState toVkStencilOpState(const StencilOpState &opState,
                                    uint32_t compareMask, uint32_t writeMask, uint32_t reference)
{
    VkStencilOpState vkState;

    vkState.failOp = toVkStencilOp(opState.failOp);
    vkState.passOp = toVkStencilOp(opState.passOp);
    vkState.depthFailOp = toVkStencilOp(opState.depthFailOp);
    vkState.compareOp = toVkCompareOp(opState.compareOp);
    vkState.compareMask = compareMask;
    vkState.writeMask = writeMask;
    vkState.reference = reference;

    return vkState;
}

VkIndexType toVkIndexType(IndexType::Enum type)
{
    switch (type) {
        case IndexType::UInt16:
            return VK_INDEX_TYPE_UINT16;
        case IndexType::UInt32:
            return VK_INDEX_TYPE_UINT32;
        default:
            return VK_INDEX_TYPE_MAX_ENUM;
    }
}

VkStencilFaceFlags toVkStencilFaceFlags(StencilFace::Enum face)
{
    switch (face) {
        case StencilFace::Front:
            return VK_STENCIL_FACE_FRONT_BIT;
        case StencilFace::Back:
            return VK_STENCIL_FACE_BACK_BIT;
        case StencilFace::FrontAndBack:
            return VK_STENCIL_FRONT_AND_BACK;
        default:
            return VK_STENCIL_FACE_FLAG_BITS_MAX_ENUM;
    }
}

VkFormat toVkVertexAttribType(VertexAttribType::Enum attrib, bool normalized)
{
    if (normalized) {
        switch (attrib) {
            case VertexAttribType::Byte:
                return VK_FORMAT_R8_SNORM;
            case VertexAttribType::UByte:
                return VK_FORMAT_R8_UNORM;
            case VertexAttribType::Short:
                return VK_FORMAT_R16_SNORM;
            case VertexAttribType::UShort:
                return VK_FORMAT_R16_UNORM;

            case VertexAttribType::Byte2:
                return VK_FORMAT_R8G8_SNORM;
            case VertexAttribType::UByte2:
                return VK_FORMAT_R8G8_UNORM;
            case VertexAttribType::Short2:
                return VK_FORMAT_R16G16_SNORM;
            case VertexAttribType::UShort2:
                return VK_FORMAT_R16G16_UNORM;

            case VertexAttribType::Byte3:
                return VK_FORMAT_R8G8B8_SNORM;
            case VertexAttribType::UByte3:
                return VK_FORMAT_R8G8B8_UNORM;
            case VertexAttribType::Short3:
                return VK_FORMAT_R16G16B16_SNORM;
            case VertexAttribType::UShort3:
                return VK_FORMAT_R16G16B16_UNORM;

            case VertexAttribType::Byte4:
                return VK_FORMAT_R8G8B8A8_SNORM;
            case VertexAttribType::UByte4:
                return VK_FORMAT_R8G8B8A8_UNORM;
            case VertexAttribType::Short4:
                return VK_FORMAT_R16G16B16A16_SNORM;
            case VertexAttribType::UShort4:
                return VK_FORMAT_R16G16B16A16_UNORM;
            default:
                GX_ASSERT_S(false, "Normalized format does not exist.");
                return VK_FORMAT_UNDEFINED;
        }
    }
    switch (attrib) {
        case VertexAttribType::Byte:
            return VK_FORMAT_R8_SINT;
        case VertexAttribType::UByte:
            return VK_FORMAT_R8_UINT;
        case VertexAttribType::Short:
            return VK_FORMAT_R16_SINT;
        case VertexAttribType::UShort:
            return VK_FORMAT_R16_UINT;
        case VertexAttribType::Half:
            return VK_FORMAT_R16_SFLOAT;
        case VertexAttribType::Int:
            return VK_FORMAT_R32_SINT;
        case VertexAttribType::UInt:
            return VK_FORMAT_R32_UINT;
        case VertexAttribType::Float:
            return VK_FORMAT_R32_SFLOAT;

        case VertexAttribType::Byte2:
            return VK_FORMAT_R8G8_SINT;
        case VertexAttribType::UByte2:
            return VK_FORMAT_R8G8_UINT;
        case VertexAttribType::Short2:
            return VK_FORMAT_R16G16_SINT;
        case VertexAttribType::UShort2:
            return VK_FORMAT_R16G16_UINT;
        case VertexAttribType::Half2:
            return VK_FORMAT_R16G16_SFLOAT;
        case VertexAttribType::Float2:
            return VK_FORMAT_R32G32_SFLOAT;

        case VertexAttribType::Byte3:
            return VK_FORMAT_R8G8B8_SINT;
        case VertexAttribType::UByte3:
            return VK_FORMAT_R8G8B8_UINT;
        case VertexAttribType::Short3:
            return VK_FORMAT_R16G16B16_SINT;
        case VertexAttribType::UShort3:
            return VK_FORMAT_R16G16B16_UINT;
        case VertexAttribType::Half3:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case VertexAttribType::Float3:
            return VK_FORMAT_R32G32B32_SFLOAT;

        case VertexAttribType::Byte4:
            return VK_FORMAT_R8G8B8A8_SINT;
        case VertexAttribType::UByte4:
            return VK_FORMAT_R8G8B8A8_UINT;
        case VertexAttribType::Short4:
            return VK_FORMAT_R16G16B16A16_SINT;
        case VertexAttribType::UShort4:
            return VK_FORMAT_R16G16B16A16_UINT;
        case VertexAttribType::Half4:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        case VertexAttribType::Float4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;

        case VertexAttribType::Count:
            GX_ASSERT_S(false, "VertexAttribType::Count not a valid property");
            return VK_FORMAT_UNDEFINED;
    }
    return VK_FORMAT_UNDEFINED;
}

VkPipelineStageFlagBits toVkPipelineStageFlagBits(PipelineStage::Enum stage)
{
    switch (stage) {
        case PipelineStage::TopOfPipe:
            return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        case PipelineStage::DrawIndirect:
            return VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
        case PipelineStage::VertexInput:
            return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
        case PipelineStage::VertexShader:
            return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
        case PipelineStage::TessellationControlShader:
            return VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
        case PipelineStage::TessellationEvaluationShader:
            return VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
        case PipelineStage::GeometryShader:
            return VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
        case PipelineStage::FragmentShader:
            return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        case PipelineStage::EarlyFragmentTests:
            return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        case PipelineStage::LateFragmentTests:
            return VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        case PipelineStage::ColorAttachmentOutput:
            return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        case PipelineStage::ComputeShader:
            return VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        case PipelineStage::Transfer:
            return VK_PIPELINE_STAGE_TRANSFER_BIT;
        case PipelineStage::BottomOfPipe:
            return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        case PipelineStage::Host:
            return VK_PIPELINE_STAGE_HOST_BIT;
        case PipelineStage::AllGraphics:
            return VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
        case PipelineStage::AllCommands:
            return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    }
}

VkPipelineStageFlags toVkPipelineStageFlags(PipelineStageMask mask)
{
    VkPipelineStageFlags flags = 0;

    if (mask & PipelineStage::TopOfPipe) {
        flags |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    }
    if (mask & PipelineStage::DrawIndirect) {
        flags |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
    }
    if (mask & PipelineStage::VertexInput) {
        flags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
    }
    if (mask & PipelineStage::VertexShader) {
        flags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
    }
    if (mask & PipelineStage::TessellationControlShader) {
        flags |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
    }
    if (mask & PipelineStage::TessellationEvaluationShader) {
        flags |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
    }
    if (mask & PipelineStage::GeometryShader) {
        flags |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
    }
    if (mask & PipelineStage::FragmentShader) {
        flags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    if (mask & PipelineStage::EarlyFragmentTests) {
        flags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    if (mask & PipelineStage::LateFragmentTests) {
        flags |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    }
    if (mask & PipelineStage::ColorAttachmentOutput) {
        flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    if (mask & PipelineStage::ComputeShader) {
        flags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    }
    if (mask & PipelineStage::Transfer) {
        flags |= VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    if (mask & PipelineStage::BottomOfPipe) {
        flags |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }
    if (mask & PipelineStage::Host) {
        flags |= VK_PIPELINE_STAGE_HOST_BIT;
    }
    if (mask & PipelineStage::AllGraphics) {
        flags |= VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    }
    if (mask & PipelineStage::AllCommands) {
        flags |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    }

    return flags;
}

VkAccessFlags toVkAccessFlags(BufferAccessMask mask)
{
    VkAccessFlags flags = 0;

    if (mask & BufferAccess::IndirectCommandRead) {
        flags |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
    }
    if (mask & BufferAccess::IndexRead) {
        flags |= VK_ACCESS_INDEX_READ_BIT;
    }
    if (mask & BufferAccess::VertexAttributeRead) {
        flags |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    }
    if (mask & BufferAccess::UniformRead) {
        flags |= VK_ACCESS_UNIFORM_READ_BIT;
    }
    if (mask & BufferAccess::InputAttachmentRead) {
        flags |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
    }
    if (mask & BufferAccess::ShaderRead) {
        flags |= VK_ACCESS_SHADER_READ_BIT;
    }
    if (mask & BufferAccess::ShaderWrite) {
        flags |= VK_ACCESS_SHADER_WRITE_BIT;
    }
    if (mask & BufferAccess::ColorAttachmentRead) {
        flags |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    }
    if (mask & BufferAccess::ColorAttachmentWrite) {
        flags |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }
    if (mask & BufferAccess::DepthStencilAttachmentRead) {
        flags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    }
    if (mask & BufferAccess::DepthStencilAttachmentWrite) {
        flags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }
    if (mask & BufferAccess::TransferRead) {
        flags |= VK_ACCESS_TRANSFER_READ_BIT;
    }
    if (mask & BufferAccess::TransferWrite) {
        flags |= VK_ACCESS_TRANSFER_WRITE_BIT;
    }
    if (mask & BufferAccess::HostRead) {
        flags |= VK_ACCESS_HOST_READ_BIT;
    }
    if (mask & BufferAccess::HostWrite) {
        flags |= VK_ACCESS_HOST_WRITE_BIT;
    }
    if (mask & BufferAccess::MemoryRead) {
        flags |= VK_ACCESS_MEMORY_READ_BIT;
    }
    if (mask & BufferAccess::MemoryWrite) {
        flags |= VK_ACCESS_MEMORY_WRITE_BIT;
    }

    return flags;
}

VkComponentSwizzle toVkSwizzle(TextureSwizzle::Enum swizzle)
{
    switch (swizzle) {
        case TextureSwizzle::Identity:
            return VK_COMPONENT_SWIZZLE_IDENTITY;
        case TextureSwizzle::Zero:
            return VK_COMPONENT_SWIZZLE_ZERO;
        case TextureSwizzle::One:
            return VK_COMPONENT_SWIZZLE_ONE;
        case TextureSwizzle::R:
            return VK_COMPONENT_SWIZZLE_R;
        case TextureSwizzle::G:
            return VK_COMPONENT_SWIZZLE_G;
        case TextureSwizzle::B:
            return VK_COMPONENT_SWIZZLE_B;
        case TextureSwizzle::A:
            return VK_COMPONENT_SWIZZLE_A;
    }
}

VkComponentMapping toVkComponentMapping(TextureSwizzleMapping sm)
{
    VkComponentMapping ret {};
    ret.r = toVkSwizzle(sm.r);
    ret.g = toVkSwizzle(sm.g);
    ret.b = toVkSwizzle(sm.b);
    ret.a = toVkSwizzle(sm.a);

    return ret;
}

VkQueryType toVkQueryType(QueryType::Enum type)
{
    switch (type) {
        case QueryType::Occlusion:
            return VK_QUERY_TYPE_OCCLUSION;
        case QueryType::PipelineStatistics:
            return VK_QUERY_TYPE_PIPELINE_STATISTICS;
        case QueryType::Timestamp:
            return VK_QUERY_TYPE_TIMESTAMP;
    }
}

VkQueryPipelineStatisticFlags toVkQueryPipelineStatisticFlags(QueryPipelineStatisticsFlags flags)
{
    VkQueryPipelineStatisticFlags ret = 0;
    if (flags & QueryPipelineStatistics::InputAssemblyVertices) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT;
    }
    if (flags & QueryPipelineStatistics::InputAssemblyPrimitives) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT;
    }
    if (flags & QueryPipelineStatistics::VertexShaderInvocations) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT;
    }
    if (flags & QueryPipelineStatistics::GeometryShaderInvocations) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT;
    }
    if (flags & QueryPipelineStatistics::GeometryShaderPrimitives) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT;
    }
    if (flags & QueryPipelineStatistics::ClippingInvocations) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT;
    }
    if (flags & QueryPipelineStatistics::ClippingPrimitives) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT;
    }
    if (flags & QueryPipelineStatistics::FragmentShaderInvocations) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT;
    }
    if (flags & QueryPipelineStatistics::TessellationControlShaderPatches) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT;
    }
    if (flags & QueryPipelineStatistics::TessellationEvaluationShaderInvocations) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT;
    }
    if (flags & QueryPipelineStatistics::ComputeShaderInvocations) {
        ret |= VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;
    }

    return ret;
}

VkQueryResultFlags toVkQueryResultFlags(QueryResultFlags flags)
{
    VkQueryResultFlags ret = VK_QUERY_RESULT_64_BIT;
    if (flags & QueryResultFlag::Wait) {
        ret |= VK_QUERY_RESULT_WAIT_BIT;
    }
    if (flags & QueryResultFlag::WithAvailability) {
        ret |= VK_QUERY_RESULT_WITH_AVAILABILITY_BIT;
    }
    if (flags & QueryResultFlag::Partial) {
        ret |= VK_QUERY_RESULT_PARTIAL_BIT;
    }

    return ret;
}

}
}
