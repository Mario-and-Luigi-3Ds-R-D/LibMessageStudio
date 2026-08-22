#pragma once

#include "LMS/commonbin.h"
#include <stddef.h>

typedef struct{
    LMS_Binary common;
    s32 clbIndex;
    s32 clrIndex;
    s32 albIndex;
    s32 atiIndex;
    s32 aliIndex;
    s32 tggIndex;
    s32 tagIndex;
    s32 tgpIndex;
    s32 tglIndex;
    s32 sylIndex;
    s32 slbIndex;
    s32 ctiIndex;
} LMS_ProjectBinary;

LMS_ProjectBinary* LMS_InitProject(const void* data);
void LMS_CloseProject(LMS_ProjectBinary* prjBinary);

s32 LMS_SearchProjectBlockByName(LMS_ProjectBinary *prjBinary, const char* blockName);

typedef struct{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} LMS_Color;

typedef enum{
    LMS_ColorResult_NoColors = -5,
    LMS_ColorResult_ColorLabelNotFound = -2,
    LMS_ColorResult_IndexOutOfRange = -1,
    LMS_ColorResult_ColorFound
} LMS_ColorResult;

s32 LMS_GetColorNum(LMS_ProjectBinary* prjBinary);

s32 LMS_GetColorIndexByName(LMS_ProjectBinary* prjBinary, const char* name);

LMS_ColorResult LMS_GetColor(LMS_ProjectBinary* prjBinary, s32 id, LMS_Color* output);
LMS_ColorResult LMS_GetColorByName(LMS_ProjectBinary* prjBinary, const char* name, LMS_Color* output);