#include "common.h"
#include "mutex.h"

#ifdef QB64_WINDOWS
MUTEX* new_mutex(){
    MUTEX *m=(MUTEX*)calloc(1,sizeof(MUTEX));
    m->handle=CreateMutex(
            NULL,              // default security attributes
            FALSE,             // initially not owned
            NULL);             // unnamed mutex
    return m;
}

void free_mutex(MUTEX *mutex){
    //todo
}

void lock_mutex(MUTEX *m){
    if (m==NULL) return;
    WaitForSingleObject(
            m->handle,    // handle to mutex
            INFINITE);  // no time-out interval
}

void unlock_mutex(MUTEX *m) {
    if (m==NULL) return;
    ReleaseMutex(m->handle);
}
#endif
#ifdef QB64_UNIX
MUTEX* new_mutex(){
	MUTEX *m=(MUTEX*)calloc(1,sizeof(MUTEX));
	pthread_mutex_init(&m->handle, NULL);
	return m;
}

void free_mutex(MUTEX *mutex){
	//todo
}

void lock_mutex(MUTEX *m){
	if (m==NULL) return;
	pthread_mutex_lock(&m->handle);
}

void unlock_mutex(MUTEX *m){
	if (m==NULL) return;
	pthread_mutex_unlock(&m->handle);
}
#endif
