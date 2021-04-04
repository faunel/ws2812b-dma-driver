# ws2812b-dma-driver for CORE2

The driver supports popular, individually addressed RGB LEDs `ws2812b` and `ws2813`. It is written in C using ST HAL. It also include a few effects written in C++ for Mbed-OS.  

<p align="center">
<img src=".img/IMG_20190806_175106.jpg" width="600px"/>
</p>

## Supported targets
* [`CORE2`](https://github.com/husarion/core2-mbed-template) 

## Supported hardware
* `ws2812b`
* `ws2813`

## Adding the library
In `lib` directory of your project run:
```bash
$ mbed add https://github.com/husarion/core2-mbed-template.git
```

## Configuration
To configure the library define below rules in your `mbed_app.json` file.

See: https://github.com/husarion/core2-mbed-template

---

To select pin number add line:

```json
"ws2812b-dma-driver.led_pin": 1
```

available pin numbers:
* `1` - pin `SERVO1_PWM`
* `2` - pin `SERVO2_PWM`
* `3` - pin `SERVO3_PWM`
* `4` - pin `SERVO4_PWM`

To choose number of leds in led strip add:
```json
"ws2812b-dma-driver.num_leds": 16
```

## Example 1

```cpp
/* main.cpp */
#include <mbed.h>
#include <ws2812b-dma-driver.h>

static const char * LED_COLORS[] =
{
    "#c90000", "#fda600", "#32ae00", "#0038ff", "#ffffff"
};

static int parseColorStr(const char * color_str, Color_t * color_ptr)
{
    uint32_t num;
    if(sscanf(color_str,"%*c%X",&num)!=1) 
        return 0;
    color_ptr->b = 0xff & num;
    color_ptr->g = 0xff & (num>>8);
    color_ptr->r = 0xff & (num>>16);
    return 1;
}

// setup servo power DC/DC converter to 5V
DigitalOut servo_sel1(SERVO_SEL1,0);
DigitalOut servo_sel2(SERVO_SEL2,0);
DigitalOut servo_power(SERVO_POWER_ON,1);

int main()
{
    DeepSleepLock lock;
    Color_t c;

    // initialize ws2812b LEDs
    ws2812b_init();
    
    int num_of_colors = sizeof(LED_COLORS)/sizeof(LED_COLORS[0]);
    while(1)
    {
        for(int i=0;i <num_of_colors;i++)
        {
            parseColorStr(LED_COLORS[i],&c);
            
            // set all leds to the parsed color
            setAll_GRB(&c);
            
            // update leds
            drawFrame();

            wait_ms(1000);
        }
    }
}
```
<!-- result 1 -->

## Example 2

```cpp
/* main.cpp */
#include <mbed.h>
#include <ws2812b-effects.h>

// setup servo power DC/DC converter to 5V
DigitalOut servo_sel1(SERVO_SEL1,0);
DigitalOut servo_sel2(SERVO_SEL2,0);
DigitalOut servo_power(SERVO_POWER_ON,1);

int main()
{
    DeepSleepLock lock;

    // initialize ws2812b LEDs
    ws2812b_init();
    while(1)
    {
        // Rainbow Cycle effect 1 cycle, 50hz/20ms refresh rate
         RainbowCycle(20,1);
    }
}
```

<!-- result 2 -->

Нужно проверить
