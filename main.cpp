#include <iostream>

int main()
{
#ifdef TEST_OPTION
    std::cout << "[ok] Test option enabled\n";
    return 0;
#else
    std::cout << "[error] Test option disabled\n";
    return 1;
#endif
}