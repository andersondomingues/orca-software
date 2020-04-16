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
 
//basic resources
#include "orca-core.h"

//#include "../../../applications/int-mult/include/int-mult.h"
//#include "../../../applications/float-mult/include/float-mult.h"
//#include "../../../applications/cpp-float-mult/include/cpp-float-mult.h"
//#include "../../../applications/cpp-mult/include/cpp-mult.h"
//#include "../../../applications/mnist/include/mnist.h"
//#include "../../../applications/mnist-ext-mult/include/mnist-ext-mult.h"
//#include "../../../applications/mnist-ext-vet-mult/include/mnist-ext-vet-mult.h"
//#include "../../../applications/mnist-ext-vet-seq-mult/include/mnist-ext-vet-seq-mult.h"
//#include "../../../applications/mnist-ext-mult-dma/include/mnist-ext-mult-dma.h"
#include "../../../applications/mnist-two-layers/include/mnist-two-layers.h"
//#include "../../../applications/simple-dma/include/simple-dma.h"

//#include "../../../applications/cpp-mult/include/cpp-mult.h"
//#include "../../../applications/ann/include/ann.h" 
//#include "../../../applications/producer-consumer/include/producer-consumer.h" 

//Task mapping routine and entry-point. Please note that 
//task mapping is done through software and the code below
//runs at the startup of each node. We select the tasks to 
//be loaded into each node according to nodes' ID. Startup
//routines that affect all applications can be handled here.
void app_main(void)
{   
	//#ifdef CPU_ID == 22
	//hfs
	//#elif CPU_ID == 32
	//

	//printf("cpu_id: %d\n", hf_cpuid());

	//hf_spawn(int_mult, 5, 4, 5, "int_mult", 1024);   //10%
	//hf_spawn(cpp_mult,5, 4, 5, "cpp_mult", 1024);   //10%
	//hf_spawn(float_mult,  5, 4, 5, "float_mult", 1024);   //10%
	//hf_spawn(cpp_float_mult,  5, 4, 5, "cpp_float_mult", 1024);   //10%
	//hf_spawn(mnist, 5, 4, 5, "mnist", 128 * 1024);   // 128 Kbytes !!!
	//hf_spawn(mnist_ext_mult, 5, 4, 5, "mnist_ext_mult", 128 * 1024);   // 128 Kbytes !!!
	//hf_spawn(mnist_ext_vet_mult, 5, 4, 5, "mnist_ext_vet_mult", 128 * 1024);   // 128 Kbytes !!
	//hf_spawn(mnist_ext_vet_seq_mult, 5, 4, 5, "mnist_ext_vet_seq_mult", 256 * 1024); // 256 Kbytes !!
	//hf_spawn(mnist_ext_mult_dma, 5, 4, 5, "mnist_ext_mult_dma", 256 * 1024); // 256 Kbytes !!
	hf_spawn(mnist_two_layers, 5, 4, 5, "mnist_two_layers", 128 * 1024); // 128 Kbytes !!
	//hf_spawn(simple_dma, 5, 4, 5, "simple_dma", 1024);
	//hf_spawn(int_mult, 5, 4, 5, "int_mult", 1024);   //10%
	//hf_spawn(float_mult, 5, 4, 5, "float_mult", 1024);   //10%
	//hf_spawn(cpp_mult, 5, 4, 5, "cpp_mult", 1024);   //10%
	//hf_spawn(ann, 5, 4, 5, "ann", 10*1024);   //10% 
	//hf_spawn(producer,5, 4, 5, "producer-task", 2048);

/*
		//PRODUTOR-CONSUMIDOR
		//hf_spawn(producer, 0, 0, 0, "producer-task", 2048);
		//hf_spawn(consumer, 0, 0, 0, "consumer-task", 2048);
	
		case 1: // << EKF
			hf_spawn(producer_pubsub, 10, 1, 10, "producer-ps-task", 1024);   //10%
			break;

		case 2: // << PID
			hf_spawn(app_spawner, 10, 1, 10, "app-spawner", 1024);          //~10%
			hf_spawn(consumer_pubsub, 10, 1, 3, "consumer-ps-task", 2048); //~10%
			hf_spawn(deadline_monitor, 10, 1, 10, "deadline-monitor", 1024); //~10%
			break;

		case 3: // << BROKER
			hf_spawn(pubsub_broker_tsk, 10, 1, 10, "broker-ps-task", 2048); //~10%
			break;

		case 4: // << new PID
			hf_spawn(app_spawner, 10, 1, 10, "app-spawner", 1024);          //~10%
			break;

		case 5: // bloater
			hf_spawn(app_bloater, 10, 1, 10, "app-bloater", 4096);          //~10%

		default: // << NONE
			break;
	}
*/	
	//spawn for all cores
	//hf_spawn(counter_test, 0, 0, 0, "counters_test", 4096);

	// 0,0,0 => allocate for best-effort

	 //allocating real-time for ~90% (9/10)
	 //10 : period
	 // 9 : capacity
	 //10 : dealine

	 //to allocate best-effort tasks, use hf_spawn(tskname, 0, 0, 0, "name", stacksize);	 
}
	 
	 

