#pragma once
#include <iostream>
#include <chrono>
namespace wzz::console{

class progress_bar_t{
public:
	explicit progress_bar_t(int width,char comp = '#',char incomp = ' ')
	 :percent(0.0),display_width(width),complete(comp),incomplete(incomp),
	  start_time(std::chrono::steady_clock::now())
	{}

	void set_percent(double per){
		percent = per;
	}
	void reset_time(){
		start_time = std::chrono::steady_clock::now();
	}
	auto get_time_ms() const noexcept{
		auto now = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
	}
	void display() const{
		auto p = static_cast<int>(display_width * percent * 0.01);
		auto cost_t = get_time_ms();
		std::cout<<"[";
		for(int i = 0; i < display_width; i++){
			if(i < p)
				std::cout<<complete;
			else if(i == p)
				std::cout<<">";
			else
				std::cout<<incomplete;
		}
		std::cout<<"] "<<static_cast<int>(percent)<<"%, "<<cost_t<<" ms.   \r";
		std::cout.flush();
	}

	void done(){
		set_percent(100.0);
		display();
		std::cout<<std::endl;
	}
private:
	double percent;
	int display_width;
	char complete;
	char incomplete;
	std::chrono::steady_clock::time_point start_time;
};



}