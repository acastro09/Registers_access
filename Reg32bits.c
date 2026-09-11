#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define SPEED_SHIFT 0
#define SPEED_MASK 0xFu

typedef enum {
    REG_OK = 0,
    REG_ERR_VALUE_OUT_OF_RANGE,
    REG_ERR_NULL
} set_status_t;

typedef struct {
    set_status_t status;
    uint32_t reg;
} speed_struct_t;

speed_struct_t reg_set_speed(uint32_t reg, uint32_t speed) {
    /*if (reg==NULL | speed==NULL){
        return REG_ERR_NULL;
    }*/
    speed_struct_t result;
    result.reg = reg;
    if (speed>15){
        result.status = REG_ERR_VALUE_OUT_OF_RANGE;
        result.reg = reg;
        return result;
    }
    result.reg = result.reg & ~(SPEED_MASK<<SPEED_SHIFT);
    speed = (speed & SPEED_MASK)<<SPEED_SHIFT;
    result.reg = result.reg | speed;
    result.status = REG_OK;
    return result;
}

uint32_t reg_get_speed(uint32_t reg) {
    uint32_t speed = reg & (SPEED_MASK<<SPEED_SHIFT);
    speed = speed >> SPEED_SHIFT;
    return speed;
}


int main(void) {
    speed_struct_t r;
    r.reg = 0x00000544;
    uint32_t speed = reg_get_speed(r.reg);
    printf("La velocidad es: %X\n", speed);
    r = reg_set_speed(r.reg, 5);
    if (r.status == 0){
        printf("el nuevo registro es: 0x%08X\n", r.reg);
    }
    else {
        printf("Error on the speed value. Overflow. No changes were done\n");
    }
    speed = reg_get_speed(r.reg);
    printf("La velocidad es: %X\n", speed);
    assert((reg_set_speed(0xFFFFFFFF, 5)).reg==0xFFFFFFF5);
    assert(reg_get_speed((reg_set_speed(0xFFFFFFFF, 5)).reg)==5);
    assert(((reg_set_speed(0x554, 5).reg) & ~0xFu)==(0x555 & ~0xFu));
    return 0;
}
