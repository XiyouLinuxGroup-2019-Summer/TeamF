#include<stdio.h>
char buf[200]="okokokoko";
int main()
{
    printf("\033[31m %s \033[0m \n",buf);
	printf("\033[32m *%s* \033[0m \n",buf);
	printf("\033[35m *你要改变颜色的内容，黄底和紫色字* \033[0m \n");
}