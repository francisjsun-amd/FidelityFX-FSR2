# FidelityFX Super Resolution Sample
# 
#  Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
# 
#  This file is part of the FidelityFX Super Resolution beta which is 
#  released under the BETA SOFTWARE EVALUATION LICENSE AGREEMENT.
# 
#  See file LICENSE.txt for full license details.

add_subdirectory(framework/dxut)
add_subdirectory(framework/amd_sdk)
add_subdirectory(framework/amd_lib)

set(FSR2_Sample_DX11_SOURCES
    FSR2DX11Sample.cpp
    FSR2DX11Sample.h
    Fsr2Wrapper.cpp
    Fsr2Wrapper.h
    ResourceFiles/GPUOpenIcon.rc
    ResourceFiles/dpiaware.manifest
)

set(FSR2_Sample_DX11_SHADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/FSR2DX11Sample.hlsl
)
set(FSR2_Sample_DX11_CONFIG
    ${CMAKE_CURRENT_SOURCE_DIR}/FSR2_Sample_DX11.json
)

copyTargetCommand("${FSR2_Sample_DX11_SHADERS}" ${CMAKE_HOME_DIRECTORY}/bin/ShaderLibDX/dx11 dx11_sample_shaders)
copyTargetCommand("${FSR2_Sample_DX11_CONFIG}" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} dx11_sample_config)

add_executable(FSR2_Sample_DX11 WIN32 ${FSR2_Sample_DX11_SOURCES} ${FSR2_Sample_DX11_SHADERS})
target_link_libraries(FSR2_Sample_DX11 PUBLIC comctl32 Usp10 NJSON amd_lib_minimal amd_sdk_minimal d3dcompiler D3D11 ffx_fsr2_api_x64 ffx_fsr2_api_dx11_x64)
set_target_properties(FSR2_Sample_DX11 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin" DEBUG_POSTFIX "d")
target_compile_definitions(FSR2_Sample_DX11 PRIVATE _UNICODE UNICODE)
add_dependencies(FSR2_Sample_DX11 dx11_sample_shaders dx11_sample_config)
