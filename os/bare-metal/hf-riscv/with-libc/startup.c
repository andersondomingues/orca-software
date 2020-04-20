//#include "samd21.h"
#include <stdint.h>

/* Initialize segments */
extern uint32_t _stext;
extern uint32_t _etext;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sstack;
extern uint32_t _estack;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void __libc_init_array(void);

/* Default empty handler */
void Dummy_Handler(void);
void Reset_Handler(void);


/* Cortex-M0+ core handlers */
void NMI_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler             ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

typedef struct _DeviceVectors
{
  /* Stack pointer */
  void* pvStack;
  /* Cortex-M handlers */
  void* pfnReset_Handler;
  void* pfnNMI_Handler;
  void* pfnHardFault_Handler;
  void* pvReservedM12;
  void* pvReservedM11;

} DeviceVectors;

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {
        /* Configure Initial Stack Pointer, using linker-generated symbols */
        .pvStack                = (void*) (&_estack),
        .pfnReset_Handler       = (void*) Reset_Handler,
        .pfnNMI_Handler         = (void*) NMI_Handler,
        .pfnHardFault_Handler   = (void*) HardFault_Handler,
        .pvReservedM12          = (void*) (0UL), /* Reserved */
        .pvReservedM11          = (void*) (0UL), /* Reserved */
};

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
        /* Initialize the data segment */
        uint32_t *pSrc = &_etext;
        uint32_t *pDest = &_sdata;

        if (pSrc != pDest) {
                for (; pDest < &_edata;) {
                        *pDest++ = *pSrc++;
                }
        }

        /* Clear the zero segment */
        for (pDest = &_sbss; pDest < &_ebss;) {
                *pDest++ = 0;
        }

        // /* Set the vector table base address */
        // pSrc = (uint32_t *) & _stext;
        // SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

        // /* Change default QOS values to have the best performance */
        // SBMATRIX->SFR[SBMATRIX_SLAVE_HMCRAMC0].reg = 2;
        // DMAC->QOSCTRL.bit.DQOS = 2;
        // DMAC->QOSCTRL.bit.FQOS = 2;
        // DMAC->QOSCTRL.bit.WRBQOS = 2;

        // /* Overwriting the default value of the NVMCTRL.CTRLB.MANW bit (errata reference 13134) */
        // NVMCTRL->CTRLB.bit.MANW = 1;


        /* Run constructors / initializers */
#ifndef CUSTOM_LIBC
        __libc_init_array();
#endif

        /* Branch to main function */
        main();

        /* Infinite loop */
        while (1);
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
        while (1) {
        }
}
