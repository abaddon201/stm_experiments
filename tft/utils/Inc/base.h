#ifndef __BASE_H
#define __BASE_H

#include "ff.h"

#ifdef __cplusplus
extern "C" {
#endif

void baseInit();

void baseErrorHandler(const char *msg);

const char* fileSystemFault(FRESULT rc);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_H */
