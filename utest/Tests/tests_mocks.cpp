#include <iostream>
#include "tests_mocks.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

bool malloc_enable = true;

void* mock_malloc(size_t size){
	if(malloc_enable)
		return (void*) mock().actualCall(__func__).withParameter("size", size).returnPointerValueOrDefault(malloc(size));
	else
		return (void*) mock().actualCall(__func__).withParameter("size", size).returnPointerValueOrDefault(NULL);
}

void mock_free(void *ptr){
	if(malloc_enable && ptr != NULL)
		free(ptr);

	mock().actualCall(__func__).withPointerParameter("ptr", ptr);
}

int mock_fprintf(FILE *stream, const char *format, ...){
#if 0
	va_list args;
	va_start(args, format);
	
	auto ret = mock().actualCall(__func__).returnIntValueOrDefault(vfprintf(stream, format, args));
	
	va_end(args);
	
	return ret;
#else
	return 0;
#endif
}

void* mock_realloc(void *ptr, size_t size){
	if(malloc_enable)
		return mock().actualCall(__func__).withPointerParameter("ptr", ptr).withParameter("size", size).returnPointerValueOrDefault(realloc(ptr, size));
	else
		return mock().actualCall(__func__).withPointerParameter("ptr", ptr).withParameter("size", size).returnPointerValueOrDefault(NULL);
}