#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "types.h"
#include "platform.h"

struct memory_page
{
   memory_page* next;
   size_t used;  // includes the size of this page header
   size_t max;
   char data[1]; // this needs to align on a 64 byte boundary?
};

struct memory_region
{
   memory_page* current;
   size_t pageSize;
   u32 allocations;
   u32 pageCount;
   size_t totalCapacity;
};

memory_region* createMemoryRegion()
{
   size_t pageSize = Platform.pageSize();
   memory_page* firstPage = (memory_page*)Platform.allocateMemory(pageSize);
   memory_region* result = (memory_region*)firstPage->data;

   result->current = firstPage;
   result->pageSize = pageSize;
   result->allocations = 0;
   result->pageCount = 1;
   result->totalCapacity = pageSize;

   firstPage->used = sizeof(memory_page) + sizeof(memory_region);
   firstPage->max = pageSize;
   firstPage->next = 0;

   return result;
}

void* allocateFromRegion(memory_region* r, size_t size, size_t = 1)
{
   void* result = 0;

   r->allocations++;

   for (memory_page* p = r->current; p; p = p->next) {
      size_t available = p->max - p->used;
      if (available >= size) {
         result = p->data + p->used;
         p->used += size;

         return result;
      }
   }

   size_t allocSize = r->pageSize * ((size / r->pageSize) + 1);
   memory_page* p = (memory_page*)Platform.allocateMemory(allocSize);
   p->max = allocSize;
   p->used = sizeof(memory_page) + size;
   p->next = r->current;
   r->current = p;
   r->pageCount++;
   r->totalCapacity += allocSize;

   result = p->data;

   return result;
}

void truncateRegion(memory_region* r)
{
   r->allocations = 0;
   for (memory_page* p = r->current; p; p = p->next) {
      p->used = sizeof(memory_page);
   }
}

void freeRegion(memory_region* r)
{
   for (memory_page* p = r->current; p; p = p->next) {
      Platform.freeMemory(p);
   }
}


#endif
