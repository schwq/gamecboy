#include "../include/cartridge.h"
#include "../include/ram.h"

cartridge_context cartridge;

static const char* rom_type_to_string[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

static const char* lic_code_to_string[0xA5] = {
    [0x00] = "None",
    [0x01] = "Nintendo R&D1",
    [0x08] = "Capcom",
    [0x13] = "Electronic Arts",
    [0x18] = "Hudson Soft",
    [0x19] = "b-ai",
    [0x20] = "kss",
    [0x22] = "pow",
    [0x24] = "PCM Complete",
    [0x25] = "san-x",
    [0x28] = "Kemco Japan",
    [0x29] = "seta",
    [0x30] = "Viacom",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Ocean/Acclaim",
    [0x34] = "Konami",
    [0x35] = "Hector",
    [0x37] = "Taito",
    [0x38] = "Hudson",
    [0x39] = "Banpresto",
    [0x41] = "Ubi Soft",
    [0x42] = "Atlus",
    [0x44] = "Malibu",
    [0x46] = "angel",
    [0x47] = "Bullet-Proof",
    [0x49] = "irem",
    [0x50] = "Absolute",
    [0x51] = "Acclaim",
    [0x52] = "Activision",
    [0x53] = "American sammy",
    [0x54] = "Konami",
    [0x55] = "Hi tech entertainment",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x58] = "Mattel",
    [0x59] = "Milton Bradley",
    [0x60] = "Titus",
    [0x61] = "Virgin",
    [0x64] = "LucasArts",
    [0x67] = "Ocean",
    [0x69] = "Electronic Arts",
    [0x70] = "Infogrames",
    [0x71] = "Interplay",
    [0x72] = "Broderbund",
    [0x73] = "sculptured",
    [0x75] = "sci",
    [0x78] = "THQ",
    [0x79] = "Accolade",
    [0x80] = "misawa",
    [0x83] = "lozc",
    [0x86] = "Tokuma Shoten Intermedia",
    [0x87] = "Tsukuda Original",
    [0x91] = "Chunsoft",
    [0x92] = "Video system",
    [0x93] = "Ocean/Acclaim",
    [0x95] = "Varie",
    [0x96] = "Yonezawa/s’pal",
    [0x97] = "Kaneko",
    [0x99] = "Pack in soft",
    [0xA4] = "Konami (Yu-Gi-Oh!)"};

const char* cart_lic_name()
{
  if (cartridge.header->new_lic_code <= 0xA4) {
    return lic_code_to_string[cartridge.header->lic_code];
  }
  return "unknown_lic_code";
}

const char* cart_type_name()
{
  if (cartridge.header->type <= 0x22) {
    return rom_type_to_string[cartridge.header->type];
  }
  return "unknown_type_name";
}

bool cart_header_checksum()
{
  u8 checksum = 0;
  for (u16 addr = 0x0134; addr <= 0x014C; addr++) {
    checksum = checksum - cartridge.rom_data[addr] - 1;
  }
  return (checksum & 0xFF);
}

/**
 * $00	0	No RAM
   $01	–	Unused  
   $02	8 KiB	1 bank
   $03	32 KiB	4 banks of 8 KiB each
   $04	128 KiB	16 banks of 8 KiB each
   $05	64 KiB	8 banks of 8 KiB each
 */

static bool cart_ram_valid(size_t i)
{
  return (cartridge.header->ram_size == 2 && i == 0) ||
         (cartridge.header->ram_size == 3 && i < 4) ||
         (cartridge.header->ram_size == 4) ||
         (cartridge.header->ram_size == 5 && i < 8);
}

static void cart_create_banks()
{
  for (size_t i = 0; i < 16; i++) {
    cartridge.banks[i] = 0;
    if (cart_ram_valid(i)) {
      cartridge.ram_bank[i] = malloc(0x2000);
      memset(cartridge.ram_bank[i], 0, 0x2000);
    }
  }
  cartridge.ram_bank = cartridge.banks[0];
  cartridge.rom_bankx = cartridge.rom_data + 0x4000;  // bank 1 rom
}

static bool cart_mbc1()
{
  return BETWEEN(cartridge.header->type, 0x01, 0x03);
}

static bool cart_battery()
{
  return cartridge.header->type == 0x03;
}

bool load_rom(const char* file_path)
{
  FILE* fp = fopen(file_path, "r");
  if (!fp) {
    printf("Cannot read game rom! %s\n", file_path);
    return false;
  }

  printf("Opened game rom: %s\n", file_path);
  fseek(fp, 0, SEEK_END);
  cartridge.rom_size = ftell(fp);
  printf("ROM file size: %zu bytes\n", cartridge.rom_size);
  rewind(fp);

  cartridge.rom_data = (u8*)malloc(cartridge.rom_size);
  cartridge.ram_enable = true;

  fclose(fp);

  cartridge.header = (cartridge_header*)(cartridge.rom_data + 0x0100);
  printf("Loaded Cartridge:\n");
  printf("\t Title        : %s\n", cartridge.header->title);
  printf("\t Type:        : %2.2X (%s)\n", cartridge.header->type,
         cart_type_name());
  printf("\t ROM size     : %d kb\n", 32 << cartridge.header->rom_size);
  printf("\t RAM size     : %2.2X\n", cartridge.header->ram_size);
  printf("\t Lic Code     : %2.2X (%s)\n", cartridge.header->lic_code,
         cart_lic_name());
  printf("\t ROM Version  : %2.2X\n", cartridge.header->version);
  printf("\t Checksum     : %2.2X (%s)\n", cartridge.header->checksum,
         cart_header_checksum() ? "PASSED" : "FAILED");

  cart_create_banks();

  cartridge.save = false;
  cartridge.battery = cart_battery();

  if (cartridge.battery)
    cart_load_save();

  return true;
}

void cart_load_save()
{
  if (!cartridge.ram_bank)
    return;

  char filename[1048];
  sprintf(filename, "%s.gb_battery", cartridge.header->title);
  FILE* fp = fopen(filename, "rb");
  if (!fp) {
    logfmt(lwarn, "Cannot open file: %s", filename);
    return;
  }
  fread(cartridge.ram_bank, 0x2000, 1, fp);
  fclose(fp);
}

void cart_write_save()
{
  if (!cartridge.ram_bank)
    return;

  char filename[1048];
  sprintf(filename, "%s.gb_battery", cartridge.header->title);
  FILE* fp = fopen(filename, "rb");
  if (!fp) {
    logfmt(lwarn, "Cannot open file: %s", filename);
    return;
  }
  fwrite(cartridge.ram_bank, 0x2000, 1, fp);
  fclose(fp);
}

u8 cart_read(u16 addr)
{

  if (!cart_mbc1() || addr < 0x4000) {
    return cartridge.rom_data[addr];  // NO MBC
  }

  if ((addr & 0xE000) == 0xA000) {  // BETWEEN(addr, 0xA000, 0xBFFF)
    if (!cartridge.ram_bank)
      return 0xFF;
    if (!cartridge.ram_enable)
      return 0xFF;

    return cartridge.ram_bank[addr - 0xA000];  // From external memory, ram bank
  }

  return cartridge.rom_bankx[addr - 0x4000];  // From the bank mapper to the rom
}

#define AND_EQ(addr, and, eq) (addr & and) == eq

#define LW4BIT_EQ(addr, eq) \
  AND_EQ(addr, 0x000F, eq)  // Lower 4 bits are equal to value

#define LW8BIT_EQ(addr, eq) AND_EQ(addr, 0x00FF, eq)

#define HI4BIT_EQ(addr, eq) AND_EQ(addr, 0xF000, eq)

#define HI8BIT_EQ(addr, eq) AND_EQ(addr, 0xFF00, eq)

void cart_write(u16 addr, u8 value)
{

  if (!cart_mbc1())
    return;  // mbc0 does not write to memory, its read only memory

  if (addr < 0x2000)
    cartridge.ram_enable = LW4BIT_EQ(addr, 0xA);

  if ((addr & 0xE000) == 0x2000) {
    if (value == 0x00)
      value = 0x01;

    value &= 0x1F;  // 5-bit

    cartridge.rom_bank_n = value;
    cartridge.rom_bankx = cartridge.rom_data + (0x4000 * value);
  }

  if ((addr & 0xE000) == 0x4000) {
    cartridge.ram_bank_n = value & 0x03;

    if (cartridge.banking_mode) {
      if (cartridge.save)
        cart_write_save();
      cartridge.ram_bank = cartridge.banks[cartridge.ram_bank_n];
    }
  }

  if ((addr & 0xE000) == 0x6000) {
    cartridge.banking_mode = value & 0x01;

    if (cartridge.banking_mode) {
      if (cartridge.save) {
        cart_write_save();
      }

      cartridge.ram_bank = cartridge.banks[cartridge.ram_bank_n];
    }

    if ((addr & 0xE000) == 0xA000) {
      if (!cartridge.banking_mode)
        return;
      if (!cartridge.ram_enable)
        return;

      cartridge.ram_bank[addr - 0xA000] = value;

      if (cartridge.battery)
        cartridge.save = true;
    }
  }
}