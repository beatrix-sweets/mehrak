# mehrak
ESP32-based project for driving a 64x64 LED matrix to display Mehrak as part of Kaveh cosplay from Genshin Impact.

# BoM
- [ESP32­WROOM­32](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf&ved=2ahUKEwigvL3qqdmKAxXj0wIHHYoKKtQQFnoECAwQAQ&usg=AOvVaw0mSRlYcJCS3Wtrmh5NsFOT)
- [Waveshare RGB-Matrix-P3-64x64](https://www.waveshare.com/wiki/RGB-Matrix-P3-64x64)
- 16P wire
- Power supply cable
- Power supply terminal adapter
- USB to DC Convert Cable
- Powerbank (providing 5V/3A port)
- external Button

# Quick Start
1. Follow the [Getting Started Guide for ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) to install ESP-IDF v 5.3.2 onto your computer. (eg: git clone -b v5.3.2 --recursive https://github.com/espressif/esp-idf.git esp-idf-v5.3.2 then run install.sh and export.sh)
2. Clone the repo with `git clone --recursive git@github.com:beatrix-sweets/mehrak.git`.
3. Setup esp-idf qith `. ~/esp/esp-idf/export.sh`.
4. Build with `idf.py build`.
5. Connect ESP32 and flash with `idf.py flash`.

## Debugging
You can display the output in the terminal using picom:

```bash
picocom -b 115200 /dev/ttyUSB0
```
exit with Ctrl-A -> Ctrl-X  (sometimes even just Ctrl-X works)

you won't be able to flash while picocom is running
