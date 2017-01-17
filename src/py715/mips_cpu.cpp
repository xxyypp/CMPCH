
//#include "stdafx.h"
#include "mips.h"
#include "../src/py715/mips_selfinclude.h"
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;

struct mips_cpu_impl
{
	uint32_t pc;
	uint32_t pcNext;
	uint32_t HI;
	uint32_t LO;

	uint32_t regs[32];

	mips_mem_h mem;

	unsigned logLevel;
	FILE *logDst;
};
uint32_t HIvalincpu = 0;

mips_error mips_cpu_get_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t *value		//!< Where to write the value to
	)
{
	if (state == 0)
		return mips_ErrorInvalidHandle;
	if (index >= 32)
		return mips_ExceptionInvalidAlignment;
	*value = state->regs[index];
	return mips_Success;
}

/*! Modifies one of the 32 general purpose MIPS registers. */
mips_error mips_cpu_set_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t value		//!< New value to write into register file
	)
{
	if (state == 0)
		return mips_ErrorInvalidHandle;
	if (index >= 32)
		return mips_ExceptionInvalidAlignment;

	state->regs[index] = value;
	return mips_Success;
}
mips_error mips_cpu_set_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t pc			//!< Address of the next instruction to exectute.
	) {
	state->pc = pc;
	return mips_Success;
}

mips_error mips_cpu_get_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t *pc		//!< Where to write the byte address too
	) {
	*pc = state->pc;
	return mips_Success;
}


mips_error mips_cpu_set_debug_level(mips_cpu_h state, unsigned level, FILE *dest)
{
	state->logLevel = level;
	state->logDst = dest;
	return mips_Success;
}

mips_cpu_h mips_cpu_create(mips_mem_h mem)
{
	mips_cpu_impl *cpu = new mips_cpu_impl;

	cpu->pc = 0;
	cpu->pcNext = 4;
	cpu->HI = 0;
	cpu->LO = 0;
	for (int i = 0; i<32; i++) {
		cpu->regs[i] = 0;
	}
	cpu->mem = mem;

	return cpu;
}

mips_error mips_cpu_reset(mips_cpu_h state) {
	//mips_cpu_impl *state = new mips_cpu_impl;
	state->pc = 0;
	for (unsigned int i = 0; i < 32; i++) {
		state->regs[i] = 0;
	}
	return mips_Success;
}

void mips_getHI(uint32_t &HILO, mips_cpu_h state) {
	HILO = state->HI;
}

void mips_getLO(uint32_t &HILO, mips_cpu_h state) {
	HILO = state->LO;
}

void mips_setHI(uint32_t HIval, mips_cpu_h state) {
	state->HI = HIval;
}
void mips_setLO(uint32_t LOval, mips_cpu_h state) {
	state->LO = LOval;
}

/*! \param pData Array of 4 bytes
\retval The big-endian equivlent
*/
uint32_t to_big(const uint8_t *pData)
{
	return
		(((uint32_t)pData[0]) << 24) | (((uint32_t)pData[1]) << 16) | (((uint32_t)pData[2]) << 8) | (((uint32_t)pData[3]) << 0);
}

mips_error mips_cpu_step(mips_cpu_h state	//! Valid (non-empty) handle to a CPU
	)
{
	uint8_t buffer[4];
	mips_error err = mips_mem_read(
		state->mem,		//!< Handle to target memory
		state->pc,	//!< Byte address to start transaction at
		4,	//!< Number of bytes to transfer
		buffer	//!< Receives the target bytes
		);
	state->pc = state->pc + state->pcNext;			// NOTE : What about updating the program counter
	if (err != 0) {
		return err;
	}
/*=====================================================================================================
=======================================================================================================*/
														//R-type
	uint32_t instruction = to_big(buffer);

	uint32_t opcode = (instruction >> 26) & 0x3F;
	uint32_t src1 = (instruction >> 21) & 0x1F;
	uint32_t src2 = (instruction >> 16) & 0x1F;
	uint32_t dst = (instruction >> 11) & 0x1F;
	uint32_t shift = (instruction >> 6) & 0x1F;
	uint32_t function = (instruction >> 0) & 0x3F;

	uint32_t jopcode = (instruction >> 26) & 0x3F;
	uint32_t jsrc = (instruction >> 21) & 0x1F;
	uint32_t jdst = (instruction >> 16) & 0x1F;
	int32_t imm = (instruction >> 0) & 0xFFFF;
	uint32_t unimm = (instruction >> 0) & 0xFFFF;

	if (opcode == 0) {
		// This is R-type
		if (state->logLevel >= 2) {
			fprintf(state->logDst, "R-Type : dst=%u, src1=%u, src2=%u, shift=%u, function=%u.\n  instr=%08x\n",
				dst, src1, src2, shift, function, instruction);
		}
/*=============================================0x20 -> add================================================*/
		if (function == 0x20) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "add %u, %u, %u.\n", dst, src1, src2);
			}
			int32_t va = state->regs[src1];
			int32_t vb = state->regs[src2];
			int32_t res = va + vb; //add
			if (((va & 0x80000000) == 0x80000000) && ((vb & 0x80000000) == 0x80000000)&& ((res & 0x80000000) == 0)) {
				return mips_ExceptionArithmeticOverflow; 
			}
			else if (((va & 0x80000000) == 0) && ((vb & 0x80000000) == 0)&& ((res & 0x80000000) == 0x80000000)) {
				return mips_ExceptionArithmeticOverflow; 
			}
			else {
				if (dst != 0) {	 // NOTE: what about zero reg?
					state->regs[dst] = res;
				}
				else {
					state->regs[dst] = 0;
				}
				return mips_Success;
			}
			return mips_ExceptionAccessViolation;
		}
/*=============================================0x21 -> addu===============================================*/
		if (function == 0x21) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "addu %u, %u, %u.\n", dst, src1, src2);
			}
			uint32_t va = state->regs[src1];
			uint32_t vb = state->regs[src2];
			uint32_t res = va + vb;

			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
			}
			else {
				state->regs[dst] = 0;
			}
			return mips_Success;
		}
/*=============================================0x24 -> and===============================================*/
		if (function == 0x24) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "and %u, %u, %u.\n", dst, src1, src2);
			}
			uint32_t va = state->regs[src1];
			uint32_t vb = state->regs[src2];
			
			uint32_t res = va & vb;
			
			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
			}
			else {
				state->regs[dst] = 0;

			}
			return mips_Success;
		}
/*=============================================0x25 -> or===============================================*/
		if (function == 0x25) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "or %u, %u, %u.\n", dst, src1, src2);
			}
			uint32_t va = state->regs[src1];
			uint32_t vb = state->regs[src2];
			uint32_t res = va | vb;
			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
			}
			else {
				state->regs[dst] = 0;
			}
			return mips_Success;
		}

/*=============================================0x22 -> sub================================================*/
		if (function == 0x22) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "sub %u, %u, %u.\n", dst, src1, src2);
			}
			int32_t va = state->regs[src1];
			int32_t vb = state->regs[src2];
			int32_t res = va - vb; //sub
			//for overflow there are two cases
			//+ive + +ive become -ive
			//-ive + -ive become +ive
			if (((va & 0x80000000) == 0x80000000) && ((vb & 0x80000000) == 0x80000000) && ((res & 0x80000000) == 0)) {
				return mips_ExceptionArithmeticOverflow;
			}
			else if (((va & 0x80000000) == 0) && ((vb & 0x80000000) == 0) && ((res & 0x80000000) == 0x80000000)) {
				return mips_ExceptionArithmeticOverflow;
			}
			else {
				if (dst != 0) {	 // NOTE: what about zero reg?
					state->regs[dst] = res;
				}
				else {
					state->regs[dst] = 0;
				}
				return mips_Success;
			}
			return mips_ExceptionAccessViolation;
		}
/*=============================================0x23 -> subu===============================================*/
		if (function == 0x23) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "subu %u, %u, %u.\n", dst, src1, src2);
			}
			uint32_t va = state->regs[src1];
			uint32_t vb = state->regs[src2];

			uint32_t res = va - vb;

			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
			}
			else {
				state->regs[dst] = 0;
			}
			return mips_Success;
		}
/*=============================================0x26 -> xor===============================================*/
		if (function == 0x26) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "xor %u, %u, %u.\n", dst, src1, src2);
			}
			uint32_t va = state->regs[src1];
			uint32_t vb = state->regs[src2];

			uint32_t res = va ^ vb;

			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
			}
			else {
				state->regs[dst] = 0;
			}
			return mips_Success;
		}
/*=============================================0x3 -> sra===============================================*/
		if (function == 0x3) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "sra %u, %u, %u.\n", dst, src2, shift);
			}
			
			uint32_t vb = state->regs[src2];
			uint32_t res = 0;

			if (((vb) & (0x80000000)) == 0x80000000) {
				res = vb >> shift;
				for (unsigned int i = 0; i < shift; i++) {
					res = res | (1 <<( (32 - shift) + i));
				}
			}
			else {
				res = vb >> shift;
			}
			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
				return mips_Success;
			}
			else {
				state->regs[dst] = 0;
				return mips_Success;
			}
			
			return mips_ErrorInvalidArgument;
		}

/*=============================================0x7 -> srav===============================================*/
		if (function == 0x7) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "srav %u, %u, %u.\n", dst, src2, src1);
			}

			uint32_t va = state->regs[src1];
			uint32_t vb = state->regs[src2];
			uint32_t res = 0;

			if (((vb)& (0x80000000)) == 0x80000000) {
				va = va & 0x1F;
				res = vb >> va;
				for (unsigned int i = 0; i < va; i++) {
					res = res | (1 << ((32 - va) + i));
				}
			}
			else {
				res = vb >> va;
			}

			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
				return mips_Success;
			}
			else {
				state->regs[dst] = 0;
				return mips_Success;

			}
			return mips_ErrorInvalidArgument;
		}
/*=============================================0x2 -> srl===============================================*/
		if (function == 0x2) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "srl %u, %u, %u.\n", dst, src2, shift);
			}

			uint32_t vb = state->regs[src2];
			uint32_t res = 0;

			res = vb >> shift;
		
			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
				return mips_Success;
			}
			else {
				state->regs[dst] = 0;
				return mips_Success;
			}
			return mips_ErrorInvalidArgument;
		}

/*=============================================0x0 -> sll===============================================*/
		if (function == 0x0) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "sll %u, %u, %u.\n", dst, src2, shift);
			}

			uint32_t vb = state->regs[src2];
			uint32_t res = 0;

			res = vb << shift;

			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
			}
			else {
				state->regs[dst] = 0;
			}
			return mips_Success;
		}

/*=============================================0x4 -> sllv===============================================*/
		if (function == 0x4) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "sllv %u, %u, %u.\n", dst, src2, src1);
			}

			uint32_t va = state->regs[src1];
			uint32_t vb = state->regs[src2];
			uint32_t res = 0;

			res = vb << (va & 0x1F);

			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
			}
			else {
				state->regs[dst] = 0;
			}
			return mips_Success;
		}

/*=============================================0x6 -> srlv===============================================*/
		if (function == 0x6) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "srlv %u, %u, %u.\n", dst, src2, src1);
			}

			uint32_t va = state->regs[src1];
			uint32_t vb = state->regs[src2];
			uint32_t res = 0;

			res = vb >> (va & 0x1F);

			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
			}
			else {
				state->regs[dst] = 0;
			}
			return mips_Success;
		}

/*=============================================0x2A -> slt===============================================*/
		if (function == 0x2A) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "slt %u, %u, %u.\n", dst, src2, src1);
			}

			int32_t va = state->regs[src1];
			int32_t vb = state->regs[src2];
			int32_t res = 0;
			if (vb > va) {
				res = 1;
			}
			else {
				res = 0;
			}

			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
			}
			else {
				state->regs[dst] = 0;

			}
			return mips_Success;
		}
/*=============================================0x2B -> sltu===============================================*/
		if (function == 0x2B) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "sltu %u, %u, %u.\n", dst, src2, src1);
			}

			uint32_t va = state->regs[src1];
			uint32_t vb = state->regs[src2];
			uint32_t res = 0;

			if (vb > va) {
				res = 1;
			}
			else {
				res = 0;
			}
			if (dst != 0) {
				state->regs[dst] = res; // NOTE: what about zero reg?
			}
			else {
				state->regs[dst] = 0;
			}
			return mips_Success;
		}
/*=============================================0x10 -> mfhi ===============================================*/
		if (function == 0x10) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "mfhi %u.\n", dst);
			}
			uint32_t va = state->HI;
			mips_error err= mips_cpu_set_register(state, dst, va);
			if (err) {
			}//without this, there will be warning when compile
			return mips_Success;
		}
/*=============================================0x11 -> mthi ===============================================*/
		if (function == 0x11) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "mthi %u.\n", src1);
			}
			state->HI = 0;
			uint32_t va = state->regs[src1];
			state->HI = va;
			uint32_t testHI = 0;
			mips_getHI(testHI, state);
			return mips_Success;
		}
/*=============================================0x12 -> mflo ===============================================*/
		if (function == 0x12) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "mflo %u.\n", dst);
			}
			uint32_t va = state->LO;
			mips_error err = mips_cpu_set_register(state, dst, va);
			if (err) {
			}
			return mips_Success;
		}
/*=============================================0x13 -> mtlo ===============================================*/
		if (function == 0x13) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "mtlo %u.\n", src1);
			}
			state->LO = 0;
			uint32_t va = state->regs[src1];
			state->LO = va;
			uint32_t testLO = 0;
			mips_getLO(testLO, state);
			return mips_Success;
		}
/*=============================================0x18 -> mult===============================================*/
		if (function == 0x18) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "mult %u, %u, %u.\n", dst, src2, src1);
			}
			int64_t va = state->regs[src1];
			int64_t vb = state->regs[src2];
			int64_t res = 0;

			int64_t valhi = 0;
			int64_t vallo = 0;

			if (va != 0) {
				if ((va & 0x80000000) == 0) {
					va = (va | 0x0);
				}
				if ((va & 0x80000000) == 0x80000000) {
					va = (va | 0xFFFFFFFF00000000);
				}
			}
			if (vb != 0) {
				if ((vb & 0x80000000) == 0) {
					vb = (vb | 0x0);
				}
				if ((vb & 0x80000000) == 0x80000000) {
					vb = (vb | 0xFFFFFFFF00000000);
				}
			}

			res = va*vb;
			valhi = ((res >> 32) & 0xFFFFFFFF);
			vallo = (res & 0xFFFFFFFF);

			state->HI = valhi;
			state->LO = vallo;

			return mips_Success;
		}

/*=============================================0x19 -> multu ===============================================*/
		if (function == 0x19) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "multu %u, %u, %u.\n", dst, src2, src1);
			}

			uint64_t va = state->regs[src1];
			uint64_t vb = state->regs[src2];
			uint64_t res = 0;

			res = va*vb;
			uint64_t valhi = 0;
			uint64_t vallo = 0;

			valhi = ((res >> 32) & 0xFFFFFFFF);
			vallo = (res & 0xFFFFFFFF);

			state->HI = valhi;
			state->LO = vallo;

			return mips_Success;
		}

/*=============================================0x1A -> div ===============================================*/
		if (function == 0x1A) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "div %u, %u, %u.\n", dst, src2, src1);
			}

			int32_t va = state->regs[src1];
			int32_t vb = state->regs[src2];
			int32_t numlo = 0;
			int32_t numhi = 0;

			numlo = (va/vb);
			numhi = va % vb;

			state->LO = numlo;
			state->HI = numhi;
			return mips_Success;
		}

/*============================================= 0x1B -> divu ===============================================*/
		if (function == 0x1B) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "divu %u, %u, %u.\n", dst, src2, src1);
			}

			uint32_t va = state->regs[src1];
			uint32_t vb = state->regs[src2];
			uint32_t numlo = 0;
			uint32_t numhi = 0;

			numlo = va / vb;
			numhi = va % vb;

			state->LO = numlo;
			state->HI = numhi;

			return mips_Success;
		}

/*============================================= 0x9 -> jalr ===============================================*/
		if (function == 0x9) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "jalr %u, %u.\n", dst, src1);
			}

			uint32_t va = state->regs[src1];
		
			if (((va)%4)==0) {
				if (dst != 0) {
					state->regs[dst] = state->pc + 4;
				}
				else {
					state->regs[dst] = 0;
				}
				state->pc = va;
				return mips_Success;
			}
			else {
				return mips_ExceptionInvalidAddress;
			}
		}

/*============================================= 0x8 -> jr ===============================================*/
		if (function == 0x8) {
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "jr %u, %u.\n", dst, src1);
			}

			uint32_t va = state->regs[src1];
			if (va % 4 == 0) {
				state->pc = va;
				return mips_Success;
			}
			else {
				return mips_ExceptionAccessViolation;
			}
			
			

			
		}
/*=====================================================================================================*/


}//R-Type end
		
/*=====================================================================================================
=======================================================================================================*/
													//I-type
	
/*=============================================0x08 -> addi===============================================*/
				  
		if (jopcode == 0x08) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "addi %u,%u, %u.\n", jdst, jsrc,imm);
			}
		
		int32_t va = state->regs[jsrc];
		
		if (imm != 0) {
			if ( (imm & 0x8000)==0) {
				imm = (imm | 0x00000000);
			}
			if ((imm & 0x8000)== 0x8000) {
				imm = (imm | 0xFFFF0000);
			}
		}

		int32_t res = va + imm; //add

								//for overflow there are two cases
								//+ive + +ive become -ive
								//-ive + -ive become +ive
		if (((va & 0x80000000) == 0x80000000) && ((imm & 0x80000000) == 0x80000000) && ((res & 0x80000000) == 0)) {
			return mips_ExceptionArithmeticOverflow;

		}
		else if (((va & 0x80000000) == 0) && ((imm & 0x80000000) == 0) && ((res & 0x80000000) == 0x80000000)) {
			return mips_ExceptionArithmeticOverflow;
		}
		else {
			if (jdst != 0) {	 // NOTE: what about zero reg?
				state->regs[jdst] = res;
			}
			else {
				state->regs[jdst] = 0;
			}
			return mips_Success;
		}
		return mips_ExceptionAccessViolation;
	}

/*=============================================0x09 -> addiu===============================================*/
		if (jopcode == 0x09) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "addiu %u, %u,%u.\n", jdst, jsrc,imm);
			}
			int32_t va = state->regs[jsrc];

			if (imm != 0) {
				if ((imm & 0x8000) == 0) {
					imm = (imm | 0x00000000);
				}
				if ((imm & 0x8000) == 0x8000) {
					imm = (imm | 0xFFFF0000);
				}
			}
			int32_t res = va + imm; //add

			
				if (jdst != 0) {	 // NOTE: what about zero reg?
					state->regs[jdst] = res;
				}
				else {
					state->regs[jdst] = 0;
				}
				return mips_Success;
		
		}

/*=============================================0xC -> andi===============================================*/
		if (jopcode == 0xC) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, unimm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "andi %u, %u,%u.\n", jdst, jsrc, unimm);
			}
			unimm = unimm | 0x0;
			uint32_t va = state->regs[jsrc];
			uint32_t jres = va & unimm;
			if (jdst != 0) {
				state->regs[jdst] = jres; // NOTE: what about zero reg?
			}
			else {
				state->regs[jdst] = 0;
			}
			return mips_Success;
		}
/*=============================================0x4 -> beq===============================================*/
		if (jopcode == 0x4) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "beq %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint32_t va = state->regs[jsrc];
			uint32_t vb = state->regs[jdst];
			imm = imm << 2;
			
			if (va == vb) {
				state->pc = imm + state->pc;
				return mips_Success;
			}
			else {
				return mips_Success;
			}

			return mips_ExceptionAccessViolation;
		}
/*=============================================0x1(JOPCODE) ===============================================*/
/*==========================================and 0x1(JDST) -> bgez===============================================*/

		if ((jopcode == 0x1)&&(jdst == 0x1)) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "bgez %u, %u,%u.\n", jdst, jsrc, imm);
			}
			int32_t va = state->regs[jsrc];
			imm = imm << 2;

			//if ((va == 0)||((va & 0x80000000)==0)) {
			if (va >= 0) {
				state->pc += imm;
				return mips_Success;
			}
			else {
				return mips_Success;
			}
			return mips_ExceptionAccessViolation;
		}
/*=============================================0x1(JOPCODE) ===============================================*/
/*==========================================and 0x11(JDST) -> bgezal===============================================*/

		if ((jopcode == 0x1) && (jdst == 0x11)) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "bgezal %u, %u,%u.\n", jdst, jsrc, imm);
			}
			int32_t va = state->regs[jsrc];
			imm = imm << 2;

			if (va>=0) {
				err = mips_cpu_set_register(state, 31, (state->pc + 4));//advance by 4 at the start of step
				state->pc += imm;
				return mips_Success;
			}
			else {
				return mips_Success;
			}
			return mips_ExceptionAccessViolation;
		}

/*=============================================0x7(JOPCODE) ===============================================*/
/*==========================================and 0(JDST) -> bgtz===============================================*/

		if ((jopcode == 0x7) && (jdst == 0)) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "bgtz %u, %u,%u.\n", jdst, jsrc, imm);
			}
			int32_t va = state->regs[jsrc];
			imm = imm << 2;
			if (va>0) {//+ive va
				state->pc += imm;
				return mips_Success;
			}
			else {//va <= 0
				return mips_Success;
			}
			return mips_ExceptionAccessViolation;
		}
/*=============================================0x6(JOPCODE) ===============================================*/
/*==========================================and 0(JDST) -> blez===============================================*/

		if ((jopcode == 0x6) && (jdst == 0)) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "blez %u, %u,%u.\n", jdst, jsrc, imm);
			}
			int32_t va = state->regs[jsrc];
			imm = imm << 2;
			if (va>0) {
				return mips_Success;
			}
			else {
				state->pc += imm;
				return mips_Success;
			}
			return mips_ExceptionAccessViolation;
		}

/*=============================================0x1(JOPCODE) ===============================================*/
/*==========================================and 0(JDST) -> bltz===============================================*/

		if ((jopcode == 0x1) && (jdst == 0)) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "bltz %u, %u,%u.\n", jdst, jsrc, imm);
			}
			int32_t va = state->regs[jsrc];
			imm = imm << 2;
			
			if (va>=0) {//+ive va
				return mips_Success;
			}
			else {//va <= 0
				state->pc += imm;
				return mips_Success;
			}
			return mips_ExceptionAccessViolation;
		}

/*=============================================0x1(JOPCODE) ===============================================*/
/*==========================================and 0x10(JDST) -> bltzal===============================================*/

		if ((jopcode == 0x1) && (jdst == 0x10)) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "bltzal %u, %u,%u.\n", jdst, jsrc, imm);
			}
			int32_t va = state->regs[jsrc];
			imm = imm << 2;
			
			if (va>=0) {
				return mips_Success;
			}
			else {
				err = mips_cpu_set_register(state, 31, (state->pc + 4));//advance by 4 at the start of step
				state->pc += imm;
				return mips_Success;
			}
			return mips_ExceptionAccessViolation;
		}

/*=============================================0x5 -> bne===============================================*/
		if (jopcode == 0x5) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "bne %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint32_t va = state->regs[jsrc];
			uint32_t vb = state->regs[jdst];
			imm = imm << 2;
			
			if (va == vb) {
				return mips_Success;
			}
			else {
				state->pc += imm;
				return mips_Success;
			}
			return mips_ExceptionAccessViolation;
		}
/*=============================================0xd -> ori ===============================================*/
		if (jopcode == 0xD) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, unimm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "ori %u, %u,%u.\n", jdst, jsrc, unimm);
			}
			uint32_t va = state->regs[jsrc];
			uint32_t jres = va | unimm;

			if (jdst != 0) {
				state->regs[jdst] = jres; // NOTE: what about zero reg?
			}
			else {
				state->regs[jdst] = 0;

			}
			return mips_Success;
		}
/*=============================================0xE -> xori ===============================================*/
		if (jopcode == 0xE) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, unimm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "xori %u, %u,%u.\n", jdst, jsrc, unimm);
			}
			uint32_t va = state->regs[jsrc];
			uint32_t jres = va ^ unimm;

			if (jdst != 0) {
				state->regs[jdst] = jres; // NOTE: what about zero reg?
			}
			else {
				state->regs[jdst] = 0;
			}
			return mips_Success;
		}

/*=============================================0xA -> slti ===============================================*/
		if (jopcode == 0xA) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "slti %u, %u,%u.\n", jdst, jsrc, imm);
			}
			int32_t va = state->regs[jsrc];
			int32_t jres = 0;

			if (imm != 0) {
				if ((imm & 0x8000) == 0) {
					imm = (imm | 0x00000000);
				}
				if ((imm & 0x8000) == 0x8000) {
					imm = (imm | 0xFFFF0000);
				}
			}
			
			if (imm > va) {
				jres = 1;
			}
			else {
				jres = 0;
			}
			if (jdst != 0) {
				state->regs[jdst] = jres; // NOTE: what about zero reg?
			}
			else {
				state->regs[jdst] = 0;
			}
			return mips_Success;
		}

/*=============================================0xB -> sltiu ===============================================*/
		if (jopcode == 0xB) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "sltiu %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint32_t va = state->regs[jsrc];
			uint32_t jres = 0;

			if (imm != 0) {
				if ((imm & 0x8000) == 0) {
					imm = (imm | 0x00000000);
				}
				if ((imm & 0x8000) == 0x8000) {
					imm = (imm | 0xFFFF0000);
				}
			}
			unimm = imm;

			if (unimm > va) {
				jres = 1;
			}
			else {
				jres = 0;
			}
			if (jdst != 0) {
				state->regs[jdst] = jres; // NOTE: what about zero reg?
			}
			else {
				state->regs[jdst] = 0;
			}
			return mips_Success;
		}

/*=============================================0x2B -> sw ===============================================*/
		if (jopcode == 0x2B) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "sw %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint32_t numjdst = state->regs[jdst];
			uint32_t numjsrc = state->regs[jsrc];

			uint32_t jLocation = numjsrc + imm;//set store location
			if ((jLocation % 4) == 0) {
				err = mips_mem_write(state->mem, jLocation, 4, (uint8_t*)&numjdst);
				return mips_Success;
			}
			else {
				return mips_ErrorInvalidArgument;
			}
			return mips_ExceptionAccessViolation;
		}

/*=============================================0x28 -> sb ===============================================*/
		if (jopcode == 0x28) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "sb %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint32_t numjdst = 0;
			uint32_t numjsrc = 0;
			uint32_t jLocation = 0;

			numjdst = state->regs[jdst];
			numjsrc = state->regs[jsrc];

			numjdst = numjdst & 0xFF;

			jLocation = numjsrc + imm;//set store location
			
			err = mips_mem_write(state->mem, jLocation, 1, (uint8_t*)&numjdst);
			return mips_Success;

		}
/*=============================================0x29 -> sh ===============================================*/
		if (jopcode == 0x29) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "sh %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint32_t numjdst = state->regs[jdst];
			numjdst = numjdst & 0xFFFF;
			uint32_t numjsrc = state->regs[jsrc];


			uint32_t jLocation = numjsrc + imm;//set store location
			if ((jLocation % 4) == 0) {
				err = mips_mem_write(state->mem, jLocation, 2, (uint8_t*)&numjdst);
				return mips_Success;
			}
			else {
				return mips_ErrorInvalidArgument;
			}
			return mips_ExceptionAccessViolation;
		}

/*=============================================0x20 -> lb ===============================================*/
		if (jopcode == 0x20) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "lb %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint32_t numjdst = 0;
			uint32_t numjsrc = state->regs[jsrc];

			if (imm != 0) {
				if ((imm & 0x8000) == 0) {
					imm = (imm | 0x00000000);
				}
				if ((imm & 0x8000) == 0x8000) {
					imm = (imm | 0xFFFF0000);
				}
			}
			uint32_t jLocation = numjsrc + imm;//set store location
			if ((jLocation % 4) == 0) {
				err = mips_mem_read(state->mem, jLocation, 1, (uint8_t*)&numjdst);
				if (numjdst != 0) {
					if ((numjdst & 0x80) == 0) {
						numjdst = (numjdst | 0x00000000);
					}
					if ((numjdst & 0x80) == 0x80) {
						numjdst = (numjdst | 0xFFFF0000);
					}
				}
				err = mips_cpu_set_register(state, jdst, numjdst);
				return mips_Success;
			}
			else {
				return mips_ErrorInvalidArgument;
			}
			return mips_ExceptionAccessViolation;
		}

/*=============================================0x24 -> lbu ===============================================*/
		if (jopcode == 0x24) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "lbu %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint32_t numjdst = 0;
			uint32_t numjsrc = state->regs[jsrc];

			if (imm != 0) {
				if ((imm & 0x8000) == 0) {
					imm = (imm | 0x00000000);
				}
				if ((imm & 0x8000) == 0x8000) {
					imm = (imm | 0xFFFF0000);
				}
			}

			uint32_t jLocation = numjsrc + imm;//set store location
			if ((jLocation % 4) == 0) {
				err = mips_mem_read(state->mem, jLocation, 1, (uint8_t*)&numjdst);
				numjdst = (numjdst | 0x00000000);
				err = mips_cpu_set_register(state, jdst, numjdst);
				return mips_Success;
			}
			else {
				return mips_ErrorInvalidArgument;
			}
			return mips_ExceptionAccessViolation;
		}
/*=============================================0x21 -> lh ===============================================*/
		if (jopcode == 0x21) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "lh %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint32_t numjdst = 0;
			uint32_t numjsrc = state->regs[jsrc];

			if (imm != 0) {
				if ((imm & 0x8000) == 0) {
					imm = (imm | 0x00000000);
				}
				if ((imm & 0x8000) == 0x8000) {
					imm = (imm | 0xFFFF0000);
				}
			}
			uint32_t jLocation = numjsrc + imm;//set store location
			if ((jLocation % 4) == 0) {
				err = mips_mem_read(state->mem, jLocation, 2, (uint8_t*)&numjdst);
				if (numjdst != 0) {
					if ((numjdst & 0x8000) == 0) {
						numjdst = (numjdst | 0x00000000);
					}
					if ((numjdst & 0x8000) == 0x8000) {
						numjdst = (numjdst | 0xFFFF0000);
					}
				}
				err = mips_cpu_set_register(state, jdst, numjdst);
				return mips_Success;
			}
			else {
				return mips_ErrorInvalidArgument;
			}
			return mips_ExceptionAccessViolation;
		}
/*=============================================0x25 -> lhu ===============================================*/
		if (jopcode == 0x25) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "lhu %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint32_t numjdst = 0;
			uint32_t numjsrc = state->regs[jsrc];

			if (imm != 0) {
				if ((imm & 0x8000) == 0) {
					imm = (imm | 0x00000000);
				}
				if ((imm & 0x8000) == 0x8000) {
					imm = (imm | 0xFFFF0000);
				}
			}

			uint32_t jLocation = numjsrc + imm;//set store location
			if ((jLocation % 4) == 0) {
				err = mips_mem_read(state->mem, jLocation, 2, (uint8_t*)&numjdst);
				numjdst = (numjdst | 0x00000000);
				err = mips_cpu_set_register(state, jdst, numjdst);
				return mips_Success;
			}
			else {
				return mips_ErrorInvalidArgument;
			}
			return mips_ExceptionAccessViolation;
		}
/*=============================================0xF -> lui ===============================================*/
		if (jopcode == 0xF) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "lui %u, %u,%u.\n", jdst, jsrc, imm);
			}

			int32_t loaded = ((imm << 16));
			err = mips_cpu_set_register(state, jdst, loaded);
			return mips_Success;
		}
/*=============================================0x23 -> lw ===============================================*/
		if (jopcode == 0x23) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "lw %u, %u,%u.\n", jdst, jsrc, imm);
			}
			uint64_t numjdst = 0;
			uint32_t numjsrc = state->regs[jsrc];

			if (imm != 0) {
				if ((imm & 0x8000) == 0) {
					imm = (imm | 0x00000000);
				}
				if ((imm & 0x8000) == 0x8000) {
					imm = (imm | 0xFFFF0000);
				}
			}

			uint32_t jLocation = numjsrc + imm;//set store location
			if ((jLocation % 4) == 0) {
				err = mips_mem_read(state->mem, jLocation, 4, (uint8_t*)&numjdst);
				if (numjdst != 0) {
					if ((numjdst & 0x80000000) == 0) {
						numjdst = (numjdst | 0x00000000);
					}
					if ((numjdst & 0x80000000) == 0x80000000) {
						numjdst = (numjdst | 0xFFFF0000);
					}
				}
				err = mips_cpu_set_register(state, jdst, numjdst);
				return mips_Success;
			}
			else {
				return mips_ErrorInvalidArgument;
			}
			return mips_ExceptionAccessViolation;
		}
/*=============================================0x22 -> lwl ===============================================*/
		if (jopcode == 0x22) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "lwl %u, %u,%u.\n", jdst, jsrc, imm);
			}
			int32_t numjdst = state->regs[jdst];
			int32_t numjsrc = state->regs[jsrc];
			
			if (imm != 0) {
				if ((imm & 0x8000) == 0) {
					imm = (imm | 0x00000000);
				}
				if ((imm & 0x8000) == 0x8000) {
					imm = (imm | 0xFFFF0000);
				}
			}

			uint32_t numsiga = 0;
			uint32_t jLocation = numjsrc + imm;//set store location

			int firstmod4;
			if (jLocation % 4 == 0) {
				firstmod4 = 1;
			}

			if (((jLocation % 4) != 0) || (firstmod4 == 1)) {
				err = mips_mem_read(state->mem, jLocation, 1, (uint8_t*)&numsiga);
				numjdst = numjdst & 0x00FFFFFF;
				numjdst = numjdst | (numsiga<<24);
				jLocation++;
				if (jLocation%4 != 0) {
					err = mips_mem_read(state->mem, jLocation, 1, (uint8_t*)&numsiga);
					numjdst = numjdst & 0xFF00FFFF;
					numjdst = numjdst | (numsiga<<16);
					jLocation++;
					if (jLocation%4 != 0) {
						err = mips_mem_read(state->mem, jLocation, 1, (uint8_t*)&numsiga);
						numjdst = numjdst & 0xFFFF00FF;
						numjdst = numjdst | (numsiga<<8);
						jLocation++;
						if (jLocation%4 != 0) {
							err = mips_mem_read(state->mem, jLocation, 1, (uint8_t*)&numsiga);
							numjdst = numjdst & 0xFFFFFF00;
							numjdst = numjdst | numsiga;
							err = mips_cpu_set_register(state, jdst, numjdst);
							return mips_Success;
						}
						else {
							err = mips_cpu_set_register(state, jdst, numjdst);
							return mips_Success;
						}
					}
					else {
						err = mips_cpu_set_register(state, jdst, numjdst);
						return mips_Success;
					}
				}
				else {
					err = mips_cpu_set_register(state, jdst, numjdst);
					return mips_Success;
				}
				
			}else {
				err = mips_cpu_set_register(state, jdst, numjdst);
				return mips_ErrorInvalidArgument;
				}
		}

		
/*=============================================0x26 -> lwr ===============================================*/
		if (jopcode == 0x26) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "I-Type : dst=%u, src=%u, Astart=%u.\n",
					jdst, jsrc, imm);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "lwr %u, %u,%u.\n", jdst, jsrc, imm);
			}
			int32_t numjdst = state->regs[jdst];
			int32_t numjsrc = state->regs[jsrc];

			if (imm != 0) {
				if ((imm & 0x8000) == 0) {
					imm = (imm | 0x00000000);
				}
				if ((imm & 0x8000) == 0x8000) {
					imm = (imm | 0xFFFF0000);
				}
			}

			uint32_t numsiga = 0;
			uint32_t jLocation = numjsrc + imm;//set store location

			int firstmod4;
			if (jLocation % 4 == 0) {
				firstmod4 = 1;
			}


			if (((jLocation % 4) != 0) || (firstmod4 == 1)) {
				err = mips_mem_read(state->mem, jLocation, 1, (uint8_t*)&numsiga);
				numjdst = numjdst & 0xFFFFFF00;
				numjdst = numjdst | (numsiga);
				jLocation--;
				if (jLocation % 4 != 0) {
					err = mips_mem_read(state->mem, jLocation, 1, (uint8_t*)&numsiga);
					numjdst = numjdst & 0xFFFF00FF;
					numjdst = numjdst | (numsiga << 8);
					jLocation--;
					if (jLocation % 4 != 0) {
						err = mips_mem_read(state->mem, jLocation, 1, (uint8_t*)&numsiga);
						numjdst = numjdst & 0xFF00FFFF;
						numjdst = numjdst | (numsiga << 16);
						jLocation--;
						if (jLocation % 4 != 0) {
							err = mips_mem_read(state->mem, jLocation, 1, (uint8_t*)&numsiga);
							numjdst = numjdst & 0x00FFFFFF;
							numjdst = numjdst | (numsiga << 24);
							err = mips_cpu_set_register(state, jdst, numjdst);
							return mips_Success;
						}
						else {
							err = mips_cpu_set_register(state, jdst, numjdst);
							return mips_Success;
						}
					}
					else {
						err = mips_cpu_set_register(state, jdst, numjdst);
						return mips_Success;
					}
				}
				else {
					err = mips_cpu_set_register(state, jdst, numjdst);
					return mips_Success;
				}

			}else {
			err = mips_cpu_set_register(state, jdst, numjdst);
			return mips_ErrorInvalidArgument;
			}
		}

/*=============================================J-TYPE START ===============================================*/
/*============================================================================================*/

		uint32_t jtypeopcode = (instruction >> 26) & 0x3F;
		uint32_t jtypemem = (instruction >> 0) & 0x3FFFFFF; //j-type mem


/*=============================================0x2 -> j ===============================================*/
		if (jtypeopcode == 0x2) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "J-Type : mem=%u.\n", jtypemem);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "j %u.\n", jtypemem);
			}
			jtypemem = jtypemem << 2;
			state->pc = ((state->pc - 4) & 0xF0000000) | (jtypemem);
			return mips_Success;
		}

/*=============================================0x3 -> jal ===============================================*/
		if (jtypeopcode == 0x3) {
			if (state->logLevel >= 2) {
				fprintf(state->logDst, "J-Type : mem=%u.\n", jtypemem);
			}
			if (state->logLevel >= 1) {
				fprintf(state->logDst, "jal %u.\n", jtypemem);
			}
			state->regs[31] = state->pc + 4;
			jtypemem = jtypemem << 2;
			state->pc = ((state->pc - 4) & 0xF0000000) | (jtypemem);
			return mips_Success;

		}
/*=================================================================================================*/
		//end else for all 3 type of fn
		else {
			return mips_ErrorNotImplemented;
		}


/*for code outside the lib(at the end)
	
=================================================================================================*/
//cpu_step fn end
}
	

void mips_cpu_free(mips_cpu_h state) {
	state->pc = 0;
	state->pcNext = 0;
	state->HI = 0;
	state->LO = 0;
	for (int i = 0; i<32; i++) {
		state->regs[i] = 0;
	}
	free(state);
}