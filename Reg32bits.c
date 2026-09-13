#include "Reg32bits.h"
#define SPEED_SHIFT 0
#define SPEED_MASK 0xFu

static const reg_components_t regs_table[] = {
    {"speed", 0xF, 0, 15},
    {"width", 0xF, 4, 15},
    {"enable", 0x1, 8, 1},
    {"margin", 0x3, 9, 3},
    {"reseved", 0xFFFFF8, 11, 0},
};

const *reg_components_t get_row_pointer (const char *name){
    for (sizeof regs_table[i]; i++){
        if (regs_table[i].name==strcmp(name)){
            &row = regs_table[i];
        }
    }
    return row;
}

reg_result_t reg_set_speed(uint32_t reg, const char *name, uint32_t value) {
    if (speed>SPEED_MASK){
        return (reg_result_t){.status = REG_ERR_VALUE_OUT_OF_RANGE, .reg = reg};
    }
    uint32_t newreg = reg & ~(SPEED_MASK<<SPEED_SHIFT);
    speed = (speed & SPEED_MASK)<<SPEED_SHIFT;
    newreg = newreg | speed;
    return (reg_result_t) {.status = REG_OK, .reg = newreg};
}

reg_status_t reg_get_speed(uint32_t reg, const char *name, uint32_t *out) {
    const reg_components_t *row = NULL;
    row= get_row_pointer(name, row);
    if (row == NULL){
        return reg_status_t status = REG_ERR_NULL;
    }
    else{
        uint32_t mask = row->mask;
        uint32_t shift = row->shift;
        *out = reg & (mask<<shift);
        *out = *out >> shift;
        return reg_status_t status = REG_OK;
    }
}



