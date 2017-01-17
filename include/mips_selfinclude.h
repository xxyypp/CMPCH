#ifndef mips_selfinclude_header
#define mips_selfinclude_header

#include "mips_mem.h"
#include "mips_cpu.h"
#include "mips_test.h"
void mips_getHI(uint32_t &HILO, mips_cpu_h state);
void mips_getLO(uint32_t &HILO, mips_cpu_h state);

void mips_setHI(uint32_t HIval, mips_cpu_h state);
void mips_setLO(uint32_t LOval, mips_cpu_h state);
#endif