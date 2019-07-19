//出差N天，留给M块巧克力，每天吃的不少于前一天吃得
#include<iostream>
using namespace std;

class K
{
    public:
     K()
     {
         this->N = 0;
         this->M = 0 ;
     }
     ~K()  = default;
   public: 
    int sum1(int s)
    {
        //如果第一天吃s个那么总共要吃多少个
        int result = 0 ; 
        for(int i = 1; i<= N ; i++)
        {
            result += s ;
            s = (s+1) / 2; // 向上取整数     如果为奇数的话向上取整
            //因为吃巧克力不可能只吃半个
        }
        return result;
    }
    int  sum()
    {
        cin >> N;
        cin >> M;
        //使用二分查找来进行搜索
        //第一天肯定是大于1小于m的
        if(N == 1)
            return M;
        int low = 1 ;
        int high = M ;
        int middle = 0 ;
        while(low < high)
        {
            middle = (low+high + 1)  / 2 ; // 向上取整
            if(M == sum1(middle))
                return middle;
            else if (M > sum1(middle))
            {
                 low = middle ;
            } 
            else  if( M < sum1(middle))
            {
                high = middle - 1;
            }
        }
        return middle;
    } 
    private:
        int N ;
        int M ;
};
int main()
{
    K a1;
    int p = a1.sum();
    cout << p << endl;
    return 0;
}
