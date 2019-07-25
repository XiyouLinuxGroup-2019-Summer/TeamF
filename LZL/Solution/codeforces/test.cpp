#include<cstdio>
#include<cstdlib>
#include <readline/readline.h>
#include <readline/history.h>
#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景
char *readline (const char *prompt);//返回值就是读取的字符串
void add_history(const char *string);//用来返回历史
typedef char *rl_compentry_func_t(const char *text, int state);
char **rl_completion_matches(const char *text, rl_compentry_func_t *entry_func);
using namespace std;
HIST_ENTRY **ptr;
int main()
{
    while(1)
    {
        char *p=readline(BEGIN(49, 34)"Myshell $ "CLOSE);
        add_history(p);
        if(!strcmp(p,"break")) break;
        free(p);
        write_history("hello");
    }
    int i=0;
    ptr=history_list();
    while(ptr[i]!=NULL)
    printf("%s\n",ptr[i++]->line);
}