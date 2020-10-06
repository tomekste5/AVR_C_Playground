#define __AVR_ATmega328P__
#include <Serial.h>
#include <avr/interrupt.h>

#define UART_MAXSTRLEN 80

    volatile uint8_t uart_str_complete = 0;     // 1 .. String komplett empfangen
    volatile uint8_t uart_str_count = 0;
    volatile char buffer[UART_MAXSTRLEN + 1];

    void shift(){
      for(int i = 1; i < uart_str_count; i++){
          buffer[i-1] = buffer[i];
      }
    }

void Serial::begin(){

    unsigned int ubrr = UBRRH_VALUE<<8|UBRRL_VALUE;
      //Baudrate setzten erst High dann low
    UBRR0H = (ubrr>>8);
    UBRR0L = (ubrr);

    UCSR0C = 0x06;
    UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);  // Asynchron 8N1 

    sei();
                  
}
Serial serial;

ISR(USART_RX_vect, ISR_BLOCK)
{
  unsigned char nextChar;
  nextChar = UDR0;
  if(nextChar != '\n'){
    if(uart_str_count < UART_MAXSTRLEN ) {
      buffer[uart_str_count] = nextChar;
      uart_str_count++;
    }
  }
}

/*ISR(USART_RX_vect, ISR_BLOCK)
{
  unsigned char nextChar;
  nextChar = UDR0;
    if(uart_str_count < UART_MAXSTRLEN ) {
      buffer[uart_str_count] = nextChar;
      uart_str_count++;
    }
}*/

uint8_t Serial::available(){
  return uart_str_count;
}

unsigned char Serial::read()
{
    if(uart_str_count > 0){
      unsigned char c =  buffer[0];
      shift();
      uart_str_count -= 1;
      return c;
    }

    return 0;
                    
}

void Serial::print(unsigned char data[]){
    int i = 0;
    while(1)  
    { 
        i = 0;
        while(data[i] != 0) 
        {
          while (!( UCSR0A & (1<<UDRE0))); 
          UDR0 = data[i];           
          i++;                          
        }
      } 
}
void Serial::println(unsigned char data[]){
    int i = 0;
    while(1) 
    { 
        i = 0;
        while(data[i] != 0) 
        {
          while (!( UCSR0A & (1<<UDRE0))); 
          UDR0 = data[i];          
          i++;                                   
        }
         while (!( UCSR0A & (1<<UDRE0)));
      } 
}
void Serial::printCharln(unsigned char data){
  while (!( UCSR0A & (1<<UDRE0))); 
  UDR0 = data;
     
  while (!( UCSR0A & (1<<UDRE0)));
  UDR0 = '\n';
}

void Serial::printChar(unsigned char data){
  while (!( UCSR0A & (1<<UDRE0))); 
  UDR0 = data;          
}
void Serial::close(){

    UCSR0B = (0<<RXEN0)|(0<<TXEN0)|(0<<RXCIE0);  // Asynchron 8N1 
    UCSR0C = (0<<USBS0)|(0<<UCSZ00);   

}
