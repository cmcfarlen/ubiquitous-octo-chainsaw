
#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

typedef struct
{
    float3 position [[attribute(0)]];
    float2 texCoord [[attribute(1)]];
} Vertex;

typedef struct
{
    float4 position [[position]];
    float2 texCoord;
} ColorInOut;

typedef struct
{
   float4x4 projectionMatrix;
   float4x4 modelViewMatrix;
} Uniforms;

vertex ColorInOut vertexShader(Vertex in [[stage_in]],
                               constant float4x4 & proj [[ buffer(1) ]])
{
    ColorInOut out;

    float4 position = float4(in.position, 1.0);
    out.position = proj * position;
    out.texCoord = in.texCoord;

    return out;
}

fragment float4 fragmentShader(ColorInOut in [[stage_in]],
                               texture2d<half> colorMap     [[ texture(0) ]])
{
    constexpr sampler colorSampler(mip_filter::linear,
                                   mag_filter::linear,
                                   min_filter::linear);

    half4 colorSample   = colorMap.sample(colorSampler, in.texCoord.xy);

    return float4(1, 1, 1, colorSample.r);
}

