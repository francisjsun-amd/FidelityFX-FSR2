// FidelityFX Super Resolution Sample
//
// Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
//
// This file is part of the FidelityFX Super Resolution beta which is 
// released under the BETA SOFTWARE EVALUATION LICENSE AGREEMENT.
//
// See file LICENSE.txt for full license details.

#include <DirectXMath.h>

//--------------------------------------------------------------------------------------
// Declare Common Sample Types
//--------------------------------------------------------------------------------------
struct float2
{
    union
    {
        struct
        {
            float x, y;
        };
        float v[2];
    };
};
struct float3
{
    union
    {
        struct
        {
            float x, y, z;
        };
        float v[3];
    };
};

__declspec(align(16)) struct float4
{
    union
    {
        DirectX::XMVECTOR v;
        float             f[4];
        struct
        {
            float x, y, z, w;
        };
    };

    float4(float ax = 0, float ay = 0, float az = 0, float aw = 0)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }

    float4(DirectX::XMVECTOR av) { v = av; }
    float4(DirectX::XMVECTOR xyz, float aw)
    {
        v = xyz;
        w = aw;
    }

    inline operator DirectX::XMVECTOR() const { return v; }
    inline operator const float*() const { return f; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
    inline operator __m128i() const { return _mm_castps_si128(v); }
    inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
};

#define float4x4 DirectX::XMMATRIX

// The declaraion of S_CAMERA_DESC matches the declaration of Camera type defined
// in AMD_Types and used throughout the libraries
__declspec(align(16)) typedef struct S_CAMERA_DESC_t
{
    float4x4 m_View;
    float4x4 m_Projection;
    float4x4 m_ViewProjection;
    float4x4 m_View_Inv;
    float4x4 m_Projection_Inv;
    float4x4 m_ViewProjection_Inv;
    float3   m_Position;
    float    m_Fov;
    float3   m_Direction;
    float    m_FarPlane;
    float3   m_Right;
    float    m_NearPlane;
    float3   m_Up;
    float    m_Aspect;
    float4   m_Color;
} S_CAMERA_DESC;

// The declaraion of S_MODEL_DESC matches the declaration of Model type defined
// in AMD_Types and used throughout the libraries
__declspec(align(16)) typedef struct S_MODEL_DESC_t
{
    float4x4 m_World;
    float4x4 m_World_Inv;
    float4   m_Position;
    float4   m_Orientation;
    float4   m_Scale;
    float4   m_Ambient;
    float4   m_Diffuse;
    float4   m_Specular;
    float    m_mipBias;
} S_MODEL_DESC;


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext);

HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pSurfaceDesc, void* pUserContext);
HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pSurfaceDesc, void* pUserContext);
void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext);
void CALLBACK OnD3D11DestroyDevice(void* pUserContext);
void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dContext, double fTime, float fElapsedTime, void* pUserContext);

HRESULT CompileShaders(ID3D11Device* pd3dDevice);
HRESULT CreateMeshes(ID3D11Device* pd3dDevice);
HRESULT SetupCamera(ID3D11Device* pd3dDevice);

void    InitApp();
HRESULT ReleaseMeshes();
HRESULT ReleaseShaders();
