#include"ex/ai_mid.h"
#include"ui_events.h"


static lv_obj_t* AI_Mid_Add_UserDialog(const char* diglogCont);
static lv_obj_t* AI_Mid_Add_AIDialog(const char* diglogCont);

uint32_t scrollBottomY = 0; /*与AI的那个可以滚动的对话界面。它可以滚动，有最大高度。获取它的最大可滚动的高度-对话框高度*/



void AI_MID_Init()
{
    
}


void AI_MID_Handler()
{
    if (aiHandle.isAISend)
    {
        aiHandle.isAISend = 0;
        const char* text = lv_textarea_get_text(ui_ScrAI_TextArea);
        if (strlen(text) == 0) return; /*文本框内容为空的话不会发送*/
        const uint16_t len=(const uint16_t)(strlen(text)+1);

        char copyText[len];
        copyText[len-1]='\0'; /*末尾为 '\0' */

        strcpy(copyText, text);

        /*显示用户发送的内容*/
        AI_Mid_Add_UserDialog(text);
        lv_textarea_set_text(ui_ScrAI_TextArea, "");

        /*显示AI等待响应*/
        lv_obj_t* label=AI_Mid_Add_AIDialog("AI等待响应中...");

        /*刷新一下UI*/
        ML_RefreshUI();

        /*开始等待AI响应*/
        char* p = NULL;
        p = AI_GetResponse(copyText);
        if (p == NULL)
        {
            printf("get ai response fail\n");
            lv_label_set_text(label,"AI响应失败,请再次发送");
            WIFI_APP_EXIT_SendData(); /*关闭服务器并发送+++ 退出透传模式*/
            
            return;
        }
        WIFI_APP_EXIT_SendData(); /*关闭服务器并发送+++ 退出透传模式*/


        lv_obj_del(label); /*删除该标签*/
        AI_Mid_Add_AIDialog(p); /*响应AI对话*/

        
    }
    else if (aiHandle.isSwitchAI)
    {
        aiHandle.isSwitchAI = 0;
        //printf("dd:%d\n", lv_dropdown_get_selected(ui_ScrAI_Dropdown));
        aiHandle.aiIndex = lv_dropdown_get_selected(ui_ScrAI_Dropdown);
    }
}


/*添加自己的对话*/
static lv_obj_t* AI_Mid_Add_UserDialog(const char* diglogCont)
{
    ui_ScrAI_LabeSendDialog1 = lv_label_create(ui_ScrAI_PanelBody);
    lv_obj_set_width(ui_ScrAI_LabeSendDialog1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ScrAI_LabeSendDialog1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_ScrAI_LabeSendDialog1, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_ScrAI_LabeSendDialog1, diglogCont);
    lv_obj_set_style_text_color(ui_ScrAI_LabeSendDialog1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ScrAI_LabeSendDialog1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_ScrAI_LabeSendDialog1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ScrAI_LabeSendDialog1, lv_color_hex(0x2655FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScrAI_LabeSendDialog1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_ScrAI_LabeSendDialog1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_ScrAI_LabeSendDialog1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_ScrAI_LabeSendDialog1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_ScrAI_LabeSendDialog1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ScrAI_LabeSendDialog1, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);


    /*限制用户发送文本框的宽度，不超过panel宽的0.8倍*/
    lv_obj_update_layout(ui_ScrAI_PanelBody);
    uint16_t panelWidth = lv_obj_get_width(ui_ScrAI_PanelBody);
    if (lv_obj_get_width(ui_ScrAI_LabeSendDialog1) > 0.8f * panelWidth)
    {
        lv_obj_set_width(ui_ScrAI_LabeSendDialog1,0.8f*panelWidth);
        lv_obj_update_layout(ui_ScrAI_PanelBody);
    }

    /*滑到底部*/
    int16_t toBottom=(int16_t)lv_obj_get_scroll_bottom(ui_ScrAI_PanelBody); /*当前位置到整个滑动区间的底部的距离像素大小*/
    if (toBottom > 0)
    {
        scrollBottomY += toBottom;
        lv_obj_scroll_to_y(ui_ScrAI_PanelBody, scrollBottomY, LV_ANIM_ON);
    }

    return ui_ScrAI_LabeSendDialog1;
}


/*添加AI的对话*/
static lv_obj_t* AI_Mid_Add_AIDialog(const char* diglogCont)
{
    ui_ScrAI_LabeAIDialog1 = lv_label_create(ui_ScrAI_PanelBody);
    lv_obj_set_width(ui_ScrAI_LabeAIDialog1, lv_pct(100));
    lv_obj_set_height(ui_ScrAI_LabeAIDialog1, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_ScrAI_LabeAIDialog1, lv_pct(0));
    lv_obj_set_y(ui_ScrAI_LabeAIDialog1, lv_pct(15));
    lv_label_set_text(ui_ScrAI_LabeAIDialog1,diglogCont);
    lv_obj_set_style_text_color(ui_ScrAI_LabeAIDialog1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_ScrAI_LabeAIDialog1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_ScrAI_LabeAIDialog1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ScrAI_LabeAIDialog1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScrAI_LabeAIDialog1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_ScrAI_LabeAIDialog1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_ScrAI_LabeAIDialog1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_ScrAI_LabeAIDialog1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_ScrAI_LabeAIDialog1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_ScrAI_LabeAIDialog1, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_update_layout(ui_ScrAI_PanelBody);


    /*滑到底部*/
    int16_t toBottom = (int16_t)lv_obj_get_scroll_bottom(ui_ScrAI_PanelBody); /*当前位置到整个滑动区间的底部的距离像素大小*/
    if (toBottom > 0)
    {
        scrollBottomY += toBottom;
        lv_obj_scroll_to_y(ui_ScrAI_PanelBody, scrollBottomY, LV_ANIM_ON);
    }

    return ui_ScrAI_LabeAIDialog1;
}
