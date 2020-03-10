/* gcc.ld - GCC linker script */

/* Use the default bootloader size from the gcc-cfg.ld file */
BTL_SIZE = DEFINED(BTL_SIZE) ? BTL_SIZE : DEFAULT_BTL_SIZE;

/* Do not reserve any space for the bootloader if it lives in the DI page */
BTL_SIZE = DEFINED(GECKO_INFO_PAGE_BTL) ? 0 : BTL_SIZE;
BTL_SIZE = DEFINED(APP_GECKO_INFO_PAGE_BTL) ? 0 : BTL_SIZE;
BTL_SIZE = DEFINED(STA_GECKO_INFO_PAGE_BTL) ? 0 : BTL_SIZE;
BTL_SIZE = DEFINED(LOCAL_STORAGE_GECKO_INFO_PAGE_BTL) ? 0 : BTL_SIZE;

/* Do not reserve any space for the bootloader if NULL_BTL is defined */
BTL_SIZE = DEFINED(NULL_BTL) ? 0 : BTL_SIZE;

  ASSERT ( BTL_SIZE >= 0, "

======================================================================
======== LINKER ERROR: BTL_SIZE must be >= 0
======================================================================
" )

/* Set the default size of the SimEEPROM to 0 bytes */
SIMEEPROM_SIZE = DEFINED(SIMEEPROM_SIZE) ? SIMEEPROM_SIZE : 0;

  ASSERT ( SIMEEPROM_SIZE >= 0 , "

======================================================================
======== LINKER ERROR: SIMEEPROM_SIZE must be >= 0
======================================================================
" )

/* Check if we're using a local storage bootloader, either legacy or Gecko. */
USING_LOCAL_STORAGE_BTL = DEFINED(LOCAL_STORAGE_BTL) ? 1 : 0;
USING_LOCAL_STORAGE_BTL = DEFINED(LOCAL_STORAGE_GECKO_INFO_PAGE_BTL) ? 1 : USING_LOCAL_STORAGE_BTL;

/* Calculate the default size of internal storage area. It is 0 if we aren't  */
/* using a local storage bootloader, otherwise roughly half of the app space. */
DEFAULT_INTERNAL_STORAGE_SIZE = ((MFB_TOP - MFB_BOTTOM + 1) - SIMEEPROM_SIZE - BTL_SIZE)/ 2 + FLASH_PAGE_SIZE;
DEFAULT_INTERNAL_STORAGE_SIZE = USING_LOCAL_STORAGE_BTL ? DEFAULT_INTERNAL_STORAGE_SIZE : 0;

/* Figure out the actual size of the internal storage area. If the user gives */
/* us a size, we use that. If not, we use the default calculated above.       */
INTERNAL_STORAGE_SIZE = DEFINED(INTERNAL_STORAGE_SIZE) ? INTERNAL_STORAGE_SIZE : DEFAULT_INTERNAL_STORAGE_SIZE;

  ASSERT ( INTERNAL_STORAGE_SIZE >= 0, "

======================================================================
======== LINKER ERROR: INTERNAL_STORAGE_SIZE must be >= 0
======================================================================
" )

/* Map out all of the regions we use in our applications */
SECTIONS {

  /* Create an initialized and uninitialize BAT region to support NULL_BTL */
  .bat.noinit MFB_BOTTOM (NOLOAD) :
  {
    KEEP( *(.bat.noinit) );
    . = ALIGN(2);  /* Align each segment to a Flash word boundary */
    PROVIDE(__BAT_NOINIT__end = .);
  }>flash
  .bat.init MFB_BOTTOM :
  {
    KEEP( *(.bat .bat.*) );
    . = ALIGN(2);  /* Align each segment to a Flash word boundary */
    PROVIDE(__BAT_INIT__end = .);
  }>flash

  /* Pick the region for the BAT that was actually used */
  PROVIDE(__BAT__begin = MFB_BOTTOM);
  PROVIDE(__BAT__size = MAX(SIZEOF(.bat.noinit), SIZEOF(.bat.init)));
  PROVIDE(__BAT__end = MFB_BOTTOM + MAX(SIZEOF(.bat.noinit), SIZEOF(.bat.init)));

  /* The location of the AAT should be either right after the bootloader or */
  /* right after the BAT if there is no bootloader. */
  . = MAX((MFB_BOTTOM + BTL_SIZE), .);
  .aat . :
  {
    PROVIDE(__AAT__begin = .);
    KEEP( *(.aat .aat.*) );
    . = ALIGN(2);  /* Align each segment to a Flash word boundary */
    PROVIDE(__AAT__end = .);
  }>flash
  PROVIDE(__AAT__size = SIZEOF(.aat));

  . = ALIGN(512);
  .intvec . :
  {
    PROVIDE(__INTVEC__begin = .);
    KEEP( *(.intvec .intvec.*) );
    . = ALIGN(2);  /* Align each segment to a Flash word boundary */
    PROVIDE(__INTVEC__end = .);
  }>flash
  PROVIDE(__INTVEC__size = SIZEOF(.intvec));

  .vectors . :
  {
    PROVIDE(__VECTORS__begin = .);
    KEEP( *(.vectors .vectors.*) );
    . = ALIGN(2);  /* Align each segment to a Flash word boundary */
    PROVIDE(__VECTORS__end = .);
  }>flash
  PROVIDE(__VECTORS__size = SIZEOF(.vectors));

  .textrw_init . :
  {
    PROVIDE(__TEXTRW_INIT__begin = .);
    * (.textrw_init .textrw_init.*);
    . = ALIGN(2);  /* Align each segment to a Flash word boundary */
    PROVIDE(__TEXTRW_INIT__end = .);
  }>flash
  PROVIDE(__TEXTRW_INIT__size = SIZEOF(.textrw_init));

  .text . :
  {
    KEEP(*(.init))
    KEEP(*(.fini))

    PROVIDE(__TEXT__begin = .);
    * (.text .text.*);
    . = ALIGN(2);  /* Align each segment to a Flash word boundary */
    PROVIDE(__TEXT__end = .);
  }>flash
  PROVIDE(__TEXT__size = SIZEOF(.text));

  .rodata . :
  {
    PROVIDE(__CONST__begin = .);
    * (.rodata .rodata.*);
    . = ALIGN(2);  /* Align each segment to a Flash word boundary */
    PROVIDE(__CONST__end = .);
  }>flash
  PROVIDE(__CONST__size = SIZEOF(.rodata));

  .data_init . :
  {
    PROVIDE(__DATA_INIT__begin = .);
    PROVIDE(__etext = .);

    /* No need to place anything here explicitly */
    /* Just reserve room for .data to relocate here */

    PROVIDE(__DATA_INIT__end = __DATA_INIT__begin + SIZEOF(.data));
  }>flash
  PROVIDE(__DATA_INIT__size = SIZEOF(.data));

  ASSERT ( . < MFB_TOP - SIMEEPROM_SIZE - INTERNAL_STORAGE_SIZE - FLASH_PAGE_SIZE + 1 , "

======================================================================
======== LINKER ERROR: Not enough flash (Application space)
======================================================================
" )

  . = MFB_TOP - SIMEEPROM_SIZE - INTERNAL_STORAGE_SIZE - FLASH_PAGE_SIZE + 1;
  .nvm . (NOLOAD) :
  {
    PROVIDE(__NVM__begin = .);
    * (.nvm .nvm.* NVM NVM.*);
    PROVIDE(__NVM__end = .);
  }>flash
  PROVIDE(__NVM__size = SIZEOF(.nvm));

  ASSERT ( __NVM__size <= FLASH_PAGE_SIZE , "

======================================================================
======== LINKER ERROR: Not enough flash (NVM)
======================================================================
" )

  . = MFB_TOP - SIMEEPROM_SIZE - INTERNAL_STORAGE_SIZE + 1;
  .simee ALIGN(FLASH_PAGE_SIZE) (NOLOAD) :
  {
    PROVIDE(__SIMEE__begin = .);
    KEEP( * (.simee .simee.*) );
    PROVIDE(__SIMEE__end = .);
  }>flash
  PROVIDE(__SIMEE__size = SIZEOF(.simee));

  ASSERT ( __SIMEE__size <= SIMEEPROM_SIZE , "

======================================================================
======== LINKER ERROR: Not enough flash (SIMEE)
======================================================================
" )

  .internal_storage . (NOLOAD) :
  {
    PROVIDE(__INTERNAL_STORAGE__begin = .);
    * (.internal_storage .internal_storage.*);
    PROVIDE(__INTERNAL_STORAGE__end = __INTERNAL_STORAGE__begin + INTERNAL_STORAGE_SIZE);
  }>flash
  PROVIDE(__INTERNAL_STORAGE__size = MAX(SIZEOF(.internal_storage), INTERNAL_STORAGE_SIZE));

  ASSERT ( . <= MFB_TOP + 1 , "

======================================================================
======== LINKER ERROR: Not enough flash (Internal storage region)
======================================================================
" )

  /* --------------------------------------------------------------------*/

  . = FIB_BOTTOM;
  .fat . (NOLOAD) :
  {
    PROVIDE(__FAT__begin = .);
    KEEP ( *(.fat .fat.*) );
    PROVIDE(__FAT__end = .);
  }
  PROVIDE(__FAT__size = SIZEOF(.fat));

  /* --------------------------------------------------------------------*/

  . = RAM_BOTTOM;

  PROVIDE(__EMHEAP_OVERLAY__begin = .);
  .resetinfo RAM_BOTTOM (NOLOAD) :
  {
    PROVIDE(__RESETINFO__begin = .);
    * (.resetinfo .resetinfo.*);
    PROVIDE(__RESETINFO__end = .);
  }>ram
  PROVIDE(__RESETINFO__size = SIZEOF(.resetinfo));

  .emheap RAM_BOTTOM (NOLOAD) :
  {
    PROVIDE(__EMHEAP__begin = .);
    KEEP (* (.emheap .emheap.*) );
    PROVIDE(__EMHEAP__end = .);
  }>ram
  PROVIDE(__EMHEAP__size = SIZEOF(.emheap));

  . = RAM_BOTTOM + MAX(SIZEOF(.resetinfo), SIZEOF(.emheap));
  PROVIDE(__EMHEAP_OVERLAY__end = .);
  PROVIDE(__EMHEAP_OVERLAY__size = MAX(SIZEOF(.resetinfo), SIZEOF(.emheap)));

  .guard_region ALIGN(0x20) (NOLOAD) :
  {
    PROVIDE(__GUARD_REGION__begin = .);
    KEEP ( *(.guard_region .guard_region.*) );
    PROVIDE(__GUARD_REGION__end = .);
  }>ram
  PROVIDE(__GUARD_REGION__size = SIZEOF(.guard_region));

  .cstack ALIGN(8) (NOLOAD):
  {
    PROVIDE(__CSTACK__begin = .);
    KEEP( *(.cstack .cstack.*) );
    PROVIDE(__StackTop = .);
    PROVIDE(__stack = __StackTop);
    PROVIDE(__CSTACK__end = .);
    __StackLimit = __CSTACK__end;
  }>ram
  PROVIDE(__CSTACK__size = SIZEOF(.cstack));

  .textrw (NOLOAD) :
  {
    PROVIDE(__TEXTRW__begin = .);
    * (.textrw .textrw.*);
    PROVIDE(__TEXTRW__end = .);
  }>ram
  PROVIDE(__TEXTRW__size = SIZEOF(.textrw));

  .data . : AT(__DATA_INIT__begin)
  {
    PROVIDE(__data_start__ = .);
    . = ALIGN(4);
    /* preinit data */
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    PROVIDE(__DATA__begin = .);
    * (.data .data.*);
    . = ALIGN(2);  /* Align each segment to a Flash word boundary */
    PROVIDE(__data_end__ = .);
    PROVIDE(__DATA__end = .);
  }>ram
  PROVIDE(__DATA__size = SIZEOF(.data));

  .memp . (NOLOAD) :
  {
    PROVIDE(_smemp = .);
    KEEP( * (.bss.memp*) );
    KEEP( *memp.o(.bss) );
  }>ram
  PROVIDE(_memp_size = SIZEOF(.memp));

  .bss . (NOLOAD) :
  {
    PROVIDE(__BSS__begin = .);
    PROVIDE(__bss_start__ = .);
    * (.bss .bss.*);
    * (COMMON);
    * (.platform .platform.*);
    PROVIDE(__bss_end__ = .);
    PROVIDE(__BSS__end = .);
  }>ram
  PROVIDE(__BSS__size = SIZEOF(.bss));

  .noinit . (NOLOAD) :
  {
    PROVIDE(__NO_INIT__begin = .);
    * (.noinit .noinit.*);
    PROVIDE(__NO_INIT__end = .);
  }>ram
  PROVIDE(__NO_INIT__size = SIZEOF(.noinit));

  APP_RAM . (NOLOAD) :
  {
    PROVIDE(__APP_RAM__begin = .);
    * (APP_RAM APP_RAM.*);
    PROVIDE(__APP_RAM__end = .);
  }>ram
  PROVIDE(__APP_RAM__size = SIZEOF(APP_RAM));

  .heap . (COPY) :
  {
    __HeapBase = .;
    __end__ = .;
    end = __end__;
    _end = __end__;
    KEEP(*(.heap*))
    __HeapLimit = .;
  }>ram

  .debugChannel . (NOLOAD) :
  {
    PROVIDE(__DEBUG_CHANNEL__begin = .);
    * (DEBUG_CHANNEL DEBUG_CHANNEL.*)
    PROVIDE(__DEBUG_CHANNEL__end = .);
  }>ram
  PROVIDE(__DEBUG_CHANNEL__size = SIZEOF(.debugChannel));

  RAM_GUARD = RAM_TOP - UNUSED_RAM_SIZE ;

  ASSERT ( . < RAM_GUARD + 1, "

======================================================================
======== LINKER ERROR: Not enough RAM
======================================================================
" )

  . = RAM_TOP - UNUSED_RAM_SIZE;
  UNRETAINED_RAM (NOLOAD) :
  {
    PROVIDE(__UNRETAINED_RAM__begin = .);
    * (UNRETAINED_RAM UNRETAINED_RAM.*)
    PROVIDE(__UNRETAINED_RAM__end = .);
  }>ram
  PROVIDE(__UNRETAINED_RAM__size = SIZEOF(UNRETAINED_RAM));

  ASSERT ( . < RAM_TOP + 1 , "

======================================================================
======== LINKER ERROR: Not enough RAM (Unretained)
======================================================================
" )

  __RAM__end = .;
  PROVIDE(end = .);
  PROVIDE(_end = .);
}
