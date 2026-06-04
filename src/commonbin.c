#include "LMS/commonbin.h"
#include "LMS/libms.h"

s32 LMSi_SearchBlockByName(LMS_Binary* binary, const char* blockName){
    u16 index = 0;
    if (binary->numBlocks > 0) {
        while (index < binary->numBlocks) {
            if (LMSi_MemCmp(binary->blocks[index].type, blockName, 4))
                return index;
            index++; // binary->numBlocks gets loaded before this
        }
    }
    return -1;
}