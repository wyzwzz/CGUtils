#include <map>
#include <CGUtils/console.hpp>
#include <gtest/gtest.h>
TEST(test_console,test_progress_bar){
	wzz::console::progress_bar_t pb(80);
	for(int i = 0; i < 100; i++){
		pb.set_percent(i);
		pb.display();
		_sleep(10);
	}
	pb.done();
}