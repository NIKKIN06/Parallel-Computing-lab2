//  1. Ознайомитися з визначенням: атомарна змінна, атомарна операція,
//     неблокуючий алгоритм. Ознайомитися з деталями атомарності в обраній
//     мові програмування.
//  2. Надати в протоколі роботи опис того, як саме досягається справжня
//	   атомарність операцій в обраній студентом мові програмування.
//  3. Виконати завдання за варіантом без використанням паралелізації.
//	   Заміряти час виконання завдання.
// 
//			24. Знайти кількість елементів, кратних 19, і найменший такий елемент. 
// 
//  4. Виконати завдання за варіантом з використанням блокуючих примітивів
//	   синхронізації. Заміряти час виконання завдання.
//  5. Виконати завдання за варіантом з використанням атомарних змінних та
//	   CAS\CMPXCHNG операцій (більш високорівнені функції, що
//	   абстрагують дані операції, не приймаються для використання в даній
//	   роботі). Заміряти час виконання завдання.
//  6. Повторити пункти 2 – 4 з використанням різної розмірності даних та
//	   фіксованою кількістю потоків виконання.
//  7. В протокол занести отримані для пункту 5 результати у вигляді графіків
//	   залежності часу від кількості даних, надати порівняльний аналіз
//	   блокуючого та неблокуючого алгоритму з використання атомарних
//	   операцій.
//  8. Надати висновок, що повинен містити аналіз отриманих результатів.

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include <mutex>
#include <thread>

using namespace std;
using namespace std::chrono;

void generate_random_numbers(vector<int> &arr, int amount)
{
	random_device rd;
	mt19937 engine(rd());
	uniform_int_distribution<> dist(1, numeric_limits<int>::max());

	arr.reserve(amount);
	
	for (int i = 0; i < amount; i++)
	{
		arr.push_back(dist(engine));
	}
}

void single_thread_task_function(vector<int> &arr, int &amount, int &min_element)
{
	for (int number : arr)
	{
		if (number % 19 == 0)
		{
			amount++;

			if (number < min_element)
			{
				min_element = number;
			}
		}
	}
}

void mutex_task_function(vector<int> &arr, int start_index, int end_index, int &amount, int &min_element, mutex &mtx)
{
	for (int i = start_index; i < end_index; i++)
	{
		if (arr[i] % 19 == 0)
		{
			mtx.lock();

			amount++;

			if (arr[i] < min_element)
			{
				min_element = arr[i];
			}

			mtx.unlock();
		}
	}
}

int main()
{
	int numbers_amount = 0;

	cin >> numbers_amount;

	vector<int> arr = {};
	generate_random_numbers(arr, numbers_amount);

	int amount = 0, min_element = numeric_limits<int>::max();

	// ----- task 3 -----

	auto start_time = high_resolution_clock::now();
	single_thread_task_function(arr, amount, min_element);
	auto end_time = high_resolution_clock::now();

	duration<double, micro> time = end_time - start_time;

	double single_thread_execution_time = time.count();

	cout << amount << "\n"
		<< ((min_element == numeric_limits<int>::max()) ? "-" : to_string(min_element)) << "\n"
		<< single_thread_execution_time << " microseconds\n";

	// ----- task 4 -----

	mutex mtx;

	vector<thread> threads;
	int thread_amount = 8;
	amount = 0;
	min_element = numeric_limits<int>::max();

	start_time = high_resolution_clock::now();

	for (int i = 0; i < thread_amount; i++)
	{
		int start_index = i * (numbers_amount / thread_amount);
		int end_index = (i == thread_amount - 1) ? numbers_amount : (i + 1) * (numbers_amount / thread_amount);
		threads.emplace_back(mutex_task_function, ref(arr), start_index, end_index, ref(amount), ref(min_element), ref(mtx));
	}

	for (auto& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}

	end_time = high_resolution_clock::now();

	time = end_time - start_time;

	double mutex_execution_time = time.count();

	cout << "\n\n" << amount << "\n"
		<< ((min_element == numeric_limits<int>::max()) ? "-" : to_string(min_element)) << "\n"
		<< mutex_execution_time << " microseconds\n";
}