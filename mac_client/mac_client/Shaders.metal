//
//  Shaders.metal
//  mac_client
//
//  Created by Chris McFarlen on 5/20/18.
//  Copyright © 2018 Chris McFarlen. All rights reserved.
//

// File for Metal kernel and shader functions

#include <metal_stdlib>
#include <simd/simd.h>

// Including header shared between this Metal shader code and Swift/C code executing Metal API commands
#import "ShaderTypes.h"

using namespace metal;

typedef struct
{
    float3 position [[attribute(VertexAttributePosition)]];
    float2 texCoord [[attribute(VertexAttributeTexcoord)]];
} Vertex;

typedef struct
{
    float4 position [[position]];
    float2 texCoord;
} ColorInOut;

vertex ColorInOut vertexShader(Vertex in [[stage_in]],
                               constant Uniforms & uniforms [[ buffer(BufferIndexUniforms) ]])
{
    ColorInOut out;

    float4 position = float4(in.position, 1.0);
    out.position = uniforms.projectionMatrix * uniforms.modelViewMatrix * position;
    out.texCoord = in.texCoord;

    return out;
}

fragment float4 fragmentShader(ColorInOut in [[stage_in]],
                               constant Uniforms & uniforms [[ buffer(BufferIndexUniforms) ]],
                               texture2d<half> colorMap     [[ texture(TextureIndexColor) ]])
{
    constexpr sampler colorSampler(mip_filter::linear,
                                   mag_filter::linear,
                                   min_filter::linear);

    half4 colorSample   = colorMap.sample(colorSampler, in.texCoord.xy);

    return float4(colorSample);
}


typedef struct
{
    float4 clipSpacePosition [[position]];
    float4 color;
    float2 textureCoordinate;
} UIRasterData;

vertex UIRasterData
uiVertexShader(uint vertexID [[vertex_id]],
               constant UIVertex* vertices [[buffer(UIVertexInputIndexVertices)]],
               constant vector_uint2* viewportSizePointer [[buffer(UIVertexInputViewportSize)]])
{
    UIRasterData out;
    
    out.clipSpacePosition = vector_float4(0.0, 0.0, 0.0, 1.0);
    
    float2 pixelSpacePosition = vertices[vertexID].position.xy;
    
    float2 viewportSize = vector_float2(*viewportSizePointer);
    
    out.clipSpacePosition.xy = pixelSpacePosition / (viewportSize / 2.0);
    
    out.color = vertices[vertexID].color;
    out.textureCoordinate = vertices[vertexID].textureCoordinate;
    
    return out;
}

fragment float4
uiFragmentShader(UIRasterData in [[stage_in]],
                 texture2d<half> colorTexture [[ texture(UITextureIndexBaseColor)]])
{
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    
    const half4 colorSample = colorTexture.sample(textureSampler, in.textureCoordinate);
    return float4(colorSample);
}

