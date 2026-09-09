#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t reg = 0x00000025;
    printf("registro completo: 0x%08X\n", reg);
    printf("Primero 4 bits: %u\n", reg & 0xF);
    return 0;
}