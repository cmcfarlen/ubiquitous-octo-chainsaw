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
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "platform.h"

typedef struct
{
    uint32 width;
    uint32 height;
    int channels;  // 1,2,3,4 channels
    uint8* data;
    uint32 length;
} TextureData;

typedef struct FontData
{
   u8* imgData;
   int w;
   int h;
   int firstChar;
   int lastChar;
   float sizes[10];  // Find the size you want in here and the index is the index in the baked char data
   stbtt_bakedchar cdata[10][96];
} FontData;

FontData* loadFontTexture(int w, int h)
{
   u32 size = 0;
   u8* ttfdata = Platform.slurp("monaco", "ttf", &size);
   u8* imgData = Platform.allocateMemory(w*h);
   
   FontData* result = Platform.allocateMemory(sizeof(FontData));
   result->imgData = imgData;
   result->w = w;
   result->h = h;
   result->firstChar = 32;
   result->lastChar = 96;
   result->sizes[0] = 32.0;
   stbtt_BakeFontBitmap(ttfdata,0, 32.0, imgData,w,h, 32,96, result->cdata[0]);
   
   return result;
}

stbtt_bakedchar* getBakedForSize(FontData* f, float sz)
{
   int idx = -1;
   stbtt_bakedchar* result = f->cdata[0];
   for (int i = 0; i < 10 && idx < 0; i++) {
      if (sz >= f->sizes[i]) {
         idx = i;
      }
   }
   if (idx >= 0) {
      result = f->cdata[idx];
   }
   
   return result;
}

vector_float2 drawChar(FontData* data, float size, int c, vector_float2 p, UIVertex* dest)
{
   stbtt_bakedchar* baked = getBakedForSize(data, 32.0);
   stbtt_aligned_quad q;
   float x = p[0];
   float y = p[1];
   
   stbtt_GetBakedQuad(baked, data->w, data->h, c - data->firstChar, &x, &y, &q, 1);
   vector_float2 p0 = {q.x0, q.y0};
   vector_float2 p1 = {q.x1, q.y0};
   vector_float2 p2 = {q.x1, q.y1};
   vector_float2 p3 = {q.x0, q.y1};
   
   vector_float2 t0 = {q.s0, q.t1};
   vector_float2 t1 = {q.s1, q.t1};
   vector_float2 t2 = {q.s1, q.t0};
   vector_float2 t3 = {q.s0, q.t0};
   
   vector_float4 color = {1, 0, 0, 1};
   
   dest[0].position          = p0;
   dest[0].textureCoordinate = t0;
   dest[0].color = color;
   dest[1].position          = p1;
   dest[1].textureCoordinate = t1;
   dest[1].color = color;
   dest[2].position          = p3;
   dest[2].textureCoordinate = t3;
   dest[2].color = color;
   dest[3].position          = p2;
   dest[3].textureCoordinate = t2;
   dest[3].color = color;
   dest[4].position          = p3;
   dest[4].textureCoordinate = t3;
   dest[4].color = color;
   dest[5].position          = p1;
   dest[5].textureCoordinate = t1;
   dest[5].color = color;
   
   return (vector_float2){x, y};
}

@implementation Renderer
{
    id <MTLDevice> _device;
    id <MTLCommandQueue> _commandQueue;
    id <MTLRenderPipelineState> _pipelineState;
    
    vector_uint2 _viewport;
    
    id <MTLBuffer> _vertexBuffer;
    NSUInteger _numVertices;
    
    id<MTLTexture> _texture;
   
   FontData* _fontData;
}

-(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view;
{
    self = [super init];
    if(self)
    {
        NSError* error = NULL;
        
        _device = view.device;
        
        /*
        u32 sz = 0;
        u8* imgData = Platform.slurp("Image", "tga", &sz);
        int w, h, channels;
        
        u8* pixelData = stbi_load_from_memory(imgData, (int)sz, &w, &h, &channels, 4);
        */
        int w = 512;
        int h = 512;
        _fontData = loadFontTexture(w, h);
        
        MTLTextureDescriptor* textureDescriptor = [[MTLTextureDescriptor alloc] init];
        
        textureDescriptor.pixelFormat = MTLPixelFormatR8Unorm;
        
        textureDescriptor.width = w;
        textureDescriptor.height = h;
        
        _texture = [_device newTextureWithDescriptor:textureDescriptor];
        NSUInteger bytesPerRow = w;
        MTLRegion region = {
            {0, 0, 0},
            {w, h, 1}
        };
        
        [_texture replaceRegion:region mipmapLevel:0 withBytes:_fontData->imgData bytesPerRow:bytesPerRow];
        
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
        
        id<MTLFunction> uiVertexFunction = [defaultLibrary newFunctionWithName:@"uiVertexShader"];
        id<MTLFunction> uiFragmentFunction = [defaultLibrary newFunctionWithName:@"uiFragmentShader"];
        
        MTLRenderPipelineDescriptor* pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"UI Pipeline";
        pipelineStateDescriptor.vertexFunction = uiVertexFunction;
        pipelineStateDescriptor.fragmentFunction = uiFragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
        pipelineStateDescriptor.colorAttachments[0].blendingEnabled = YES;
        pipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
        pipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
        pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
        pipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
        pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
        pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
        
        _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                 error:&error];
        
        if (!_pipelineState) {
            NSLog(@"Failed to create pipeline state, error %@", error);
            return nil;
        }
      
       // NSData* vertexData = [Renderer generateVertexData];
       NSUInteger dataSize = sizeof(UIVertex)  * 6 * 5;
       NSMutableData *vertexData = [[NSMutableData alloc] initWithLength:dataSize];
       
       UIVertex* currentQuad = vertexData.mutableBytes;
       vector_float2 p = {50, 50};
       p = drawChar(_fontData, 32.0, 'H', p, currentQuad + 0);
       p = drawChar(_fontData, 32.0, 'e', p, currentQuad + 6);
       p = drawChar(_fontData, 32.0, 'l', p, currentQuad + 12);
       p = drawChar(_fontData, 32.0, 'l', p, currentQuad + 18);
       p = drawChar(_fontData, 32.0, 'o', p, currentQuad + 24);
       
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

        view.clearColor = (MTLClearColor){0.2f, 0.2f, 0.2f, 1.0f};
        
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
        { { -200,   200 },    { 1, 0, 0, 1 }, { 0.0f, 0.0f } },
        { {  200,   200 },    { 0, 0, 1, 1 }, { 1.0f, 0.0f }  },
        { { -200,  -200 },    { 0, 1, 0, 1 }, { 0.0f, 1.0f }  },
        
        { {  200,  -200 },    { 1, 0, 0, 1 }, { 1.0f, 1.0f }  },
        { { -200,  -200 },    { 0, 1, 0, 1 }, { 0.0f, 1.0f }  },
        { {  200,   200 },    { 0, 0, 1, 1 }, { 1.0f, 0.0f }  },
    };
    const NSUInteger NUM_COLUMNS = 1;
    const NSUInteger NUM_ROWS = 1;
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
