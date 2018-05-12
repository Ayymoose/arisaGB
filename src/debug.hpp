#ifndef DEBUG_H
#define DEBUH_H

#define GB_LOG

#ifdef GB_LOG
	#define LOG(M, ...) (fprintf(stdout,"[LOG] " M "\n",## __VA_ARGS__))
	#define DPRINTF(M, ...) (fprintf(stdout,"[DEBUG] " M ,## __VA_ARGS__))
#else
	#define LOG(M, ...)
#endif

#endif
