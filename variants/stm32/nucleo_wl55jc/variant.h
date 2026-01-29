/*
NUCLEO_WL55JC
https://www.st.com/en/evaluation-tools/nucleo-wl55jc.html
*/

/*
This variant is a work in progress.
Do not expect a working Meshtastic device with this target.
*/

#ifndef _VARIANT_NUCLEO_WL55JC_
#define _VARIANT_NUCLEO_WL55JC_

#define USE_STM32WLx

#define LED_PIN PB15
#define LED_STATE_ON 1

#define NUCLEO_WL55JC

#if (defined(LED_BUILTIN) && LED_BUILTIN == PNUM_NOT_DEFINED)
#undef LED_BUILTIN
#define LED_BUILTIN (LED_PIN)
#endif

#endif
