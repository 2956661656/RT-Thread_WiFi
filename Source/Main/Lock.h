#pragma once

extern rt_mutex_t buffer_lock;

extern rt_thread_t bright_thread;
extern rt_thread_t temper_thread;

void InitLock(void);

