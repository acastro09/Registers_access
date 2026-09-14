#include <stdio.h>
#include "Reg32bits.h"
#include <assert.h>

uint32_t out;

static void test_speed_get (void){
    assert((reg_get_bits(0x00000554u, "speed", &out)==REG_OK));
    assert(out==0x4u);
    printf ("PASS: get speed\n");
}


static void test_speed_set (void){
    assert(reg_get_bits((reg_set_bits(0xFFFFFFFF,"speed", 0X5u)).reg,"speed", &out)==REG_OK);
    assert(out==0x5u);
    printf ("PASS: speed was set.\n");
    assert(reg_set_bits(0xFFFFFFFF,"speed", 5).status==REG_OK);
    printf("PASS: status is OK on speed update\n");
    assert(((reg_set_bits(0x554,"speed", 5).reg) & ~0xFu)==(0x554 & ~0xFu));
    printf ("PASS: no other bits affected when updating speed\n");

}

static void test_out_of_range_speed (void){
    assert(reg_get_bits((reg_set_bits(0xFFFFFFF5,"speed", 20)).reg,"speed", &out)==REG_OK);
    assert(out==0x5u);
    printf ("PASS: speed was NOT set.\n");
    assert((reg_set_bits(0xFFFFFFFF, "speed", 20)).status==REG_ERR_VALUE_OUT_OF_RANGE);
    printf("PASS: Not affecting other bits when modifying speed\n");

}

static void test_boundaries(void){
    assert(reg_set_bits(0x554,"speed",15).status==REG_OK);
    assert(reg_set_bits(0x554,"speed",16).status!=REG_OK);
    assert(reg_get_bits((reg_set_bits(0xFFFFFFFF,"speed", 5)).reg,"speed", &out)==REG_OK);
    assert(out==0x5u);
    printf("PASS: Speed boundaries well defined\n");
}


int main(void) {
    test_speed_get();
    test_speed_set();
    test_out_of_range_speed();
    test_boundaries();
    printf("All tests PASSED\n");
    return 0;
}