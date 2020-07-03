#include <hfunit-example.h>
#include <stdio.h>
//#include "../../../libs/orca-baremetal-dma-driver/include/orca-baremetal-dma-driver.h"

int main(){

	int x = 3 + 2;
	int y = 5;
	
	hfunit_assert(x != y, "x equals to y");

	//return failed results back to hardware
	return hfunit_get_test_failed();
}
