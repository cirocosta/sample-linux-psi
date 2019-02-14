SAMPLE LINUX PSI

        > A demonstration of how Linux PSI can help troubleshoot what's driving that high load.


        This is a feature that is landing in the new releases of Linux after v4.20, essentially
        being available after Ubuntu Disco (the 19.04 release that should be a thing in April).

        The rationale for this lightning talk was really to just showcase how this feature can
        help us in the future better assess what caused issues in a system.



        LOAD

                If you go around the office, you can probably see some graphs titled "load", tracking
                the `load average` metric that can be seen from `uptime`.


                        ```
                                                                     \/    \/     \/
                        20:34:28 up  1:47,  2 users,  load average: 0.00, 0.29, 5.33
                        ```


                And, there's a good reason for looking at that metric - it can give you not only a good
                idea of the running thread task demand in the system.

                If we assume that the CPU threads are these units that serve the tasks with their time,
                what does load averages lets us see is an aggregate view of how many are being at the
                given moment and how many are queued to be run - waiting for their time.


                        ```
                                tasks waiting
                        .---------------------------------.
                        |         t1  t8    t4            |
                        |              t6        t5       |
                        |     t2   t7       t3            |
                        *---------------------------------*


                                tasks being served
                        .---------------------------------.
                        |       CPU-0 -  t0               |
                        |       CPU-1 -  t3               |
                        *---------------------------------*

                        ```

                Naturally, if there are many many threads being created, trying to run, and not having a bunch
                of CPUs to run them, then the load averages increase.


                        ```
                        ./src/cpu.c
                        ```


                One tricky thing about these numbers is that you must know how many cores the machine has to
                determine what a "good" really means.


                        ```

                        A machine w/ 64 threads would be just fine with a load of 64 - every task is being served.

                        ```


                Aside from that, there's also the fact that not all of those tasks piling up in the stack of
                tasks to be run are necessarily consuming CPU resources - they must just be in an uninterruptible
                state waiting for a syscall to return.


                For instance, on Wings, we've seen some times the `btrfs` filesystem kernel module dying and all of
                the system calls that get issues against files that pertain such filesystem blocking forever, leading
                to this huuuuge load:


                        https://bit.ly/2tmkuYR


