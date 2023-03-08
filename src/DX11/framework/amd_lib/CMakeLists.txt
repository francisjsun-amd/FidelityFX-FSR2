# FidelityFX Super Resolution Sample
# 
#  Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
# 
#  This file is part of the FidelityFX Super Resolution beta which is 
#  released under the BETA SOFTWARE EVALUATION LICENSE AGREEMENT.
# 
#  See file LICENSE.txt for full license details.

file(GLOB_RECURSE AMD_LIB_SOURCES *.cpp)
file(GLOB_RECURSE AMD_LIB_HEADERS *.h)

add_library(amd_lib_minimal ${AMD_LIB_SOURCES} ${AMD_LIB_HEADERS})
target_include_directories(amd_lib_minimal PUBLIC shared/common/inc PUBLIC shared/d3d11/inc)
target_compile_definitions(amd_lib_minimal PRIVATE AMD_LIB_MINIMAL _UNICODE UNICODE)