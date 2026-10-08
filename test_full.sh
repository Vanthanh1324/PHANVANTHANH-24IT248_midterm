#!/bin/sh

PROG="./ls"
TEST_DIR="test_ls"
PASS=0
FAIL=0

pass()
{
    echo "[PASS] $1"
    PASS=$((PASS + 1))
}

fail()
{
    echo "[FAIL] $1"
    FAIL=$((FAIL + 1))
}

run_test()
{
    NAME="$1"
    shift

    if $PROG "$@" >/dev/null 2>&1
    then
        pass "$NAME"
    else
        fail "$NAME"
    fi
}

check_contains()
{
    NAME="$1"
    shift

    if "$@" >/dev/null 2>&1
    then
        pass "$NAME"
    else
        fail "$NAME"
    fi
}

echo "======================================"
echo "        ls - Full Test"
echo "======================================"

echo ""
echo "1. Build"
echo "--------------------------------------"

if make clean >/dev/null 2>&1 && make >/dev/null 2>&1
then
    pass "Build"
else
    fail "Build"
    exit 1
fi

echo ""
echo "2. Basic options"
echo "--------------------------------------"

run_test "No option"
run_test "Directory operand" src
run_test "-a" -a
run_test "-A" -A
run_test "-c" -c
run_test "-d" -d src
run_test "-F" -F
run_test "-f" -f
run_test "-h" -h
run_test "-i" -i
run_test "-k" -k
run_test "-l" -l
run_test "-n" -n
run_test "-q" -q
run_test "-R" -R
run_test "-r" -r
run_test "-S" -S
run_test "-s" -s
run_test "-t" -t
run_test "-u" -u
run_test "-w" -w

echo ""
echo "3. Combined options"
echo "--------------------------------------"

run_test "-la" -l -a
run_test "-lh" -l -h
run_test "-li" -l -i
run_test "-ln" -l -n
run_test "-ls" -l -s
run_test "-lt" -l -t
run_test "-lS" -l -S
run_test "-ltr" -l -t -r
run_test "-lR" -l -R
run_test "-laR" -l -a -R
run_test "-lah" -l -a -h
run_test "-lis" -l -i -s
run_test "-aRF" -a -R -F

echo ""
echo "4. Override options"
echo "--------------------------------------"

run_test "-qw" -q -w
run_test "-wq" -w -q
run_test "-cu" -c -u
run_test "-uc" -u -c
run_test "-Rd" -R -d
run_test "-dR" -d -R
run_test "-kh" -k -h
run_test "-hk" -h -k
run_test "-St" -S -t
run_test "-tS" -t -S

echo ""
echo "5. Sorting and operands"
echo "--------------------------------------"

run_test "Normal sorting" src
run_test "Reverse sorting" -r src
run_test "Time sorting" -t src
run_test "Reverse time sorting" -t -r src
run_test "Size sorting" -S src
run_test "Reverse size sorting" -S -r src
run_test "Unsorted" -f src
run_test "Multiple directories" src include
run_test "Reverse multiple directories" -r src include
run_test "Directory mode multiple operands" -d src include

echo ""
echo "6. Recursive"
echo "--------------------------------------"

run_test "Recursive" -R src
run_test "Recursive reverse" -R -r src
run_test "Recursive long" -R -l src

echo ""
echo "7. Special files"
echo "--------------------------------------"

rm -rf "$TEST_DIR"
mkdir "$TEST_DIR"
touch "$TEST_DIR/normal"
touch "$TEST_DIR/executable"
chmod +x "$TEST_DIR/executable"
touch "$TEST_DIR/file with space"
ln -s normal "$TEST_DIR/link"
mkfifo "$TEST_DIR/fifo"

if $PROG "$TEST_DIR" >/dev/null 2>&1
then pass "Special files directory"
else fail "Special files directory"
fi

if $PROG -F "$TEST_DIR" >/dev/null 2>&1
then pass "-F special files"
else fail "-F special files"
fi

if $PROG -l "$TEST_DIR" >/dev/null 2>&1
then pass "-l special files"
else fail "-l special files"
fi

if $PROG -i "$TEST_DIR" >/dev/null 2>&1
then pass "-i special files"
else fail "-i special files"
fi

echo ""
echo "8. Hidden files"
echo "--------------------------------------"

touch "$TEST_DIR/.hidden"

if $PROG -a "$TEST_DIR" | grep -q "\.hidden"
then pass "-a includes hidden file"
else fail "-a includes hidden file"
fi

if $PROG -A "$TEST_DIR" | grep -q "\.hidden"
then pass "-A includes hidden file"
else fail "-A includes hidden file"
fi

if $PROG -A "$TEST_DIR" | grep -q "^\.\$"
then fail "-A excludes dot"
else pass "-A excludes dot"
fi

echo ""
echo "9. Error handling"
echo "--------------------------------------"

if $PROG /this/path/does/not/exist >/dev/null 2>&1
then fail "Non-existent path returns error"
else pass "Non-existent path returns error"
fi

if $PROG -Z >/dev/null 2>&1
then fail "Invalid option returns error"
else pass "Invalid option returns error"
fi

echo ""
echo "10. Output checks"
echo "--------------------------------------"

if $PROG src | grep -q "main.c"
then pass "Normal output contains main.c"
else fail "Normal output contains main.c"
fi

if $PROG -l src | grep -q "main.c"
then pass "Long output contains main.c"
else fail "Long output contains main.c"
fi

if $PROG -i src | grep -q "main.c"
then pass "Inode output contains main.c"
else fail "Inode output contains main.c"
fi

echo ""
echo "11. Cleanup"
echo "--------------------------------------"

rm -rf "$TEST_DIR"

echo ""
echo "======================================"
echo "Test result"
echo "======================================"
echo "PASS: $PASS"
echo "FAIL: $FAIL"
echo "======================================"

if [ "$FAIL" -eq 0 ]
then
    echo "ALL TESTS PASSED"
    exit 0
else
    echo "SOME TESTS FAILED"
    exit 1
fi
