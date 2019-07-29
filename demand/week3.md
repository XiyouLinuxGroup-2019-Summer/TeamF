#include< iostream >
<br>#include< unistd.h >
<br>#include< sys/types.h >
<br>using namespace std;
<br>int main()
<br>{
    <br>//子进程创建了几个，为什么执行顺序会是这个样子
    <br>for(int i = 1; i < 4; i ++)
   <br> {
       <br> pid_t t = fork();
        <br>if(t == 0)
       <br> {
          <br>  cout << "child  "  << getpid()<< endl;
        }
      <br>  else
        {
          <br>  cout << "parent  "  << getpid()<< endl;
      <br>  }   
   <br> }
  <br>  return 0;
<br>}

//解释一下 子进程创建了几个，为什么执行顺序会是这个样子

解释一下exec族的作用
