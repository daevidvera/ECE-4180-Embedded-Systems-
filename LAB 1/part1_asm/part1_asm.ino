// #define BUTTON 5
// #define LED 6

// #define GPIO_MATRIX 0x60091000
#define IO_MUX  0x60090000
// int* GPIO_OUTPUT_ENABLE  = (int*) (GPIO_MATRIX + 0x0024);
// int* GPIO_OUTPUT_SET = (int*) (GPIO_MATRIX + 0x0008);
// int* GPIO_OUTPUT_CLEAR = (int*) (GPIO_MATRIX + 0x000C);

int* GPIO_OUTPUT_ENABLE = (int*) 0x60091024; 
int* GPIO_OUTPUT_SET    = (int*) 0x60091008;
int* GPIO_OUTPUT_CLEAR  = (int*) 0x6009100C;
int* IO_MUX_INPUT = (int*) (IO_MUX + 0x14);
int* IO_MUX_GPIO4       = (int*) 0x60090014;
int* IO_MUX_GPIO7 = (int*)(IO_MUX + 0x20);
int* GPIO_INPUT = (int*) 0x6009103C;


void setup() {
  // put your setup code here, to run once:
  *IO_MUX_GPIO7 |= (1 << 12);

  *GPIO_OUTPUT_ENABLE |= 0x80; // gpio 7 
  *IO_MUX_GPIO4 |= (1 << 7);  // this is pulldown 


}

void loop() {
  // put your main code here, to run repeatedly:

  if ((*GPIO_INPUT & (1 << 4)) == 0) {
    *GPIO_OUTPUT_CLEAR = 0x80;
  } 
  else {
    *GPIO_OUTPUT_SET = 0x80;
  }
  

}
