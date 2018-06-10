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

#include "platform.h"
#include "font.h"
#include "matrix.h"

#pragma pack(push, 1)
struct vertex
{
   vec3 P;
   vec2 T;
   vec4 C;
   vec3 N;
};
#pragma pack(pop)

struct vertex_buffer
{
   u32 max;
   u32 vcnt;
   u32 icnt;
   vertex* vertices;
   u32* indices;

   // metal stuff
   id <MTLBuffer> buffers[2];
   MTLVertexDescriptor* vao;
};

void addTestTriangle(vertex_buffer* b)
{
   u32 v = b->vcnt;
   u32 i = b->icnt;

   b->vertices[v+0].P = (vec3){-0.5f, -0.5f, 0.0f};
   b->vertices[v+1].P = (vec3){ 0.5f, -0.5f, 0.0f};
   b->vertices[v+2].P = (vec3){0.0f, 0.5f, 0.0f};

   b->indices[i+0] = v + 0;
   b->indices[i+1] = v + 1;
   b->indices[i+2] = v + 2;

   b->vcnt += 3;
   b->icnt += 3;
}

void addTextured2DQuad(vertex_buffer* b, vec2 pll, vec2 pur, vec2 tll, vec2 tur)
{
   u32 v = b->vcnt;
   u32 i = b->icnt;

   b->vertices[v+0].P = Vec3(pll); // lower-left
   b->vertices[v+0].T = tll;
   b->vertices[v+1].P = Vec3(pur.x, pll.y); // lower right
   b->vertices[v+1].T = Vec2(tur.x, tll.y);
   b->vertices[v+2].P = Vec3(pur); // upper-right
   b->vertices[v+2].T = tur;
   b->vertices[v+3].P = Vec3(pll.x, pur.y); // top left
   b->vertices[v+3].T = Vec2(tll.x, tur.y);

   b->indices[i+0] = v + 0;
   b->indices[i+1] = v + 1;
   b->indices[i+2] = v + 2;
   b->indices[i+3] = v + 0;
   b->indices[i+4] = v + 2;
   b->indices[i+5] = v + 3;

   b->vcnt += 4;
   b->icnt += 6;
}

void initVertexBuffer(vertex_buffer* b, u32 max, id <MTLDevice> d)
{
   b->max = max;
   b->vcnt = 0;
   b->icnt = 0;

   b->buffers[0] = [d newBufferWithLength:max*sizeof(vertex) options:MTLResourceStorageModeShared];
   b->buffers[1] = [d newBufferWithLength:max*6*sizeof(u32) options:MTLResourceStorageModeShared];

   b->vertices = (vertex*)b->buffers[0].contents;
   b->indices = (u32*)b->buffers[1].contents;

   b->vao = [[MTLVertexDescriptor alloc] init];
   // P
   b->vao.attributes[0].format = MTLVertexFormatFloat3;
   b->vao.attributes[0].offset = 0;
   b->vao.attributes[0].bufferIndex = 0;
   // T
   b->vao.attributes[1].format = MTLVertexFormatFloat2;
   b->vao.attributes[1].offset = 3 * sizeof(f32);
   b->vao.attributes[1].bufferIndex = 0;
   // C
   b->vao.attributes[2].format = MTLVertexFormatFloat4;
   b->vao.attributes[2].offset = 5 * sizeof(f32);
   b->vao.attributes[2].bufferIndex = 0;
   // N
   b->vao.attributes[3].format = MTLVertexFormatFloat3;
   b->vao.attributes[3].offset = 9 * sizeof(f32);
   b->vao.attributes[3].bufferIndex = 0;

   b->vao.layouts[0].stride = sizeof(vertex);
   b->vao.layouts[0].stepRate = 1;
   b->vao.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
}

@implementation Renderer
{
    id <MTLDevice> _device;
    id <MTLCommandQueue> _commandQueue;
    id <MTLRenderPipelineState> _pipelineState;


    vector_uint2 _viewport;

    vertex_buffer _vbuffer;

    mat4 _proj;

    id<MTLTexture> _texture;

    screen_font* _fontData;
}

-(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view;
{
    self = [super init];
    if(self)
    {
        NSError* error = NULL;

        _device = view.device;

        u32 w = 512;
        u32 h = 512;
        _fontData = createFont((int)w, (int)h, "monaco");

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

        [_texture replaceRegion:region mipmapLevel:0 withBytes:_fontData->textureData bytesPerRow:bytesPerRow];

        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];

        id<MTLFunction> uiVertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
        id<MTLFunction> uiFragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];

        initVertexBuffer(&_vbuffer, 512, _device);

        MTLRenderPipelineDescriptor* pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"UI Pipeline";
        pipelineStateDescriptor.vertexFunction = uiVertexFunction;
        pipelineStateDescriptor.fragmentFunction = uiFragmentFunction;
        pipelineStateDescriptor.vertexDescriptor = _vbuffer.vao;
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

       screen_font_size* fs = findFontSize(_fontData, 30);
       drawString(&_vbuffer, fs, 50.0f, 50.0f, "Hello");

//       addTestTriangle(&_vbuffer);

       id <MTLRenderCommandEncoder> renderEncoder =
        [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";

        double x = (double)_viewport.x;
        double y = (double)_viewport.y;
        [renderEncoder setViewport:(MTLViewport){0.0, 0.0, x, y, -1.0, 1.0}];

        [renderEncoder setRenderPipelineState:_pipelineState];

        [renderEncoder setVertexBuffer:_vbuffer.buffers[0]
                                offset:0
                               atIndex:0];

        [renderEncoder setVertexBytes:&_proj
                               length:sizeof(_proj)
                              atIndex:1];

        [renderEncoder setFragmentTexture:_texture atIndex:0];

        [renderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                 indexCount:_vbuffer.icnt
                                  indexType:MTLIndexTypeUInt32
                                indexBuffer:_vbuffer.buffers[1]
                          indexBufferOffset:0];

        _vbuffer.vcnt = 0;
        _vbuffer.icnt = 0;

        [renderEncoder endEncoding];

        [commandBuffer presentDrawable:view.currentDrawable];
    }

    [commandBuffer commit];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
   _viewport.x = size.width;
   _viewport.y = size.height;
   _proj = Ortho2D(0.0f, (f32)_viewport.x, 0.0f, (f32)_viewport.y, -1.0f, 1.0f);
}

@end
