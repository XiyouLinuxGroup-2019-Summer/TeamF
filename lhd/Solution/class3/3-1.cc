/* 给定一个无序数组， 包含正数、 负数和 0， 要求从中找出 3 个数
的乘积， 使得乘积最大， 要求时间复杂度： O(n)， 空间复杂度： O(1) */
//分治法降低时间复杂度
//两个负数和最大正数相乘，三个正数相乘
//选择时间复杂度
#include<iostream>
#include<algorithm>
using namespace std;
void getlay(long num[] , int len)
{
    long max1 ,max2 , max3, min1,min2;
    max1 = max2 =max3 =min1 =min2 = 0;
    for(int i = 0 ; i < len ;i++)
    {
        if(num[i] != 0 )
        {
            if(num[i] > max1)
            {
                    max3 = max2;
                    max2 = max1;
                    max1 = num[i];
            }else if(num[i]>max2){
                    max3 = max2;
                    max2 = num[i];
            }else if(num[i]>max3){
                    max3 = num[i];
            }else if(num[i]<min1 ){
                    min2 = min1;
                    min1 = num[i];
            }else if(num[i]>min1 && num[i]<min2){
                    min2 = num[i];
            }
        }
        else 
            continue;
     }
     long ans  = max(max1*max2*max3,max1*min1*min2);
     cout << ans << endl;
}
int main()
{
    //开两个数组，一个数组保存最大的三个树
    //另外一个数组保存最小的三个数字
    int n ;
    cin >> n;
    long  * array  = new long[n];
    for(int i = 0 ; i < n ; i++)
    {
        cin >> array[i];
    }
    getlay(array,n);
    delete [] array ; 
    return 0;
}
