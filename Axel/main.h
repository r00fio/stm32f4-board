
void Usart_Printf(void);
GPIO_InitTypeDef buildUsartGpioInit(uint32_t pin);
void splitAndSend(int16_t coordinate);
void USART_Configuration(void);
void sendData(int8_t ch);
void USART_PutChar(char ch);
int println(char *str);