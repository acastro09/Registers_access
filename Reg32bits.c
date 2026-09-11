#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define SPEED_SHIFT 0
#define SPEED_MASK 0xFu

typedef enum {
    REG_OK = 0,
    REG_ERR_VALUE_OUT_OF_RANGE,
    REG_ERR_NULL
} reg_status_t;

typedef struct {
    reg_status_t status;
    uint32_t reg;
} reg_result_t;

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


int main(void) {
    
    uint32_t reg = 0x00000544;
    uint32_t speed = reg_get_speed(reg);
    printf("La velocidad es: %X\n", speed);
    reg_result_t r = reg_set_speed(reg, 5);
    if (r.status == REG_OK){
        reg = r.reg;
        printf("el nuevo registro es: 0x%08X\n", r.reg);
    }
    else {
        printf("Error on the speed value. Overflow. No changes were done\n");
    }
    speed = reg_get_speed(reg);
    printf("La velocidad es: %X\n", speed);
    uint32_t antes = reg;
    reg_result_t malo = reg_set_speed(reg, 20);
    assert(malo.status == REG_ERR_VALUE_OUT_OF_RANGE);
    assert(malo.reg ==antes);
    assert(reg_set_speed(0x554,15).status==REG_OK);
    assert(reg_set_speed(0x554,16).status!=REG_OK);
    assert((reg_set_speed(0xFFFFFFFF, 5)).reg==0xFFFFFFF5);
    assert(reg_get_speed((reg_set_speed(0xFFFFFFFF, 5)).reg)==5);
    assert(((reg_set_speed(0x554, 5).reg) & ~0xFu)==(0x554 & ~0xFu));
    return 0;
}
