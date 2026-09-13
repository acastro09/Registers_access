#include "Reg32bits.h"
#define SPEED_SHIFT 0
#define SPEED_MASK 0xFu
#include <string.h>

static const reg_components_t regs_table[] = {
    {"speed", 0xF, 0, 15},
    {"width", 0xF, 4, 15},
    {"enable", 0x1, 8, 1},
    {"margin", 0x3, 9, 3},
    {"reserved", 0xFFFFF8, 11, 0},
};

static const reg_components_t *get_row_pointer (const char *name){
    const reg_components_t *row = NULL;
    for (size_t i = 0; i< sizeof regs_table/sizeof regs_table[0]; i++){
        if(strcmp(regs_table[i].name,name)==0){
            row = &regs_table[i];
            break;
        }
    }
    return row;
}

reg_result_t reg_set_bits(uint32_t reg, const char *name, uint32_t value) {
    if (name== NULL){
        return (reg_result_t){.status = REG_ERR_NO_PROPER_NAME, .reg = reg};
    }
    const reg_components_t *row = NULL;
    row= get_row_pointer(name);
    if (row == NULL){
        return (reg_result_t){.status = REG_ERR_NOT_IN_TABLE , .reg = reg};
    }
    else{
        if (value > row->highest_value){
            return (reg_result_t){.status = REG_ERR_VALUE_OUT_OF_RANGE, .reg = reg};
    }   
        uint32_t mask = row->mask;
        uint32_t shift = row->shift;
        uint32_t newreg = reg & ~(mask<<shift);
        value = (value & mask)<<shift;
        newreg = newreg | value;
        return (reg_result_t) {.status = REG_OK, .reg = newreg};
    }

}

reg_status_t reg_get_bits(uint32_t reg, const char *name, uint32_t *out) {
    if (out == NULL){
        return REG_ERR_NULL;
    }
    if (name== NULL){
        return REG_ERR_NO_PROPER_NAME;
    }
    const reg_components_t *row = NULL;
    row= get_row_pointer(name);
    if (row == NULL){
        return REG_ERR_NOT_IN_TABLE;
    }
    else{
        uint32_t mask = row->mask;
        uint32_t shift = row->shift;
        *out = reg & (mask<<shift);
        *out = *out >> shift;
        return REG_OK;
    }
}



