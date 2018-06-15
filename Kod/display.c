#include <stdint.h>         /* Declarations of uint_32 and the like */
#include <pic32mx.h>        /* Declarations of system-specific addresses etc */
#include "declaration.h"    /* Declarations of project specific functions */
#include <stdbool.h>    // bool

static uint8_t buffer[512] = {0};


#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)


uint8_t spi_send_recv(uint8_t data) {
  while (!(SPI2STAT & 0x08));
  SPI2BUF = data;
  while (!(SPI2STAT & 1));
  return SPI2BUF;
}


void quicksleep(int cyc) {
  int i;
  for (i = cyc; i > 0; i--);
}

void display_image(int x, const uint8_t *data) {
  int i, j;

  for (i = 0; i < 4; i++) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;

    spi_send_recv(0x22);
    spi_send_recv(i);

    spi_send_recv(x & 0xF);
    spi_send_recv(0x10 | ((x >> 4) & 0xF));

    DISPLAY_CHANGE_TO_DATA_MODE;

    for (j = 0; j < 32; j++)
      spi_send_recv(~data[i * 32 + j]);
  }
}

/**
 * Commands to initalize the oled sqreen. If they are not done
 * in this order the screen could be damaged!
 */
void display_init(void) {
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  quicksleep(10);
  DISPLAY_ACTIVATE_VDD;
  quicksleep(1000000);

  spi_send_recv(0xAE);
  DISPLAY_ACTIVATE_RESET;
  quicksleep(10);
  DISPLAY_DO_NOT_RESET;
  quicksleep(10);

  spi_send_recv(0x8D);
  spi_send_recv(0x14);

  spi_send_recv(0xD9);
  spi_send_recv(0xF1);

  DISPLAY_ACTIVATE_VBAT;
  quicksleep(10000000);

  spi_send_recv(0xA1);
  spi_send_recv(0xC8);

  spi_send_recv(0xDA);
  spi_send_recv(0x20);

  spi_send_recv(0xAF);
}


//Draws a line at y postion
void draw_line(int y) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 3; j++)
    {
      buffer[i * 128 + y + j] = buffer[i * 128 + y + j] ^ 0xff;
    }
  }
}

/**
 * This function puts the data from the buffer provided to it and sends it to
 * the screen. The buffer is always 4 * 128 bytes (512 bytes) big.
 * Gets its data from the static buffer.
 */
void render(void) {
  int page, j;
  // 4 stripes across the display called pages
  // each stripe is 8 pixels high and can hold 128 bytes
  for (page = 0; page < 4; page++) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;

    spi_send_recv(0x22);    // Command to set the page
    spi_send_recv(page);    // Set the current page

    // Don't really know how these work
    // But they have something to do with the x value from
    // the left side of the screen
    spi_send_recv(0x0);
    spi_send_recv(0x10);

    DISPLAY_CHANGE_TO_DATA_MODE;

    // j is the x axis of the current page
    for (j = 0; j < 128; j++) {
      // Each byte sent to this function is a 8 pixel high column on the display
      // the lsb is the top most pixel and the msb is the most bottom pixel
      spi_send_recv(buffer[page * 128 + j]);
      // Clear the buffer after drawing it
      buffer[page * 128 + j] = 0x0;
    }
  }
}

/**
 * Don't really know how this one works but it clears the screen
 * of items not being used.
 */
void display_update(void) {
  int i, j, k;
  int c;
  for (i = 0; i < 4; i++) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22);
    spi_send_recv(i);

    spi_send_recv(0x0);
    spi_send_recv(0x10);

    DISPLAY_CHANGE_TO_DATA_MODE;

    for (j = 0; j < 16; j++) {
      c = textbuffer[i][j];
      if (c & 0x80)
        continue;

      for (k = 0; k < 8; k++)
        spi_send_recv(font[c * 8 + k]);
    }
  }



}


//function that does all the graphics! beautiful
void addToBufferSquare(uint32_t image[], int x, int y, int width, int height){
  int i, j;
  for (i = 0; i < height; i++)
  {
    uint32_t row = image[i] << x;
    if ((y+i)<1 || (y+i)>128)
    {
      continue;
    }
    for (j = 0; j < 4; j++)
    {
      buffer[j * 128 + y + i] ^= (row & (0x000000ff << (j * 8))) >> (j * 8);
    }
  }
}


void draw_number(int number, int x, int y) {
  int i, j, firstNumber, secondNumber;

  firstNumber = number % 10;
  if (number > 9) {
    secondNumber = firstNumber;
    firstNumber = (number - secondNumber) / 10;
  }
  addToBufferSquare(numbers[firstNumber],x,y,8,8);

  if (number > 9) {
    addToBufferSquare(numbers[secondNumber],(x-9),y,8,8);
  }

}

void addToBuffer(struct Entity entity) {
  addToBufferSquare(entity.imageData.image,entity.x,entity.y,entity.imageData.width,entity.imageData.height);
}

void addToBufferImage(const uint32_t image[] ) {
  addToBufferSquare(image, 0,0,32,128);
}