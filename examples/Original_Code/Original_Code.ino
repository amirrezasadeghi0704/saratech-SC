#include <saratech_sc.h>

void setup() {
    saratech.begin();
}

void loop() {
    saratech.sensor();
    saratech.printall();
    if (saratech.is_goaller)
        saratech.goaller();
    else
        saratech.forward();
}
