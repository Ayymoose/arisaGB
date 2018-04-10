#include "gb.hpp"
#include "rom.hpp"

gb::gb() {
	gb_cpu.reset();
}

// TODO: Complete
// At present, loads 16kB into Bank 0
void gb::load_rom(const rom& r) {

}
void gb::start() {
	for (int i=0; i<10; i++) {
		gb_cpu.execute();
	}
}

void gb::stop() {

}
