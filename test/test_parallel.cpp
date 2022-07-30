#include <gtest/gtest.h>
#include <CGUtils/parallel.hpp>
#include <CGUtils/console.hpp>
#include <iostream>
using namespace wzz::thread;

TEST(test_parallel,test_thread_group){
	thread_group_t group;
	group.start(1, {});

	auto task1 = group.create_task([]() {
		LOG_INFO("Ohai!\n");
	});
	auto task2 = group.create_task([]() {
		LOG_INFO("Ohai 2!\n");
	});
	auto task3 = group.create_task([]() {
		LOG_INFO("Ohai 3!\n");
	});
	group.enqueue_task(*task3, []() {
		LOG_INFO("Brrr :3\n");
	});
	task1->id = 1;
	task2->id = 2;
	task3->id = 3;
	group.add_dependency(*task1, *task3);
	group.add_dependency(*task2, *task3);
	group.add_dependency(*task1, *task2);
	group.submit(task1);
	group.submit(task2);
	group.submit(task3);

	group.wait_idle();
}

TEST(test_parallel,test_paraller_forrange_pooled){
	thread_group_t thread_group;
	thread_group.start(10);
	parallel_forrange(0,10,[](int thread_index,int x){
		std::cout<<"x: "<<x;
		  for(int i = 0; i < 10; i++){
			std::cout<<" "<<i;
		}
		std::cout<<std::endl;
	},thread_group);
}
