//
//  Renderer.m
//  mac_client
//
//  Created by Chris McFarlen on 5/20/18.
//  Copyright © 2018 Chris McFarlen. All rights reserved.
//

#import <simd/simd.h>
#import <ModelIO/ModelIO.h>

#import "Renderer.h"

// Include header shared between C code here, which executes Metal API commands, and .metal files
#import "ShaderTypes.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "platform.h"

typedef struct
{
    uint32 width;
    uint32 height;
    uint8* data;
    uint32 length;
} TextureData;

@implementation Renderer
{
    id <MTLDevice> _device;
    id <MTLCommandQueue> _commandQueue;
    id <MTLRenderPipelineState> _pipelineState;
    
    vector_uint2 _viewport;
    
    id <MTLBuffer> _vertexBuffer;
    NSUInteger _numVertices;
    
    id<MTLTexture> _texture;
}

-(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view;
{
    self = [super init];
    if(self)
    {
        NSError* error = NULL;
        
        _device = view.device;
        
        u32 sz = 0;
        u8* imgData = Platform.slurp("Image", &sz);
        int w, h, channels;
        
        u8* pixelData = stbi_load_from_memory(imgData, (int)sz, &w, &h, &channels, 4);
        
        MTLTextureDescriptor* textureDescriptor = [[MTLTextureDescriptor alloc] init];
        
        textureDescriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
        
        textureDescriptor.width = w;
        textureDescriptor.height = h;
        
        _texture = [_device newTextureWithDescriptor:textureDescriptor];
        NSUInteger bytesPerRow = 4 * w;
        MTLRegion region = {
            {0, 0, 0},
            {w, h, 1}
        };
        
        [_texture replaceRegion:region mipmapLevel:0 withBytes:pixelData bytesPerRow:bytesPerRow];
        
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        
        id<MTLFunction> uiVertexFunction = [defaultLibrary newFunctionWithName:@"uiVertexShader"];
        id<MTLFunction> uiFragmentFunction = [defaultLibrary newFunctionWithName:@"uiFragmentShader"];
        
        MTLRenderPipelineDescriptor* pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"UI Pipeline";
        pipelineStateDescriptor.vertexFunction = uiVertexFunction;
        pipelineStateDescriptor.fragmentFunction = uiFragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
        
        _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                 error:&error];
        
        if (!_pipelineState) {
            NSLog(@"Failed to create pipeline state, error %@", error);
            return nil;
        }
      
        NSData* vertexData = [Renderer generateVertexData];
        
        _vertexBuffer = [_device newBufferWithLength:vertexData.length options:MTLResourceStorageModeShared];
        
        memcpy(_vertexBuffer.contents, vertexData.bytes, vertexData.length);
        _numVertices = vertexData.length / sizeof(UIVertex);

        _commandQueue = [_device newCommandQueue];
    }

    return self;
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";


    /// Delay getting the currentRenderPassDescriptor until we absolutely need it to avoid
    ///   holding onto the drawable and blocking the display pipeline any longer than necessary
    MTLRenderPassDescriptor* renderPassDescriptor = view.currentRenderPassDescriptor;

    if(renderPassDescriptor != nil) {

        /// Final pass rendering code here

        id <MTLRenderCommandEncoder> renderEncoder =
        [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";

        [renderEncoder setViewport:(MTLViewport){0.0, 0.0, _viewport.x, _viewport.y, -1.0, 1.0}];
        
        [renderEncoder setRenderPipelineState:_pipelineState];
        
        [renderEncoder setVertexBuffer:_vertexBuffer
                                offset:0
                               atIndex:UIVertexInputIndexVertices];
        
        [renderEncoder setVertexBytes:&_viewport
                               length:sizeof(_viewport)
                              atIndex:UIVertexInputViewportSize];
        
        [renderEncoder setFragmentTexture:_texture atIndex:UITextureIndexBaseColor];
        
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:_numVertices];
        
        [renderEncoder endEncoding];

        [commandBuffer presentDrawable:view.currentDrawable];
    }

    [commandBuffer commit];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    _viewport.x = size.width;
    _viewport.y = size.height;
}

+ (nonnull NSData *)generateVertexData
{
    const UIVertex quadVertices[] =
    {
        // Pixel positions, RGBA colors
        { { -20,   20 },    { 1, 0, 0, 1 }, { 0.0f, 1.0f } },
        { {  20,   20 },    { 0, 0, 1, 1 }, { 1.0f, 1.0f }  },
        { { -20,  -20 },    { 0, 1, 0, 1 }, { 0.0f, 0.0f }  },
        
        { {  20,  -20 },    { 1, 0, 0, 1 }, { 1.0f, 0.0f }  },
        { { -20,  -20 },    { 0, 1, 0, 1 }, { 0.0f, 0.0f }  },
        { {  20,   20 },    { 0, 0, 1, 1 }, { 1.0f, 1.0f }  },
    };
    const NSUInteger NUM_COLUMNS = 25;
    const NSUInteger NUM_ROWS = 15;
    const NSUInteger NUM_VERTICES_PER_QUAD = sizeof(quadVertices) / sizeof(UIVertex);
    const float QUAD_SPACING = 50.0;
    
    NSUInteger dataSize = sizeof(quadVertices) * NUM_COLUMNS * NUM_ROWS;
    NSMutableData *vertexData = [[NSMutableData alloc] initWithLength:dataSize];
    
    UIVertex* currentQuad = vertexData.mutableBytes;
    
    for(NSUInteger row = 0; row < NUM_ROWS; row++)
    {
        for(NSUInteger column = 0; column < NUM_COLUMNS; column++)
        {
            vector_float2 upperLeftPosition;
            upperLeftPosition.x = ((-((float)NUM_COLUMNS) / 2.0) + column) * QUAD_SPACING + QUAD_SPACING/2.0;
            upperLeftPosition.y = ((-((float)NUM_ROWS) / 2.0) + row) * QUAD_SPACING + QUAD_SPACING/2.0;
            
            memcpy(currentQuad, &quadVertices, sizeof(quadVertices));
            
            for (NSUInteger vertexInQuad = 0; vertexInQuad < NUM_VERTICES_PER_QUAD; vertexInQuad++)
            {
                currentQuad[vertexInQuad].position += upperLeftPosition;
            }
            
            currentQuad += 6;
        }
    }
    return vertexData;
}


#pragma mark Matrix Math Utilities

matrix_float4x4 matrix4x4_translation(float tx, float ty, float tz)
{
    return (matrix_float4x4) {{
        { 1,   0,  0,  0 },
        { 0,   1,  0,  0 },
        { 0,   0,  1,  0 },
        { tx, ty, tz,  1 }
    }};
}

static matrix_float4x4 matrix4x4_rotation(float radians, vector_float3 axis)
{
    axis = vector_normalize(axis);
    float ct = cosf(radians);
    float st = sinf(radians);
    float ci = 1 - ct;
    float x = axis.x, y = axis.y, z = axis.z;

    return (matrix_float4x4) {{
        { ct + x * x * ci,     y * x * ci + z * st, z * x * ci - y * st, 0},
        { x * y * ci - z * st,     ct + y * y * ci, z * y * ci + x * st, 0},
        { x * z * ci + y * st, y * z * ci - x * st,     ct + z * z * ci, 0},
        {                   0,                   0,                   0, 1}
    }};
}

matrix_float4x4 matrix_perspective_right_hand(float fovyRadians, float aspect, float nearZ, float farZ)
{
    float ys = 1 / tanf(fovyRadians * 0.5);
    float xs = ys / aspect;
    float zs = farZ / (nearZ - farZ);

    return (matrix_float4x4) {{
        { xs,   0,          0,  0 },
        {  0,  ys,          0,  0 },
        {  0,   0,         zs, -1 },
        {  0,   0, nearZ * zs,  0 }
    }};
}

@end
