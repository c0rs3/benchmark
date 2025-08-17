#include<timer.h>
#include<filelog.h>

using namespace benchtools;

int main() {
    Logger logger("log.txt", std::ios::app);
    logger.log("Starting something...");
    {
        Timer timer1;
        for (size_t i = 0; i < 1000; i++) {
            std::cout << " LOG " << std::flush;
        }
    }
    logger.log("Ran successfully!");
    std::clog << last_duration.count() << std::endl;
    logger.log(last_duration.count());
    return 0;
}