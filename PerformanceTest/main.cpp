#include "pch.h"
#include "ThreadManager.h"
#include "MemoryTest/BlockMemoryPool.h"
#include "MemoryTest/MemoryManager.h"

constexpr int TestNum = 1000'00;// 00;

BlockMemoryPool_Vector<int> intPool(TestNum);
BlockMemoryPool_Array<int> intArrPool(TestNum);
MemoryPool_MyArr<int> intMyArrPool;

int main(void) {
	std::cout << "PERFORMANCE TEST PROJECT \n";
	constexpr int Thread_Num = 4;


	for (int i = 0; i < Thread_Num; ++i) {
		THREAD_MGR->RunThread("MemoryPool_MyArr TEST", [&]() {

			auto start3 = std::chrono::high_resolution_clock::now();


			// 두 번째 루프의 시간 측정
			for (int i = 0; i < TestNum; ++i) {

				for (int i = 0; i < 100; ++i) {
					int* a = intMyArrPool.Allocate();
					//std::cout << "[" << TLS_MGR->Get_TlsInfoData()->id << "] : " << a << "\n";
					intMyArrPool.Deallocate(a);

				}

			}

			auto end3 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration3 = end3 - start3;
			std::cout << "MemoryPool_MyArr loop duration: " << duration3.count() << " seconds" << std::endl;

			});
	}

	THREAD_MGR->JoinAllThreads();

	for (int i = 0; i < Thread_Num; ++i) {
		THREAD_MGR->RunThread("BlockMemoryPool_Array TEST", [&]() {

			auto start3 = std::chrono::high_resolution_clock::now();


			// 두 번째 루프의 시간 측정
			for (int i = 0; i < TestNum; ++i) {
				for (int i = 0; i < 100; ++i) {
					int* a = intArrPool.allocate();
					intArrPool.deallocate(a);
				}

			}

			auto end3 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration3 = end3 - start3;
			std::cout << "BlockMemoryPool_Array loop duration: " << duration3.count() << " seconds" << std::endl;

			});
	}


	THREAD_MGR->JoinAllThreads();


	return 0;

	for (int i = 0; i < Thread_Num; ++i) {
		THREAD_MGR->RunThread("BlockMemoryPool_Vector TEST", [&]() {

			auto start3 = std::chrono::high_resolution_clock::now();


			// 두 번째 루프의 시간 측정
			for (int i = 0; i < TestNum; ++i) {


				int* a = intPool.allocate();
				intPool.deallocate(a);
			
			}

			auto end3 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration3 = end3 - start3;
			std::cout << "BlockMemoryPool_Vector loop duration: " << duration3.count() << " seconds" << std::endl;

			});
	}

	THREAD_MGR->JoinAllThreads();




	MEMORY->InitMemories();

	for (int i = 0; i < Thread_Num; ++i) {
		THREAD_MGR->RunThread("MEMORY MANAGER TEST", [&]() {

			auto start3 = std::chrono::high_resolution_clock::now();

			// 두 번째 루프의 시간 측정
			for (int i = 0; i < TestNum; ++i) {

				int* a = MEMORY->New<int>();
				MEMORY->Delete<int>(a);
			}

			auto end3 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration3 = end3 - start3;
			std::cout << "MEMORY loop duration: " << duration3.count() << " seconds" << std::endl;

			});
	}

	THREAD_MGR->JoinAllThreads();


	for (int i = 0; i < Thread_Num; ++i) {
		THREAD_MGR->RunThread("NEW/DELETE TEST", [&]() {

			auto start3 = std::chrono::high_resolution_clock::now(); 


			// 두 번째 루프의 시간 측정
			for (int i = 0; i < TestNum; ++i) {

				int* a = new int;
				delete a;
			}

			auto end3 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration3 = end3 - start3;
			std::cout << "NEW/DELETE loop duration: " << duration3.count() << " seconds" << std::endl;

			});
	}

	THREAD_MGR->JoinAllThreads();




	return 0;
}