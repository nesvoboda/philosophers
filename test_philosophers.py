import os
import subprocess
from time import sleep
from statistics import mean
import threading

# How many 'long' tests are needed
N_LONG_TESTS = 1
# How many seconds must a program run uninterrupted
LONG_TEST_LENGTH = 10

# array = []

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def assert_runs_for_at_least(command, seconds, binary, test_name):
    # Run a given command
    f = open(f"{binary}_{test_name}_out.txt", "w")
    process = subprocess.Popen(command, stdout=f,
                               shell=True)
    print(f"")
    # Wait for some time
    code = process.poll()
    slept = 0
    while (slept < seconds):
        sleep(1)
        slept += 1
        code = process.poll()
        # Exit immediately, if the process has died
        if code is not None:
            f.close()
            return False

    code = process.poll()
    if code is None:
        # If the process is still running, the test has passed
        process.kill()
        f.close()
        print(f"{bcolors.OKGREEN}[{seconds} SEC] {bcolors.ENDC}", end="")
        return True
    # If the process isn't running anymore, the test has failed
    f.close()
    return False


def measure_starvation_timing(binary, array):
    # Run a philosopher binary with deadly parameters
    data = subprocess.getoutput(f"{binary} 3 310 200 100")
    # Get the time of death
    last_line = data[data.rfind('\n') + 1:]
    death_time = int(last_line[:last_line.find(' ')])
    result = death_time - 310
    # Append the delay to the array of results
    array.append(result)


def run_long_test(binary, test, test_name):
    for i in range(0, N_LONG_TESTS):
        res = assert_runs_for_at_least(
            f"{binary} {test}",
            LONG_TEST_LENGTH,
            binary,
            f"{test_name}_{i}")
        if res is False:
            print(f"\n\n{binary} failed test {test}")
            return False
    print(f"\n\n✅ Pass!\n")
    return True


def run_starvation_measures(binary):
    results = []
    for i in range(10):
        measure_starvation_timing(binary, results)
        if results[-1] > 10:
            print(f"\n\n{binary} filed starvation test :(")
            return False
        else:
            print(f"{bcolors.OKGREEN}[{results[-1]} MS] {bcolors.ENDC}", end="")
    print(f"\n\n✅ average delay: {mean(results)} ms!\n\n")
    return True


def test_program(binary):
    print("Section 1: Long tests.\n\n")
    print("--- 4 410 200 200 ---")
    if run_long_test(binary, "4 410 200 200", 'long_test_1') is False:
        return False
    print("--- 5 800 200 200 ---")
    if run_long_test(binary, "5 800 200 200", 'long_test_2') is False:
        return False

    print("Section 2: Starvation test\n\n")
    if run_starvation_measures(binary) is False:
        return False
    return True

def make_all_binaries():
    subprocess.run(["make", "-C", "./philo_one/"])
    subprocess.run(["make", "-C", "./philo_two/"])
    subprocess.run(["make", "-C", "./philo_three/"])


if __name__ == "__main__":
    print(f"\n{bcolors.OKBLUE}-- MAKING BINARIES ---{bcolors.ENDC} \n")
    make_all_binaries()
    print("There are two sections in the tests.")
    print("\nSection 1: Long tests.\n\n"
          "In these tests, philosophers must not die.\n"
          f"We will run each of the tests {N_LONG_TESTS} times.\n"
          "Test will pass, if your program runs for more than\n")
    print("\nSection 2: Starvation test\n\n"
          "In this test, one philosopher must die.\n"
          "We will check the time of their death, and make sure\n"
          "their death is showed within 10ms of their death\n"
          f"{LONG_TEST_LENGTH} seconds every time.\n\n")
    print(f"\n\n{bcolors.OKBLUE}-- PHILO_ONE ---{bcolors.ENDC} \n\n")
    test_program('./philo_one/philo_one')
    print(f"\n\n{bcolors.OKBLUE}-- PHILO_TWO ---{bcolors.ENDC} \n\n")
    test_program('./philo_two/philo_two')
    print(f"\n\n{bcolors.OKBLUE}-- PHILO_THREE ---{bcolors.ENDC} \n\n")
    test_program('./philo_three/philo_three')


