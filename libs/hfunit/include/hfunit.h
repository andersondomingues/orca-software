/*
 * Implementation file for ORCA-LIB library.
 * Copyright (C) 2018-2019 Anderson Domingues, <ti.andersondomingues@gmail.com>
 * This file is part of project URSA (http://https://github.com/andersondomingues/ursa).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */
#ifndef __HF_UNIT_H
#define __HF_UNIT_H

#include <stdint.h>

/*** **********************************************************
 * 
 * HF-RISC Unit Testing framework 
 * 
 * -> edit these macroses below to set the behavior of HF-UNIT
 * -> see usage examples in the dir software/tests 
 * 
 ************************************************************ */
  
/** number of executed tests. **/
extern int _hfunit_counter_total_tests;

/** number of tests that passed. **/
extern int _hfunit_counter_passed_tests;

/** number of tests that failed. **/
extern int _hfunit_counter_failed_tests;
  
// floating point comparison precision
#define HFUNIT_PRECISION 0.00001

// define it to show message for every test
#define HFUNIT_VERBOSE 1

// enable terminal color
#define HFUNIT_TERMINAL 1

// define the output message format in case verbose mode is on
#ifdef HFUNIT_VERBOSE
	#ifdef HFUNIT_TERMINAL
		#define HFUNIT_MSG_FAIL(assertion_name); printf("hfunit: assertion %d (\"%s\") \e[0;31mfailed\e[0;30m\n", _hfunit_counter_total_tests, assertion_name);
		#define HFUNIT_MSG_PASS(assertion_name); printf("hfunit: assertion %d (\"%s\") \e[0;32mpassed\e[0;30m\n", _hfunit_counter_total_tests, assertion_name);
	#else
		#define HFUNIT_MSG_FAIL(assertion_name); printf("hfunit: assertion %d (\"%s\") failed\n", _hfunit_counter_total_tests, assertion_name);
		#define HFUNIT_MSG_PASS(assertion_name); printf("hfunit: assertion %d (\"%s\") passed\n", _hfunit_counter_total_tests, assertion_name);
	#endif
#else
	#define HFUNIT_MSG_FAIL(assertion_name);
	#define HFUNIT_MSG_PASS(assertion_name);
#endif

#ifdef HFUNIT_VERBOSE
	#define HFUNIT_MSG(msg); printf("hfunit: %s\n", msg);
	#define HFUNIT_MSG_P1(msg, par); printf("hfunit: %s: %d\n", msg, par);
#else
	#define HFUNIT_MSG(msg);
	#define HFUNIT_MSG_P1(msg, par);
#endif

//base functions
/**
 * @brief Reset internal counters and starts a new test session.
 */
void hfunit_init();

/**
 * @brief Entry-point for the unit framework, replaces "main"
 * @param argc optional number of arguments
 * @param argv optional array of arguments
 * @return status code 
 */
int main_test(int argc, char** argv);

/**
 * @brief Report test statistics ifdef HFUNIT_VERBOSE
 */
void hfunit_report();

/**
 * @brief Functiona that actually tests something
 */
void hfunit_test();

/**
 * @brief Asserts a condition and prints respective messages. 
 * @param condition to be asserted, must be a boolean expression
 * @param assertion_name a name to appear in the printed message 
 */
void hfunit_assert(int condition, char* assertion_name);

/**
 * @brief Getter for the number of executed tests.
 * @return The number of tests that passed since the last call of hfunit_init().
 */
uint32_t hfunit_get_test_count();

/**
 * @brief Getter for the number of tests that passed.
 * @return The number of tests that were executed since the last call of hfunit_init().
 */
uint32_t hfunit_get_test_passed();

/**
 * @brief Getter for the number of tests that failed.
 * @return The number of tests that failed since the last call of hfunit_init().
 */
uint32_t hfunit_get_test_failed();

//auxiliary functions

/**
 * @brief Compare two vectors of integer types
 * @param v1 the first vector
 * @param v2 the second vector
 * @param size the size of both vectors
 * @return the number of elements unique to some vector
 */
int hfunit_veccomp(void* v1, void* v2, uint32_t size);

/**
 * @brief Compare two vectors of floats
 * @param v1 the first vector
 * @param v2 the second vector
 * @param size the size of both vectors
 * @return the number of elements unique to some vector
 */
int hfunit_comp_f(float f1, float f2, uint32_t size);


// =======================
// util functions
// =======================
// debug function to print data in binary format
// source: https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
void printBits(int const size, void const * const ptr);


#endif /* __HF_UNIT_H */

