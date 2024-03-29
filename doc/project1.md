# Project 1 Design Document
> SJTU MS110  
> Spring 2019

## GROUP

- JiaSen Li <lijiasen0921@126.com>
- Erzhi Liu <654105282@qq.com>
- Boning Li <lbn187@sjtu.edu.cn>
- Enze Sun <sun_en_ze@126.com>


## PROJECT PARTS

#### DATA STRUCTURES

    struct thread
      {
        /* Owned by thread.c. */
        tid_t tid;                          /* Thread identifier. */
        enum thread_status status;          /* Thread state. */
        char name[16];                      /* Name (for debugging purposes). */
        uint8_t *stack;                     /* Saved stack pointer. */
        int priority;                       /* Priority. */
        struct list_elem allelem;           /* List element for all threads list. */

        uint64_t sleep_until;
        /* Shared between thread.c and synch.c. */
        struct list_elem elem;              /* List element. */

    #ifdef USERPROG
        /* Owned by userprog/process.c. */
        uint32_t *pagedir;                  /* Page directory. */
    #endif

        /* Owned by thread.c. */
        unsigned magic;                     /* Detects stack overflow. */
        int nice;
        struct realnum recent_cpu;
        //TODO: 
        //uint64_t sleep_until;
        struct lock *wait_lock;
        int donation;                       /* This is the donation value */
        uint8_t uses_donation;
        //:TODO
        //Project2:
        int return_value;
        //:Project2
        
        //struct lock 
        
      };

    /* If false (default), use priority scheduler.
       If true, use multi-level feedback queue scheduler.
       Controlled by kernel command-line option "-o mlfqs". */
    /* Lock. */

### A. ALARM CLOCK  

#### ALGORITHMS

> A1: Briefly describe the algorithmic flow for a call to `timer_sleep()`,
> including the effects of the timer interrupt handler.

1.  Get current thread and record its wake up ticks.
2.  Turn off interrupts, put current thread out of ready list then turn on the interrupts.
3.  Let current thread go to sleep until the time interrupt at which it should wake up.
4.  Timer interrupt handler checks the sleeping threads and wakes up all threads which are ready according to the wake up ticks.

> A2: What steps are taken to minimize the amount of time spent in
> the timer interrupt handler?

When putting one thread into sleeping thread queue, we carefully keep its order according to the wake up ticks. Hence, we only need to check from left to right to see whether the sleeping thread needs to wake up when handling timer interrupt. This minimizes the amount of time spent in the handler.

#### SYNCHRONIZATION

> A3: How are race conditions avoided when multiple threads call
> `timer_sleep()` simultaneously? How are race conditions avoided when a timer interrupt occurs
> during a call to `timer_sleep()`?

We turn interrupts off and later on while inserting a thread into sleeping thread queue. Hence only one thread will be operating on the queue. This can be seen as an atomic operation. 

### PRIORITY SCHEDULING

#### ALGORITHMS

> B1: How do you ensure that the highest priority thread waiting for
> a lock, semaphore, or condition variable wakes up first?

When a thread has to wait for a semaphore, we need to put the thread into its waiting queue. When the semaphore is ready we only need to wake up the highest priority thread in the queue.

> B2: Describe the sequence of events when a call to `lock_acquire()`
> causes a priority donation.  How is nested donation handled?

1.  Turn off interrupts.
2.  If the lock is held by another thread, we update the priority of all the threads on the waiting chain with the priority of current thread.
3.  Turn on interrupts.
4.  Down the semaphore. If the lock is held by another thread, current thread will go to sleep automatically. Otherwise it will successfully down the semaphore.
5.  Modify the donation priority of the thread. Then Turn off interrupts. Actually modifying the donation priority of the thread is not necessary for priority scheduler. Because itself has higher or equal priority than the waiting queue. But we just do so to enable some other personal changes of scheduler by users. 
6.  Modify the holder of the lock, and the lock into current threads' holding list, and update the priority of current thread. At this step, the thread must be awake, so there should be no parent thread in the forest of the hold and wait. 

> B3: Describe the sequence of events when `lock_release()` is called
> on a lock that a higher-priority thread is waiting for.

1.  Turn off interrupts.
2.  Remove the lock from the holding list of current thread, and update the priority of current thread. Set the waiting thread of the lock into NULL. At this step, the thread must be awake, so there should be no parent thread in the forest of the hold and wait. 
3.  Up the semaphore.
4.  Turn on interrupts.

> B4: What to do after setting the priority?

1. Set the priority to the target priority
2. Unset the donation of the waiting chain
3. Reset the donation of the waiting chain 

To be safe, the 3 steps are written with interrupt off. 

