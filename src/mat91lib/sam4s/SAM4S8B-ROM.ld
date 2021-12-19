MEMORY
{
        /* The first 16kB is ROM for the bootloader.  */
        FLASH (rx) : ORIGIN = 0x00400000, LENGTH = 512K
        SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

INCLUDE SAM4SX-ROM.ld
