#ifndef ARCH_I386_TIME_H
#define ARCH_I386_TIME_H

#include<kernel/io.h>
#include<stdint.h>
#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

struct kernel_time {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t century;
};

void init_rtc(void);
uint8_t read_cmos(uint8_t);
void write_cmos(uint8_t, uint8_t);
struct kernel_time read_rtc_hardware_time(void);
struct kernel_time rtc_time(void);
void convert_bcd_to_binary(struct kernel_time*);
void update_system_time(void);
struct kernel_time convert_timestamp_to_datetime(uint32_t);
uint32_t convert_datetime_to_timestamp(struct kernel_time);
void get_system_time(struct kernel_time *);
int set_systme_time(struct kernel_time *);

#endif