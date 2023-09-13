#ifndef __TEST_MOCKS_H__
#define __TEST_MOCKS_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void* mock_malloc(size_t size);
void mock_free(void *ptr);
int mock_fprintf(FILE *stream, const char *format, ...);
void* mock_realloc(void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif //__TEST_MOCKS_H__