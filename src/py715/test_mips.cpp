#include "mips.h"
#include "../src/py715/mips_selfinclude.h"
#include <iostream>

using namespace std;

int main()
{
	mips_mem_h mem = mips_mem_create_ram(4096);

	mips_cpu_h cpu = mips_cpu_create(mem);

	mips_error e = mips_cpu_set_debug_level(cpu, 4, stderr);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_set_debug_level : failed.\n");
		exit(1);
	}
	//define and initialise pc;
	uint32_t startLocation;
	uint32_t pcCurrentVal;
	
	/*========================================ADD================================================
	================================================================================================*/
	// add two +ive nums

	mips_test_begin_suite();

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	int testId = mips_test_begin_test("add");
	
	// 1 - Setup an instruction in ram
	uint32_t instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x20 << 0);//fn 

	uint8_t buffer[4]; //create a buffer 8bit
	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 4);
	e = mips_cpu_set_register(cpu, 5, 5);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}
	// 4 -Check the result
	uint32_t got;
	e = mips_cpu_get_register(cpu, 3, &got);

	int passed = got == 4+5;

	mips_test_end_test(testId, passed, "4+5 !=9 ");

	/*========================================ADD================================================
	================================================================================================*/
	//add two -ive number 

	//set pc and startlocation
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("add");

	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(6ul << 21) // srca = r6
		|
		(7ul << 16) // srcb = r7
		|
		(5ul << 11) // dst = r5
		|
		(0ul << 6) // shift = 0
		|
		(0x20 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != (mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, -40);
	e = mips_cpu_set_register(cpu, 7, -8);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != (mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 0xFFFFFFD0;
	mips_test_end_test(testId, passed, "-40-8 != -48");

	/*========================================ADD================================================
	================================================================================================*/
	//add +ive with -ive  
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("add");

	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x20 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 40);
	e = mips_cpu_set_register(cpu, 5, -8);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);
	passed = got == 32;
	mips_test_end_test(testId, passed, "40-8 != 32");

	/*========================================ADD================================================
	================================================================================================*/
	//add -ive with -ive with signed overflow
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("add");

	// 1 - Setup an instruction in ram
	// add r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x20 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 0x80000000);
	e = mips_cpu_set_register(cpu, 5, 0xFFFFFFFF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)&&(e != mips_ExceptionArithmeticOverflow)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	passed = (e == mips_ExceptionArithmeticOverflow);
	e = mips_cpu_get_register(cpu, 3, &got);


	mips_test_end_test(testId, passed, "Overflow");

/*========================================ADDI================================================
================================================================================================*/
//add immediate
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("addi");

	// 1 - Setup an instruction in ram
	instr =
		(0x08ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0xDFFF << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 1);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = (got == 0xFFFFE000);
	mips_test_end_test(testId, passed, "0xDFFF + 1 != 0xE000");
	
/*========================================ADDI================================================
================================================================================================*/
	//add immediate with overflow
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("addi");

	// 1 - Setup an instruction in ram
	// addi r5, r6, r7
	instr =
		(0x08ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0xFFFF << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x80000000);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success) && (e != mips_ExceptionArithmeticOverflow)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	passed = (e == mips_ExceptionArithmeticOverflow);

	mips_test_end_test(testId, passed, "Not overflow");

/*========================================ADDIU================================================
================================================================================================
*/	
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("addiu");

	// 1 - Setup an instruction in ram
	// addiu r5, r6
	instr =
		(0x09ul << 26) // jopcode
		|
		(6ul << 21) // jsrc = r6
		|
		(5ul << 16) // jdst = r5
		|
		(1 << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 1);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != (mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = (got == 2);
	mips_test_end_test(testId, passed, "1+1!=2");

/*========================================ADDU================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	
	passed = 0;
	got = 0;
	testId = mips_test_begin_test("addu");

	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x21 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 40);
	e = mips_cpu_set_register(cpu, 5, 50);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	e = mips_cpu_get_register(cpu, 3, &got);
	passed = got == 40 + 50;
	mips_test_end_test(testId, passed, "50 + 50 != 90");

/*========================================ADDU with dest = 0======================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("addu");

	// 1 - Setup an instruction in ram
	// addu r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(0ul << 11) // dst = r0
		|
		(0ul << 6) // shift = 0
		|
		(0x21 << 0);

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 40);
	e = mips_cpu_set_register(cpu, 5, 50);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 0, &got);
	passed = got == 0;
	mips_test_end_test(testId, passed, "r0 <> 0");
/*========================================AND===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	
	testId = mips_test_begin_test("and");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	// addu r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(6ul << 21) // srca = r6
		|
		(7ul << 16) // srcb = r7
		|
		(5ul << 11) // dst = r5
		|
		(0ul << 6) // shift = 0
		|
		(0x24 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFFFFFFFF);
	e = mips_cpu_set_register(cpu, 7, 0X0000FFFF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 0xFFFF;
	mips_test_end_test(testId, passed, "!=FFFF");
	
/*========================================ANDI===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("andi");

	// 1 - Setup an instruction in ram
	instr =
		(0xCul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0xFFFF << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);

	passed = got == 0xF;

	mips_test_end_test(testId, passed, "!=0xF");

/*========================================OR===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("or");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	// addu r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(6ul << 21) // srca = r6
		|
		(7ul << 16) // srcb = r7
		|
		(5ul << 11) // dst = r5
		|
		(0ul << 6) // shift = 0
		|
		(0x25 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFFFFFFFF);
	e = mips_cpu_set_register(cpu, 7, 0X0000FFFF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 0xFFFFFFFF;
	mips_test_end_test(testId, passed, "!=FFFFFFFF");

/*========================================SRA with +ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SRA");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(0ul << 21) // srca = 0
		|
		(6ul << 16) // srcb = r6
		|
		(5ul << 11) // dst = r5
		|
		(2ul << 6) // shift = 2
		|
		(0x3 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 8);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 2;
	mips_test_end_test(testId, passed, "!=2");

/*========================================SRA with -ve num===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SRA");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(0ul << 21) // srca = 0
		|
		(6ul << 16) // srcb = r6
		|
		(5ul << 11) // dst = r5
		|
		(2ul << 6) // shift = 2
		|
		(0x3 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x80000000);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 0xE0000000;
	mips_test_end_test(testId, passed, "!=0xFFC00000");

/*========================================SRAV with +ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SRAV");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(6ul << 21) // srca = r6
		|
		(7ul << 16) // srcb = r7
		|
		(5ul << 11) // dst = r5
		|
		(0ul << 6) // shift = 2
		|
		(0x7 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 2);//srca
	e = mips_cpu_set_register(cpu, 7, 8);//srcb
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = (got == (8 >> (2 & 0x1F)));
	mips_test_end_test(testId, passed, "!=2");

/*========================================SRAV with -ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SRAV");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(6ul << 21) // srca = r6
		|
		(7ul << 16) // srcb = r7
		|
		(5ul << 11) // dst = r5
		|
		(0ul << 6) // shift = 2
		|
		(0x7 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x20);//shift
	e = mips_cpu_set_register(cpu, 7, 0xFFFFFF00);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = (got == (0xFFFFFF00 >> (0x20 & 0x1F)));
	mips_test_end_test(testId, passed, "!=0xFFC00000");

/*========================================SRL with +ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SRL");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(0ul << 21) // srca = 0
		|
		(6ul << 16) // srcb = r6
		|
		(5ul << 11) // dst = r5
		|
		(2ul << 6) // shift = 2
		|
		(0x2 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xF0000000);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 0x3C000000;
	mips_test_end_test(testId, passed, "!=2");

/*========================================SRL with -ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SRL");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(0ul << 21) // srca = 0
		|
		(6ul << 16) // srcb = r6
		|
		(5ul << 11) // dst = r5
		|
		(2ul << 6) // shift = 2
		|
		(0x2 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF000000);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 0x3FC00000;
	mips_test_end_test(testId, passed, "!=0x3FC00000");

/*========================================SLL with +ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SLL");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(0ul << 21) // srca = 0
		|
		(6ul << 16) // srcb = r6
		|
		(5ul << 11) // dst = r5
		|
		(2ul << 6) // shift = 2
		|
		(0 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 8);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 32;
	mips_test_end_test(testId, passed, "!=32");

/*========================================SLL with -ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SLL");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(0ul << 21) // srca = 0
		|
		(6ul << 16) // srcb = r6
		|
		(5ul << 11) // dst = r5
		|
		(2ul << 6) // shift = 2
		|
		(0x0 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x0F000000);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 0x3C000000;
	mips_test_end_test(testId, passed, "!=0x3C000000");

/*========================================SRLV with +ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SRLV");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(6ul << 21) // srca = r6
		|
		(7ul << 16) // srcb = r7
		|
		(5ul << 11) // dst = r5
		|
		(0ul << 6) // shift = 0
		|
		(0x6 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 2);
	e = mips_cpu_set_register(cpu, 7, 8);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 2;
	mips_test_end_test(testId, passed, "!=2");

/*========================================SRLV with -ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SRLV");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(6ul << 21) // srca = r6
		|
		(7ul << 16) // srcb = r7
		|
		(5ul << 11) // dst = r5
		|
		(0ul << 6) // shift = 0
		|
		(0x6 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;
	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x38);
	e = mips_cpu_set_register(cpu, 7, 0xFF000000);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 0xFF;
	mips_test_end_test(testId, passed, "!=0xFF");

/*========================================SLLV with +ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SLLV");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(6ul << 21) // srca = r6
		|
		(7ul << 16) // srcb = r7
		|
		(5ul << 11) // dst = r5
		|
		(0ul << 6) // shift = 0
		|
		(0x4 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 2);//src1
	e = mips_cpu_set_register(cpu, 7, 8);//src2
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 32;
	mips_test_end_test(testId, passed, "!=32");

/*========================================SLLV with -ive num shift===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("SLLV");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	instr =
		(0ul << 26) // opcode = 0
		|
		(6ul << 21) // srca = r6
		|
		(7ul << 16) // srcb = r7
		|
		(5ul << 11) // dst = r5
		|
		(0ul << 6) // shift = 0
		|
		(0x4 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x38);
	e = mips_cpu_set_register(cpu, 7, 0xF);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = got == 0xF000000;
	mips_test_end_test(testId, passed, "!=0xF000000");
/*========================================SUB================================================
================================================================================================*/
	//subtract 
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("sub");

	// 1 - Setup an instruction in ram
	// sub r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x22 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 40);
	e = mips_cpu_set_register(cpu, 5, 8);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);


	passed = got == 40 - 8;

	mips_test_end_test(testId, passed, "40-8 != 32");

/*========================================SUB================================================
================================================================================================*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("sub");

	// 1 - Setup an instruction in ram
	// sub r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x22 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 0xFFFFFFFF);
	e = mips_cpu_set_register(cpu, 5, 0x12345678);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}
	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);
	passed = got == 0xEDCBA987;
	mips_test_end_test(testId, passed, "£¡=0xEDCBA987");
	

/*========================================SUBU================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("subu");

	// 1 - Setup an instruction in ram
	// subu r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x23 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 0xFFFFFFFF);
	e = mips_cpu_set_register(cpu, 5, 0xFFFFFFF0);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);


	passed = (got == 0xFFFFFFFF- 0xFFFFFFF0);

	mips_test_end_test(testId, passed, "0xFFFFFFFF- 0xFFFFFFF0 != 0xF");


/*========================================BEQ -> equal ================================================
=============================================================================================
*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	uint32_t pcOrig = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BEQ");

	// 1 - Setup an instruction in ram

	instr =
		(0x4ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0xFFFF << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF);
	e = mips_cpu_set_register(cpu, 5, 0xFF);
	//set immediate constant

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	uint32_t pcGot;
	pcGot = 0xFFFF << 2;
	pcGot = pcGot + pcOrig+4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}

	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig+4);//set pc back

/*========================================BEQ -> !equal ================================================
=============================================================================================
*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	pcOrig = 0;
	pcGot = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BEQ");

	// 1 - Setup an instruction in ram

	instr =
		(0x4ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0x10 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF);
	e = mips_cpu_set_register(cpu, 5, 0x0F);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
		
	pcGot = pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}


	mips_test_end_test(testId, passed, "pc val is not correct");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back
	
/*========================================BGEZ -> src>=0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	pcOrig = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BGEZ");

	// 1 - Setup an instruction in ram

	instr =
		(0x1ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0x1ul << 16) // dst = 1
		|
		(0x20 << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	pcGot = 0x20 << 2;
	pcGot = pcGot + pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}


	//passed = got == 0xF;

	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*========================================BGEZ -> src<0================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	pcOrig = 0;
	pcGot = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BGEZ");

	// 1 - Setup an instruction in ram

	instr =
		(0x1ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0x1ul << 16) // dst = r5
		|
		(0x4 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF000000);


	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	pcGot = pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}

	mips_test_end_test(testId, passed, "pc val is not correct");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*======================================== BGEZAL  -> src>=0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	uint32_t reggot = 0;

	pcOrig = 0;//originval of pc got from function
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BGEZAL");

	// 1 - Setup an instruction in ram

	instr =
		(0x1ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0x11ul << 16) // dst = 17
		|
		(0x8 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x0F);
	//e = mips_cpu_set_register(cpu, 5, 0x01);
	
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e= mips_cpu_get_register(cpu,31,&reggot); //get reg[31]

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	pcGot = 0x8 << 2;//shift pc = imm << 2
	pcGot = pcGot + pcOrig + 4;
	
	if ((pcGot == pcCurrentVal)&&(reggot == pcOrig + 8)) {
		passed = 1;
	}


	//passed = got == 0xF;

	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*======================================== BGEZAL  -> src<0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	reggot = 0;

	pcOrig = 0;//originval of pc got from function
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BGEZAL");

	// 1 - Setup an instruction in ram

	instr =
		(0x1ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0x11ul << 16) // dst = 17
		|
		(0x8 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xF0000000);
	//e = mips_cpu_set_register(cpu, 5, 0x01);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	pcGot = pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}

	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*========================================BGTZ -> src>0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	pcOrig = 0; //pc origin val before process
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BGTZ");

	// 1 - Setup an instruction in ram

	instr =
		(0x7ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0ul << 16) // dst = 1
		|
		(0x8 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF);
	
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);


	pcGot = 0x8 << 2;
	pcGot = pcGot + pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}


	//passed = got == 0xF;

	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

 /*========================================BGTZ -> src<=0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	pcOrig = 0; //pc origin val before process
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BGTZ");

	// 1 - Setup an instruction in ram

	instr =
		(0x7ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0ul << 16) // dst = 1
		|
		(0x8 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF000000);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	pcGot = pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}

	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*========================================BLEZ -> src<0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	pcOrig = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLEZ");

	// 1 - Setup an instruction in ram

	instr =
		(0x6ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0ul << 16) // dst = 1
		|
		(0x8 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF000000);
	
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	pcGot = 0x8 << 2;
	pcGot = pcGot + pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}


	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back


/*========================================BLEZ -> src>=0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	pcOrig = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLEZ");

	// 1 - Setup an instruction in ram

	instr =
		(0x6ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0ul << 16) // dst = 1
		|
		(0x8 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	pcGot = pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}


	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*========================================BLTZ -> src<0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	pcOrig = 0; //pc origin val before process
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLTZ");

	// 1 - Setup an instruction in ram

	instr =
		(0x1ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0ul << 16) // dst = 1
		|
		(0x8 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF000000);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);


	pcGot = 0x8 << 2;
	pcGot = pcGot + pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}


	//passed = got == 0xF;

	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*========================================BLTZ -> src>=0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	pcOrig = 0; //pc origin val before process
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLTZ");

	// 1 - Setup an instruction in ram

	instr =
		(0x1ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0ul << 16) // dst = 1
		|
		(0x8 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	pcGot = pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}

	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*======================================== BLTZAL  -> src<0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	reggot = 0;

	pcOrig = 0;//originval of pc got from function
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLTZAL");

	// 1 - Setup an instruction in ram

	instr =
		(0x1ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0x10ul << 16) // dst = 16
		|
		(0x8 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF000000);
	//e = mips_cpu_set_register(cpu, 5, 0x01);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 31, &reggot); //get reg[31]
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	pcGot = 0x8 << 2;//shift pc = imm << 2
	pcGot = pcGot + pcOrig + 4;
	if ((pcGot == pcCurrentVal) && (reggot == pcOrig + 8)) {
		passed = 1;
	}


	//passed = got == 0xF;

	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*======================================== BLTZAL  -> src>=0 ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	reggot = 0;

	pcOrig = 0;//originval of pc got from function
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLTZAL");

	// 1 - Setup an instruction in ram

	instr =
		(0x1ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(0x10ul << 16) // dst = 16
		|
		(0x8 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF12);
	//e = mips_cpu_set_register(cpu, 5, 0x01);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	pcGot = pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}
	mips_test_end_test(testId, passed, "PC val is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*========================================BNE -> !equal ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	pcOrig = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BNE");

	// 1 - Setup an instruction in ram

	instr =
		(0x5ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0x10 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF);
	e = mips_cpu_set_register(cpu, 5, 0x0F);
	//set immediate constant

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	pcGot = 0x10 << 2;
	pcGot = pcGot + pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}
	//passed = got == 0xF;

	mips_test_end_test(testId, passed, "when !=, pc is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*========================================BNE -> equal ================================================
=============================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	pcOrig = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BNE");

	// 1 - Setup an instruction in ram

	instr =
		(0x5ul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0x10 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;



	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF);
	e = mips_cpu_set_register(cpu, 5, 0xFF);
	//set immediate constant

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	pcGot = pcOrig + 4;
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}

	mips_test_end_test(testId, passed, "when =, pc is wrong");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*========================================XOR===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	testId = mips_test_begin_test("XOR");
	passed = 0;
	got = 0;
	// 1 - Setup an instruction in ram
	// addu r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(6ul << 21) // srca = r6
		|
		(7ul << 16) // srcb = r7
		|
		(5ul << 11) // dst = r5
		|
		(0ul << 6) // shift = 0
		|
		(0x26 << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFFFFFFFF);
	e = mips_cpu_set_register(cpu, 7, 0X0000FFFF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = (got ==( (0xFFFFFFFF) ^ (0X0000FFFF)));

	mips_test_end_test(testId, passed, "!=FFFF0000");

/*========================================ORI===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("ORI");

	// 1 - Setup an instruction in ram

	instr =
		(0xDul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0x8FFF << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x5);
	//set immediate constant

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);

	passed = (got ==( (0x8FFF)|(0x5)));

	mips_test_end_test(testId, passed, "!=0xF");

/*========================================XORI===================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("XORI");

	// 1 - Setup an instruction in ram

	instr =
		(0xEul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0x8FFF << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x5);
	//set immediate constant

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);
	passed = (got == ((0x8FFF) ^ (0x5)));

	mips_test_end_test(testId, passed, "!=0xF");

/*========================================SLT (srca < srcb) -> true ================================================
================================================================================================*/
//subtract 
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("SLT");

	// 1 - Setup an instruction in ram
	// slt r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x2A << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 8);
	e = mips_cpu_set_register(cpu, 5, 9);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);


	passed = got == 1;

	mips_test_end_test(testId, passed, "!true");

/*========================================SLT (srca < srcb) -> false ================================================
================================================================================================*/
	//subtract 
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("SLT");

	// 1 - Setup an instruction in ram
	// slt r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x2A << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 8);
	e = mips_cpu_set_register(cpu, 5, 7);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);

	passed = got == 0;

	mips_test_end_test(testId, passed, "!false");

/*========================================SLT (srca < srcb in negative val) -> true ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("SLT");

	// 1 - Setup an instruction in ram
	// slt r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x2A << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu

	e = mips_cpu_set_register(cpu, 4, 0xFFFFFFFF);//-1
	e = mips_cpu_set_register(cpu, 5, 1);//1

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);

	passed = got == 1;

	mips_test_end_test(testId, passed, "!true");

/*========================================SLT (srca < srcb in negative val) -> true ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("SLT");

	// 1 - Setup an instruction in ram
	// slt r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x2A << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	//signed int intsrca=-50
	//signed int intsrcb=-80
	e = mips_cpu_set_register(cpu, 4, 0x7FFFFFFF);
	e = mips_cpu_set_register(cpu, 5, 0xF00F0000);

										 // 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);

	passed = got == 0;

	mips_test_end_test(testId, passed, "!false");


/*========================================SLTI (jsrc < imm) -> true-> 1 ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("SLTI");

	// 1 - Setup an instruction in ram

	instr =
		(0xAul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0x7FFF << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	//signed int intsrca=-50
	//signed int intsrcb=-80
	e = mips_cpu_set_register(cpu, 6, 0xFF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);

	passed = got == 1;

	mips_test_end_test(testId, passed, "!true");

/*========================================SLTI (jsrc > imm) -> false-> 0 ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("SLTI");

	// 1 - Setup an instruction in ram

	instr =
		(0xAul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0xFFF << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	//signed int intsrca=-50
	//signed int intsrcb=-80
	e = mips_cpu_set_register(cpu, 6, 0x8FFF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);

	passed = got == 0;

	mips_test_end_test(testId, passed, "!false");

/*========================================SLTI (jsrc < imm in -ive) -> true-> 1 ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("SLTI");

	// 1 - Setup an instruction in ram

	instr =
		(0xAul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0xFFFF << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xF0FF0000);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);

	passed = got == 1;

	mips_test_end_test(testId, passed, "!true");

/*========================================SLTIU (jsrc < imm) -> true-> 1 ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("SLTIU");

	// 1 - Setup an instruction in ram

	instr =
		(0xBul << 26) // opcode
		|
		(6ul << 21) // src = r6
		|
		(5ul << 16) // dst = r5
		|
		(0xFFFF << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0xFF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);

	passed = got == 1;

	mips_test_end_test(testId, passed, "!true");


/*========================================SLTU (srca < srcb) -> true ================================================
================================================================================================*/
	
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("SLTU");

	// 1 - Setup an instruction in ram
	// slt r3, r4, r5
	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x2B << 0);//fn 


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 0x8FFFFFFF);
	e = mips_cpu_set_register(cpu, 5, 0x9FFFFFFF);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);


	passed = got == 1;

	mips_test_end_test(testId, passed, "!true");

/*======================================== SW ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("SW");

	// 1 - Setup an instruction in ram

	instr =
		(0x2Bul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(40 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 5, 0xFF123456);//jdst
	e = mips_cpu_set_register(cpu, 6, 100);//jsrc

	uint32_t storedLocation = 0;
	storedLocation = 100 + 40;

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	uint32_t gotInstruction = 0;
	e = mips_mem_read(mem, storedLocation,4, (uint8_t*)&gotInstruction);

	passed = gotInstruction == 0xFF123456;

	mips_test_end_test(testId, passed, "Failed");

/*======================================== SB ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	testId = mips_test_begin_test("SB");

	// 1 - Setup an instruction in ram

	instr =
		(0x28ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(40 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 5, 0xFF12);//jdst
	e = mips_cpu_set_register(cpu, 6, 100);//jsrc

	storedLocation = 100 + 40;

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_mem_read(mem, storedLocation, 1, (uint8_t*)&gotInstruction);

	passed = gotInstruction == 0x12;

	mips_test_end_test(testId, passed, "Failed");
/*======================================== SH ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	testId = mips_test_begin_test("SH");

	// 1 - Setup an instruction in ram

	instr =
		(0x29ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(80 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 5, 0x12345678);//jdst
	e = mips_cpu_set_register(cpu, 6, 140);//jsrc


	storedLocation = 140 + 80;

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_mem_read(mem, storedLocation, 2, (uint8_t*)&gotInstruction);

	passed = gotInstruction == 0x5678;

	mips_test_end_test(testId, passed, "Failed");

/*======================================== MFHI ================================================
================================================================================================*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("MFHI");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(0ul << 21) // srca = r4
		|
		(0ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x10 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;
	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	uint32_t HIbackup = 0;
	mips_getHI(HIbackup, cpu);
	uint32_t HIval = 0x1234;
	mips_setHI(HIval, cpu);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);
	passed = got == 0x1234;

	mips_test_end_test(testId, passed, "!true");
	mips_setHI(HIbackup, cpu);//set HI back
/*======================================== MFLO ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	testId = mips_test_begin_test("MFLO");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(3ul << 21) // srca = r3
		|
		(0ul << 16) // srcb = 0
		|
		(3ul << 11) // dst = 0
		|
		(0ul << 6) // shift = 0
		|
		(0x12 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	uint32_t LObackup = 0;
	mips_getLO(LObackup, cpu);
	uint32_t LOval = 0x46;
	mips_setLO(LOval, cpu);
	
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 3, &got);
	passed = got == 0x46;

	mips_test_end_test(testId, passed, "!true");
	mips_setLO(LObackup, cpu);//set lo back
	

/*======================================== MTHI ================================================
================================================================================================*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	HIbackup = 0;
	testId = mips_test_begin_test("MTHI");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(3ul << 21) // srca = r3
		|
		(0ul << 16) // srcb = 0
		|
		(0ul << 11) // dst = 0
		|
		(0ul << 6) // shift = 0
		|
		(0x11 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 3, 0x123);//srca
	mips_getHI(HIbackup, cpu);
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	uint32_t HI=0;
	mips_getHI(HI, cpu);
	passed = HI == 0x123;

	mips_test_end_test(testId, passed, "!true");
	mips_setHI(HIbackup, cpu);//set HI back

/*======================================== MTLO ================================================
================================================================================================*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	LObackup = 0;
	testId = mips_test_begin_test("MTLO");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(3ul << 21) // srca = r3
		|
		(0ul << 16) // srcb = 0
		|
		(0ul << 11) // dst = 0
		|
		(0ul << 6) // shift = 0
		|
		(0x13 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 3, 0x123456);//dst
	mips_getLO(LObackup, cpu);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	uint32_t LO = 0;
	mips_getLO(LO, cpu);

	passed = LO == 0x123456;

	mips_test_end_test(testId, passed, "!true");
	mips_setLO(LObackup, cpu);//set lo back

/*======================================== MULT with 2 +ive num ================================================
================================================================================================*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	LO = 0;
	HI = 0;
	testId = mips_test_begin_test("MULT");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x18 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 0x26921735);//srca
	e = mips_cpu_set_register(cpu, 5, 0x64);//srcb
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	mips_getHI(HI, cpu);
	mips_getLO(LO, cpu);
	passed = ((LO == 0x111110B4)&&(HI==0xF));

	mips_test_end_test(testId, passed, "!=39483");
/*======================================== MULT with 2 -ive num ================================================
================================================================================================*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	LO = 0;
	HI = 0;
	testId = mips_test_begin_test("MULT");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x18 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 0x81111111);//srca
	e = mips_cpu_set_register(cpu, 5, 0xF1111111);//srcb
										   // 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	mips_getHI(HI, cpu);
	mips_getLO(LO, cpu);
	passed = ((LO == 0xD7654321)&&(HI==0x76789AB));

	mips_test_end_test(testId, passed, "HI!=0xD7654321");

/*======================================== MULT with 1 +ive 1-ive num ================================================
================================================================================================*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	LO = 0;
	HI = 0;
	testId = mips_test_begin_test("MULT");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x18 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 0x8000000F);//srca
	e = mips_cpu_set_register(cpu, 5, 0x50000000);//srcb
												  // 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	mips_getHI(HI, cpu);
	mips_getLO(LO, cpu);
	passed = ((LO == 0xB0000000)&&( HI==0xD8000004));

	mips_test_end_test(testId, passed, "!=-12");

/*======================================== MULTU with 2 +ive num ================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	LO = 0;
	HI = 0;
	testId = mips_test_begin_test("MULTU");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x19 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 123);//srca
	e = mips_cpu_set_register(cpu, 5, 321);//srcb
										   // 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	mips_getHI(HI, cpu);
	mips_getLO(LO, cpu);
	passed = ((LO == 123 * 321)&&(HI== 0));

	mips_test_end_test(testId, passed, "!correct");
/*======================================== MULTU with 2 signed -ive num ================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	LO = 0;
	HI = 0;
	testId = mips_test_begin_test("MULTU");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x19 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 0x81111111);//srca
	e = mips_cpu_set_register(cpu, 5, 0xF1111111);//srcb
												  // 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	mips_getHI(HI, cpu);
	mips_getLO(LO, cpu);
	passed = ((LO == 0xD7654321) && (HI == 0x7989ABCD));

	mips_test_end_test(testId, passed, "!=0xD7654321");

/*======================================== MULTU with 1 +ive 1-ive num ================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	LO = 0;
	HI = 0;
	testId = mips_test_begin_test("MULTU");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x19 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 0x8000000F);//srca
	e = mips_cpu_set_register(cpu, 5, 0x50000000);//srcb
												  // 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	mips_getHI(HI, cpu);
	mips_getLO(LO, cpu);
	passed = ((LO == 0xB0000000) && (HI == 0x28000004));

	mips_test_end_test(testId, passed, "!=0xB0000000");

/*======================================== DIV with 1 +ive 1 +ive num ================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	LO = 0;
	HI = 0;
	testId = mips_test_begin_test("DIV");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x1A << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 16);//srca
	e = mips_cpu_set_register(cpu, 5, 8);//srcb
	
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	mips_getHI(HI, cpu);
	mips_getLO(LO, cpu);
	passed = ((LO == (2)) && (HI == (0)));

	mips_test_end_test(testId, passed, "res!TRUE");

/*======================================== DIV with 1 +ive 1 -ive num with remainder================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	LO = 0;
	HI = 0;
	testId = mips_test_begin_test("DIV");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x1A << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 0xFFFFFFAE);//srca=-82
	e = mips_cpu_set_register(cpu, 5, 9);//srcb

										 // 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	mips_getHI(HI, cpu);
	mips_getLO(LO, cpu);
	passed = ((LO == (0xFFFFFFF7)) && (HI == (0xFFFFFFFF)));

	mips_test_end_test(testId, passed, "res!TRUE");
	
/*======================================== DIVU with 1 +ive 1 +ive num ================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	LO = 0;
	HI = 0;
	testId = mips_test_begin_test("DIVU");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(5ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x1B << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 4, 16);//srca
	e = mips_cpu_set_register(cpu, 5, 8);//srcb

										 // 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	mips_getHI(HI, cpu);
	mips_getLO(LO, cpu);
	passed = ((LO == (16 / 8)) && (HI == (16 % 8)));

	mips_test_end_test(testId, passed, "res!TRUE");

/*======================================== JUMP ================================================
=============================================================================================
*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	pcOrig = 0;
	pcGot = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("J");

	// 1 - Setup an instruction in ram

	instr =
		(0x2ul << 26) // opcode
		|
		(0x100 << 0);//jmem=256

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}
	// 4 -Check the result

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	pcGot = (1024);
	if (pcGot == pcCurrentVal) {
		passed = 1;
	}


	mips_test_end_test(testId, passed, "pc val is not correct");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back


/*======================================== JAL ================================================
=============================================================================================
*/

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	pcOrig = 0;
	pcGot = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("JAL");

	// 1 - Setup an instruction in ram

	instr =
		(0x3ul << 26) // opcode
		|
		(0x120 << 0);//jmem=288

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}
	// 4 -Check the result

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	
	pcGot = (((pcOrig + 4) & (0xF0000000) )| (0x120<<2));
	e = mips_cpu_get_register(cpu, 31, &got);

	if ((pcGot == pcCurrentVal)&&(got==pcOrig+8)) {
		passed = 1;
	}


	mips_test_end_test(testId, passed, "pc val is not correct");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back
	
/*======================================== JALR ================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	pcOrig = 0;
	pcGot = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("JALR");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(4ul << 21) // srca = r4
		|
		(0ul << 16) // srcb = r5
		|
		(3ul << 11) // dst = r3
		|
		(0ul << 6) // shift = 0
		|
		(0x9 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 3, 15);//dst
	e = mips_cpu_set_register(cpu, 4, 256);//srca

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}
	// 4 -Check the result

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	e = mips_cpu_get_register(cpu, 3, &got);//dst
	pcGot = 256;


	if ((pcGot == pcCurrentVal) && (got == pcOrig + 8)) {
		passed = 1;
	}


	mips_test_end_test(testId, passed, "pc val is not correct");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*======================================== JR ================================================
================================================================================================
*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;
	pcOrig = 0;
	pcGot = 0;
	pcOrig = pcCurrentVal;

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("JR");

	// 1 - Setup an instruction in ram

	instr =
		(0ul << 26) // opcode = 0
		|
		(3ul << 21) // srca = r3
		|
		(0ul << 16) // srcb = 0
		|
		(0ul << 11) // dst = 0
		|
		(0ul << 6) // shift = 0
		|
		(0x8 << 0);//fn 

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);

	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 3, 280);//dst

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if ((e != mips_Success)) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}
	// 4 -Check the result

	e = mips_cpu_get_pc(cpu, &pcCurrentVal);

	e = mips_cpu_get_register(cpu, 3, &got);//srca
										
	pcGot = got;


	if ((pcGot == pcCurrentVal) ) {
		passed = 1;
	}

	mips_test_end_test(testId, passed, "pc val is not correct");
	e = mips_cpu_set_pc(cpu, pcOrig + 4);//set pc back

/*======================================== LB ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	testId = mips_test_begin_test("LB");

	// 1 - Setup an instruction in ram

	instr =
		(0x20ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(0x200 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 100);//jsrc
	storedLocation = 0x64 + 0x200;
	int8_t memwritesigned8 = 0x12;

	e = mips_mem_write(mem, storedLocation, 1, (uint8_t*)&memwritesigned8);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);//jsrc

	passed = got == 0x12;

	mips_test_end_test(testId, passed, "Failed");
/*======================================== LBU ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	testId = mips_test_begin_test("LBU");

	// 1 - Setup an instruction in ram

	instr =
		(0x24ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(0x210 << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x34);//jsrc
	storedLocation = 0x34 + 0x210;
	uint8_t memwriteunsigned8 = 0x34;
	e = mips_mem_write(mem, storedLocation, 1, (uint8_t*)&memwriteunsigned8);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);//jsrc
	passed = got == 0x34;
	mips_test_end_test(testId, passed, "Failed");

/*======================================== LH ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	testId = mips_test_begin_test("LH");

	// 1 - Setup an instruction in ram

	instr =
		(0x21ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(0x200 << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x120);//jsrc
	storedLocation = 0x120 + 0x200;
	int16_t memwritesignedsixteen = 0x1234;
	e = mips_mem_write(mem, storedLocation, 2, (uint8_t*)&memwritesignedsixteen);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);//jsrc
	passed = got == 0x1234;
	mips_test_end_test(testId, passed, "Failed");
/*======================================== LHU ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	testId = mips_test_begin_test("LHU");

	// 1 - Setup an instruction in ram

	instr =
		(0x25ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(0x200 << 0);//Immediate constant


	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;


	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x120);//jsrc
	storedLocation = 0x120 + 0x200;
	uint16_t memwriteunsignedsixteen = 0x1234;
	e = mips_mem_write(mem, storedLocation, 2, (uint8_t*)&memwriteunsignedsixteen);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);//jsrc
	passed = got == 0x1234;
	mips_test_end_test(testId, passed, "Failed");

/*======================================== LUI ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	testId = mips_test_begin_test("LUI");

	// 1 - Setup an instruction in ram

	instr =
		(0xFul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(0x200 << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);//jsrc
	passed = (got == (0x200<<16));
	mips_test_end_test(testId, passed, "Failed");
/*======================================== LW ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	testId = mips_test_begin_test("LW");

	// 1 - Setup an instruction in ram

	instr =
		(0x23ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(0x80 << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;

	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 6, 0x2A0);//jsrc
	storedLocation = 0x2A0 + 0x80;
	int32_t memwritesigned32 = 0x12345678;
	e = mips_mem_write(mem, storedLocation, 4, (uint8_t*)&memwritesigned32);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);//jsrc
	passed = got == 0x12345678;
	mips_test_end_test(testId, passed, "Failed");

/*======================================== LWL (with mod4 =0 at first)================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	memwritesigned32 = 0;
	testId = mips_test_begin_test("LWL");

	// 1 - Setup an instruction in ram
	instr =
		(0x22ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(0x80 << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;
	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 5, 0x88888888);//jDST
	e = mips_cpu_set_register(cpu, 6, 0x120);//jsrc
	storedLocation = 0x120 + 0x80;
	int8_t memwrite1 = 0x12;
	int8_t memwrite2 = 0x34;
	int8_t memwrite3 = 0x56;
	int8_t memwrite4 = 0x78;
	e = mips_mem_write(mem, storedLocation, 1, (uint8_t*)&memwrite1);
	e = mips_mem_write(mem, storedLocation + 1, 1, (uint8_t*)&memwrite2);
	e = mips_mem_write(mem, storedLocation + 2, 1, (uint8_t*)&memwrite3);
	e = mips_mem_write(mem, storedLocation + 3, 1, (uint8_t*)&memwrite4);
	/*
	 memwritesigned32 = 0x12345678;
	e = mips_mem_write(mem, storedLocation, 4, (uint8_t*)&memwritesigned32);
	*/
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);//jsrc
	passed = got == 0x12345678;
	mips_test_end_test(testId, passed, "Failed");

/*======================================== LWL (with mod4 !=0 at first)================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	memwritesigned32 = 0;
	testId = mips_test_begin_test("LWL");

	// 1 - Setup an instruction in ram
	instr =
		(0x22ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(0 << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;
	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}

	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 5, 0x88888888);//jDST
	e = mips_cpu_set_register(cpu, 6, 417);//jsrc
	storedLocation = 417;
	memwrite1 = 0x12;
	memwrite2 = 0x34;
	memwrite3 = 0x56;
	memwrite4 = 0x78;
	e = mips_mem_write(mem, storedLocation, 1, (uint8_t*)&memwrite1);
	e = mips_mem_write(mem, storedLocation + 1, 1, (uint8_t*)&memwrite2);
	e = mips_mem_write(mem, storedLocation + 2, 1, (uint8_t*)&memwrite3);
	e = mips_mem_write(mem, storedLocation + 3, 1, (uint8_t*)&memwrite4);
	/*
	memwritesigned32 = 0x12345678;
	e = mips_mem_write(mem, storedLocation, 4, (uint8_t*)&memwritesigned32);
	*/
	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}

	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);//jsrc
	passed = got == 0x12345688;
	mips_test_end_test(testId, passed, "Failed");

/*======================================== LWR ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	memwritesigned32 = 0;
	testId = mips_test_begin_test("LWR");

	// 1 - Setup an instruction in ram

	instr =
		(0x26ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(0x80 << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;
	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}
	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 5, 0x88888888);//jDST
	e = mips_cpu_set_register(cpu, 6, 0x120);//jsrc
	storedLocation = 0x120 + 0x80;
	memwrite1 = 0x12;
	memwrite2 = 0x34;
	memwrite3 = 0x56;
	memwrite4 = 0x78;
	e = mips_mem_write(mem, storedLocation, 1, (uint8_t*)&memwrite1);
	e = mips_mem_write(mem, storedLocation - 1, 1, (uint8_t*)&memwrite2);
	e = mips_mem_write(mem, storedLocation - 2, 1, (uint8_t*)&memwrite3);
	e = mips_mem_write(mem, storedLocation - 3, 1, (uint8_t*)&memwrite4);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}
	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);//jsrc
	passed = got == 0x78563412;
	mips_test_end_test(testId, passed, "Failed");

/*======================================== LWR(start at 2) ================================================
================================================================================================*/
	e = mips_cpu_get_pc(cpu, &pcCurrentVal);
	startLocation = pcCurrentVal;

	passed = 0;
	got = 0;
	storedLocation = 0;
	gotInstruction = 0;
	memwritesigned32 = 0;
	testId = mips_test_begin_test("LWR");

	// 1 - Setup an instruction in ram

	instr =
		(0x26ul << 26) // opcode
		|
		(6ul << 21) // Jsrc = r6
		|
		(5ul << 16) // Jdst = r5
		|
		(0 << 0);//Immediate constant

	buffer[0] = (instr >> 24) & 0xFF;
	buffer[1] = (instr >> 16) & 0xFF;
	buffer[2] = (instr >> 8) & 0xFF;
	buffer[3] = (instr >> 0) & 0xFF;
	e = mips_mem_write(
		mem,	        //!< Handle to target memory
		startLocation,	            //!< Byte address to start transaction at
		4,	            //!< Number of bytes to transfer
		buffer	        //!< Receives the target bytes
		);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_mem_write : failed.\n");
		exit(1);
	}
	// 2 - put register values in cpu
	e = mips_cpu_set_register(cpu, 5, 0x88888888);//jDST
	e = mips_cpu_set_register(cpu, 6, 399);//jsrc
	storedLocation = 399;
	memwrite1 = 0x12;
	memwrite2 = 0x34;
	memwrite3 = 0x56;
	memwrite4 = 0x78;
	e = mips_mem_write(mem, storedLocation, 1, (uint8_t*)&memwrite1);
	e = mips_mem_write(mem, storedLocation - 1, 1, (uint8_t*)&memwrite2);
	e = mips_mem_write(mem, storedLocation - 2, 1, (uint8_t*)&memwrite3);
	e = mips_mem_write(mem, storedLocation - 3, 1, (uint8_t*)&memwrite4);

	// 3 - step CPU
	e = mips_cpu_step(cpu);
	if (e != mips_Success) {
		e = mips_cpu_set_pc(cpu, pcCurrentVal);
		fprintf(stderr, "mips_cpu_step : failed.\n");
		exit(1);
	}
	// 4 -Check the result
	e = mips_cpu_get_register(cpu, 5, &got);//jsrc
	passed = got == 0x88563412;
	mips_test_end_test(testId, passed, "Failed");

/*=====================================================================================================*/
	//test all end
	mips_test_end_suite();
	return 0;
}