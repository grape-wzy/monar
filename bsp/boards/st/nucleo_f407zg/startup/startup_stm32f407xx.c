#include <stdint.h>

void SystemInit(void);
int main(void);

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

void Reset_Handler(void);
static void mn_default_handler(void);

void NMI_Handler(void) __attribute__((weak, alias("mn_default_handler")));
void HardFault_Handler(void) __attribute__((weak, alias("mn_default_handler")));
void MemManage_Handler(void) __attribute__((weak, alias("mn_default_handler")));
void BusFault_Handler(void) __attribute__((weak, alias("mn_default_handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("mn_default_handler")));
void SVC_Handler(void) __attribute__((weak, alias("mn_default_handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("mn_default_handler")));
void PendSV_Handler(void) __attribute__((weak, alias("mn_default_handler")));
void SysTick_Handler(void) __attribute__((weak, alias("mn_default_handler")));

__attribute__((section(".isr_vector")))
void (*const mn_vector_table[])(void) = {
    (void (*)(void))(&_estack),
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler
};

void Reset_Handler(void)
{
    uint32_t *src;
    uint32_t *dst;

    src = &_sidata;
    dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0u;
    }

    SystemInit();
    (void)main();

    while (1) {
    }
}

static void mn_default_handler(void)
{
    while (1) {
    }
}
