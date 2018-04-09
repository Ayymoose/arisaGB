#include <string>
#include <iostream>

#ifndef ROM_H
#define ROM_H

// Header start address
#define CARTRIDGE_HEADER 0x134
// Length of title of game (max 16 bytes)
#define TITLE_LENGTH 16

// Length of the logo in bytes
#define NINTENDO_LOGO_LEN 48		
// Starting address of the logo 
#define NINTENDO_LOGO_ADDR 0x104

// 2MB
#define ROM_MAX_SIZE (2<<22)

class z80;

class rom {

	public:
		rom();
		~rom();
		// Load a ROM from file into memory 
		void load_rom(std::string file_name);

		friend z80;

		// Debug function
		// Reference to magic numbers 
		// http://www.zophar.net/fileuploads/2/10597teazh/gbrom.txt
		friend std::ostream& operator<<(std::ostream &os, const rom &r) {
			
			std::string feature, cartridge_type, rom_size, ram_size, dest_code, licensee_code, new_licensee;
			os << "ROM title: " << r.title << "\n";
	
		    unsigned char low_high = static_cast<int>(r.license_high) & 0x0F;
			unsigned char low_low = static_cast<int>(r.license_low) & 0x0F;

			switch ((low_high << 4) | low_low) {
			  case 0x00:
				new_licensee = "None"; break;
			  case 0x13:
				new_licensee = "Electronic Arts"; break;
			  case 0x20:
				new_licensee = "KSS"; break;
			  case 0x25:
				new_licensee = "San-X"; break;
			  case 0x30:
				new_licensee = "Viacom"; break;
			  case 0x33:
				new_licensee = "Ocean/Acclaim"; break;
			  case 0x37:
				new_licensee = "Taito"; break;
			  case 0x41:
				new_licensee = "Ubisoft"; break;
			  case 0x46:
				new_licensee = "Angel"; break;
			  case 0x50:
				new_licensee = "Absolute"; break;
			  case 0x53:
				new_licensee = "American Sammy"; break;
			  case 0x56:
				new_licensee = "LJN"; break;
			  case 0x59:
				new_licensee = "Milton Bradley"; break;
			  case 0x64:
				new_licensee = "Lucasarts"; break;
			  case 0x70:
				new_licensee = "Infogrames"; break;
			  case 0x73:
				new_licensee = "Sculptured"; break;
			  case 0x79:
				new_licensee = "Accolade"; break;
			  case 0x86:
				new_licensee = "Tokuma Shoten"; break;
			  case 0x92:
				new_licensee = "Video System"; break;
			  case 0x96:
				new_licensee = "Yonezawa/s'pal"; break;
			  case 0x01:
				new_licensee = "Nintendo"; break;
			  case 0x18:
				new_licensee = "Hudsonsoft"; break;
			  case 0x22:
				new_licensee = "Pow"; break;
			  case 0x28:
				new_licensee = "Kemco Japan"; break;
			  case 0x31:
				new_licensee = "Nintendo"; break;
			  case 0x34:
				new_licensee = "Konami"; break;
			  case 0x38:
				new_licensee = "Hudson"; break;
			  case 0x42:
				new_licensee = "Atlus"; break;
			  case 0x47:
				new_licensee = "Pullet-Proof"; break;
			  case 0x51:
				new_licensee = "Acclaim "; break;
			  case 0x54:
				new_licensee = "Konami"; break;
			  case 0x57:
				new_licensee = "Matchbox "; break;
			  case 0x60:
				new_licensee = "Titus"; break;
			  case 0x67:
				new_licensee = "Ocean"; break;
			  case 0x71:
				new_licensee = "Interplay"; break;
			  case 0x75:
				new_licensee = "SCI"; break;
			  case 0x80:
				new_licensee = "Misawa"; break;
			  case 0x87:
				new_licensee = "Tsukuda Ori"; break;
			  case 0x93:
				new_licensee = "Ocean/Acclaim"; break;
			  case 0x97:
				new_licensee = "Kaneko"; break;
			  case 0x08:
				new_licensee = "Capcom"; break;
			  case 0x19:
				new_licensee = "B-AI"; break;
			  case 0x24:
				new_licensee = "PCM Complete"; break;
			  case 0x29:
				new_licensee = "Seta"; break;
			  case 0x32:
				new_licensee = "Bandia"; break;
			  case 0x35:
				new_licensee = "Hector"; break;
			  case 0x39:
				new_licensee = "Banpresto"; break;
			  case 0x44:
				new_licensee = "Malibu"; break;
			  case 0x49:
				new_licensee = "Irem"; break;
			  case 0x52:
				new_licensee = "Activision"; break;
			  case 0x55:
				new_licensee = "Hi Tech Entertainment"; break;
			  case 0x58:
				new_licensee = "Mattel"; break;
			  case 0x61:
				new_licensee = "Virgin"; break;
			  case 0x69:
				new_licensee = "Electronic Arts"; break;
			  case 0x72:
				new_licensee = "Broderbund"; break;
			  case 0x78:
				new_licensee = "THQ"; break;
			  case 0x83:
				new_licensee = "Lozc"; break;
			  case 0x91:
				new_licensee = "Chunsoft"; break;
			  case 0x95:
				new_licensee = "Varie"; break;
			  case 0x99:
				new_licensee = "Pack-In-Video"; break;
			  default:
				new_licensee = "Unknown"; break;
			}
			
			os << "New Licensee: " << new_licensee << "\n";	

			switch (static_cast<int>(r.feature)) {
				case 0x00: feature = "Gameboy functions"; break;
				case 0x03: feature = "Super GameBoy functions"; break;
				default: feature = "Unknown";
			}

			os << "Feature: " << feature << "\n";

			switch (static_cast<int>(r.cartridge_type)) {
			  case 0x00:
				cartridge_type = "ROM"; break;
			  case 0x01:
				cartridge_type = "MBC1"; break;
			  case 0x02:
				cartridge_type = "MBC1+RAM"; break;
			  case 0x03:
				cartridge_type = "MBC1+RAM+BATTERY"; break;
			  case 0x05:
				cartridge_type = "MBC2"; break;
			  case 0x06:
				cartridge_type = "MBC2+BATTERY"; break;
			  case 0x08:
				cartridge_type = "ROM+RAM "; break;
			  case 0x09:
				cartridge_type = "ROM+RAM+BATTERY"; break;
			  case 0x0B:
				cartridge_type = "MMM01"; break;
			  case 0x0C:
				cartridge_type = "MMM01+RAM"; break;
			  case 0x0D:
				cartridge_type = "MMM01+RAM+BATTERY"; break;
			  case 0x0F:
				cartridge_type = "MBC3+TIMER+BATTERY"; break;
			  case 0x10:
				cartridge_type = "MBC3+TIMER+RAM+BATTERY"; break;
			  case 0x11:
				cartridge_type = "MBC3"; break;
			  case 0x12:
				cartridge_type = "MBC3+RAM"; break;
			  case 0x13:
				cartridge_type = "MBC3+RAM+BATTERY"; break;
			  case 0x15:
				cartridge_type = "MBC4"; break;
			  case 0x16:
				cartridge_type = "MBC4+RAM"; break;
			  case 0x17:
				cartridge_type = "MBC4+RAM+BATTERY"; break;
			  case 0x19:
				cartridge_type = "MBC5"; break;
			  case 0x1A:
				cartridge_type = "MBC5+RAM"; break;
			  case 0x1B:
				cartridge_type = "MBC5+RAM+BATTERY"; break;
			  case 0x1C:
				cartridge_type = "MBC5+RUMBLE"; break;
			  case 0x1D:
				cartridge_type = "MBC5+RUMBLE+RAM"; break;
			  case 0x1E:
				cartridge_type = "MBC5+RUMBLE+RAM+BATTERY"; break;
			  case 0xFC:
				cartridge_type = "POCKET CAMERA"; break;
			  case 0xFD:
				cartridge_type = "Bandai TAMA5"; break;
			  case 0xFE:
				cartridge_type = "HuC3"; break;
			  case 0xFF:
				cartridge_type = "HuC1+RAM+BATTERY"; break;
			  default:
				cartridge_type = "Unknown"; break;
			}

			os << "Cartridge Type: " << cartridge_type << "\n";

			

			switch (static_cast<int>(r.rom_size)) {
			  case 0x00:
				rom_size = "32KB"; break;
			  case 0x01:
				rom_size = "64KB"; break;
			  case 0x02:
				rom_size = "128KB"; break;
			  case 0x03:
				rom_size = "256KB"; break;
			  case 0x04:
				rom_size = "512KB"; break;
			  case 0x05:
				rom_size = "1MB"; break;
			  case 0x06:
				rom_size = "2MB"; break;
			  case 0x52:
				rom_size = "1.1MB"; break;
			  case 0x53:
				rom_size = "1.2MB"; break;
			  case 0x54:
				rom_size = "1.5MB"; break;
			  default:
				rom_size = "Unknown"; break;
			}

			os << "ROM Size: " << rom_size << "\n";

			switch (static_cast<int>(r.ram_size)) {
			  case 0x00:
				ram_size = "None"; break;
			  case 0x01:
				ram_size = "2kB"; break;
			  case 0x02:
				ram_size = "8kB"; break;
			  case 0x03:
				ram_size = "32kB"; break;
			  case 0x04:
				ram_size = "128kB"; break;
			  default:
				ram_size = "Unknown"; break;
			}

			os << "RAM Size: " << ram_size << "\n";

			switch (static_cast<int>(r.dest_code)) {
			  case 0x00:
				dest_code = "Japanese"; break;
			  case 0x01:
				dest_code = "Non-Japanese"; break;
			  default:
				dest_code = "Unknown"; break;
			}
		
			os << "Destination Code: " << dest_code << "\n";

			switch (static_cast<int>(r.licensee_code))
			  {
			  case 0x00:
				licensee_code = "None"; break;
			  case 0x01:
				licensee_code = "Nintendo"; break;
			  case 0x08:
				licensee_code = "Capcom"; break;
			  case 0x09:
				licensee_code = "Hot-B"; break;
			  case 0x0A:
				licensee_code = "Jaleco "; break;
			  case 0x0B:
				licensee_code = "Coconuts"; break;
			  case 0x0C:
				licensee_code = "Elite Systems"; break;
			  case 0x13:
				licensee_code = "Electronic Arts"; break;
			  case 0x18:
				licensee_code = "Hudsonsoft"; break;
			  case 0x19:
				licensee_code = "ITC Entertainment"; break;
			  case 0x1A:
				licensee_code = "Yanoman"; break;
			  case 0x1D:
				licensee_code = "Clary"; break;
			  case 0x1F:
				licensee_code = "Virgin"; break;
			  case 0x24:
				licensee_code = "PCM Complete"; break;
			  case 0x25:
				licensee_code = "San-X"; break;
			  case 0x28:
				licensee_code = "Kotobuki Systems"; break;
			  case 0x29:
				licensee_code = "Seta"; break;
			  case 0x30:
				licensee_code = "Infogrames"; break;
			  case 0x31:
				licensee_code = "Nintendo"; break;
			  case 0x32:
				licensee_code = "Bandai"; break;
			  case 0x33:
				licensee_code = "Unknown"; break;
			  case 0x34:
				licensee_code = "Konami "; break;
			  case 0x35:
				licensee_code = "Hector"; break;
			  case 0x38:
				licensee_code = "Capcom"; break;
			  case 0x39:
				licensee_code = "Banpresto"; break;
			  case 0x3C:
				licensee_code = "Entertainment i"; break;
			  case 0x3E:
				licensee_code = "Gremlin"; break;
			  case 0x41:
				licensee_code = "Ubisoft "; break;
			  case 0x42:
				licensee_code = "Atlus"; break;
			  case 0x44:
				licensee_code = "Malibu"; break;
			  case 0x46:
				licensee_code = "Angel"; break;
			  case 0x47:
				licensee_code = "Spectrum Holoby"; break;
			  case 0x49:
				licensee_code = "Irem"; break;
			  case 0x4A:
				licensee_code = "Virgin"; break;
			  case 0x4D:
				licensee_code = "Malibu"; break;
			  case 0x4F:
				licensee_code = "U.S. Gold"; break;
			  case 0x50:
				licensee_code = "Absolute"; break;
			  case 0x51:
				licensee_code = "Acclaim"; break;
			  case 0x52:
				licensee_code = "Activision"; break;
			  case 0x53:
				licensee_code = "American Sammy"; break;
			  case 0x54:
				licensee_code = "Gametek"; break;
			  case 0x55:
				licensee_code = "Park Place"; break;
			  case 0x56:
				licensee_code = "LJN"; break;
			  case 0x57:
				licensee_code = "Natchbox"; break;
			  case 0x59:
				licensee_code = "Milton Bradley"; break;
			  case 0x5A:
				licensee_code = "Mindscape"; break;
			  case 0x5B:
				licensee_code = "Romstar"; break;
			  case 0x5C:
				licensee_code = "Naxat Soft"; break;
			  case 0x5D:
				licensee_code = "Tradewest"; break;
			  case 0x60:
				licensee_code = "Titus"; break;
			  case 0x61:
				licensee_code = "Virgin"; break;
			  case 0x67:
				licensee_code = "Ocean"; break;
			  case 0x69:
				licensee_code = "Electronic Arts"; break;
			  case 0x6E:
				licensee_code = "Elite Systems"; break;
			  case 0x6F:
				licensee_code = "Electro Brain"; break;
			  case 0x70:
				licensee_code = "Infogrames"; break;
			  case 0x71:
				licensee_code = "Interplay"; break;
			  case 0x72:
				licensee_code = "Broderbund"; break;
			  case 0x73:
				licensee_code = "Sculptered Soft"; break;
			  case 0x75:
				licensee_code = "The Sales Curve"; break;
			  case 0x78:
				licensee_code = "THQ"; break;
			  case 0x79:
				licensee_code = "Accolade"; break;
			  case 0x7A:
				licensee_code = "Triffix Entertainment"; break;
			  case 0x7C:
				licensee_code = "Microprose"; break;
			  case 0x7F:
				licensee_code = "Kemco"; break;
			  case 0x80:
				licensee_code = "Misawa Entertainment"; break;
			  case 0x83:
				licensee_code = "Lozc"; break;
			  case 0x86:
				licensee_code = "Tokuma Shoten"; break;
			  case 0x8B:
				licensee_code = "Bullet-proof Software"; break;
			  case 0x8C:
				licensee_code = "Vic Tokai"; break;
			  case 0x8E:
				licensee_code = "Ape"; break;
			  case 0x8F:
				licensee_code = "I'max"; break;
			  case 0x91:
				licensee_code = "Chunsoft"; break;
			  case 0x92:
				licensee_code = "Video System"; break;
			  case 0x93:
				licensee_code = "Tsuburava"; break;
			  case 0x95:
				licensee_code = "Varie"; break;
			  case 0x96:
				licensee_code = "Yonezawa/s'pal"; break;
			  case 0x97:
				licensee_code = "Kaneko"; break;
			  case 0x99:
				licensee_code = "Arc"; break;
			  case 0x9A:
				licensee_code = "Nihon Bussan"; break;
			  case 0x9B:
				licensee_code = "Tecmo"; break;
			  case 0x9C:
				licensee_code = "Imagineer"; break;
			  case 0x9D:
				licensee_code = "Banpresto"; break;
			  case 0x9F:
				licensee_code = "Nova"; break;
			  case 0xA1:
				licensee_code = "Hori Electric"; break;
			  case 0xA2:
				licensee_code = "Bandai"; break;
			  case 0xA4:
				licensee_code = "Konami"; break;
			  case 0xA6:
				licensee_code = "Kawada"; break;
			  case 0xA7:
				licensee_code = "Takara"; break;
			  case 0xA9:
				licensee_code = "Technos Japan"; break;
			  case 0xAA:
				licensee_code = "Broderbund "; break;
			  case 0xAC:
				licensee_code = "Toei Animation"; break;
			  case 0xAD:
				licensee_code = "Toho"; break;
			  case 0xAF:
				licensee_code = "Namco"; break;
			  case 0xB0:
				licensee_code = "Acclaim"; break;
			  case 0xB1:
				licensee_code = "Ascii/Nexoft"; break;
			  case 0xB2:
				licensee_code = "Bandai"; break;
			  case 0xB4:
				licensee_code = "Enix"; break;
			  case 0xB6:
				licensee_code = "HAL"; break;
			  case 0xB7:
				licensee_code = "SNK"; break;
			  case 0xB9:
				licensee_code = "Pony Canyon"; break;
			  case 0xBA:
				licensee_code = "Culture Brain"; break;
			  case 0xBB:
				licensee_code = "Sunsoft"; break;
			  case 0xBD:
				licensee_code = "Sony Imagesoft"; break;
			  case 0xBF:
				licensee_code = "Sammy"; break;
			  case 0xC0:
				licensee_code = "Taito"; break;
			  case 0xC2:
				licensee_code = "Kemco"; break;
			  case 0xC3:
				licensee_code = "Squaresoft"; break;
			  case 0xC4:
				licensee_code = "Tokuma Shoten"; break;
			  case 0xC5:
				licensee_code = "Data East"; break;
			  case 0xC6:
				licensee_code = "Tonkin House"; break;
			  case 0xC8:
				licensee_code = "Koei"; break;
			  case 0xC9:
				licensee_code = "UFL"; break;
			  case 0xCA:
				licensee_code = "Ultra"; break;
			  case 0xCB:
				licensee_code = "VAP"; break;
			  case 0xCC:
				licensee_code = "USE"; break;
			  case 0xCD:
				licensee_code = "Meldac"; break;
			  case 0xCE:
				licensee_code = "Pony Canyon"; break;
			  case 0xCF:
				licensee_code = "Angel"; break;
			  case 0xD0:
				licensee_code = "Taito"; break;
			  case 0xD1:
				licensee_code = "Sofel"; break;
			  case 0xD2:
				licensee_code = "Quest"; break;
			  case 0xD3:
				licensee_code = "Sigma Enterprises"; break;
			  case 0xD4:
				licensee_code = "Ask Kodansha"; break;
			  case 0xD6:
				licensee_code = "Naxat Soft"; break;
			  case 0xD7:
				licensee_code = "Copya Systems"; break;
			  case 0xD9:
				licensee_code = "Banpresto"; break;
			  case 0xDA:
				licensee_code = "Tomy"; break;
			  case 0xDB:
				licensee_code = "LJN"; break;
			  case 0xDD:
				licensee_code = "NCS"; break;
			  case 0xDE:
				licensee_code = "Human"; break;
			  case 0xDF:
				licensee_code = "Altron"; break;
			  case 0xE0:
				licensee_code = "Jaleco "; break;
			  case 0xE1:
				licensee_code = "Towachiki"; break;
			  case 0xE2:
				licensee_code = "Uutaka"; break;
			  case 0xE3:
				licensee_code = "Varie"; break;
			  case 0xE5:
				licensee_code = "Epoch"; break;
			  case 0xE7:
				licensee_code = "Athena"; break;
			  case 0xE8:
				licensee_code = "Asmik"; break;
			  case 0xE9:
				licensee_code = "Natsume"; break;
			  case 0xEA:
				licensee_code = "King Records"; break;
			  case 0xEB:
				licensee_code = "Atlus"; break;
			  case 0xEC:
				licensee_code = "Epic/Sony Records"; break;
			  case 0xEE:
				licensee_code = "IGS"; break;
			  case 0xF0:
				licensee_code = "A Wave"; break;
			  case 0xF3:
				licensee_code = "Extreme Entertainment"; break;
			  case 0xFF:
				licensee_code = "LJN"; break;
			  default:
				licensee_code = "Unknown"; break;
			}

			os << "Licensee Code: " << licensee_code;

			return os;
		}

	private:
		unsigned char *rom_data;
		std::string title; 				/* Title of the game in upper case ASCII */
		unsigned char license_high;    /* Licensee (High nibble) */
		unsigned char license_low;     /* Licensee (Low nibble) */
		unsigned char feature;        /* SGB Indicator */
		unsigned char cartridge_type;  /* Cartridge type */
		unsigned char rom_size;        /* ROM Size */
		unsigned char ram_size;        /* RAM Size */
		unsigned char dest_code;       /* Destination code */
		unsigned char licensee_code;   /* Licensee code (old) */
};


#endif
