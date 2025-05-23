#define BYTE_AT(adr) (*(unsigned char volatile *) adr)

#define CNSTAT BYTE_AT(0xFFFFFFCA)
#define CNDATA BYTE_AT(0xFFFFFFCB)

#define CNCAS   0x50            //CAS MODE
#define CNDISC  0x51            //DISC MODE
#define CNFAT   0x52            //FAT MODE
#define CNRESET 0x53            //RESET

#define CNDIR   0x20            //DIR
#define CNCD    0x21            //CHANGE DIR
#define CNMKDIR 0x22            //MAKE DIR
#define CNRM    0x23            //REMOVE
#define CNOPEN  0x24            //OPEN FILE
#define CNCLOSE 0x25            //CLOSE FILE
#define CNCREAT 0x26            //CREATE FILE
#define CNBLOAD 0x29            //BINARY LOAD
#define CNBSAVE 0x2A            //BINARY SAVE
#define CNTLOAD 0x2B            //TEXT LOAD
#define CNTSAVE 0x2C            //TEXT SAVE
#define CNEXIST 0x2D            //FILE EXISTS
#define CNTEST  0x2E            //TEST MODE


// Start
void _start(void)
{
    asm("pea main(%pc)\n\t");
}


#include "includes/gp_text.c"

// Print a string.
void printString(const char *s)
{
    while (*s != 0)
    {
        GP_CO2(*s);
        s++;
    }
}

/* liest vom CAS-neo DATA register */
char casRd(void)
{
    while (!(CNSTAT & 1));
    return CNDATA;
}

/* schreibt ins CAS-neo DATA register */
void casWd(char data)
{
    while (!(CNSTAT &2));
    CNDATA = data;
}

/* initialisiert die  CAS-neo im FAT mode */
void initCas(void)
{
    char temp;
    CNSTAT = CNRESET;
    CNSTAT = CNFAT;
    if (casRd() != 0x39)
        printString("CAS-neo init error\r\n");
}

/* gibt den Verzeichnisinhalt einer SD-Karte im FAT-Modus der CAS-neo aus */
void casDir(void)
{
    char character;
    casWd(CNDIR);
    character = casRd();
    while (character != 0)
    {
        GP_CO2(character);
        character = casRd();
    }
}


int main(void)
{
    /* CAS-neo initialisieren und Verzeichnis ausgeben */
    initCas();
    casDir();
}
