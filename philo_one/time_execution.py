import subprocess
from statistics import mean
import threading

threadLock = threading.Lock()
array = []


class myThread (threading.Thread):
    def add_result(self):
        data = subprocess.getoutput("./philo_one 3 310 200 100")
        last_line = data[data.rfind('\n') + 1:]
        death_time = int(last_line[:last_line.find(' ')])
        result = death_time - 310
        # print(f"Run. Result: {result}")
        array.append(result)

    def run(self):
        for i in range(0, 20):
            self.add_result()


if __name__ == '__main__':
    ts = []
    for i in range(100):
        thread = myThread()
        thread.start()
        ts.append(thread)
    for i in range(100):
        ts[i].join()
    print(f"Collected {len(array)} results")
    print(f"Max is {max(array)}")
    print(f"The mean is {mean(array)}")
