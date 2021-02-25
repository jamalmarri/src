#!/bin/sh

echo -e "\033[38;5;6m[INFO]\033[0m Beginning test script"

FAILED_TESTS=0
PASSED_TESTS=0

# query dmesg. Do we see any output from the attach/match functions?
if sudo dmesg | grep -q "RTC8523 TEST"; then
  echo -e "\033[38;5;2m[SUCCESS]\033[0m dmesg output detected"
else
  echo -e "\033[31;1;4m[FAILURE]\033[0m no dmesg output detected"
fi

# Test to see if RTC device is mounted
# i2cscan iic0 should return 0x68

echo -e "\033[38;5;6m[INFO]\033[0m Ending test script"
