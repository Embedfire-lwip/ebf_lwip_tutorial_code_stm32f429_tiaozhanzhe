#ifndef _CJSON_PROCESS_H_
#define _CJSON_PROCESS_H_
#include "cJSON.h"
#include "stdint.h"

typedef struct cJSON *cJSON_t;

typedef struct cJSON_Item {
	struct cJSON_Item *next,*prev;	
  uint8_t type;
	char *name;		
	union {
		double valuedouble;
		char *valuestring;
		struct cJSON_Item *child;
	};
}cJSON_Item;

//typedef enum {
//  cJSON_Str = 0,
//  cJSON_Num,
//  cJSON_Double,
//  number_of_type, /* 触发回调的事件 */
//}cJSON_Type;



#endif

