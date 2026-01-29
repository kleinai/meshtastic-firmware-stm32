/* https://www.st.com/resource/en/user_manual/um2592-stm32wl-nucleo64-board-mb1389-stmicroelectronics.pdf
 * NUCLEO_WL55JC module transmits HP or LP
 * Receive: PC3=1, PC4=0, PC5=1
 * Transmit(high output power, SMPS mode): PC3=1, PC4=0, PC5=1
 * Transmit(low output power, SMPS mode): PC3=1, PC4=1, PC5=1
 */
static const RADIOLIB_PIN_TYPE rfswitch_pins[5] = {PC4, PC5, PC3, RADIOLIB_NC, RADIOLIB_NC};

static const Module::RfSwitchMode_t rfswitch_table[5] = {
    {STM32WLx::MODE_IDLE, {LOW, LOW, LOW}}, {STM32WLx::MODE_RX, {LOW, HIGH, LOW}}, {STM32WLx::MODE_TX_HP, {LOW, HIGH, HIGH}}, {STM32WLx::MODE_TX_LP, {HIGH, HIGH, HIGH}}, END_OF_MODE_TABLE};
