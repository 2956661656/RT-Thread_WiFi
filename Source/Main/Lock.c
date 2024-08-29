#include "config.h"

rt_mutex_t buffer_lock;

void InitLock(void)
{
	buffer_lock = rt_mutex_create("buffer_lock", RT_IPC_FLAG_PRIO );
}
INIT_PREV_EXPORT(InitLock);
