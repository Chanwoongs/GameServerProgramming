// �Ҽ� ã�� ���α׷� (��Ƽ ������ ���, CPU �ھ� Ȱ��)
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>

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
	// �� ������� �� ���� ���� ����Ѵ�.
	int num = 1;

	vector<int> primes;

	auto t0 = chrono::system_clock::now();

	// �۵��� ��Ŀ ������
	vector<shared_ptr<thread>> threads;

	for (int i = 0; i < ThreadCount; i++)
	{
		shared_ptr<thread> thread(new thread([&]()
			{
				// �� �������� ���� �Լ�
				// ���� ������ �� ������ ������ ����.
				while (true)
				{
					// ���� ����
					int n;
					n = num;
					num++;

					// ��ǥ���� ���� �� Ż��
					if (n >= MaxCount)
						break;

					if (IsPrimeNumber(n))
					{
						primes.push_back(n);
					}
				}
			})); // ERROR

		// ������ ��ü�� �ϴ� ���� �ִ´�.
		threads.push_back(thread);
	}

	// ��� �����尡 ���� ��ĥ ������ ��ٸ���.
	for (auto thread : threads)
	{
		thread->join();
	}
	// ��

	auto t1 = chrono::system_clock::now();

	auto duration = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
	cout << "Took " << duration << " milliseconds." << endl; 

	PrintPrimeNumbers(primes);

	return 0;
}
