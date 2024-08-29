## Peripherals

### RTC

RTC is to deliver the time information to the register. rtc_core consists of many counters, for counting second, minute, hour, day, and etc. The time information could be read from the register bus by the rtc_reg.

#### RTC_CORE

RTC Core is to deliver the time information, and send those info to rtc register by the static info and pulse info. 
The additional signal, fast_sim_time and fast_sim_data, is for debugging purpose, when it is configured, 1 clock means 1s. 

### Timer

Timer is to setup a pulse, based on 1ms, 1us.

### ws281x

ws281x is to drive ws281x type of LED. 

### pwm

PWM is to drive some motors. 

### NEC_IR

NEC_IR is for near-field infrared connections. 

### I2CM

I2CM is the master side of I2C io, consists of bit control and byte control. Bit control is to recognize the command pattern based on SCL(serial clock) and SDA(seriral data) transmission. 








