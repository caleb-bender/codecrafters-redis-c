#!/bin/bash
# shell script to run all e2e tests

# variables to track passed, failed, total tests
passed=0
failed=0
total=0

# run each test file (*.exp) in the test/e2e directory
# for each test, increment total, and if it passes, increment passed, else increment failed
# the summary of results will be printed at the end
for test_file in tests/e2e/*.exp; do
    echo "Running test: $test_file"
    if expect "$test_file"; then
        echo "Test passed: $test_file"
        passed=$((passed + 1))
    else
        echo "Test failed: $test_file"
        failed=$((failed + 1))
    fi
    total=$((total + 1))
done

# print summary of results
echo "E2E Test Summary:"
echo "Total tests: $total"
echo "Passed: $passed"
echo "Failed: $failed"