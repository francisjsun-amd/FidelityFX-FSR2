// FidelityFX Super Resolution Sample
//
// Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
//
// This file is part of the FidelityFX Super Resolution beta which is 
// released under the BETA SOFTWARE EVALUATION LICENSE AGREEMENT.
//
// See file LICENSE.txt for full license details.

struct S_MODEL_DESC
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
};

struct S_CAMERA_DESC
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
};


struct DirectionalLightInfo
{
    float3 direction;
    float3 color;
    float  specPower;
};

static DirectionalLightInfo directionalLights[] = {
    { { -0.7943764, -0.32935333, 0.5103845 }, { 1.0, 0.7, 0.6 }, 50.0 },
};

cbuffer CB_MODEL_DATA : register(b0) { S_MODEL_DESC g_Model; }

cbuffer CB_VIEWER_DATA : register(b1) { S_CAMERA_DESC g_CurrViewer; S_CAMERA_DESC g_PrevViewer;  }

//--------------------------------------------------------------------------------------
// Buffers, Textures and Samplers
//--------------------------------------------------------------------------------------
Texture2D g_t2dDiffuse : register(t0);

SamplerState g_ssLinear : register(s0);

//--------------------------------------------------------------------------------------
// Shader structures
//--------------------------------------------------------------------------------------

struct VS_RenderInput
{
    float3 m_Position : POSITION;
    float3 m_Normal : NORMAL;
    float2 m_TexCoord : TEXCOORD;
};

struct PS_RenderInput
{
    float4 m_Position : SV_Position;
    float4 m_CurrPos : POSITION0;
    float4 m_PrevPos : POSITION1;
    float3 m_worldPos : WORLDPOS;
    float3 m_Normal : NORMAL;
    float2 m_TexCoord : TEXCOORD;
};

struct PS_RenderOutput
{
    float4 m_Color : SV_Target0;
    float2 m_MotionVect : SV_Target1;
};

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
PS_RenderInput VS_RenderModel(VS_RenderInput In)
{
    PS_RenderInput Out;

    // Transform the position from object space to homogeneous projection space
    Out.m_Position = mul(float4(In.m_Position, 1.0f), mul(g_Model.m_World, g_CurrViewer.m_ViewProjection));
    Out.m_CurrPos = Out.m_Position;
    Out.m_PrevPos = mul(float4(In.m_Position, 1.0f), mul(g_Model.m_World, g_PrevViewer.m_ViewProjection));
    Out.m_worldPos = mul(float4(In.m_Position, 1.0f), g_Model.m_World).xyz;

    // Transform the normal from object space to world space
    Out.m_Normal = normalize(mul(In.m_Normal, (float3x3)g_Model.m_World));

    // Pass through texture coords
    Out.m_TexCoord = In.m_TexCoord;

    return Out;
}


void CalcDirectionalLight(in const DirectionalLightInfo light, in const float3 worldPos, in const float3 normal, in out float3 lightColor)
{
    lightColor += saturate(dot(normal, -light.direction)) * light.color * g_Model.m_Diffuse.rgb;
    float3 lightReflect = normalize(reflect(light.direction, normal));
    float  spec         = saturate(dot(normalize(g_CurrViewer.m_Position - worldPos), lightReflect));
    float  specFactor   = pow(spec, light.specPower);
    lightColor += specFactor * light.color * g_Model.m_Specular.rgb;
}

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
PS_RenderOutput PS_RenderModel(PS_RenderInput In)
{
    float4 texColor = float4(1, 1, 1, 1);  //
    texColor        = g_t2dDiffuse.SampleBias(g_ssLinear, In.m_TexCoord, g_Model.m_mipBias);

    float3 lightColor = float3(0, 0, 0);

    CalcDirectionalLight(directionalLights[0], In.m_worldPos, In.m_Normal, lightColor);

    float3 ambient = float3(0, 0, 0);
    ambient += lerp(float3(0.08, 0.08, 0.05), float3(0.09, 0.1, 0.33), In.m_Normal.y * 0.5 + 0.5);

    float3 color = (lightColor + ambient);
    color *= texColor.xyz;

    PS_RenderOutput Out;
    Out.m_Color = float4(color, 1.0);
    Out.m_MotionVect = In.m_CurrPos.xy / In.m_CurrPos.w - In.m_PrevPos.xy / In.m_PrevPos.w;

    return Out;
}
