/* @author ntwong0
 * oof program
 */

#include "Arduino.h"
#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string>
#include <iostream>
#include "my_i2c.h"
#include "esp_task_wdt.h"
#include "my_accel_to_angles.h"
#include "driver/mcpwm.h"
#include "esp32_servo.h"

constexpr uint8_t kLedBuiltin = 2;
// const char kSsid[] = "MyESP32AP";
// const char kPassword[] = "testpassword";
// const char kSsid[] = "Oyster";
// const char kPassword[] = "blackpearl666";
// const char kSsid[] = "F2610-G-2PW2100";
// const char kPassword[] = "alpacasforever";
const char kSsid[] = "TP-Link_B127";
const char kPassword[] = "36483617";

AsyncWebServer server(80);
AsyncEventSource events("/events");

void vAccelXHRTask(void * parameter);
void vAccelTask(void *pvParameter);
void vServoTask(void *pvParameter);
void vServoXHRTask(void *pvParameter);

uint32_t degree_of_rotation;
char imu_buf[12];

void setup()
{
    Serial.begin(115200);

    pinMode(kLedBuiltin, OUTPUT);

    // WiFi.softAP(kSsid, kPassword);
    // Serial.println();
    // Serial.print("IP address: ");
    // Serial.println(WiFi.softAPIP());

    wl_status_t status = WL_DISCONNECTED;
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect\n");
      
      // Connect to WPA/WPA2 network:
      status = WiFi.begin(kSsid, kPassword);

      // wait 10 seconds for connection:
      delay(10000);
    }
    Serial.print("Connected\n");

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");


    xTaskCreate(
                vAccelXHRTask,     /* Task function. */
                "vAccelXHRTask",   /* String with name of task. */
                10000,        /* Stack size in words. */
                NULL,        /* Parameter passed as input of the task */
                1,           /* Priority of the task. */
                NULL);       /* Task handle. */

    xTaskCreate(
                vAccelTask,     /* Task function. */
                "vAccelTask",   /* String with name of task. */
                10000,        /* Stack size in words. */
                NULL,        /* Parameter passed as input of the task */
                1,           /* Priority of the task. */
                NULL);       /* Task handle. */

    xTaskCreate(
                vServoXHRTask,     /* Task function. */
                "vServoXHRTask",   /* String with name of task. */
                10000,        /* Stack size in words. */
                NULL,        /* Parameter passed as input of the task */
                1,           /* Priority of the task. */
                NULL);       /* Task handle. */


    events.onConnect([](AsyncEventSourceClient *client)
    {
      if(client->lastId())
      {
        Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
      }
      // send event with message "hello!", id current millis
      // and set reconnect delay to 1 second
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
      client->send("hello!", NULL, millis(), 1000);
      delay(1000);
    });
    //HTTP Basic authentication
    // events.setAuthentication("user", "pass");
    server.addHandler(&events);
    server.begin();
}

void loop() {
  delay(100);
}

void vAccelXHRTask( void * parameter )
{
  // server.on("/calculator", HTTP_POST, [](AsyncWebServerRequest *request){
  //     int op = 2; //0 for add, 1 for subtract, 2 if uninitialized
  //     int result = 0;
  //     int args = request->args();
  //     for(int i=0;i<args;i++) {
  //       Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
  //       //check if arg is op code or value
  //       if (!strcmp(request->argName(i).c_str(),"operation")) {
  //         //decide if performing addition or subtraction
  //         op = (!strcmp(request->arg(i).c_str(),"\"add\"") ? 0 : 1);
  //       } else {
  //         int current_val = atoi(request->arg(i).c_str());
  //         if (op == 0) { //addition
  //           result += current_val;
  //         }
  //         else { //subtraction
  //           if (i == 1) {
  //             result = current_val; 
  //           } else {
  //             result -= current_val;
  //           }
  //         }
  //       }
  //     }

  //   std::cout << "Result: " << result << std::endl;

  //   //convert int to char for concatenation in return string
  //   char result_str[10];
  //   itoa(result, result_str, 10);
  //   request->send(200, "text/plain", result_str);
  // });


  server.on("/accel", HTTP_POST, [](AsyncWebServerRequest *request){
      char str_buffer[36];
      snprintf(str_buffer, 36,
        "%02x%02x,%02x%02x,%02x%02x,%02x%02x,%02x%02x,%02x%02x",
        imu_buf[0], imu_buf[1], imu_buf[2], imu_buf[3], imu_buf[4], imu_buf[5],
        imu_buf[6], imu_buf[7], imu_buf[8], imu_buf[9], imu_buf[10], imu_buf[11]
      );

    // char result_str[10];
    // itoa(result, result_str, 10);
    // request->send(200, "text/plain", result_str);
    request->send(200, "text/plain", str_buffer);
  });

  for(;;)
  {
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

void vTemplateTask(void* pvParameter)
{
  //setup
  for(;;)
  {
    //task
  }
  vTaskDelete(NULL);
}

#define MPU_I2C_ADDR                0x68
// #define MPU_I2C_ADDR                0x73 //MPU-9255
#define MPU_PWR_MGMT_1_REG          0x6B
#define MPU_ACCEL_XOUT_H_REG        0x3B
const char MPU_PWR_MGMT_1_CMD = 0x00;
void vAccelTask(void* pvParameter)
{
  // Initialize I2C comm to MPU-6050
  //   const int MPU = 0x68;
  /* As written in .ino:
   *   Wire.begin();
   *   Wire.beginTransmission(MPU);
   *   Wire.write(0x6B);
   *   Wire.write(0);
   *   Wire.endTransmission(true);
   */
  my_I2C_master_init( GPIO_NUM_22,  //sda, use IO22/GPIO22
                      GPIO_NUM_23); //scl, use IO23/GPIO23
  my_I2C_master_write(MPU_I2C_ADDR, MPU_PWR_MGMT_1_REG, 1, &MPU_PWR_MGMT_1_CMD);
  for(;;)
  {
    esp_task_wdt_reset();
    /* As written in .ino:
     * Wire.beginTransmission(MPU);
     * Wire.write(0x3B);
     * Wire.endTransmission(false);
     * Wire.requestFrom(MPU,12,true);
     * AcX=Wire.read()<<8|Wire.read();
     * AcY=Wire.read()<<8|Wire.read();
     * AcZ=Wire.read()<<8|Wire.read();
     * GyX=Wire.read()<<8|Wire.read();
     * GyY=Wire.read()<<8|Wire.read();
     * GyZ=Wire.read()<<8|Wire.read();
     */
    my_I2C_master_read(MPU_I2C_ADDR, MPU_ACCEL_XOUT_H_REG, 12, imu_buf);
    degree_of_rotation = normalize_degree_for_servo(
      accelVal_to_degrees(
        (int8_t) imu_buf[0], 
        (int8_t) imu_buf[2]));
    //<debugging>
    // for(int i = 0; i < 6; i++)
    //   Serial.printf("buf%i: %i%i\n", i, imu_buf[i], imu_buf[i+1]);

    // Serial.printf("ACCEL_X: %hhd %2x\n", imu_buf[0],  imu_buf[1]);
    // Serial.printf("ACCEL_Y: %hhd %2x\n", imu_buf[2],  imu_buf[3]);
    // Serial.printf("ACCEL_Z: %hhd %2x\n", imu_buf[4],  imu_buf[5]);
    // Serial.printf(" GYRO_X: %hhd %2x\n", imu_buf[6],  imu_buf[7]);
    // Serial.printf(" GYRO_Y: %hhd %2x\n", imu_buf[8],  imu_buf[9]);
    // Serial.printf(" GYRO_Z: %hhd %2x\n", imu_buf[10], imu_buf[11]);
    
    // Serial.printf("x: %f\n", aY_to_x((int8_t) imu_buf[2]));
    // Serial.printf("y: %f\n", aX_to_y((int8_t) imu_buf[0]));

    // Serial.printf("%hd degrees\n", 
    //   accelVal_to_degrees(
    //     (int8_t) imu_buf[0], 
    //     (int8_t) imu_buf[2]
    // ));

    // Serial.printf("%d degrees\n", degree_of_rotation);
    // Serial.printf("=============\n");
    //</debugging>
    
    //TODO send via XHR
    vTaskDelay(1);
  }
  vTaskDelete(NULL);
}
    
// use GPIO15 for servo control
void vServoTask(void* pvParameter)
{
  // gpio init
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 15);

  // mcpwm config
  mcpwm_config_t pwm_config;
                             //frequency = 50Hz, i.e. for every servo motor 
  pwm_config.frequency = 50; //time period should be 20ms
  pwm_config.cmpr_a = 0;     //duty cycle of PWMxA = 0
  pwm_config.cmpr_b = 0;     //duty cycle of PWMxb = 0
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

  //Configure PWM0A & PWM0B with above settings
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
  for(;;)
  {
    // for (count = 0; count < SERVO_MAX_DEGREE; count++) {
    //     printf("Angle of rotation: %d\n", count);
    //     angle = servo_per_degree_init(count);
    //     printf("pulse width: %dus\n", angle);
    //     mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
    //     vTaskDelay(10);     //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
    // }

    mcpwm_set_duty_in_us(
      MCPWM_UNIT_0, 
      MCPWM_TIMER_0, 
      MCPWM_OPR_A, 
      servo_per_degree_init(180-degree_of_rotation)
    );
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

// use GPIO15 for servo control
void vServoXHRTask(void* pvParameter)
{
  // gpio init
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 15);

  // mcpwm config
  mcpwm_config_t pwm_config;
                             //frequency = 50Hz, i.e. for every servo motor 
  pwm_config.frequency = 50; //time period should be 20ms
  pwm_config.cmpr_a = 0;     //duty cycle of PWMxA = 0
  pwm_config.cmpr_b = 0;     //duty cycle of PWMxb = 0
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

  //Configure PWM0A & PWM0B with above settings
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

  server.on("/servo", HTTP_POST, [](AsyncWebServerRequest *request){
    int args = request->args();
    if(args != 1) request->send(200, "text/plain", "BAD REQUEST: argument count insufficient");
    else 
    {
      uint32_t temp_val = atoi(request->arg((size_t)0).c_str());
      if(temp_val < 0 || temp_val > 180) request->send(200, "text/plain", "BAD REQUEST: invalid value received");
      else
      {
        uint32_t deg_val = temp_val;
        Serial.printf("received %i\n", deg_val);
        request->send(200, "text/plain", "ACK");
        mcpwm_set_duty_in_us(
          MCPWM_UNIT_0, 
          MCPWM_TIMER_0, 
          MCPWM_OPR_A, 
          servo_per_degree_init(180-deg_val)
        );
      }
    }
    vTaskDelay(10);
  });
  for(;;)
  {
    // for (count = 0; count < SERVO_MAX_DEGREE; count++) {
    //     printf("Angle of rotation: %d\n", count);
    //     angle = servo_per_degree_init(count);
    //     printf("pulse width: %dus\n", angle);
    //     mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
    //     vTaskDelay(10);     //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
    // }

    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}