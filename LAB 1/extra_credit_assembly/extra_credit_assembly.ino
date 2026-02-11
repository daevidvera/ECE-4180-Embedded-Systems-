

void setup() {
  asm volatile(
    // Enable GPIO7 as output
    "li   t0, 0x60091024\n"    // *GPIO_OUTPUT_ENABLE |= 0x80;
    "lw   t1, 0(t0)\n"
    "ori  t1, t1, 0x80\n" 
    "sw   t1, 0(t0)\n"

    // setting up  GPIO7 input as function 1
    "li t0, 60090020\n" // io mux + 0x20
    "lw t1, 0(t0)\n" 
    "li t2, 1\n" // 1 
    "slli t2, t2, 12\n" // 1 << 12
    "or t1, t1, t2\n" // gpio7 |= 1 << 12
    "sw t1, 0(t0)\n"

    // Enable pull-down on GPIO4
    "li   t0, 0x60090014\n"    // *IO_MUX_GPIO4 |= ;
    "lw   t1, 0(t0)\n"
    "li t2, 1\n" // 1 
    "slli t2, t2, 7\n" // 1 << 7
    "or t1, t1, t2\n" // io_mux_gpio4 |= 1 << 7 
    "sw   t1, 0(t0)\n"

  );
}

void loop() {
  asm volatile(
    // Read GPIO input register
    "li   t0, 0x6009103C\n"    // if((*GPIO_INPUT & (1 << 4)) == 0)
    "lw   t1, 0(t0)\n"
    "andi t2, t1, 0x10\n"
    "bne  x0, t2, else\n"

    // if
    "li   t0, 0x6009100C\n"    // *GPIO_OUTPUT_CLEAR = 0x80;
    "li   t1, 0x80\n"
    "sw   t1, 0(t0)\n"
    "jal  x0, skipElse\n"

    // else
    "else:\n"
    "li   t0, 0x60091008\n"    // *GPIO_OUTPUT_SET = 0x80;
    "li   t1, 0x80\n"
    "sw   t1, 0(t0)\n"

    "skipElse:\n"
  );
}