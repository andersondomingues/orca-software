/*
 * Header file for EXAMPLE-COUNTERS application.
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
 
#ifndef _CPP_EXAMPLE_H
#define _CPP_EXAMPLE_H

#include <iostream>

#ifdef HELLFIREOS
#include <hellfire.h>
#endif

#ifndef HFRISCV_ENABLE_COUNTERS
#error "This application requires HFRISC_ENABLE_COUNTERS to be set."
#endif

#ifndef MEMORY_ENABLE_COUNTERS
#error "This application requires MEMORY_ENABLE_COUNTERS to be set."
#endif

class ExampleBaseClass
{
public:
	ExampleBaseClass() {
		std::cout << "ExampleBaseClass()" << std::endl;
	}

	virtual ~ExampleBaseClass() {
		std::cout << "~ExampleBaseClass()" << std::endl;
	}

	virtual void print_something_virt() {
		std::cout << "ExampleBaseClass::print_something_virt()" << std::endl;
	}

	void print_something_novirt() {
		std::cout << "ExampleBaseClass::print_something_novirt()" << std::endl;
	}
};

class ExampleSubClass : public ExampleBaseClass
{
public:
	ExampleSubClass() {
		std::cout << "ExampleSubClass()" << std::endl;
	}

	virtual ~ExampleSubClass() {
		std::cout << "~ExampleSubClass()" << std::endl;
	}

	virtual void print_something_virt() {
		std::cout << "ExampleSubClass::print_something_virt()" << std::endl;
	}

	void print_something_novirt() {
		std::cout << "ExampleSubClass::print_something_novirt()" << std::endl;
	}
};

void cpp_example(void); // __attribute__((section (".tasks")));

#endif /* _CPP_EXAMPLE_H */
