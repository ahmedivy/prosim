#!/bin/sh
export LC_ALL=C

echo ======================================================
echo ====================== TEST $1 =======================
echo ======================================================
if timeout 10 ./$2/$3 < tests/test.$1.in > tests/test.$1.raw; then 
  cat tests/test.$1.raw | sort > tests/test.$1.out
  if diff -b tests/test.$1.out tests/test.$1.expected > /dev/null; then
    if grep "IS_CONCURRENT" tests/test.$1.cfg > /dev/null; then
      for x in {0..100}; do
        if diff tests/test.$1.raw tests/test.$1.out > /dev/null; then
          if [ $x -eq 100 ]; then
            echo FAILED: Output is correct, but no concurrency is apparent
            echo Threads appear to be executed in sequential order
            exit 1
          else 
            echo RETRYING: Output is correct, but no concurrency is apparent
            timeout 10 ./$2/$3 < tests/test.$1.in > tests/test.$1.raw
          fi
        else 
          break
        fi
      done
      #if diff tests/test.$1.raw tests/test.$1.out > /dev/null; then
      #fi
    fi
    echo PASSED
  else
    echo FAILED
    echo ======
    echo ______________Your_output______________ ____________Expected_Output_____________
   #echo +++++++++++++++++++++++++++++++++++++++ ++++++++++++++++++++++++++++++++++++++++
    diff -b -y -W 80 tests/test.$1.out tests/test.$1.expected
    echo =====================
    echo Your Output:
    echo ++++++++++++++++
    cat tests/test.$1.out
    echo =====================
    echo Expected Output:
    echo ++++++++++++++++
    cat tests/test.$1.expected
    exit 1
  fi
elif [ $? -eq 124 ]; then
  echo TIMEOUT
  exit 1
else 
  echo Abnormal program termination: the program crashed
  echo Exit code $?
  exit 1
fi
