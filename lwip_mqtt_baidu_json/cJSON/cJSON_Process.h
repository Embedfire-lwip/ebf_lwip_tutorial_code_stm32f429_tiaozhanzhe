#ifndef _CJSON_PROCESS_H_
#define _CJSON_PROCESS_H_
#include "cJSON.h"
#include "stdint.h"


#define   NAME          "name"     
#define   NUMBER        "number"  
#define   BOOL          "bool" 

#define   DEFAULT_NAME          "fire"     
#define   DEFAULT_NUMBER        123.3  
#define   DEFAULT_BOOL          1 


cJSON* cJSON_Data_Init(void);
char* cJSON_Update(const cJSON * const object,const char * const string,void * d);

#endif

