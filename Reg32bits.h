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
    uint32_t reg;
} reg_result_t;

reg_result_t reg_set_speed(uint32_t reg, uint32_t speed);
uint32_t reg_get_speed(uint32_t reg);

#endif