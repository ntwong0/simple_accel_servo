/* @author ntwong0
 * Standard functions to interface to I2C bus
 */ 

#include "driver/i2c.h"

#ifndef __MY_I2C_H__
#define __MY_I2C_H__

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

#define I2C_SLAVE_SCL_IO CONFIG_I2C_SLAVE_SCL               /*!< gpio number for i2c slave clock */
#define I2C_SLAVE_SDA_IO CONFIG_I2C_SLAVE_SDA               /*!< gpio number for i2c slave data */
#define I2C_SLAVE_NUM I2C_NUMBER(CONFIG_I2C_SLAVE_PORT_NUM) /*!< I2C port number for slave dev */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave rx buffer size */

#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY        /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define BH1750_SENSOR_ADDR CONFIG_BH1750_ADDR   /*!< slave address for BH1750 sensor */
#define BH1750_CMD_START CONFIG_BH1750_OPMODE   /*!< Operation mode */
#define ESP_SLAVE_ADDR CONFIG_I2C_SLAVE_ADDRESS /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */

#define I2C_EXAMPLE_MASTER_FREQ_HZ 100000
#define I2C_EXAMPLE_MASTER_NUM I2C_NUM_0
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE 0
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE 0

void my_I2C_master_init(gpio_num_t sda, gpio_num_t scl)
{
     // i2c_port_t i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t config;
    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = sda;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.scl_io_num = scl;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(I2C_EXAMPLE_MASTER_NUM, &config);
    i2c_driver_install(I2C_EXAMPLE_MASTER_NUM, config.mode, I2C_EXAMPLE_MASTER_RX_BUF_DISABLE, I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
}

void my_I2C_master_write(uint8_t slv_addr, uint8_t slv_cmd, uint32_t str_len, const char* str)
{
    if(str_len < 1) return;
    else
    {
        // int ret;
        uint32_t bytes_remaining = str_len; 
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, slv_addr << 1 | WRITE_BIT, ACK_CHECK_EN);
        i2c_master_write_byte(cmd, slv_cmd, ACK_CHECK_EN);
        while(bytes_remaining > 0)
        {
            i2c_master_write_byte(cmd, 
                (uint8_t) str[str_len - bytes_remaining], 
                ACK_CHECK_EN);
            bytes_remaining--;
        }
        i2c_master_stop(cmd);
        // ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
        i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
    }
}

void my_I2C_master_read(uint8_t slv_addr, uint8_t slv_cmd, uint32_t str_len, char* str)
{
    if(str_len < 1) return;
    else
    {
        // int ret;
        uint32_t bytes_remaining = str_len; 
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, slv_addr << 1 | WRITE_BIT, ACK_CHECK_EN);
        i2c_master_write_byte(cmd, slv_cmd, ACK_CHECK_EN);
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
        vTaskDelay(30 / portTICK_RATE_MS);
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, slv_addr << 1 | READ_BIT, ACK_CHECK_EN);
        while(bytes_remaining > 1)
        {
            i2c_master_read_byte(cmd,  
                (uint8_t*) &str[str_len - bytes_remaining], 
                (i2c_ack_type_t) ACK_VAL);
            bytes_remaining--;
        }
        i2c_master_read_byte(cmd, (uint8_t*) &str[str_len - bytes_remaining], (i2c_ack_type_t) NACK_VAL);
        i2c_master_stop(cmd);
        // ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
        i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
    }
}

#endif //__MY_I2C_H__