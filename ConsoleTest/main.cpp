#include "stdafx.h"
#include "Test.h"

const bool learn = true;

int main()
{
    Init();
    if (learn)
    {
        Learn();
    }
    else
    {
        Test();
    }
    return 0;
}