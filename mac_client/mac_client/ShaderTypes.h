//
//  ShaderTypes.h
//  mac_client
//
//  Created by Chris McFarlen on 5/20/18.
//  Copyright © 2018 Chris McFarlen. All rights reserved.
//

//
//  Header containing types and enum constants shared between Metal shaders and Swift/ObjC source
//
#ifndef ShaderTypes_h
#define ShaderTypes_h

#ifdef __METAL_VERSION__
#define NS_ENUM(_type, _name) enum _name : _type _name; enum _name : _type
#define NSInteger metal::int32_t
#else
#import <Foundation/Foundation.h>
#endif

#include <simd/simd.h>

typedef enum UIVertexInputIndex
{
    UIVertexInputIndexVertices = 0,
    UIVertexInputViewportSize = 1
} UIVertexInputIndex;

typedef enum UITextureIndex
{
    UITextureIndexBaseColor = 0,
} UITextureIndex;

typedef struct
{
    vector_float2 position;
    vector_float4 color;
    vector_float2 textureCoordinate;
} UIVertex;

typedef NS_ENUM(NSInteger, BufferIndex)
{
    BufferIndexMeshPositions = 0,
    BufferIndexMeshGenerics  = 1,
    BufferIndexUniforms      = 2
};

typedef NS_ENUM(NSInteger, VertexAttribute)
{
    VertexAttributePosition  = 0,
    VertexAttributeTexcoord  = 1,
};

typedef NS_ENUM(NSInteger, TextureIndex)
{
    TextureIndexColor    = 0,
};

typedef struct
{
    matrix_float4x4 projectionMatrix;
    matrix_float4x4 modelViewMatrix;
} Uniforms;

#endif /* ShaderTypes_h */

