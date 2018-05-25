#ifndef _MEMORY_H_
#define _MEMORY_H_

// https://en.wikipedia.org/wiki/Region-based_memory_management

#include "platform.h"

// dynamic region
// fixed region
// temporary region
// mark/truncate
// bootstrapped allocation
//

struct memory_page
{
   memory_page* next;
   size_t size;
   char* start;
   char* one_past_end;
};

struct memory_region
{
   memory_page* first_page;
   memory_page* current_page;
   size_t page_size;
   size_t waste;
   u32 allocations;
   char* p;
};

size_t Min(size_t a, size_t b)
{
   if (a < b) {
      return a;
   }
   return b;
}

size_t Max(size_t a, size_t b)
{
   if (a > b) {
      return a;
   }
   return b;
}

memory_region* createMemoryRegion(size_t usePageSize = 0)
{
   size_t pageSize = usePageSize ? usePageSize : Platform.pageSize();
   size_t minPageSize = sizeof(memory_page) + sizeof(memory_region);

   size_t allocSize = pageSize * ((Max(pageSize, minPageSize) / pageSize) + 1);
   memory_page* firstPage = (memory_page*)Platform.allocateMemory(allocSize);

   firstPage->next = 0;
   firstPage->size = allocSize;
   firstPage->start = ((char*)firstPage) + sizeof(memory_page) + sizeof(memory_region);
   firstPage->one_past_end = ((char*)firstPage) + allocSize;

   memory_region* result = (memory_region*)(((char*)firstPage) + sizeof(memory_page));

   result->first_page = firstPage;
   result->current_page = firstPage;
   result->page_size = pageSize;
   result->waste = 0;
   result->allocations = 0;
   result->p = firstPage->start;

   return result;
}

void* allocateFromRegion(memory_region* r, size_t size, size_t = 1)
{
   void* result = 0;

   r->allocations++;

   size_t available = r->current_page->one_past_end - r->p;

   while (available < size && r->current_page->next)
   {
      r->current_page = r->current_page->next;
      r->p = r->current_page->start;
      available = r->current_page->one_past_end - r->p;
   }

   if (available >= size) {
      result = r->p;
      r->p += size;
   }
   else {
      size_t allocSize = r->page_size * (((size + sizeof(memory_page)) / r->page_size) + 1);
      memory_page* p = (memory_page*)Platform.allocateMemory(allocSize);

      p->size = allocSize;
      p->start = ((char*)p) + sizeof(memory_page);
      p->one_past_end =  ((char*)p) + allocSize;

      p->next = 0;
      r->current_page->next = p;
      r->current_page = p;
      r->p = p->start + size;
      result = p->start;
   }

   return result;
}

void truncateRegion(memory_region* r)
{
   r->allocations = 0;
   r->p = r->first_page->start;
   r->current_page = r->first_page;
}

void freeRegion(memory_region* r)
{
   memory_page* p = r->first_page;
   while (p) {
      memory_page* tmp = p;
      p = p->next;
      Platform.freeMemory(tmp);
   }
}


#endif
