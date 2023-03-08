# FidelityFX Super Resolution Sample
# 
#  Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
# 
#  This file is part of the FidelityFX Super Resolution beta which is 
#  released under the BETA SOFTWARE EVALUATION LICENSE AGREEMENT.
# 
#  See file LICENSE.txt for full license details.

file(GLOB_RECURSE AMD_SDK_SOURCES *.cpp)
file(GLOB_RECURSE AMD_SDK_HEADERS *.h)

set(amd_sdk_minimal_shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/Line.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/Sprite.hlsl
)

copyCommand("${amd_sdk_minimal_shaders}" ${CMAKE_HOME_DIRECTORY}/bin/ShaderLibDX/dx11)
add_library(amd_sdk_minimal ${AMD_SDK_SOURCES} ${AMD_SDK_HEADERS} ${amd_sdk_minimal_shaders})
target_include_directories(amd_sdk_minimal PUBLIC inc)
target_compile_definitions(amd_sdk_minimal PRIVATE AMD_SDK_MINIMAL _UNICODE UNICODE)
target_link_libraries(amd_sdk_minimal PUBLIC DXUT DXUTOpt)

