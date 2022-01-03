# MBed

Some simple example using the Mbed online compiler with STM microcontroller




disable the tx interrupt first

```
void UART_CH2_SERIAL_INIT()
{
    UART_CH2.attach(NULL,Serial::TxIrq);
    UART_CH2.attach(&UART_CH2_Rx_interrupt, Serial::RxIrq);
    UART_CH2_rx_StringCommand[0] = 'i';
    UART_CH2_rx_CommandLength = 1;
    UART_CH2_send("UART_CH2_SERIAL_INIT",sizeof("UART_CH2_SERIAL_INIT"));
}
```

