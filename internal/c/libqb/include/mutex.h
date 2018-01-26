#ifndef INC_MUTEX_H
#define INC_MUTEX_H

#ifdef QB64_WINDOWS
struct MUTEX {
    HANDLE handle;
};
#endif
#ifdef QB64_UNIX
struct MUTEX {
    pthread_mutex_t handle;
};
#endif

MUTEX *new_mutex();
void free_mutex(MUTEX *mutex);
void lock_mutex(MUTEX *mutex);
void unlock_mutex(MUTEX *mutex);

#endif
