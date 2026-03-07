#include"Core/main_lib.h"


Album_Handle albumHandle = { 0 };

void Album_SetImgNum(uint16_t num) { albumHandle.num = num; };
uint16_t Album_GetImgNum() {return albumHandle.num; };
void Album_SetImgIndex(uint16_t cur_index) { albumHandle.index = cur_index; };
uint16_t Album_GetImgIndex() { return albumHandle.index; };



