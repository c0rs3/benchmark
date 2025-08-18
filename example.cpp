#include <benchmark.h>

using namespace benchtools;

int main() {
    Logger logger("log.txt", PUSHBACK);
    logger.log("Starting something...");
    {
        Timer timer1;
        for (size_t i = 0; i < 1000; i++) {
            std::cout << " LOG " << std::flush;
        }
    }
    logger.log("Ran successfully!");
    std::clog << LAST_DURATION.count() << std::endl;
    logger.log(LAST_DURATION.count());
    return 0;
}
