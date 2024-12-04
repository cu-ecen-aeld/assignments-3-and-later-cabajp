#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
//#define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    

    DEBUG_LOG("wait %d", thread_func_args->wait_to_obtain_ms);
    usleep(thread_func_args->wait_to_obtain_ms * 1000);
    
    if(pthread_mutex_lock(thread_func_args->mutex))
    {
	thread_func_args->thread_complete_success = false;
	ERROR_LOG("lock err\n");
	return thread_param;    	
    }
    
    DEBUG_LOG("wait %d", thread_func_args->wait_to_release_ms);
    usleep(thread_func_args->wait_to_release_ms * 1000);
    
    if(pthread_mutex_unlock(thread_func_args->mutex))
    {
    	thread_func_args->thread_complete_success = false;
	ERROR_LOG("unlock err");
	return thread_param;    	
    }
    
    thread_func_args->thread_complete_success = true;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    struct thread_data* ptr = (struct thread_data*)malloc(sizeof(struct thread_data)); 

    ptr->mutex = mutex;	  
    ptr->wait_to_obtain_ms = wait_to_obtain_ms;
    ptr->wait_to_release_ms = wait_to_release_ms;
 
    DEBUG_LOG("starting");
    
    int ret = pthread_create (thread, NULL, threadfunc, (void *) ptr);
    if (ret) {
        //errno = ret;
        //perror("pthread_create");
        ERROR_LOG("starting thread err");
        return false;
    }
    
    DEBUG_LOG("starting thread id %ld", *thread);
    return true;
}

