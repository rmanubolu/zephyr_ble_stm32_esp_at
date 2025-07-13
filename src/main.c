/*
 * Copyright (c) 2025 Ramesh Manubolu
 */

 #include <stdio.h>
 #include <zephyr/kernel.h>
 
// sleep time
 #define SLEEP_TIME_MS   1000
 
 int main(void)
 {
     while (1) {
         k_msleep(SLEEP_TIME_MS);
     }
     return 0;
 }
 