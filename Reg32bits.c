#include <Reg32bits.h>
#define SPEED_SHIFT 0
#define SPEED_MASK 0xFu

reg_result_t reg_set_speed(uint32_t reg, uint32_t speed) {
    if (speed>SPEED_MASK){
        return (reg_result_t){.status = REG_ERR_VALUE_OUT_OF_RANGE, .reg = reg};
    }
    uint32_t newreg = reg & ~(SPEED_MASK<<SPEED_SHIFT);
    speed = (speed & SPEED_MASK)<<SPEED_SHIFT;
    newreg = newreg | speed;
    return (reg_result_t) {.status = REG_OK, .reg = newreg};
}

uint32_t reg_get_speed(uint32_t reg) {
    uint32_t speed = reg & (SPEED_MASK<<SPEED_SHIFT);
    speed = speed >> SPEED_SHIFT;
    return speed;
}



