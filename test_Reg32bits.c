#include <stdio.h>
#include "Reg32bits.h"
#include <assert.h>



static void test_speed_get (void){
    uint32_t out=0xDEADBEEF;
    assert((reg_get_bits(0x00000554u, "speed", &out)==REG_OK));
    assert(out==0x4u);
    printf ("PASS: get speed\n");
    assert((reg_get_bits(0x00000554u,"speed", NULL)==REG_ERR_NULL));
    printf ("PASS: NULL output pointer handled\n");

}


static void test_speed_set (void){
    uint32_t out=0xDEADBEEF;
    assert(reg_get_bits((reg_set_bits(0xFFFFFFFF,"speed", 0X5u)).reg,"speed", &out)==REG_OK);
    assert(out==0x5u);
    printf ("PASS: speed was set.\n");
    assert(reg_set_bits(0xFFFFFFFF,"speed", 5).status==REG_OK);
    printf("PASS: status is OK on speed update\n");
    assert(((reg_set_bits(0x554,"speed", 5).reg) & ~0xFu)==(0x554 & ~0xFu));
    printf ("PASS: no other bits affected when updating speed\n");

}

static void test_out_of_range_speed (void){
    uint32_t out=0xDEADBEEF;
    assert(reg_get_bits((reg_set_bits(0xFFFFFFF5,"speed", 20)).reg,"speed", &out)==REG_OK);
    assert(out==0x5u);
    printf ("PASS: speed was NOT set.\n");
    assert((reg_set_bits(0xFFFFFFFF, "speed", 20)).status==REG_ERR_VALUE_OUT_OF_RANGE);
    printf("PASS: Not affecting other bits when modifying speed\n");

}


static void test_boundaries(void){
    uint32_t out=0xDEADBEEF;
    assert(reg_set_bits(0x554,"speed",15).status==REG_OK);
    assert(reg_set_bits(0x554,"speed",16).status!=REG_OK);
    assert(reg_get_bits((reg_set_bits(0xFFFFFFFF,"speed", 5)).reg,"speed", &out)==REG_OK);
    assert(out==0x5u);
    printf("PASS: Speed boundaries well defined\n");
}


static void test_invalid_name_get(void){
    uint32_t out=0xDEADBEEF;
    assert((reg_get_bits(0xFFFFFFFF, "test", &out))==REG_ERR_NOT_IN_TABLE);
    assert(out==0xDEADBEEF);
    printf("PASS: No proper lecture since incorrect name.\n");
}


static void test_invalid_name_set(void){
    reg_result_t newreg=reg_set_bits(0xFFFFFFFF, "test", 1);
    assert(newreg.status==REG_ERR_NOT_IN_TABLE);
    assert(newreg.reg==0xFFFFFFFF);
    printf("PASS: No changes since invalid space name.\n");
}


static void test_invalid_null_name_get(void){
    uint32_t out=0xDEADBEEF;
    assert((reg_get_bits(0x00000554u, NULL, &out)==REG_ERR_NO_PROPER_NAME));
    assert(out==0xDEADBEEF);
    printf ("PASS: Not able to get values when name is null\n");
}

static void test_invalid_null_name_set(void){
    reg_result_t newreg=reg_set_bits(0xFFFFFFFF, NULL, 1);
    assert(newreg.status==REG_ERR_NO_PROPER_NAME);
    assert(newreg.reg==0xFFFFFFFF);
    printf("PASS: No changes since NULL space name.\n");
}


int main(void) {
    test_speed_get();
    test_speed_set();
    test_out_of_range_speed();
    test_boundaries();
    test_invalid_name_set();
    test_invalid_null_name_get();
    test_invalid_null_name_set();
    test_invalid_name_get();
    printf("All tests Finished\n");
    return 0;
}