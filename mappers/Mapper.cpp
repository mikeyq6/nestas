#include "../inc/mappers/Mapper.h"

Mapper::Mapper(const char *raw_cartridge_data) {
    if (raw_cartridge_data[0] == 'N' && raw_cartridge_data[1] == 'E' && raw_cartridge_data[2] == 'S' && 
        raw_cartridge_data[3] == 0x1a && (raw_cartridge_data[7] & 0x0c) == 0x08) {
        rom_format = NES2;
    } else {
        rom_format = NES;
    }

    parse_header(raw_cartridge_data);
    read_rom_data(raw_cartridge_data);
}

Mapper::~Mapper() {
    delete[] prg_rom;
    if(chr_rom_size > 0) delete[] chr_rom;
    if(prg_ram_size > 0) delete[] prg_ram;
    if(prg_nvram_size > 0) delete[] prg_nvram;
    if(chr_ram_size > 0) delete[] chr_ram;
    if(chr_ram_size > 0) delete[] chr_nvram;
    if(has_trainer) delete[] trainer_data;
}

void Mapper::parse_header(const char *raw_cartridge_data) {
    // Common values
    nametable_layout = (raw_cartridge_data[6] & 0x01) ? HORIZONTAL : VERTICAL;
    has_battery = (raw_cartridge_data[6] & 0x02) != 0;
    has_trainer = (raw_cartridge_data[6] & 0x04) != 0;
    use_alternative_nametables = (raw_cartridge_data[6] & 0x08) != 0;

    if(rom_format == NES2) {
        if((raw_cartridge_data[9] & 0xf) == 0xf) {
            uint8_t exp = raw_cartridge_data[4] & 0xfc;
            uint8_t mt = ((raw_cartridge_data[4] & 0x03) * 2) + 1;
            prg_rom_size = mt * (2 << exp);
        } else {
            prg_rom_size = (raw_cartridge_data[4] | ((raw_cartridge_data[9] & 0xf) << 8)) * 0x4000;
        }
        if((raw_cartridge_data[9] & 0xf0) == 0xf0) {
            uint8_t exp = raw_cartridge_data[5] & 0xfc;
            uint8_t mt = ((raw_cartridge_data[5] & 0x03) * 2) + 1;
            chr_rom_size = mt * (2 << exp);
        } else {
            chr_rom_size = ((raw_cartridge_data[5] & 0x0f) | ((raw_cartridge_data[9] & 0xf0) << 4)) * 0x2000;
        }
        uint8_t shift_count = raw_cartridge_data[10] & 0x0f;
        if(shift_count > 0) {
            prg_ram_size = 0x40 << shift_count;
        } else {
            prg_ram_size = 0;
        }
        shift_count = (raw_cartridge_data[10] & 0xf0) >> 4;
        if(shift_count > 0) {
            prg_nvram_size = 0x40 << shift_count;
        } else {
            prg_nvram_size = 0;
        }
        shift_count = raw_cartridge_data[11] & 0x0f;
        if(shift_count > 0) {
            chr_ram_size = 0x40 << shift_count;
        } else {
            chr_ram_size = 0;
        }
        shift_count = (raw_cartridge_data[11] & 0xf0) >> 4;
        if(shift_count > 0) {
            chr_nvram_size = 0x40 << shift_count;
        } else {
            chr_nvram_size = 0;
        }

        console_type = static_cast<ConsoleType>(raw_cartridge_data[7] & 0x03);
        mapper_number = ((raw_cartridge_data[8] & 0xf) << 8) | ((raw_cartridge_data[6] & 0xf0) >> 4) | (raw_cartridge_data[7] & 0xf0);
        submapper_number = (raw_cartridge_data[8] & 0xf0) >> 4;
        cpu_ppu_timing = static_cast<CPUPPUTiming>(raw_cartridge_data[12] & 0x03);

        if(console_type == NINTENDO_VS) {
            ppu_type = (raw_cartridge_data[13] & 0x0f);
            hardware_type = (raw_cartridge_data[13] & 0xf0) >> 4;
        } else if(console_type == EXTENDED_CONSOLE) {
            extended_console_type = raw_cartridge_data[13] & 0xf;
        }
        num_miscellaneous_roms = raw_cartridge_data[14] & 0x03;
        // TODO: Implement miscellaneous roms data, although it's not used often, see https://www.nesdev.org/wiki/NES_2.0#Miscellaneous_ROM_Area

        default_expansion_device = raw_cartridge_data[15] & 0x3f;
    } else { // rom_format == NES
        prg_rom_size = raw_cartridge_data[4] * 0x4000; // 16KB units
        chr_rom_size = raw_cartridge_data[5] * 0x2000; // 8KB units
        uses_chr_ram = (raw_cartridge_data[5] == 0);
        mapper_number = ((raw_cartridge_data[6] & 0xf0) >> 4) | (raw_cartridge_data[7] & 0xf0);
        if(raw_cartridge_data[7] & 0x01) {
            console_type = NINTENDO_VS;
        } else if(raw_cartridge_data[7] & 0x02) {
            console_type = PLAYCHOICE_10;
        } else {
            console_type = NES_FAM;
        }
        prg_ram_size = raw_cartridge_data[8] == 0 ? 0x2000 : raw_cartridge_data[8] * 0x2000; // 8KB units, but some cartridges use 0 to indicate 8KB
        tv_system = (raw_cartridge_data[9] & 0x01) ? PAL : NTSC; // Not really used, but we can store it for informational purposes

        // TODO: Implement flag 10, although it's not really used by any known cartridges

        /*  Older versions of the iNES emulator ignored bytes 7-15, and several ROM management tools wrote 
            messages in there. Commonly, these will be filled with "DiskDude!", which results in 64 being 
            added to the mapper number.
            A general rule of thumb: if the last 4 bytes are not all zero, and the header is not marked 
            for NES 2.0 format, an emulator should either mask off the upper 4 bits of the mapper number 
            or simply refuse to load the ROM. */
    }
    prg_rom = new uint8_t[prg_rom_size];
    if(chr_rom_size > 0) {
        chr_rom = new uint8_t[chr_rom_size];
    }
    if(prg_ram_size > 0) {
        prg_ram = new uint8_t[prg_ram_size];
    } else {
        prg_ram = nullptr;
    }
    if(prg_nvram_size > 0) {
        prg_nvram = new uint8_t[prg_nvram_size];
    } else {
        prg_nvram = nullptr;
    }
    if(chr_ram_size > 0) {
        chr_ram = new uint8_t[chr_ram_size];
    } else {
        chr_ram = nullptr;
    }
    if(chr_nvram_size > 0) {
        chr_nvram = new uint8_t[chr_nvram_size];
    } else {
        chr_nvram = nullptr;
    }
    if(has_trainer) {
        trainer_data = new uint8_t[TRAINER_SIZE];
    } else {
        trainer_data = nullptr;
    }
}

void Mapper::read_rom_data(const char *raw_cartridge_data) {
    if(has_trainer) {
        for(uint32_t i = 0; i < TRAINER_SIZE; i++) {
            trainer_data[i] = raw_cartridge_data[16 + i];
        }
    }
    for(uint64_t i = 0, j = (has_trainer ? TRAINER_SIZE : 0); i < prg_rom_size; i++, j++) {
        prg_rom[i] = raw_cartridge_data[16 + j];
    }
    for(uint64_t i = 0, j = (has_trainer ? TRAINER_SIZE : 0); i < chr_rom_size; i++, j++) {
        chr_rom[i] = raw_cartridge_data[16 + prg_rom_size + j];
    }
}