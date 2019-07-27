#include <stdio.h>
#include <unistd.h>
char * get_exe_path( char * buf, int count)
{
    int i;
    int rslt = readlink("/usr/bin/zstdcat", buf, count - 1);
    if (rslt < 0 || (rslt >= count - 1))
    {
        return NULL;
    }
    buf[rslt] = '\0';
    for (i = rslt; i >= 0; i--)
    {
        if (buf[i] == '/')
        {
            buf[i + 1] = '\0';
            break;
        }
    }
    return buf;
}

int main(int argc, char ** argv)
{
    char buf[1024];
    printf("%s\n", get_exe_path(buf, 1024));
    return 0;
}
