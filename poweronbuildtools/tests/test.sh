#!/bin/sh

echo -e "\033[38;5;6m[INFO]\033[0m Beginning test script"

TOTAL_TESTS=3
FAILED_TESTS=0
PASSED_TESTS=0

# Test1
# query dmesg. Do we see any output from the attach/match functions?
if dmesg | grep -q "RTC8523 TEST"; then
  echo -e "\033[38;5;2m[SUCCESS]\033[0m dmesg output detected"
  ((PASSED_TESTS=PASSED_TESTS+1))
else
  echo -e "\033[31;1;4m[FAILURE]\033[0m No dmesg output detected"
  ((FAILED_TESTS=FAILED_TESTS+1))
fi

# Test2
# Test to see if RTC device is mounted
# i2cscan iic0 should return 0x68
FOUND_DEVICE=0
for FILE in /dev/iic* ; do
  # If it isn't iic0
  if [ $FILE != "/dev/iic0" ]; then
    # run i2cscan on the found device
    echo -e "\033[38;5;2m[INFO] Found iic device:\033[0m ${FILE}"
    if i2cscan $FILE; then
      ((FOUND_DEVICE=1))
    fi
  fi
done
if [ $FOUND_DEVICE -eq 1 ]; then
  echo -e "\033[38;5;2m[SUCCESS]\033[0m Device detected"
  ((PASSED_TESTS=PASSED_TESTS+1))
else
  echo -e "\033[31;1;4m[FAILURE]\033[0m No device detected"
  ((FAILED_TESTS=FAILED_TESTS+1))
fi

# Test3
# Fetch time/date from RTC


# Test4
# Write data to RTC


# Test5
# Read same data back from RTC



# Test6
# Print the results
echo -e "\033[38;5;6m[INFO]\033[0m Ending test script"
echo '------------------'
echo -e "\033[38;5;2m[INFO] Tests Passed:\033[0m ${PASSED_TESTS}"
echo -e "\033[38;5;1m[INFO] Tests Failed:\033[0m ${FAILED_TESTS}"
echo -e "\033[38;5;6m[INFO] Results:\033[0m ${PASSED_TESTS}/${TOTAL_TESTS}"
echo '------------------'
