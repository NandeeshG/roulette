#include "logger.h"
#include <iostream>

int main()
{
    using namespace casino::logger;
    Logger lg = Logger("main");

    lg.debug(true, 'a', 1, 2.3, "bcd");
    lg.info('a', 1, 2.3, "bcd");
    lg.warning(1, 2.3, "bcd");
    lg.error(2.3, "bcd");
    lg.severe("bcd");

    return 0;
}