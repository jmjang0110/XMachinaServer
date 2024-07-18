//#include "pch.h"
//#include "ThreadManager.h"
//#include "MemoryTest/BlockMemoryPool.h"
//#include "MemoryTest/MemoryManager.h"
//
//constexpr int TestNum2 = 1000'00;
//constexpr int TestNum = 1000'0000;
//
//BlockMemoryPool_Vector<int> intPool(TestNum);
//BlockMemoryPool_Array<int> intArrPool(TestNum);
//MemoryPool_MyArr<int> intMyArrPool;
//
//int main(void) {
//	std::cout << "PERFORMANCE TEST PROJECT \n";
//	constexpr int Thread_Num = 4;
//
//
//	for (int i = 0; i < Thread_Num; ++i) {
//		THREAD_MGR->RunThread("MemoryPool_MyArr TEST", [&]() {
//
//			auto start3 = std::chrono::high_resolution_clock::now();
//
//
//			// 두 번째 루프의 시간 측정
//			for (int i = 0; i < TestNum2; ++i) {
//
//				for (int i = 0; i < 100; ++i) {
//					int* a = intMyArrPool.Allocate();
//					//std::cout << "[" << TLS_MGR->Get_TlsInfoData()->id << "] : " << a << "\n";
//					intMyArrPool.Deallocate(a);
//
//				}
//
//			}
//
//			auto end3 = std::chrono::high_resolution_clock::now();
//			std::chrono::duration<double> duration3 = end3 - start3;
//			std::cout << "MemoryPool_MyArr loop duration: " << duration3.count() << " seconds" << std::endl;
//
//			});
//	}
//
//	THREAD_MGR->JoinAllThreads();
//
//	for (int i = 0; i < Thread_Num; ++i) {
//		THREAD_MGR->RunThread("BlockMemoryPool_Array TEST", [&]() {
//
//			auto start3 = std::chrono::high_resolution_clock::now();
//
//
//			// 두 번째 루프의 시간 측정
//			for (int i = 0; i < TestNum2; ++i) {
//				for (int i = 0; i < 100; ++i) {
//					int* a = intArrPool.allocate();
//					intArrPool.deallocate(a);
//				}
//
//			}
//
//			auto end3 = std::chrono::high_resolution_clock::now();
//			std::chrono::duration<double> duration3 = end3 - start3;
//			std::cout << "BlockMemoryPool_Array loop duration: " << duration3.count() << " seconds" << std::endl;
//
//			});
//	}
//
//
//	THREAD_MGR->JoinAllThreads();
//
//
//	//return 0;
//
//	for (int i = 0; i < Thread_Num; ++i) {
//		THREAD_MGR->RunThread("BlockMemoryPool_Vector TEST", [&]() {
//
//			auto start3 = std::chrono::high_resolution_clock::now();
//
//
//			// 두 번째 루프의 시간 측정
//			for (int i = 0; i < TestNum; ++i) {
//
//
//				int* a = intPool.allocate();
//				intPool.deallocate(a);
//			
//			}
//
//			auto end3 = std::chrono::high_resolution_clock::now();
//			std::chrono::duration<double> duration3 = end3 - start3;
//			std::cout << "BlockMemoryPool_Vector loop duration: " << duration3.count() << " seconds" << std::endl;
//
//			});
//	}
//
//	THREAD_MGR->JoinAllThreads();
//
//
//
//
//	MEMORY->InitMemories();
//
//	for (int i = 0; i < Thread_Num; ++i) {
//		THREAD_MGR->RunThread("MEMORY MANAGER TEST", [&]() {
//
//			auto start3 = std::chrono::high_resolution_clock::now();
//
//			// 두 번째 루프의 시간 측정
//			for (int i = 0; i < TestNum; ++i) {
//
//				int* a = MEMORY->New<int>();
//				MEMORY->Delete<int>(a);
//			}
//
//			auto end3 = std::chrono::high_resolution_clock::now();
//			std::chrono::duration<double> duration3 = end3 - start3;
//			std::cout << "MEMORY loop duration: " << duration3.count() << " seconds" << std::endl;
//
//			});
//	}
//
//	THREAD_MGR->JoinAllThreads();
//
//
//	for (int i = 0; i < Thread_Num; ++i) {
//		THREAD_MGR->RunThread("NEW/DELETE TEST", [&]() {
//
//			auto start3 = std::chrono::high_resolution_clock::now(); 
//
//
//			// 두 번째 루프의 시간 측정
//			for (int i = 0; i < TestNum; ++i) {
//
//				int* a = new int;
//				delete a;
//			}
//
//			auto end3 = std::chrono::high_resolution_clock::now();
//			std::chrono::duration<double> duration3 = end3 - start3;
//			std::cout << "NEW/DELETE loop duration: " << duration3.count() << " seconds" << std::endl;
//
//			});
//	}
//
//	THREAD_MGR->JoinAllThreads();
//
//
//
//
//	return 0;
//}



#include "pch.h"
#include "SpinLock.h"
#include "ThreadManager.h"


std::atomic<int> a;

int aa;
int aaa;
SRWLock srwLock;
std::mutex m;

int b;



struct Atomic_Vec3
{
	std::atomic<float> x;
	std::atomic<float> y;
	std::atomic<float> z;

};

struct Vec3
{
	float x;
	float y;
	float z;

};


void singleTest()
{
	for (int i = 0; i < 4'000'000'00; ++i) {
		b++;
	}
}

void Sum()
{
	for (int i = 0; i < 1'000'000'00; ++i) {
		a.fetch_add(1);
	}

}

void Sum2()
{
	for (int i = 0; i < 1'000'000'00; ++i) {
		srwLock.LockWrite();
		aa++;
		srwLock.UnlockWrite();
	}

}
void Sum3()
{
	for (int i = 0; i < 1'000'000'00; ++i) {
		m.lock();
		aaa++;
		m.unlock();
	}

}

SRWLock lock;
Vec3 v;
Atomic_Vec3 vvv;

void Set(Vec3 vv) {
	lock.LockWrite();
	v = vv;
	lock.UnlockWrite();
}

void Set2(Vec3 vv) {
	vvv.x.store(vv.x);
	vvv.y.store(vv.y);
	vvv.z.store(vv.z);
}


int main(void) {

	auto start3 = std::chrono::high_resolution_clock::now();

	Vec3 v = Vec3(1,1,1);
	for (int i = 0; i < 4; ++i) {

		THREAD_MGR->RunThread("Sum", [&]() {
			for (int i = 0; i < 100'000'000; ++i)
				Set(v);
			});
	}


	THREAD_MGR->JoinAllThreads();
	auto end3 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration3 = end3 - start3;
	std::cout  << " - Sum " << duration3.count() << " seconds" << std::endl;

	for (int i = 0; i < 4; ++i) {

		THREAD_MGR->RunThread("Sum2", [&]() {
			for (int i = 0; i < 100'000'000; ++i)
				Set2(v);
			});
	}


	THREAD_MGR->JoinAllThreads();
	 end3 = std::chrono::high_resolution_clock::now();
     duration3 = end3 - start3;
	std::cout << " - Sum2 " << duration3.count() << " seconds" << std::endl;


	//auto start3 = std::chrono::high_resolution_clock::now();

	//for (int i = 0; i < 4; ++i) {

	//	THREAD_MGR->RunThread("atomic int Sum", Sum);


	//}


	//THREAD_MGR->JoinAllThreads();
	//auto end3 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> duration3 = end3 - start3;
	//std::cout << a.load() << " - atomic int Sum loop duration: " << duration3.count() << " seconds" << std::endl;

	//auto start2 = std::chrono::high_resolution_clock::now();

	//for (int i = 0; i < 4; ++i)
	//{

	//	THREAD_MGR->RunThread("SRWLock Sum", Sum2);


	//}

	//THREAD_MGR->JoinAllThreads();
	//auto end2 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> duration2 = end2 - start2;
	//std::cout << aa << " - SRWLock Sum loop duration: " << duration2.count() << " seconds" << std::endl;

	// start2 = std::chrono::high_resolution_clock::now();
	// singleTest();
	// end2 = std::chrono::high_resolution_clock::now();
	// duration2 = end2 - start2;
	// std::cout << b << " - Single Thread  loop duration: " << duration2.count() << " seconds" << std::endl;


}
