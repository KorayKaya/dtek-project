/**
 * @file    display.c
 * @author  F Lundevall
 * @author  Axel Isaksson
 * @author  Joel Wachsler (wachsler@kth.se)
 * @author  Marcus Werlinder (werli@kth.se)
 * @date    2015-2016
 * @copyright For copyright and licensing, see file COPYING
 *
 * This file was written 2016 by Joel wachsler and Marcus Werlinder
 * Content of this file is based on the file "mipslabwork.c" 2015 by F Lundevall
 * Some parts are original code written by Axel Isaksson
 */

#include <stdint.h>         /* Declarations of uint_32 and the like */
#include <pic32mx.h>        /* Declarations of system-specific addresses etc */
#include "declaration.h"    /* Declarations of project specific functions */
#include <stdbool.h>    // bool

// Declare static buffer for the screen
static uint8_t buffer[512] = {0};
static uint8_t image1[13] = {0};
static uint8_t image2[13] = {0};



#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/**
 * SPI2 helper function to determine if the interface is ready.
 */
uint8_t spi_send_recv(uint8_t data) {
    while(!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while(!(SPI2STAT & 1));
    return SPI2BUF;
}

static uint8_t buffer_copy[512];


void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}



void display_image(int x, const uint8_t *data) {
	int i, j;

	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 32; j++)
			spi_send_recv(~data[i*32 + j]);
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






/**
 * Draw the menu.
 */
void draw_menu(void){
    unsigned short i;

    for(i = 0; i < 4; i++)
        buffer[(i + 1) * 128 - 1] |= 255;

    for(i = 0; i < 512; i++)
        buffer[i] |= menuFont[i];
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
    for(page = 0; page < 4; page++) {
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
        for(j = 0; j < 128; j++) {
            // Each byte sent to this function is a 8 pixel high column on the display
            // the lsb is the top most pixel and the msb is the most bottom pixel
            spi_send_recv(buffer[page*128 + j]);
            // Clear the buffer after drawing it
            buffer[page*128 + j] = 0x0;
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
    for(i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0x0);
        spi_send_recv(0x10);

        DISPLAY_CHANGE_TO_DATA_MODE;

        for(j = 0; j < 16; j++) {
            c = textbuffer[i][j];
            if(c & 0x80)
                continue;

            for(k = 0; k < 8; k++)
                spi_send_recv(font[c*8 + k]);
        }
    }



}

void gravity(Object* object,int speed) {
object->y*=speed;
}



void x1(Object* object){if(object->x+1<32)object->x++;}
void x2(Object* object){if(object->x>object->imageData.width)object->x--;}
void y1(Object* object){if(object->y+1<128)object->y++;}
void y2(Object* object){if(object->y>object->imageData.height)object->y--;}

bool twoPages = false;

void generatePixel(Object object){
  int position = object.x%8;
  shiftPixelsRight(7-position,object);
  if(position<object.imageData.width-1){
    shiftPixelsLeft(position+1, object);
    twoPages=true;}
}

void draw_object(Object object) {
int i, k;



int currentPage = checkCurrentPage(object.x);


if(-1<object.y){  //check kansek onödig man kan komma runt det genom att kalla på en metod innan.
for(i = 0; i < object.imageData.height; i++){buffer[128*currentPage+(128-object.y)+i] |= image1[i];}

if(twoPages){for( k = 0; k < object.imageData.height; k++){buffer[128*(currentPage-1)+(128-object.y)+k] |= image2[k];}}
}

twoPages=false;

}



int checkCurrentPage(int x){
  if(x>-1 && x<8){return 0;}
  else if(x>7 && x<16){return 1;}
  else if(x>15 && x<24){return 2;}
  else {return 3;}
}

void shiftPixelsLeft(int shift, Object object){int i;for(i = 0; i < object.imageData.height; i++){image2[i] = object.imageData.image[i]<<shift;}}
void shiftPixelsRight(int shift, Object object){int i;for(i= 0; i < object.imageData.height; i++){image1[i] = object.imageData.image[i]>>shift;}}




static void main_menu(void) {



if(btns&8)
y1(&rocket);
if(btns&4)
y2(&rocket);
if(btns&2)
x1(&rocket);
if(btns&1)
x2(&rocket);


 gravity(&met1,1);
gravity(&met2,1);
gravity(&met3,2);
    gravity(&met4,3);



generatePixel(rocket);
    draw_object(rocket);
    generatePixel(met1);
    draw_object(met1);
    generatePixel(met2);
    draw_object(met2);
    generatePixel(met3);
    draw_object(met3);
    generatePixel(met4);
    draw_object(met4);


    draw_object(rocket);
    draw_menu();
}
