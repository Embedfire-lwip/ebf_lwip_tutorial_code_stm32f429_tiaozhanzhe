#include "cJSON_Process.h"


/*******************************************************************
 *                          变量声明                               
 *******************************************************************/
static struct cJSON_Item* cJSON_Item_Head = NULL;


void Make_cJSON_Data(cJSON_Item* item)
{
  struct cJSON_Item *pass_item = cJSON_Item_Head;
  
  if(pass_item == NULL)
  {
    item->next = cJSON_Item_Head;
    item->prev = cJSON_Item_Head;
    cJSON_Item_Head = item;
  }
//  else
//  {
//    ;
//  }
}

char *makeJson()
{
    cJSON * pJsonRoot = NULL;

    pJsonRoot = cJSON_CreateObject();   /*创建项目*/
    if(NULL == pJsonRoot)
    {
        return NULL;
    }
    cJSON_AddStringToObject(pJsonRoot, "hello", "hello world"); /*添加元素  键值对*/
    cJSON_AddNumberToObject(pJsonRoot, "number", 10010);
    cJSON_AddBoolToObject(pJsonRoot, "bool", 1);

    cJSON * pSubJson = NULL;
    pSubJson = cJSON_CreateObject();    /*创建项目*/
    if(NULL == pSubJson)
    {
        cJSON_Delete(pJsonRoot);
        return NULL;
    }
    cJSON_AddStringToObject(pSubJson, "subjsonobj", "a sub json string");  /*添加元素 键值对*/
    cJSON_AddItemToObject(pJsonRoot, "subobj", pSubJson);   /*将新创建的pSubJson加入pJsonRoot列表中*/

    char * p = cJSON_Print(pJsonRoot);  /*p 指向的字符串是json格式的*/

    if(NULL == p)
    {
        //convert json list to string faild, exit
        //because sub json pSubJson han been add to pJsonRoot, so just delete pJsonRoot, if you also delete pSubJson, it will coredump, and error is : double free
        cJSON_Delete(pJsonRoot);   /*不能删除pSubJson*/
        return NULL;
    }

    cJSON_Delete(pJsonRoot);    /*删除列表   这个内存和cJSON_Print返回的内存不一样*/

    return p;
}


void test(void)
{
  cJSON_Item test;
  
  cJSON_Item test1;
  
  test.type = cJSON_Number;
  test.name = "abc";
  test.valuedouble = 123;
  
  test1.type = cJSON_String;
  test1.name = "def";
  test1.valuestring = "test1"; 
  
  test.child = &test1;
}







