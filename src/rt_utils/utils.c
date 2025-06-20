#include "minirt.h"

int count_tokens(char **strs)
{
    int i;

    i = -1;
    while (strs[++i]);
    return (i);
}
