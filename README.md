# Registers_access

 

A small C learning project: a table-driven layer for reading and writing

named bit fields inside a 32-bit register.

 

 

## What it does

 

The library models a single 32-bit register split into named fields:

 

| Field    | Bits  | Width  | Valid range |

|----------|-------|--------|-------------|

| `speed`  | 0-3   | 4 bits | 0-15        |

| `width`  | 4-7   | 4 bits | 0-15        |

| `enable` | 8     | 1 bit  | 0-1         |

| `margin` | 9-10  | 2 bits | 0-3         |

 

Bits 11-31 are not mapped to any field and cannot be reached through the API.

 

Callers address fields by name instead of juggling shifts and masks:

 

```c

uint32_t reg = 0x00000000;

reg = reg_set_bits(reg, "speed",  5).reg;

reg = reg_set_bits(reg, "margin", 2).reg;   /* reg == 0x405 */

 

uint32_t speed;

if (reg_get_bits(reg, "speed", &speed) == REG_OK)

    printf("speed = %u\n", speed);

```

 

## API

 

```c

reg_result_t reg_set_bits(uint32_t reg, const char *name, uint32_t value);

reg_status_t reg_get_bits(uint32_t reg, const char *name, uint32_t *out);

```

 

Both functions are pure: the register is passed by value and a new value is

returned. Nothing is read from or written to hardware — this is a bit

manipulation layer, not an MMIO layer.

 

`reg_set_bits` returns a struct carrying both the status and the resulting

register value. On any error, the returned register is the original one,

unmodified.

 

`reg_get_bits` returns the status and writes the extracted field through

`out`. On any error, `*out` is left untouched, so the caller's variable keeps

whatever it had.

 

### Status codes

 

| Code                         | Meaning                              |

|------------------------------|--------------------------------------|

| `REG_OK`                     | operation succeeded                  |

| `REG_ERR_VALUE_OUT_OF_RANGE` | value does not fit in the field      |

| `REG_ERR_NULL`               | output pointer was `NULL`            |

| `REG_ERR_NOT_IN_TABLE`       | no field is named that               |

| `REG_ERR_NO_PROPER_NAME`     | name pointer was `NULL`              |

 

## Design

 

The register layout lives in a descriptor table, private to `Reg32bits.c`:

 

```c

static const reg_components_t regs_table[] = {

    {"speed",  0xF, 0},

    {"width",  0xF, 4},

    {"enable", 0x1, 8},

    {"margin", 0x3, 9},

};

```

 

Each row holds the field's name, its mask relative to the field (not to the

register) and its shift. Both public functions look the row up by name and

derive everything else from it.

 

The consequence is the point of the exercise: **adding a field is adding one

row.** No new function, no extra branch, no new `#define` — and no existing

code to modify.

 

The mask doubles as the range check, since the largest value a field can hold

is its own mask.

 

## Build and run

 

Requires a C11 compiler.

 

```

gcc -Wall -Wextra -std=c11 -o test_Reg32bits Reg32bits.c test_Reg32bits.c

./test_Reg32bits

```

 

## Tests

 

`test_Reg32bits.c` is a standalone `assert`-based suite. For every field it

verifies:

 

- reading a known value

- writing and reading it back

- rejection of out-of-range values, leaving the register untouched

- the exact accept/reject boundary (`15`/`16`, `1`/`2`, `3`/`4`)

- that writing one field leaves its neighbours alone — checked from both an

  all-zeros and an all-ones register, so spurious set bits *and* spurious

  cleared bits are caught

 

Plus a round-trip that writes all four fields onto a clean register and

checks the exact resulting value, and five tests covering the error

contracts: unknown name, `NULL` name and `NULL` output pointer on both

functions, asserting that neither the register nor the output variable is

disturbed on failure.

 

Note that `assert` is compiled out under `-DNDEBUG`, which would turn the

whole suite into a no-op.

 

## Scope

 

Deliberately out of scope: multiple register layouts, actual hardware access,

thread safety, and any protection of the reserved bits beyond simply not

mapping them.