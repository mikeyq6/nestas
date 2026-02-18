#pragma once

#include <cstdint>

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

enum RomFormat {
    NES,
    NES2
};

enum ConsoleType {
    NES_FAM,
    NINTENDO_VS,
    PLAYCHOICE_10,
    EXTENDED_CONSOLE
};

enum NametableLayout {
    HORIZONTAL,
    VERTICAL
};

enum CPUPPUTiming {
    RP2C02, // NTFS
    RP2C07, // PAL
    MULTIPLE,
    UA6538 // Dendy
};

enum TVSystem {
    NTSC,
    PAL
};

class Mapper {
public:
    Mapper(const char *raw_cartridge_data);
    virtual ~Mapper();
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t value) = 0;

protected:
    constexpr static auto TRAINER_SIZE = 0x200; // 512 bytes
    RomFormat rom_format;
    NametableLayout nametable_layout;
    CPUPPUTiming cpu_ppu_timing;
    TVSystem tv_system;
    bool has_battery;
    bool has_trainer;
    bool use_alternative_nametables;
    bool uses_chr_ram;
    uint8_t *prg_rom;
    uint64_t prg_rom_size = 0;
    uint8_t *chr_rom;
    uint64_t chr_rom_size = 0;
    uint8_t *prg_ram;
    uint32_t prg_ram_size = 0;
    uint8_t *prg_nvram;
    uint32_t prg_nvram_size = 0;
    uint8_t *chr_ram;
    uint32_t chr_ram_size = 0;
    uint8_t *chr_nvram;
    uint32_t chr_nvram_size = 0;
    uint8_t *trainer_data;
    ConsoleType console_type;
    uint16_t mapper_number = 0;
    uint8_t submapper_number = 0;
    uint8_t ppu_type;
    uint8_t hardware_type;
    uint8_t extended_console_type;
    uint8_t num_miscellaneous_roms = 0;
    uint8_t default_expansion_device;

private:
    void parse_header(const char *raw_cartridge_data);
    void read_rom_data(const char *raw_cartridge_data);
};