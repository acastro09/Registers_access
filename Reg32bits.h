/*Public decalration, it confirms the element exists and its form. It does not say how it works*/
#ifndef REG32BITS_H
#define REG32BITS_H
#include <stdint.h>

typedef enum {
    REG_OK = 0,
    REG_ERR_VALUE_OUT_OF_RANGE,
    REG_ERR_NULL,
    REG_ERR_NOT_IN_TABLE,
    REG_ERR_NO_PROPER_NAME,
} reg_status_t;

typedef struct {
    reg_status_t status;
    uint32_t reg;
} reg_result_t;

typedef struct {
    const char *name;
    uint32_t mask;
    int shift;

} reg_components_t;


reg_result_t reg_set_bits(uint32_t reg, const char *name, uint32_t value);
reg_status_t reg_get_bits(uint32_t reg, const char *name, uint32_t *out);

#endif