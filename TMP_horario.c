#include "blinds.h"
#include <time.h>
#include <unistd.h>

void wait_till_midnight(){

    time_t rawtime;
    struct tm *timeinfo;

    while (1) {
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        if (timeinfo->tm_hour == 0) {
            return;
        }
    }
}

int main(){

    home h = fread_home("casa");

    wait_till_midnight();
    
    
}