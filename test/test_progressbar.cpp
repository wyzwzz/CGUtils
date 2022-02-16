//
// Created by wyz on 2021/9/8.
//
#include <CGUtils/progressbar.hpp>
#include <future>
#include <thread>
class A{
  int idx;
public:
  A(){
    static int iid = 0;
    idx = iid++;
  }
  ~A(){
    std::cout<<"destruct of A"<<std::endl;
  }
  std::string GetStr() const{
    std::cout<<__FUNCTION__ <<std::endl;
    return "index_"+std::to_string(idx);
  }
};
int main(){

  auto print=[](const A& a)->std::string{
    std::cout<<"call print"<<std::endl;
    return a.GetStr();
  };
  auto f=[&print]()->std::string{
    A a;
    std::cout<<"start into f"<<std::endl;
    return print(a);
  };
  auto task = std::packaged_task<std::string()>(f);
  task();
  auto ret = task.get_future();
  std::cout<<"before call future->get()"<<std::endl;
  std::cout<<ret.get()<<std::endl;
  std::cout<<"after call future->get()"<<std::endl;
  return 0;
}