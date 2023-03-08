// FidelityFX Super Resolution Sample
//
// Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
//
// This file is part of the FidelityFX Super Resolution beta which is 
// released under the BETA SOFTWARE EVALUATION LICENSE AGREEMENT.
//
// See file LICENSE.txt for full license details.

#pragma once

#include <d3d11.h>

#include "../ffx-fsr2-api/ffx_fsr2.h"

#include <vector>


class Fsr2Wrapper
{
public:
    struct ContextParameters
    {

        uint32_t        flags = 0;
        FfxDimensions2D maxRenderSize = { 0, 0 };
        FfxDimensions2D displaySize = { 0, 0 };
        ID3D11Device*   device = nullptr;
    };

    struct DrawParameters
    {
        ID3D11DeviceContext* deviceContext = nullptr;

        // Inputs
        ID3D11Texture2D* unresolvedColorResource = nullptr;
        ID3D11Texture2D* motionvectorResource = nullptr;
        ID3D11Texture2D* depthbufferResource = nullptr;
        ID3D11Texture2D* reactiveMapResource = nullptr;
        ID3D11Texture2D* transparencyAndCompositionResource = nullptr;

        // Output
        ID3D11Texture2D* resolvedColorResource = nullptr;

        // Arguments
        uint32_t renderWidth = 0;
        uint32_t renderHeight = 0;

        bool cameraReset = false;
        float cameraJitterX = 0.f;
        float cameraJitterY = 0.f;

        bool enableSharpening = true;
        float sharpness = 0.f;

        float frameTimeDelta = 0.f;

        float nearPlane = 1.f;
        float farPlane = 10.f;
        float fovH = 90.f;
    }; 

public:
    void Create(ContextParameters params);
    void Destroy();

    void Draw(const DrawParameters& params);

    bool IsCreated() const { return m_created; }
    FfxDimensions2D GetDisplaySize() const { return m_contextDesc.displaySize; }

private:
    bool m_created = false;

    FfxFsr2Context m_context;
    FfxFsr2ContextDescription m_contextDesc;
    ContextParameters m_contextParams;

    std::vector<char> m_scratchBuffer;
};
