#undef TRACE_SYSTEM
#define TRACE_SYSTEM ncc

#if !defined(_TRACE_NCC_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_NCC_H

#include <linux/types.h>
//#include <linux/tracepoint.h>
#include <linux/trace_events.h>
#include <trace/events/gfpflags.h>

TRACE_EVENT(ncc_open,
	TP_PROTO(const char *name, int flags),

	TP_ARGS(name, flags),

	TP_STRUCT__entry(
		__field(	char *,	name		)
		__field(	int,	flags		)
		__field(	int,	acc_mode		)
	),

	TP_fast_assign(
		__entry->name	= name ? name : "NA";
		__entry->flags	= flags;
		__entry->acc_mode	= (flags & 0x3);
	),

	TP_printk("Name=%s flags=0x%lu acc_mode=%d",__entry->name,__entry->flags,__entry->acc_mode)
);

#endif /* _TRACE_NCC_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
