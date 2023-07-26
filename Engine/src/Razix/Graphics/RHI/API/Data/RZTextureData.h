#pragma once

namespace Razix {
    namespace Graphics {

        struct RZTextureProperties
        {
            /* The type of the texture */
            enum class Type
            {
                Texture_1D = 0,
                Texture_2D,
                Texture_2DArray,
                Texture_3D,
                Texture_RenderTarget,
                Texture_DepthTarget,
                Texture_CubeMap,
                Texture_CubeMapArray,
                Texture_SwapchainImage
            };

            /* The format of the Texture resource */
            enum class Format
            {
                R8,
                R32_INT,
                R32_UINT,
                R32F,
                RG8,
                RGB8,
                RGBA8,
                RGB16,
                RGBA16,
                RGB32,
                RGBA32,
                RGBA32F,
                RGB,
                RGBA,
                DEPTH16_UNORM,
                DEPTH32F,
                STENCIL,
                DEPTH_STENCIL,
                SCREEN,
                BGRA8_UNORM,
                R11G11B10A2_UINT,
                R11G11B10A2_SFLOAT,
                NONE
            };

            /* Wrap mode for the texture texels */
            enum class Wrapping
            {
                REPEAT,
                MIRRORED_REPEAT,
                CLAMP_TO_EDGE,
                CLAMP_TO_BORDER
            };

            /* Filtering for the Texture */
            struct Filtering
            {
                enum class FilterMode
                {
                    LINEAR,
                    NEAREST
                };
                FilterMode minFilter = FilterMode::LINEAR;
                FilterMode magFilter = FilterMode::LINEAR;

                Filtering() {}
                Filtering(FilterMode min, FilterMode max)
                    : minFilter(min), magFilter(max) {}
            };
        };
    }    // namespace Graphics
}    // namespace Razix