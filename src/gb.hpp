#ifndef GB_H
#define GB_H

#include "z80.hpp"
#include "rom.hpp"

class gb {
public:
	gb();
	void load_rom(const rom& r);
	void start();
	void set_context(unsigned char **screen);
	//void stop();
	void reset();
private:
	z80 gb_cpu;
	unsigned char *context;
};

#endif
