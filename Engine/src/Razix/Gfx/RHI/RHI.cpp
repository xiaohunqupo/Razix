// clang-format off
#include "rzxpch.h"
// clang-format on
#include "RHI.h"

#include "Razix/Core/App/RZApplication.h"

#include "Razix/Gfx/Materials/RZMaterial.h"
#include "Razix/Gfx/RHI/API/RZGraphicsContext.h"
#include "Razix/Gfx/RZShaderLibrary.h"

#ifdef RAZIX_RENDER_API_OPENGL
    #include "Razix/Platform/API/OpenGL/GLRenderContext.h"
#endif

#ifdef RAZIX_RENDER_API_VULKAN
    #include "Razix/Platform/API/Vulkan/VKRenderContext.h"
#endif

#ifdef RAZIX_RENDER_API_DIRECTX12
    #include "Razix/Platform/API/DirectX12/DX12RenderContext.h"
#endif

namespace Razix {
    namespace Gfx {

        RHI* RHI::s_APIInstance = nullptr;

        void RHI::Create(u32 width, u32 height)
        {
            RAZIX_PROFILE_FUNCTIONC(RZ_PROFILE_COLOR_GRAPHICS);

            RAZIX_CORE_INFO("Creating API Renderer...");
            RAZIX_CORE_INFO("\t Backend API : {0}", Gfx::RZGraphicsContext::GetRenderAPIString());

            switch (Gfx::RZGraphicsContext::GetRenderAPI()) {
#ifdef RAZIX_RENDER_API_OPENGL
                case Razix::Gfx::RenderAPI::OPENGL: s_APIInstance = new GLRenderContext(width, height); break;
#endif
#ifdef RAZIX_RENDER_API_VULKAN
                case Razix::Gfx::RenderAPI::VULKAN: s_APIInstance = new VKRenderContext(width, height); break;
#endif
#ifdef RAZIX_RENDER_API_DIRECTX12
                case Razix::Gfx::RenderAPI::D3D12: s_APIInstance = new DX12RenderContext(width, height); break;
#endif
                default: s_APIInstance = nullptr; break;
            }
        }

        void RHI::Release()
        {
            Gfx::RZMaterial::ReleaseDefaultTexture();
            // Shutting down the shader library
            Gfx::RZShaderLibrary::Get().ShutDown();

            s_APIInstance->DestroyAPIImpl();
            delete s_APIInstance;

            // Release the context at last
            Gfx::RZGraphicsContext::GetContext()->Release();
        }
    }    // namespace Gfx
}    // namespace Razix