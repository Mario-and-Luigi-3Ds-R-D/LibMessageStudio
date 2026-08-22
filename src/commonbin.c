#include "LMS/commonbin.h"
#include "LMS/libms.h"

s32 LMSi_GetHashTableIndexFromLabel(const char* label, u32 numSlots){
    u32 hash = 0;

    for (int i = 0; label[i] != '\0'; ++i)
        hash = hash * 0x492 + label[i];

    return hash % numSlots;
}

s32 LMSi_SearchBlockByName(LMS_Binary* binary, const char* blockName){
    u16 blocks = binary->numBlocks;
    for (u16 index = 0; index < binary->numBlocks; index++) {
        if (LMSi_MemCmp(binary->blocks[index].type, blockName, 4))
            return index;
        blocks = binary->numBlocks;
    }
    return -1;
}

LMS_BinaryBlock* LMSi_GetBlockInfoByName(LMS_Binary* binary, const char* name){
    for (s32 i = 0; i < binary->numBlocks; i++) {
        if (LMSi_MemCmp(binary->blocks[i].type, name, sizeof(binary->blocks[i].type))) {
            return &binary->blocks[i];
        }
    }

    return NULL;
}

void LMSi_AnalyzeMessageBinary(LMS_Binary* binary, const char* magic){
    LMSi_AnalyzeMessageHeader(binary);
    LMSi_AnalyzeMessageBlocks(binary);
}

// nonmatch
void LMSi_AnalyzeMessageHeader(LMS_Binary* binary){
    const char* data = (const char*)binary->data;

    binary->encoding = *(LMS_MessageEncoding*)&data[0x0C];
    binary->numBlocks = *(u16*)&data[0x0E];

    if (!binary->numBlocks) {
        binary->blocks = NULL;
    }
    else {
        binary->blocks = (LMS_BinaryBlock*)LMSi_Malloc(binary->numBlocks * sizeof(LMS_BinaryBlock));
    }

    binary->fileSize = *(const u32*)(data + 10);
}

// nonmatch
LMS_Binary* LMSi_AnalyzeMessageBlocks(LMS_Binary* binary){
    s32 curBlockDataOffset = 0x20;
    int numBlocks = binary->numBlocks;
    int i = 0;

    if (numBlocks <= 0){
        return binary;
    }

    for (; i < binary->numBlocks; i++){
        LMS_BinaryBlock* curBlock = &binary->blocks[i];

        curBlock->data = (s8*)binary->data + (curBlockDataOffset + 0x10);
        s32 headerEnd = curBlockDataOffset + 0x10;

        s32 off = curBlockDataOffset;
        
        curBlock->type[0] = ((s8*)binary->data)[off];
        off++;
        curBlock->type[1] = ((s8*)binary->data)[off];
        off++;
        curBlock->type[2] = ((s8*)binary->data)[off];
        off++;
        curBlock->type[3] = ((s8*)binary->data)[off];
        off++;

        s32 size = *(s32*)&((s8*)binary->data)[off];
        off += 4;
        curBlock->size = size;

        s32 nextOffset = size + headerEnd;
        nextOffset += 0xF;
        
        curBlock->unk = *(s16*)&((s8*)binary->data)[off];

        curBlockDataOffset = nextOffset & ~0xF;
    }

    return binary;
}