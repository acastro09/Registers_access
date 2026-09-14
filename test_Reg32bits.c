#include <stdio.h>
#include "Reg32bits.h"
#include <assert.h>



static void test_bits_get (void){
    uint32_t out=0xDEADBEEF;
    assert((reg_get_bits(0x00000554u, "speed", &out)==REG_OK));
    assert(out==0x4u);
    assert((reg_get_bits(0x00000554u, "width", &out)==REG_OK));
    assert(out==0x5u);
    assert((reg_get_bits(0x00000554u, "enable", &out)==REG_OK));
    assert(out==0x1u);
    assert((reg_get_bits(0x00000554u, "margin", &out)==REG_OK));
    assert(out==0x2u);
    printf ("PASS: get bits\n");
}


static void test_bits_set (void){
    uint32_t out=0xDEADBEEF;
    assert(reg_get_bits((reg_set_bits(0xFFFFFFFF,"speed", 0x5u)).reg,"speed", &out)==REG_OK);
    assert(out==0x5u);
    assert(reg_get_bits((reg_set_bits(0xFFFFFFFF,"width", 0x5u)).reg,"width", &out)==REG_OK);
    assert(out==0x5u);
    assert(reg_get_bits((reg_set_bits(0xFFFFFFFF,"enable", 0x1u)).reg,"enable", &out)==REG_OK);
    assert(out==0x1u);
    assert(reg_get_bits((reg_set_bits(0xFFFFFFFF,"margin", 0x2u)).reg,"margin", &out)==REG_OK);
    assert(out==0x2u);
    printf ("PASS: bits were set.\n");
    uint32_t reg = 0x00000000;
    assert(reg_set_bits(reg,"speed", 5).status==REG_OK);
    assert(reg_set_bits(reg,"width", 5).status==REG_OK);
    assert(reg_set_bits(reg,"enable", 1).status==REG_OK);
    assert(reg_set_bits(reg,"margin", 2).status==REG_OK);
    reg= reg_set_bits(reg,"speed", 5).reg;
    reg= reg_set_bits(reg,"width", 5).reg;
    reg= reg_set_bits(reg,"enable", 1).reg;
    reg= reg_set_bits(reg,"margin", 2).reg;
    assert (reg==0x00000555u);
    printf("PASS: status is OK on speed update\n");
    reg = 0x00000000;
    assert(((reg_set_bits(reg,"speed", 5).reg) & ~0xFu)==(0x005 & ~0xFu));
    assert(((reg_set_bits(reg,"width", 5).reg) & ~0xF0u)==(0x050 & ~0xF0u));
    assert(((reg_set_bits(reg,"enable", 1).reg) & ~0x100u)==(0x100 & ~0x100u));
    assert(((reg_set_bits(reg,"margin", 2).reg) & ~0x600u)==(0x400 & ~0x600u));
    printf ("PASS: no other bits affected when updating bits\n");
}

static void test_out_of_range_bits (void){
    uint32_t out=0xDEADBEEF;
    assert(reg_get_bits((reg_set_bits(0xFFFFFFF5,"speed", 20)).reg,"speed", &out)==REG_OK);
    assert(out==0x5u);
    assert(reg_get_bits((reg_set_bits(0xFFFFFF5F,"width", 20)).reg,"width", &out)==REG_OK);
    assert(out==0x5u);
    assert(reg_get_bits((reg_set_bits(0xFFFFFFF5,"enable", 4)).reg,"enable", &out)==REG_OK);
    assert(out==0x1u);
    assert(reg_get_bits((reg_set_bits(0xFFFFFFF5,"margin", 8)).reg,"margin", &out)==REG_OK);
    assert(out==0x3u);
    printf ("PASS: bits were NOT set.\n");
    assert((reg_set_bits(0xFFFFFFFF, "speed", 20)).status==REG_ERR_VALUE_OUT_OF_RANGE);
    assert((reg_set_bits(0xFFFFFFFF, "width", 20)).status==REG_ERR_VALUE_OUT_OF_RANGE);
    assert((reg_set_bits(0xFFFFFFFF, "enable", 20)).status==REG_ERR_VALUE_OUT_OF_RANGE);
    assert((reg_set_bits(0xFFFFFFFF, "margin", 20)).status==REG_ERR_VALUE_OUT_OF_RANGE);
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

static void test_invalid_null_out_get(void){
    assert((reg_get_bits(0x00000554u,"speed", NULL)==REG_ERR_NULL));
    printf ("PASS: NULL output pointer handled\n");
}


int main(void) {
    test_bits_get();
    test_bits_set();
    test_out_of_range_bits();
    test_boundaries();
    test_invalid_name_set();
    test_invalid_null_name_get();
    test_invalid_null_name_set();
    test_invalid_name_get();
    test_invalid_null_out_get();
    printf("All tests Finished\n");
    return 0;
}
