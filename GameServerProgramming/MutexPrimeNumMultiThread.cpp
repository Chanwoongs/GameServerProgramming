// 소수 찾기 프로그램 (멀티 스레딩 사용, CPU 코어 활용)
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>
#include <mutex>

using namespace std;

const int MaxCount = 150000;
const int ThreadCount = 4;

bool IsPrimeNumber(int number)
{
	if (number == 1)
		return false;
	if (number == 2 || number == 3)
		return true;
	for (int i = 2; i < number - 1; i++)
	{
		if ((number % i) == 0)
			return false;
	}
	return true;
}

void PrintPrimeNumbers(const vector<int>& primes)
{
	for (int prime : primes)
	{
		cout << prime << endl;
	}
}

int main()
{
	// 각 스레드는 이 값을 꺼내 사용한다.
	int num = 1;
	recursive_mutex num_mutex;

	vector<int> primes;
	recursive_mutex primes_mutex;

	auto t0 = chrono::system_clock::now();

	// 작동할 워커 스레드
	vector<shared_ptr<thread>> threads;

	for (int i = 0; i < ThreadCount; i++)
	{
		shared_ptr<thread> thread(new thread([&]()
			{
				// 각 스레드의 메인 함수
				// 값을 가져올 수 있으면 루프를 돈다.
				while (true)
				{
					// 숫자 증가
					int n;
					{
						lock_guard<recursive_mutex> num_lock(num_mutex);
						n = num;
						n++;
					}

					// 목표까지 실행 후 탈출
					if (n >= MaxCount)
						break;

					if (IsPrimeNumber(n))
					{
						lock_guard<recursive_mutex> primes_lock(primes_mutex);
						primes.push_back(n);
					}
				}
			})); // ERROR

		// 스레드 객체를 일단 갖고 있는다.
		threads.push_back(thread);
	}

	// 모든 스레드가 일을 마칠 때까지 기다린다.
	for (auto thread : threads)
	{
		thread->join();
	}
	// 끝

	auto t1 = chrono::system_clock::now();

	auto duration = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
	cout << "Took " << duration << " milliseconds." << endl;

	PrintPrimeNumbers(primes);

	return 0;
}
