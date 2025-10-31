
In this repository we have classic "race coondition problem".

The problem is that the operations a++ and a-- are not atomic. They consist of three separate steps (Read-Modify-Write) and "race condition" occur when our 8 threads perfom these steps at the same time.

Fot fix this problem we have 2 different methods:

 -mutex:
    its a synchronization primitive that is used to protect the shared data from being accessed by multiple threads simultaneously.
    With its help we provide access to the variable to only one thread during a period of time.

 -atomic:
    This is a more modern and often more efficient approach for simple types. Instead of using an external lock, we change the type of the variable itself to be atomic. The main idea is that we combine (Read-Modify-Write) into a single operation that is performed by the processor in a single unit of time.
    