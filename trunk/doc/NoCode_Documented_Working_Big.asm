BITS 32

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; MZ (DOS) header                          ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
;	 WORD   e_magic;                     // Magic number
;    WORD   e_cblp;                      // Bytes on last page of file
;    WORD   e_cp;                        // Pages in file
;    WORD   e_crlc;                      // Relocations
;    WORD   e_cparhdr;                   // Size of header in paragraphs
;    WORD   e_minalloc;                  // Minimum extra paragraphs needed
;    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
;    WORD   e_ss;                        // Initial (relative) SS value
;    WORD   e_sp;                        // Initial SP value
;    WORD   e_csum;                      // Checksum
;    WORD   e_ip;                        // Initial IP value
;    WORD   e_cs;                        // Initial (relative) CS value
;    WORD   e_lfarlc;                    // File address of relocation table
;    WORD   e_ovno;                      // Overlay number
;    WORD   e_res[4];                    // Reserved words
;    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
;    WORD   e_oeminfo;                   // OEM information; e_oemid specific
;    WORD   e_res2[10];                  // Reserved words
;    LONG   e_lfanew;                    // File address of new exe header
;  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
image_start:
db 0x4d, 0x5a			;"MZ"
times 58 db 0			; Tout a zero jusqu'a e_lfanew
db 0x40, 0, 0, 0		; e_lfanew => offset vers signature PE

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; PE header                                ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;typedef struct _IMAGE_NT_HEADERS {
;    DWORD Signature;
;    IMAGE_FILE_HEADER FileHeader;
;    IMAGE_OPTIONAL_HEADER32 OptionalHeader;
;} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;typedef struct _IMAGE_FILE_HEADER {
;    WORD    Machine;
;    WORD    NumberOfSections;
;    DWORD   TimeDateStamp;
;    DWORD   PointerToSymbolTable;
;    DWORD   NumberOfSymbols;
;    WORD    SizeOfOptionalHeader;
;    WORD    Characteristics;
;} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
db 0x50, 0x45, 0, 0					; "PE"
db 0x4c, 0x01						; Machine (i386)
db 0x02, 0							; Nombre de sections
times 12 db 0						; TimeDateStamp PointerToSymbolTable NumberOfSymbols
dw optional_header_size				; SizeOfOptionalHeader (etait db 0xe0, 0)
db 0x02, 0x01						; Characteristics (Executable 32 bits)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; PE optionnal header                      ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;typedef struct _IMAGE_OPTIONAL_HEADER {
;    //
;    // Standard fields.
;    //
;
;    WORD    Magic;
;    BYTE    MajorLinkerVersion;
;    BYTE    MinorLinkerVersion;
;    DWORD   SizeOfCode;
;    DWORD   SizeOfInitializedData;
;    DWORD   SizeOfUninitializedData;
;    DWORD   AddressOfEntryPoint;
;    DWORD   BaseOfCode;
;    DWORD   BaseOfData;
;
;    //
;    // NT additional fields.
;    //
;
;    DWORD   ImageBase;
;    DWORD   SectionAlignment;
;    DWORD   FileAlignment;
;    WORD    MajorOperatingSystemVersion;
;    WORD    MinorOperatingSystemVersion;
;    WORD    MajorImageVersion;
;    WORD    MinorImageVersion;
;    WORD    MajorSubsystemVersion;
;    WORD    MinorSubsystemVersion;
;    DWORD   Win32VersionValue;
;    DWORD   SizeOfImage;
;    DWORD   SizeOfHeaders;
;    DWORD   CheckSum;
;    WORD    Subsystem;
;    WORD    DllCharacteristics;
;    DWORD   SizeOfStackReserve;
;    DWORD   SizeOfStackCommit;
;    DWORD   SizeOfHeapReserve;
;    DWORD   SizeOfHeapCommit;
;    DWORD   LoaderFlags;
;    DWORD   NumberOfRvaAndSizes;
;    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
;} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;typedef struct _IMAGE_DATA_DIRECTORY {
;    DWORD   VirtualAddress;
;    DWORD   Size;
;} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;
;
;#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
optional_header:
db 0x0b, 0x01		; Magic
db 0				; MajorLinkerVersion
db 0				; MinorLinkerVersion
dd code_size		; db 0x20, 0, 0, 0	; SizeOfCode en dur
dd data_size		; db 0xa0, 0, 0, 0	; SizeOfInitializedData encore en dur
times 4 db 0		; SizeOfUninitializedData
dd code_start		; db 0xa0, 0x01, 0, 0	; AddressOfEntryPoint
dd code_start		; db 0xa0, 0x01, 0, 0 ; BaseOfCode, le meme (RVA)
dd data_start		; db 0xc0, 0x01, 0, 0 ; BaseOfData (RVA)
dd image_base		; db 0, 0, 0x10, 0	; ImageBase 1Meg a l'arrache
db 0x20, 0x00, 0, 0	; SectionAlignment sur 32, parce que j'en ai envie ^^
db 0x20, 0x00, 0, 0	; FileAlignment sur 32, parce que quand meme...
db 0x04, 0			; MajorOperatingSystemVersion
db 0, 0				; MinorOperatingSystemVersion
db 0, 0				; MajorImageVersion
db 0, 0				; MinorImageVersion
db 4, 0				; MajorSubsystemVersion
db 0, 0				; MinorSubsystemVersion
times 4 db 0		; Win32VersionValue
dd image_size		; db 0x60, 0x02, 0, 0	; SizeOfImage (la belle taille de toutes les sections en dur)
dd headers_size		; db 0xa0, 0x01, 0, 0	; SizeOfHeaders (offset vers la premiere section... bin ouai encore en dur)
times 4 db 0		; CheckSum
db 0x02, 0			; Subsystem (GUI)
db 0, 0				; DllCharacteristics (spa une dll... osef)
db 0, 0, 0x10, 0	; SizeOfStackReserve
db 0, 0x10, 0, 0	; SizeOfStackCommit
db 0, 0, 0x10, 0	; SizeOfHeapReserve
db 0, 0x10, 0, 0	; SizeOfHeapCommit
times 4 db 0		; LoaderFlags
db 0x10, 0, 0, 0	; NumberOfRvaAndSizes

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Data directories                         ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 0  - Export
; 1  - Import
; 2  - Resources 
; 3  - Exceptions
; 4  - Certificates
; 5  - Relocalisations
; 6  - Debug info 
; 7  - Copyright
; 8  - Global pointer [for 64 bits]
; 9  - Thread local storage
; 10 - Load configuration
; 11 - Bound import
; 12 - Import adresses
; 13 - Delay Import
; 14 - COM
; 15 - ?
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
times 8 db 0

db 0xe0, 0x01, 0, 0	; Import
db 0x6f, 0, 0, 0	; Import size

times 112 db 0		; Pas besoin du reste

optional_header_size equ $ - optional_header

;Section header (code)
db ".code", 0, 0, 0
times 4 db 0				;Virtual size
dd code_start				;db 0xa0, 0x01, 0, 0 start of code section (RVA)
dd code_size				;db 0x20, 0, 0, 0 taille du code
dd code_start				;db 0xa0, 0x01, 0, 0 offset vers section de code dans le fichier
times 8 db 0				;PointerToRelocations PointerToLinenumbers 
times 4 db 0				;NumberOfRelocations NumberOfLinenumbers

db 0x20, 0, 0, 0x60			;Caracteristiques code, read, exec

;Section header (data)
db ".data", 0, 0, 0
times 4 db 0				;Virtual size
dd data_start				;db 0xc0, 0x01, 0, 0		;Rva debut section data
dd data_size				;db 0xa0, 0, 0, 0			;Taille section data
dd data_start				;db 0xc0, 0x01, 0, 0		;Offset reel dans le fichier
times 8 db 0				;PointerToRelocations PointerToLinenumbers 
times 4 db 0				;NumberOfRelocations NumberOfLinenumbers
db 0x40, 0, 0, 0xc0			;Carac initialized, read, write

;padding
times 24 db 0

;Code section
headers_size equ $ - image_start
code_start:
db 0x6a			; push 30 (MB_OK | MB_ICONEXCLAMATION)
db 0x30

db 0x68
dd image_base + data_start				;push titre

db 0x68
dd image_base + data_start				;push message

db 0x6A									;push 0
db 0x0

db 0x2e
db 0xff
db 0x15
dd image_base + 0x228					;call MessageBoxA

db 0xc3									;ret
times 10 db 0
code_size equ $ - code_start

;Data section
data_start:
db "Hello Dudes!"
times 4 db 0
times 4 db 0

times 12 db 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 1 - Import Table
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; typedef struct _IMAGE_IMPORT_DESCRIPTOR {
;    union {
;        DWORD   Characteristics;            // 0 for terminating null import descriptor
;        DWORD   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
;    };
;    DWORD   TimeDateStamp;                  // 0 if not bound,
;                                            // -1 if bound, and real date\time stamp
;                                            //     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
;                                            // O.W. date/time stamp of DLL bound to (Old BIND)
;
;    DWORD   ForwarderChain;                 // -1 if no forwarders
;    DWORD   Name;
;    DWORD   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
;} IMAGE_IMPORT_DESCRIPTOR;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
db 0x18, 0x02, 0, 0		; OriginalFirstThunk (RVA iat)
times 4 db 0			; TimeDateStamp
times 4 db 0xff			; ForwarderChain
db 0x08, 0x02, 0, 0		; Dll name (RVA) => Sur user32.dll pour nous
db 0x24, 0x02, 0, 0		; Import Adress Table RVA

times 4 db 0
times 4 db 0
times 4 db 0
times 4 db 0
times 4 db 0

db "user32.dll", 0
times 5 db 0			; padding

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; typedef struct _IMAGE_THUNK_DATA32 {
;    union {
;        DWORD ForwarderString;      // PBYTE 
;        DWORD Function;             // PDWORD
;        DWORD Ordinal;
;        DWORD AddressOfData;        // PIMAGE_IMPORT_BY_NAME
;    } u1;
; } IMAGE_THUNK_DATA32;
; IAT
db 0x30, 0x02, 0, 0		; RVA "MessageBoxA"
db 0x40, 0x02, 0, 0		; RVA "MessageBoxA"
times 4 db 0			; fin

db 0x30, 0x02, 0, 0		; RVA "MessageBoxA"
db 0x40, 0x02, 0, 0		; RVA "MessageBoxA"
times 4 db 0			; fin

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;typedef struct _IMAGE_IMPORT_BY_NAME {
;    WORD    Hint;
;    BYTE    Name[1];
;} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
db 0x01 
db 0
db "MessageBoxA", 0, 0, 0
    
db 0x02
db 0
db "MessageBoxA", 0

times 18 db 0			; padding

data_size equ $ - data_start
image_size equ $ - image_start
image_base equ 0x400000
