------------------------------------------------------------------------------
-- Shaders build HLSL

-- https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/SPIR-V.rst

-- Engine distributed DXC location
filter "system:windows"
    dxcLocation = "%{wks.location}../Engine/Content/Shaders/Tools/dxc/bin/x64/"
filter "system:macosx"
    VulkanSDK = os.getenv("VULKAN_SDK")
    dxcLocation = "%{VulkanSDK}/bin/"
filter {}

group "Engine/content"
    project "Shaders"
        kind "Utility"

        files
        { 
            -- Shader files
            "content/Shaders/ShaderCommon/**",
            -- HLSL - primary language for all platforms shader gen
            "content/Shaders/HLSL/**.h",
            "content/Shaders/HLSL/**.hlsl",
            "content/Shaders/HLSL/**.hlsli",
            "content/Shaders/HLSL/**.vert.hlsl",
            "content/Shaders/HLSL/**.geom.hlsl",
            "content/Shaders/HLSL/**.frag.hlsl",
            "content/Shaders/HLSL/**.comp.hlsl",
            -- GLSL (Deprecated)
            --"content/Shaders/GLSL/**",
            -- PSSL (PlayStation 5)
            --"content/Shaders/PSSL/**.pssl",
            --"content/Shaders/PSSL/**.h",
            --"content/Shaders/PSSL/**.hs",
            -- MSL (Apple)
            -- Razix Shader File
            "content/Shaders/Razix/**.rzsf",
            -- Generated Reflectiond data and other platform shaders.
            --"content/Shaders/Generated/**"
        }

    -- Note: All shaders are built using SM6

    -- TODO: Add as rules, every shader file type will have it's own rule
    -- TODO: customize per build config, release/distr shaders will need signing and extra optimization flags (-O3 etc.)
    -- Don't build the these shader types, they are compiled by the engine once and cached/include only
    filter { "files:**.glsl or **.hlsl or **.pssl or **.cg or **.rzsf"}
        flags { "ExcludeFromBuild" }
    filter {}

    -- Build GLSL files based on their extension
    -------------------
    -- VERTEX SHADER
    -------------------
    filter { "files:**.vert.hlsl" }
        removeflags "ExcludeFromBuild"
        buildmessage 'Compiling HLSL Vertex shader : %{file.name}'
        buildcommands
        {
            -- Compile CSO binary
            "echo [Compiling] CSO for DX12 backend...",
            '"%{dxcLocation}/dxc" -D __HLSL__ -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" -E VS_MAIN -T vs_6_0  "%{file.directory}/%{file.name}" -Fo "%{wks.location}/../Engine/content/Shaders/Compiled/CSO/%{file.basename}.cso"',
            -- Compile SPIRV binary
            "echo [Compiling] SPIRV for VK backend",
            '"%{dxcLocation}/dxc" -spirv -D __GLSL__ -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" -E VS_MAIN -T vs_6_0 -fspv-target-env=vulkan1.3 "%{file.directory}/%{file.name}" -Fo "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename }.spv"',
            -- Generate GLSL (for reference only)???
            "echo [Generating] GLSL from SPIRV",
            'spirv-cross "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename}.spv" --stage vert --entry VS_MAIN -V --output "%{wks.location}/../Engine/content/Shaders/Generated/GLSL/%{file.basename}.glsl"',
            -- Generate Reflection Data JSON
            "echo [Generating] exporting shader Reflection data",
            'spirv-cross "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename}.spv" --reflect --stage vert --entry VS_MAIN --output "%{wks.location}/../Engine/content/Shaders/Generated/ReflectionData/%{file.basename}.json"',
            -- Generate Assembly
            "echo [Generating] exporting shader assembly listing",
            '"%{dxcLocation}/dxc" -D __HLSL__ -E VS_MAIN -T vs_6_0 -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" "%{file.directory}/%{file.name}" -Fc "%{wks.location}/../Engine/content/Shaders/Generated/Assembly/%{file.basename}.isa"'
        }
        buildoutputs
        {
            "%{wks.location}/../Engine/content/Shaders/Compiled/CSO/%{file.basename }.cso",
            "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename }.spv",
            -----------------------------------------------------------------------------
            "%{wks.location}/../Engine/content/Shaders/Generated/GLSL/%{file.basename }.glsl",
            "%{wks.location}/../Engine/content/Shaders/Generated/ReflectionData/%{file.basename }.json",
            "%{wks.location}/../Engine/content/Shaders/Generated/Assembly/%{file.basename }.isa"
        }
    -------------------
    -- PIXEL SHADER
    -------------------
    filter { "files:**.frag.hlsl" }
        removeflags "ExcludeFromBuild"
        buildmessage 'Compiling HLSL Pixel shader : %{file.name}'
        buildcommands
        {
            -- Compile CSO binary
            "echo [Compiling] CSO for DX12 backend...",
            '"%{dxcLocation}/dxc" -D __HLSL__ -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" -E PS_MAIN -T ps_6_0 "%{file.directory}/%{file.name}" -Fo "%{wks.location}/../Engine/content/Shaders/Compiled/CSO/%{file.basename}.cso"',
            -- Compile SPIRV binary
            "echo [Compiling] SPIRV for VK backend",
            '"%{dxcLocation}/dxc" -spirv -D __GLSL__ -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" -E PS_MAIN -T ps_6_0 -fspv-target-env=vulkan1.3 "%{file.directory}/%{file.name}" -Fo "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename}.spv"',
            -- Generate GLSL (for reference only)???
            "echo [Generating] GLSL from SPIRV",
            'spirv-cross "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename}.spv" --stage frag --entry PS_MAIN -V --output "%{wks.location}/../Engine/content/Shaders/Generated/GLSL/%{file.basename}.glsl"',
            -- Generate Reflection Data JSON
            "echo [Generating] exporting shader Reflection data",
            'spirv-cross "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename}.spv" --reflect --stage frag --entry PS_MAIN --output "%{wks.location}/../Engine/content/Shaders/Generated/ReflectionData/%{file.basename}.json"',
            -- Generate Assembly
            "echo [Generating] exporting shader assembly listing",
            '"%{dxcLocation}/dxc" -D __HLSL__ -E PS_MAIN -T ps_6_0 -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" "%{file.directory}/%{file.name}" -Fc "%{wks.location}/../Engine/content/Shaders/Generated/Assembly/%{file.basename}.isa"'
        }
        buildoutputs
        {
            "%{wks.location}/../Engine/content/Shaders/Compiled/CSO/%{file.basename }.cso",
            "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename }.spv",
            -----------------------------------------------------------------------------
            "%{wks.location}/../Engine/content/Shaders/Generated/GLSL/%{file.basename }.glsl",
            "%{wks.location}/../Engine/content/Shaders/Generated/ReflectionData/%{file.basename }.json",
            "%{wks.location}/../Engine/content/Shaders/Generated/Assembly/%{file.basename }.isa"
        }
    -------------------
    -- COMPUTE SHADER
    -------------------
    filter { "files:**.comp.hlsl" }
        removeflags "ExcludeFromBuild"
        buildmessage 'Compiling HLSL Compute shader : %{file.name}'
        buildcommands
        {
            -- Compile CSO binary
            "echo [Compiling] CSO for DX12 backend...",
            '"%{dxcLocation}/dxc" -D __HLSL__ -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" -E CS_MAIN -T cs_6_0  "%{file.directory}/%{file.name}" -Fo "%{wks.location}/../Engine/content/Shaders/Compiled/CSO/%{file.basename}.cso"',
            -- Compile SPIRV binary
            "echo [Compiling] SPIRV for VK backend",
            '"%{dxcLocation}/dxc" -spirv -D __GLSL__ -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" -E CS_MAIN -T cs_6_0 -fspv-target-env=vulkan1.3 "%{file.directory}/%{file.name}" -Fo "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename }.spv"',
            -- Generate GLSL (for reference only)???
            "echo [Generating] GLSL from SPIRV",
            'spirv-cross "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename}.spv" --stage comp --entry CS_MAIN -V --output "%{wks.location}/../Engine/content/Shaders/Generated/GLSL/%{file.basename}.glsl"',
            -- Generate Reflection Data JSON
            "echo [Generating] exporting shader Reflection data",
            'spirv-cross "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename}.spv" --reflect --stage comp --entry CS_MAIN --output "%{wks.location}/../Engine/content/Shaders/Generated/ReflectionData/%{file.basename}.json"',
            -- Generate Assembly
            "echo [Generating] exporting shader assembly listing",
            '"%{dxcLocation}/dxc" -D __HLSL__ -E CS_MAIN -T cs_6_0 -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" "%{file.directory}/%{file.name}" -Fc "%{wks.location}/../Engine/content/Shaders/Generated/Assembly/%{file.basename}.isa"'
        }
        buildoutputs
        {
            "%{wks.location}/../Engine/content/Shaders/Compiled/CSO/%{file.basename }.cso",
            "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename }.spv",
            -----------------------------------------------------------------------------
            "%{wks.location}/../Engine/content/Shaders/Generated/GLSL/%{file.basename }.glsl",
            "%{wks.location}/../Engine/content/Shaders/Generated/ReflectionData/%{file.basename }.json",
            "%{wks.location}/../Engine/content/Shaders/Generated/Assembly/%{file.basename }.isa"
        }
    -------------------
    -- GEOMETRY SHADER
    -------------------
    filter { "files:**.geom.hlsl" }
        removeflags "ExcludeFromBuild"
        buildmessage 'Compiling HLSL Geometry shader : %{file.name}'
        buildcommands
        {
            -- Compile CSO binary
            "echo [Compiling] CSO for DX12 backend...",
            '"%{dxcLocation}/dxc" -D __HLSL__ -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" -E GS_MAIN -T gs_6_0  "%{file.directory}/%{file.name}" -Fo "%{wks.location}/../Engine/content/Shaders/Compiled/CSO/%{file.basename}.cso"',
            -- Compile SPIRV binary
            "echo [Compiling] SPIRV for VK backend",
            '"%{dxcLocation}/dxc" -spirv -D __GLSL__ -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" -E GS_MAIN -T gs_6_0 -fspv-target-env=vulkan1.3 "%{file.directory}/%{file.name}" -Fo "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename }.spv"',
            -- Generate GLSL (for reference only)???
            "echo [Generating] GLSL from SPIRV",
            'spirv-cross "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename}.spv" --stage geom --entry GS_MAIN -V --output "%{wks.location}/../Engine/content/Shaders/Generated/GLSL/%{file.basename}.glsl"',
            -- Generate Reflection Data JSON
            "echo [Generating] exporting shader Reflection data",
            'spirv-cross "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename}.spv" --reflect --stage geom --entry GS_MAIN --output "%{wks.location}/../Engine/content/Shaders/Generated/ReflectionData/%{file.basename}.json"',
            -- Generate Assembly
            "echo [Generating] exporting shader assembly listing",
            '"%{dxcLocation}/dxc" -D __HLSL__ -E GS_MAIN -T gs_6_0 -I "%{wks.location}/../Engine/content/Shaders/HLSL" -I "%{wks.location}/../Engine/content/Shaders/ShaderCommon" "%{file.directory}/%{file.name}" -Fc "%{wks.location}/../Engine/content/Shaders/Generated/Assembly/%{file.basename}.isa"'
        }
        buildoutputs
        {
            "%{wks.location}/../Engine/content/Shaders/Compiled/CSO/%{file.basename }.cso",
            "%{wks.location}/../Engine/content/Shaders/Compiled/SPIRV/%{file.basename }.spv",

            -----------------------------------------------------------------------------
            "%{wks.location}/../Engine/content/Shaders/Generated/GLSL/%{file.basename }.glsl",
            "%{wks.location}/../Engine/content/Shaders/Generated/ReflectionData/%{file.basename }.json",
            "%{wks.location}/../Engine/content/Shaders/Generated/Assembly/%{file.basename }.isa"
        }
group""
------------------------------------------------------------------------------
