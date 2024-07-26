#include<arch/i386/time.h>

uint8_t read_cmos(uint8_t reg)
{
    outb(CMOS_ADDRESS, reg);
    return inb(CMOS_DATA);
}

void write_cmos(uint8_t reg, uint8_t value)
{
    outb(CMOS_ADDRESS, reg);
    outb(CMOS_DATA, value);
}


uint8_t bcd_to_binary(uint8_t bcd) {
    return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

void convert_bcd_to_binary(struct kernel_time* rtc_datetime) {
    rtc_datetime->second = bcd_to_binary(rtc_datetime->second);
    rtc_datetime->minute = bcd_to_binary(rtc_datetime->minute);
    rtc_datetime->hour = bcd_to_binary(rtc_datetime->hour);
    rtc_datetime->day = bcd_to_binary(rtc_datetime->day);
    rtc_datetime->month = bcd_to_binary(rtc_datetime->month);
    rtc_datetime->year = bcd_to_binary(rtc_datetime->year);
    rtc_datetime->century = bcd_to_binary(rtc_datetime->century);
}

struct kernel_time read_rtc_hardware_time()
{
    struct kernel_time time;

    outb(CMOS_ADDRESS, 0x8A);
    outb(CMOS_DATA, 0x20);

    time.second = read_cmos(0x00);
    time.minute = read_cmos(0x02);
    time.hour = read_cmos(0x04);
    time.day = read_cmos(0x07);
    time.month = read_cmos(0x08);
    time.year = read_cmos(0x09);
    time.century = read_cmos(0x32);

    outb(CMOS_ADDRESS, 0x8A);
    outb(CMOS_DATA, 0x00);
    return time;
}

struct kernel_time rtc_time() {
    struct kernel_time time = read_rtc_hardware_time();
    convert_bcd_to_binary(&time);
    return time;
}