#include "LMS/msgfile.h"
#include "LMS/libms.h"

typedef struct TextHeader {
    u32 numMessages;
} TextHeader;

LMS_MessageBinary* LMS_InitMessage(const void* data){
    LMS_MessageBinary* msg = (LMS_MessageBinary*)LMSi_Malloc(sizeof(LMS_MessageBinary));
    msg->common.data = data;
    LMSi_AnalyzeMessageBinary(&msg->common, "MsgStdBn");
    msg->labelsIndex = LMSi_SearchBlockByName(&msg->common, "LBL1");
    msg->textIndex = LMSi_SearchBlockByName(&msg->common, "TXT2");
    msg->attributesIndex = LMSi_SearchBlockByName(&msg->common, "ATR1");
    msg->styleIndex = LMSi_SearchBlockByName(&msg->common, "TSY1");
    return msg;
}

void LMS_CloseMessage(LMS_MessageBinary* binary){
    if (binary->common.blocks)
        LMSi_Free(binary->common.blocks);
    LMSi_Free(binary);
}

int LMS_GetText(LMS_MessageBinary* binary, int index){
    if (binary->textIndex == -1)
        return NULL;
        
    s32* txt2Data = (s32*)binary->common.blocks[binary->textIndex].data;
    if (txt2Data[0] <= index)
        return NULL;

    u32 offset = 4 + (index << 2);
    return (s32)txt2Data + *(s32*)((u32)txt2Data + offset);
}

int LMS_GetTextNum(LMS_MessageBinary* binary){
    if (binary->textIndex == -1)
        return -1;
    return ((TextHeader*)binary->common.blocks[binary->textIndex].data)->numMessages;
}

int LMS_GetAttrNum(LMS_MessageBinary* binary, int index) {
    s32 attrNum = binary->styleIndex;
    if (attrNum == -1) {
        return 0xFFFFFFFD;
    }
    s32* data = (s32*)binary->common.blocks[attrNum].data;
    return data[index];
}