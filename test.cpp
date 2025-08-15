#include<timer.h>
#include<filelog.h>
#include<iostream>

using namespace benchtools;

int main() {
    Logger logger("log.txt", std::ios::app);
    logger.log("Starting something idk");
    {
        Timer timer1;
        for (size_t i = 0; i < 1000; i++) {
            std::cout << " xd " << std::flush;
        }
    }
    logger.log("Ran successfully");
    std::clog << last_duration.count() << std::endl;
    logger.log(last_duration.count());
    return 0;
}