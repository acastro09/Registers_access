#include <stdio.h>
#include <Reg32bits.c>
#include <assert.h>

static void test_speed_get (void){
    assert((reg_get_speed(0x00000554u)==0x4u));
    printf ("PASS: get speed\n");
}


static void test_speed_set (void){
    assert(reg_get_speed((reg_set_speed(0xFFFFFFFF, 5)).reg)==5);
    printf ("PASS: speed was set.\n");
    assert(reg_get_speed((reg_set_speed(0xFFFFFFFF, 5)).status)==REG_OK);
    printf("PASS: status is OK\n");
    assert(((reg_set_speed(0x554, 5).reg) & ~0xFu)==(0x554 & ~0xFu));
    printf ("PASS: no other bits affected\n");

}

static void test_out_of_range_speed (void){
    assert(reg_get_speed((reg_set_speed(0xFFFFFFF5, 20)).reg)==5);
    printf ("PASS: speed was NOT set.\n");
    assert(reg_get_speed((reg_set_speed(0xFFFFFFFF, 20)).status)==REG_ERR_VALUE_OUT_OF_RANGE);
    printf("PASS: Not affecting other bits\n");

}

static void test_boundaries(void):{
    assert(reg_set_speed(0x554,15).status==REG_OK);
    assert(reg_set_speed(0x554,16).status!=REG_OK);
    assert(reg_get_speed((reg_set_speed(0xFFFFFFFF, 5)).reg)==5);
    printf("PASS: Boundaries well defined\n");
}


int main(void) {
    test_speed_get();
    test_speed_set();
    test_out_of_range_speed();
    test_boundaries();
    printf("All tests PASSED\n")
    return 0;
}