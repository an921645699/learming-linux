#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<event.h>
#include<signal.h>
#include<time.h>

void sig_cb(int fd,short ev,void* arg)
{
    if(ev&EV_SIGNAL)
        printf("sig = %d\n",fd);
}

void timeout_cb(int fd,short ev,void* arg)
{
    if(ev&EV_TIMEOUT)
        printf("time out\n");
}

int main()
{
    struct event_base* base = event_init();//创建libevent实例
    assert( base != NULL);

    struct event * sig_ev = evsignal_new(base,SIGINT,sig_cb,NULL);//event_new()
    event_add(sig_ev,NULL);
    struct event * time_ev = evtimer_new(base,timeout_cb,NULL);
    
    struct timeval tv = {5,0};
    event_add(time_ev,&tv);

    event_base_dispatch(base);

    event_free(sig_ev);
    event_free(time_ev);
    event_base_free(base);

}
