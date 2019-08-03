#include<iostream>
#include<queue>
#include<pthread.h>
#include<unistd.h>
using namespace std;

class cs
{
    public:
        static cs * pthis;
        static void * customer(void *arg);
        static void * producer(void *arg);
        cs();
        cs& operator=(const cs &tmp);
        cs(const cs &tmp);
        ~cs();
    private:
        queue<int>que;
        void tempa(){cout << "消费者已消费！\n";}
        void tempb(){cout << "生产者已生产！\n";}
        pthread_mutex_t mux;
        pthread_cond_t  con;
        pthread_t com_ptr;
        pthread_t pro_ptr;

};

cs::cs(const cs &tmp)
{

}

cs& cs::operator=(const cs &tmp)
{

}

cs::~cs()
{
    pthread_cond_broadcast(NULL);
    pthread_join(com_ptr,NULL);
    pthread_join(pro_ptr,NULL);
    std::cout << "两个线程已销毁\n";
}

cs::cs():com_ptr(0),pro_ptr(0)
{
    pthread_mutex_init(&mux,NULL);
    pthread_cond_init(&con,NULL);
    pthread_create(&com_ptr,NULL,pthis->producer,NULL);
    pthread_create(&pro_ptr,NULL,pthis->customer,NULL);
}

void * cs::producer(void *arg)
{
    //cout << "ddddd\n";
    while(1)
    {
        pthread_mutex_lock(&(pthis->mux));
        pthis->que.push(1);
        pthis->tempb();
        pthread_cond_signal(&(pthis->con));
        pthread_mutex_unlock(&(pthis->mux));
        sleep(2);
    }
}

void *cs::customer(void *arg)
{
    cout << "消费者\n";
    while(1)
    {
        pthread_mutex_lock(&(pthis->mux));
        while(pthis->que.empty())
        {
            pthread_cond_wait(&(pthis->con),&(pthis->mux));
        }
        if(pthis->que.empty())
        {
            cerr << "模型出现错误！\n";
            break;  //这个函数只要退出就会发生错误
        }
        pthis->que.pop();
        pthis->tempa();
        pthread_mutex_unlock(&(pthis->mux));
    }
}
cs * cs::pthis=NULL;   //静态成员必须要赋值一次
int main()
{
    cs tmp;
    cs::pthis = &tmp;  //有点奇怪
    while(1)
    sleep(30);
    return 0;
}