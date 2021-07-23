//
// Created by wyz on 2021/7/23.
//
#include <CGUtils/timer.hpp>
#include <windows.h>
int main(int argc,char** argv){
    AutoTimer auto_timer;
    Timer timer;
    timer.start();
    _sleep(1000);
    timer.stop();
    timer.print_duration();
    timer.print_duration("s");
    timer.print_duration("us");
    timer.print_duration("ns");
    timer.start();
    _sleep(1234);
    timer.stop();
    timer.print_duration("ms");
    return 0;
}
