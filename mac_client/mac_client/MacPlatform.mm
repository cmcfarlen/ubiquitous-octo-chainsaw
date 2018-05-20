//
//  MacPlatform.m
//  mac_client
//
//  Created by Chris McFarlen on 5/20/18.
//  Copyright © 2018 Chris McFarlen. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "platform.h"

u8* slurp(const char* resource, u32* size)
{
    NSError * error;
    NSURL *fileLocation = [[NSBundle mainBundle] URLForResource:[[NSString alloc] initWithUTF8String:resource]
                                                       withExtension:@"tga"];
    NSData *fileData = [[NSData alloc] initWithContentsOfURL:fileLocation
                                                     options:0x0
                                                       error:&error];
    
    u8* result = (u8*)malloc(fileData.length);
    
    memcpy(result, fileData.bytes, fileData.length);
    *size = fileData.length;
    return result;
}

void log(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    NSLogv([[NSString alloc] initWithUTF8String:fmt], args);
    va_end(args);
}

void* allocateMemory(size_t size)
{
    return malloc(size);
}

u32 pageSize()
{
    return 4096;
}

u32 processorCount()
{
    return 4;
}

platform_api Platform = {.log = log,
                         .slurp = slurp,
                         .allocateMemory = allocateMemory,
                         .pageSize = pageSize,
                         .processorCount = processorCount};
