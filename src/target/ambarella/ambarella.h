#ifndef __AMBARELLA_H__
#define __AMBARELLA_H__

#include "stdint.h"
#include <helper/list.h>

#define DEFAULT_COMMAND_TIMEOUT_SEC 5

typedef struct {
	struct list_head list;
	uint16_t low, high;
	char *name;
} range_list_t;

#endif /* __AMBARELLA_H__ */