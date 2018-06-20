//
//  Renderer.m
//  mac_client
//
//  Created by Chris McFarlen on 5/20/18.
//  Copyright © 2018 Chris McFarlen. All rights reserved.
//

#import <simd/simd.h>
#import <ModelIO/ModelIO.h>
#include <CoreServices/CoreServices.h>
#include <dlfcn.h>
#include <mach/mach_time.h>

#import "Renderer.h"

#include "platform.h"
#include "font.h"
#include "game.h"
#include "matrix.h"
#include "vertex_buffer.h"

struct metal_vertex_buffer
{
   vertex_buffer b;

   // metal stuff
   id <MTLBuffer> buffers[2];
   MTLVertexDescriptor* vao;
   id <MTLRenderCommandEncoder> renderEncoder;
};

f64 now()
{
   // I've been told this returns nanoseconds;
   u64 n = mach_absolute_time();

   return n / 1000000000.0;
}

void initVertexBuffer(metal_vertex_buffer* vb, u32 max, id <MTLDevice> d)
{

   vertex_buffer* b = &vb->b;
   b->max = max;
   b->vcnt = 0;
   b->icnt = 0;

   b->p = vb;

   vb->buffers[0] = [d newBufferWithLength:max*sizeof(vertex) options:MTLResourceStorageModeShared];
   vb->buffers[1] = [d newBufferWithLength:max*6*sizeof(u32) options:MTLResourceStorageModeShared];

   b->vertices = (vertex*)vb->buffers[0].contents;
   b->indices = (u32*)vb->buffers[1].contents;

   vb->vao = [[MTLVertexDescriptor alloc] init];
   // P
   vb->vao.attributes[0].format = MTLVertexFormatFloat3;
   vb->vao.attributes[0].offset = 0;
   vb->vao.attributes[0].bufferIndex = 0;
   // T
   vb->vao.attributes[1].format = MTLVertexFormatFloat2;
   vb->vao.attributes[1].offset = 3 * sizeof(f32);
   vb->vao.attributes[1].bufferIndex = 0;
   // C
   vb->vao.attributes[2].format = MTLVertexFormatFloat4;
   vb->vao.attributes[2].offset = 5 * sizeof(f32);
   vb->vao.attributes[2].bufferIndex = 0;
   // N
   vb->vao.attributes[3].format = MTLVertexFormatFloat3;
   vb->vao.attributes[3].offset = 9 * sizeof(f32);
   vb->vao.attributes[3].bufferIndex = 0;

   vb->vao.layouts[0].stride = sizeof(vertex);
   vb->vao.layouts[0].stepRate = 1;
   vb->vao.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
}

void drawBufferElements(vertex_buffer* tvb, PrimitiveType prim)
{
   metal_vertex_buffer* mvb = (metal_vertex_buffer*)tvb->p;

   [mvb->renderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                  indexCount:tvb->icnt
                                   indexType:MTLIndexTypeUInt32
                                 indexBuffer:mvb->buffers[1]
                           indexBufferOffset:0];
}

void drawBuffer(vertex_buffer* tvb, PrimitiveType prim)
{
}


@implementation Renderer
{
    id <MTLDevice> _device;
    id <MTLCommandQueue> _commandQueue;
    id <MTLRenderPipelineState> _pipelineState;


    vector_uint2 _viewport;

    metal_vertex_buffer _vbuffer;

    mat4 _proj;

    id<MTLTexture> _texture;

    screen_font* _fontData;

    bool _reloadFlag;
    FSEventStreamRef stream;

    // game API
    void* code_handle;
    game_api game;
    game_state state;
    game_input input;

    f64 last_t;
}

static void filewatch_callback(ConstFSEventStreamRef streamRef,
                               void *clientCallBackInfo,
                               size_t numEvents,
                               void *eventPaths,
                               const FSEventStreamEventFlags eventFlags[],
                               const FSEventStreamEventId eventIds[])
{

    NSLog(@"Got filewatch callback");
    [(__bridge Renderer*)clientCallBackInfo codeNeedsToReload];

}

-(void)codeNeedsToReload
{
   _reloadFlag = true;
}

-(void)reloadCode
{
   const char* path = "/Users/cmcfarlen/projects/ubiquitous-octo-chainsaw/build/game.dylib";

   if (code_handle) {
      dlclose(code_handle);
   }

   code_handle = dlopen(path, RTLD_NOW);

   if (!code_handle) {
      NSLog(@"Failed to open %s, error %s", path,  dlerror());
   }

   game.InitializeGame = (InitializeGame_t)dlsym(code_handle, "InitializeGame");
   game.UpdateGameState = (UpdateGameState_t)dlsym(code_handle, "UpdateGameState");

   if (game.InitializeGame == 0)
   {
      NSLog(@"Error loading game init code.");
   }

   if (game.UpdateGameState == 0)
   {
      NSLog(@"Error loading game update code.");
   }
}

-(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view
{
    self = [super init];
    if(self)
    {

        // Load game code and init game
        [self reloadCode];

        last_t = now();

        state.Platform = Platform;
        game.InitializeGame(&state);

        NSError* error = NULL;

        // TODO: figure out how to find the directory to use (relative?)
        NSString* cwd = [[NSString alloc] initWithUTF8String:"/Users/cmcfarlen/projects/ubiquitous-octo-chainsaw/build"];

        CFStringRef watchpath = (__bridge CFStringRef)cwd;
        NSLog(@"watch path: %@", watchpath);
        CFArrayRef watcharray = CFArrayCreate(NULL, (const void**)&watchpath, 1, NULL);
        CFRunLoopRef runloop = CFRunLoopGetCurrent();
        FSEventStreamContext ctx = {0, (__bridge void *)(self), 0, 0, 0};

        stream = FSEventStreamCreate(NULL, &filewatch_callback, &ctx, watcharray, kFSEventStreamEventIdSinceNow, 0.0, kFSEventStreamCreateFlagNone);

        CFRelease(watcharray);

        FSEventStreamScheduleWithRunLoop(stream, runloop, kCFRunLoopDefaultMode);

        FSEventStreamStart(stream);

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


    if (_reloadFlag) {
       NSLog(@"Code needs to reload!!!!");

       [self reloadCode];

       _reloadFlag = false;
    }

    vertex_buffer *vb = &_vbuffer.b;
    NSUInteger pressedButtons = [NSEvent pressedMouseButtons];

    NSPoint mouseP = [[view window] mouseLocationOutsideOfEventStream];
    f64 now_t = now();
    f64 dt = now_t - last_t;

    last_t = now_t;

    input.mouse_p = Vec2(mouseP.x, mouseP.y);
    input.dt = (f32)dt;

    game.UpdateGameState(&state, &input);


    /// Delay getting the currentRenderPassDescriptor until we absolutely need it to avoid
    ///   holding onto the drawable and blocking the display pipeline any longer than necessary
    MTLRenderPassDescriptor* renderPassDescriptor = view.currentRenderPassDescriptor;

    if(renderPassDescriptor != nil) {

       view.clearColor = (MTLClearColor){0.2f, 0.2f, 0.2f, 1.0f};

       /// Final pass rendering code here
       id <MTLRenderCommandEncoder> renderEncoder =
          [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
       renderEncoder.label = @"MyRenderEncoder";

       _vbuffer.renderEncoder = renderEncoder;

       screen_font_size* fs = findFontSize(_fontData, 30);
       f32 cx = 50.0f;
       f32 cy = 50.0f;
       cx += drawString(vb, fs, cx, cy, "Hello");
       cx += drawVec2(vb, fs, cx, cy, input.mouse_p);

       cx += drawFloat(vb, fs, cx, cy, input.dt, 4);


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
                                 indexCount:_vbuffer.b.icnt
                                  indexType:MTLIndexTypeUInt32
                                indexBuffer:_vbuffer.buffers[1]
                          indexBufferOffset:0];

       _vbuffer.b.vcnt = 0;
       _vbuffer.b.icnt = 0;

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
