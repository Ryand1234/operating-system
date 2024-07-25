#ifndef ARCH_I386_TIME_H
#define ARCH_I386_TIME_H

##include "x86.h"

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

void init_rtc();
uint32_t read_rtc_time();
void update_system_time();
struct kernel_time convert_timestamp_to_datetime(uint32_t timestamp);
uint32_t convert_datetime_to_timestamp(struct kernel_time);
void get_system_time(struct kernel_time *);
int set_systme_time(struct kernel_time *);

#endif