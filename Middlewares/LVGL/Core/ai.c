#include"Core/ai.h"

AI_Handle aiHandle = { 0 };
#define AI_NAME1 "xinghuolite"
#define AI_NAME2 "wxlite"
#define AI_NAME3 "wxspeed" 
#define AI_NAME4 "wxtiny"


char* AI_GetResponse(const char* dialog)
{
    // printf("dialog size=%d,%s\n", strlen(dialog), dialog);
    char host[18] = "cn.apihz.cn";
    char partPath[96] = { 0 };

    /*判断用哪个AI*/
    char* aiName = NULL;
    if (aiHandle.aiIndex == AI_INDEX_XUN_FEI_LITE) aiName = AI_NAME1;
    else if (aiHandle.aiIndex == AI_INDEX_BAI_DU_LITE) aiName = AI_NAME2;
    else if (aiHandle.aiIndex == AI_INDEX_BAI_DU_SPEED) aiName = AI_NAME3;
    else if(aiHandle.aiIndex==AI_INDEX_BAI_DU_TINY) aiName = AI_NAME4;

    sprintf(partPath, "/api/ai/%s.php?id=10011472&key=a941467e768c88fd3282c39d5d41a65f&words=", aiName);


    /*拼接完整路径*/
    const uint16_t size=strlen(partPath) + strlen(dialog) + 1;
    char path[size];
    memset(path,0,size);
    sprintf(path, "%s%s", partPath, dialog);


    /*开始获取响应*/
    char* response=NULL;
    uint8_t ret= WIFI_APP_GetAPIResponse(host, path,&response);
    UART_ClearRxFlag(3); /*清除接收标志，标志着可以继续接收数据*/
    if(ret) 
    {
        return NULL;
    }

    printf("size:%d\n%s\n", strlen(response), response);


    
    /*1.先判断是否有返回结果*/
    char* p = NULL; /*临时指针*/
    p = strstr(response, "code");
    if (p == NULL)
    {
        printf("get ai response fail\n");
        return NULL;
    }

    char code[4] = { 0 };
    /*正则表达式匹配得到返回code值*/
    sscanf(p, "code\":%3[^\"],", code);
    // printf("%s\n", code);
    if (strcmp(code, "200") != 0)
    {
        printf("get ai response fail,fail code:%s\n", code);
        return NULL;
    }

    /*获取响应*/
    p = strstr(p, "msg");
    if (p == NULL)
    {
        printf("get ai response fail2\n");
        return NULL;
    }

    uint16_t len = strlen(p);
    for(uint8_t i=0;i<9;i++) p[len-1-i]='\0';
    // p[len - 1] = '\0'; /*把 } 改为 \0*/
    // p[len - 2] = '\0'; /*把 " 改为 \0*/
    
    // printf("%s\n",&p[len-20]);

    // p[len - 3] = '\0'; /*把 } 改为 \0*/
    // p[len - 4] = '\0'; /*把 " 改为 \0*/
    // p[len - 5] = '\0'; /*把 " 改为 \0*/
    // p[len - 6] = '\0'; /*把 " 改为 \0*/

    return (p + sizeof(char) * 6); /*返回数据的起始地址*/
}



/*获取返回json的一个键值对中的一个值*/
//static char* AI_GetResponse(char* str, char* key, char* value)
//{
//    char* p = NULL;
//    p = strstr(str, key);
//    if (p == NULL)
//    {
//        printf("get %s fail\n", key);
//        return str;
//    }
//
//    p = strstr(p, ":");
//    if (p == NULL)
//    {
//        printf("get %s fail\n", key);
//        return str;
//    }
//
//    //printf("---> %s\n", p);
//    sscanf(p, ":\"%15[^\"]\"", value);
//    printf("value=%s\n", value);
//
//    return p;
//}
