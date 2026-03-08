#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_lcd_gc9a01.h"
#include "esp_lcd_panel_commands.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_io_interface.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_vendor.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "kit.c"

#define LCD_HOST SPI2_HOST
#define PIN_NUM_SCLK 10
#define PIN_NUM_MOSI 11
#define PIN_NUM_MISO 12
#define PIN_NUM_CS 9
#define PIN_NUM_DC 8
#define PIN_NUM_BKL 2
#define PIN_NUM_RESET 14
#define LCD_H_RES 240
#define LCD_V_RES 240
#define LCD_PIXEL_CLOCK_HZ 20 * 1000 * 1000
#define LCD_CMD_BITS 8
#define LCD_PARAM_BITS 8

esp_lcd_panel_io_handle_t io_handle = NULL;
esp_lcd_panel_handle_t panel_handle = NULL;


static void display_init(void){
    gpio_set_direction(PIN_NUM_BKL, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_NUM_BKL, 1);
    spi_bus_config_t buscfg = {
    .sclk_io_num = PIN_NUM_SCLK,
    .mosi_io_num = PIN_NUM_MOSI,
    .miso_io_num = PIN_NUM_MISO,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = LCD_H_RES * 80 * sizeof(uint16_t), // transfer 80 lines of pixels (assume pixel is RGB565) at most in one SPI transaction
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO)); // Enable the DMA feature

    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_NUM_DC,
        .cs_gpio_num = PIN_NUM_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    // Attach the LCD to the SPI bus
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
    .reset_gpio_num = PIN_NUM_RESET,
    .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
    .bits_per_pixel = 16,
    };
    // Create LCD panel handle for ST7789, with the SPI IO device handle
    ESP_ERROR_CHECK(esp_lcd_new_panel_gc9a01(io_handle, &panel_config, &panel_handle));


    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_invert_color(panel_handle, true);
    esp_lcd_panel_disp_on_off(panel_handle, true);
}


void app_main(void)
{
    //initialze spi peripheral
    display_init(); 

   // esp_lcd_panel_swap_xy(panel_handle, true);  

    
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 240, 240, image_data_kit);
    while(1) {
        printf("Hello from app main\n");
        printf("Pixels: %d\n", sizeof(image_data_kit) / sizeof(uint16_t));
        sleep(1);
    }
}
