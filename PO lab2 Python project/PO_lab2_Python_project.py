import subprocess
import matplotlib.pyplot as plt

CPP_PATH = "../PO lab2 C++ project/x64/Debug/PO lab2 C++ project.exe"

arr_sizes = [100000, 500000, 1000000, 5000000, 10000000]

single_thread_times = []
mutex_times = []
atomic_times = []

for size in arr_sizes:
    process = subprocess.Popen([CPP_PATH], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    stdout, stderr = process.communicate(input=f"{size}\n")

    times = stdout.strip().split()

    if len(times) == 3:
        single_thread_times.append(float(times[0]))
        mutex_times.append(float(times[1]))
        atomic_times.append(float(times[2]))
    else:
        print(f"Result reading error: {stdout}")

plt.figure(figsize=(12, 8))

plt.plot(arr_sizes, single_thread_times, label='Single thread', marker='o', color='green')
plt.plot(arr_sizes, mutex_times, label='Mutex', marker='o', color='red')
plt.plot(arr_sizes, atomic_times, label='Atomic', marker='o', color='blue')

plt.title('Порівняння блокуючого та неблокуючого алгоритмів', fontsize=14)
plt.xlabel('Кількість елементів у масиві', fontsize=12)
plt.ylabel('Час виконання (ms)', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=12)

plt.ticklabel_format(style='plain', axis='x')

plt.show()