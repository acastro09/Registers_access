/*Public decalration, it confirms the element exists and its form. It does not say how it works*/
#ifndef REG32BITS_H
#define REG32BITS_H
#include <stdint.h>

typedef enum {
    REG_OK = 0,
    REG_ERR_VALUE_OUT_OF_RANGE,
    REG_ERR_NULL
} reg_status_t;

typedef struct {
    reg_status_t status;
    const char *name;
    uint4_t mask;
    int shift;
    int highest_value;

} reg_components_t;

static const reg_components_t regs_table[] = {
    {"speed", 0xF, 0, 15},
    {"width", 0xF, 4, 15},
    {"enable", 0x1, 8, 1},
    {"margin", 0x3, 9, 3},
    {"reseved", 0xFFFFF8, 11, 0},
};

reg_result_t reg_set_bits(uint32_t reg, const char &name, uint32_t value);
uint32_t reg_get_bits(uint32_t reg, const char &name);

#endif