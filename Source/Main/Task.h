#pragma once

typedef struct
{
	char *name;
	void (*entry)(void *parameter);
	void       *parameter;
	rt_uint32_t stack_size;
	rt_uint8_t  priority;
	rt_uint32_t tick;
} TaskStruct;

void TaskInit(void);

