// clang-format off
#include "rzxpch.h"
// clang-format on
#include "RZCompositionPass.h"

#include "Razix/Core/App/RZApplication.h"
#include "Razix/Core/Markers/RZMarkers.h"
#include "Razix/Core/RZEngine.h"

#include "Razix/Gfx/RZMesh.h"
#include "Razix/Gfx/RZMeshFactory.h"

#include "Razix/Gfx/Materials/RZMaterial.h"

#include "Razix/Gfx/Passes/Data/GlobalData.h"

#include "Razix/Gfx/RHI/RHI.h"

#include "Razix/Gfx/RHI/API/RZDrawCommandBuffer.h"
#include "Razix/Gfx/RHI/API/RZGraphicsContext.h"
#include "Razix/Gfx/RHI/API/RZIndexBuffer.h"
#include "Razix/Gfx/RHI/API/RZPipeline.h"
#include "Razix/Gfx/RHI/API/RZShader.h"
#include "Razix/Gfx/RHI/API/RZSwapchain.h"
#include "Razix/Gfx/RHI/API/RZVertexBuffer.h"

#include "Razix/Gfx/RHI/API/Data/RZPipelineData.h"

#include "Razix/Gfx/Resources/RZFrameGraphTexture.h"

namespace Razix {
    namespace Gfx {

        void RZCompositionPass::addPass(FrameGraph::RZFrameGraph& framegraph, Razix::RZScene* scene, RZRendererSettings* settings)
        {
            auto compositionShader = Gfx::RZShaderLibrary::Get().getBuiltInShader(ShaderBuiltin::Composition);

            RZPipelineDesc pipelineInfo{};
            // Build the pipeline here for this pass
            pipelineInfo.name                   = "Pipeline.Composition";
            pipelineInfo.shader                 = compositionShader;
            pipelineInfo.colorAttachmentFormats = {TextureFormat::BGRA8_UNORM};
            pipelineInfo.cullMode               = Gfx::CullMode::None;
            pipelineInfo.drawType               = Gfx::DrawType::Triangle;
            pipelineInfo.transparencyEnabled    = false;
            pipelineInfo.depthBiasEnabled       = false;
            pipelineInfo.depthTestEnabled       = false;
            pipelineInfo.depthWriteEnabled      = false;
            // Create the pipeline
            m_Pipeline = RZResourceManager::Get().createPipeline(pipelineInfo);

            // Get the final output
            FrameGraph::RZFrameGraphResource FinalOutputRenderTarget = framegraph.getBlackboard().getFinalOutputID();
#if 1
            framegraph.addCallbackPass(
                "Pass.Builtin.Code.Composition",
                [&](auto& data, FrameGraph::RZPassResourceBuilder& builder) {
                    // Set this as a standalone pass (should not be culled)
                    builder.setAsStandAlonePass();

                    // Read the Final RT from where ever it's given from
                    builder.read(FinalOutputRenderTarget);
                },
                [=](const auto& data, FrameGraph::RZPassResourceDirectory& resources) {
                    RAZIX_PROFILE_FUNCTIONC(RZ_PROFILE_COLOR_GRAPHICS);

                    RAZIX_TIME_STAMP_BEGIN("Composition Pass");
                    RAZIX_MARK_BEGIN("Pass.Builtin.Code.Composition", glm::vec4(0.5f));

                    auto cmdBuffer = RHI::GetCurrentCommandBuffer();

                    if (FrameGraph::RZFrameGraph::IsFirstFrame()) {
                        auto& shaderBindVars = RZResourceManager::Get().getShaderResource(compositionShader)->getBindVars();

                        RZDescriptor* descriptor = nullptr;

                        descriptor = shaderBindVars["CompositionTarget"];
                        if (descriptor)
                            descriptor->texture = resources.get<FrameGraph::RZFrameGraphTexture>(FinalOutputRenderTarget).getHandle();

                        RZResourceManager::Get().getShaderResource(compositionShader)->updateBindVarsHeaps();
                    }

                    RenderingInfo info{};
                    info.resolution       = Resolution::kWindow;
                    info.colorAttachments = {
                        {Gfx::RHI::GetSwapchain()->GetCurrentImage(), {true, ClearColorPresets::OpaqueBlack}}};
                    info.resize = true;

                    RHI::BeginRendering(cmdBuffer, info);

                    // Bind pipeline and stuff
                    RHI::BindPipeline(m_Pipeline, cmdBuffer);

                    scene->drawScene(m_Pipeline, SceneDrawGeometryMode::ScreenQuad);

                    RHI::EndRendering(cmdBuffer);

                    RAZIX_MARK_END();
                    RAZIX_TIME_STAMP_END();
                },
                [=](FrameGraph::RZPassResourceDirectory& resources, u32 width, u32 height) {
                    RZGraphicsContext::GetContext()->Wait();
                });
#endif
        }

        void RZCompositionPass::destroy()
        {
            RZResourceManager::Get().destroyPipeline(m_Pipeline);
        }
    }    // namespace Gfx
}    // namespace Razix