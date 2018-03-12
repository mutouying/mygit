#include <stdio.h>
#include "cJSON.h"
#include <stdlib.h>

char* getcjson();
void parsejson(char* rjson);
char* getJsonArray(int size);
void parseJsonArray(char* p);

int main()
{
	char* p = getcjson();
	printf("%s\n",p);
	parsejson(p);
	p = getJsonArray(10);
	parseJsonArray(p);
	free(p);
	return 0;
}

char* getcjson()
{
	cJSON* pjson = NULL;
	pjson = cJSON_CreateObject();
	if(pjson == NULL)
		perror("create json fail!\n");
	cJSON_AddStringToObject(pjson , "姓名" , "方操");
	cJSON_AddStringToObject(pjson , "性别" , "男");
	cJSON_AddNumberToObject(pjson , "年龄" , 25);
	cJSON* sjson = NULL;
	sjson = cJSON_CreateObject();
	if(sjson == NULL)
		perror("create json fail!\n");
	cJSON_AddStringToObject(sjson , "爱好" ,"LOL");
	cJSON_AddNumberToObject(sjson , "身高" , 172);
	cJSON_AddItemToObject(pjson , "subobj" , sjson);

	char* p = cJSON_Print(pjson);
	cJSON_Delete(pjson);
	return p;
}

void parsejson(char* rjson)
{
	cJSON* pjson = cJSON_Parse(rjson);
	cJSON* subjson = cJSON_GetObjectItem(pjson,"姓名");
	printf("%s\n",subjson->valuestring);
	subjson = cJSON_GetObjectItem(pjson,"年龄");
	printf("%d\n",subjson->valueint);
	cJSON_Delete(pjson);
}

char* getJsonArray(int size)
{
	cJSON* root = cJSON_CreateArray();
	int i = 0;
	for(i ; i < size ; i++)
		cJSON_AddNumberToObject(root , "fc" , i);
	char* p = cJSON_Print(root);
	cJSON_Delete(root);
	return p;
}

void parseJsonArray(char* p)
{
	cJSON* root = cJSON_Parse(p);
	int size = cJSON_GetArraySize(root);	
	int i = 0;
	cJSON* sjson = NULL;
	for(i ; i < size ; i++)
	{
		sjson = cJSON_GetArrayItem(root , i);
		printf("%d ",sjson->valueint);
	}	
	putchar(10);
	cJSON_Delete(root);
}


