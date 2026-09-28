# RTOS 03

This is lab 3, refer [here](https://github.com/uofu-embed/rtos/tree/main/labs/03.threads) for the manual.

## Activity 0

The execution contexts is main_thread and side_thread. the entry point is main.

The shared contexts are the stdout and counter.

We just created a semaphor and then didn't do anything with it.

Side thread will print first, then main second.

After running main code, side_thread prints out first, then main_thread second. Our predictions were true.
