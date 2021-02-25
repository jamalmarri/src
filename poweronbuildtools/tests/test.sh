#!/bin/sh

echo -e "\033[38;5;6m[INFO]\033[0m Beginning test script"

TOTAL_TESTS=3
FAILED_TESTS=0
PASSED_TESTS=0

# query dmesg. Do we see any output from the attach/match functions?
if dmesg | grep -q "RTC8523 TEST"; then
  echo -e "\033[38;5;2m[SUCCESS]\033[0m dmesg output detected"
  ((PASSED_TESTS=PASSED_TESTS+1))
else
  echo -e "\033[31;1;4m[FAILURE]\033[0m no dmesg output detected"
  ((FAILED_TESTS=FAILED_TESTS+1))
fi

# Test to see if RTC device is mounted
# i2cscan iic0 should return 0x68

echo -e "\033[38;5;6m[INFO]\033[0m Ending test script"

# Print the results
echo -e "\033[38;5;2m[INFO] Tests Passed:\033[0m ${PASSED_TESTS}"
echo -e "\033[38;5;1m[INFO] Tests Failed:\033[0m ${FAILED_TESTS}"
echo -e "\033[38;5;6m[INFO] Results:\033[0m ${PASSED_TESTS}/${TOTAL_TESTS}"
