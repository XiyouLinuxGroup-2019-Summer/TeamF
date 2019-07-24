#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<iterator>
using namespace std;
class K
{
    public:
    void get()
    {
        cin >> A >> B;
        sort(B.rbegin(),B.rend());
        int j = 0 ;
        for(size_t i = 0; i < A.size() ; i++)
        {
            if(B[j] > A[i] )
            {
                A[i] = B[j];
                j++;
            }
        }
        cout << A << endl;
    }
    private:
    string A;
    string B;
};
int main()
{
    K a1;
    a1.get();
    return 0;
}
