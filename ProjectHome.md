a DSL for describing binary data.

Generate encoder/decoder as standalone library or interpreted at runtime.

# examples #
```
//A TYPE PASCAL STRING
pascalString={len:#8 [#8 (len)]}

char=#8
// A CSTRING
cstring1=[c:char (?c!=0)]
cstring2=[char (?)]

// A CSTRING : IMPLICIT USE OF REGISTERS
cstringshort=[#8(?)]

// OPCODE MAPPING
// * just 2 type functions and 1 enum
modRM(mod,ro,rm)={rm:#3 ro:#3 mod:#2}
sib(ss,sib,base)={base:#3 id:#3 ss:#2}
R32_MAP=<%EAX=0,ECX,EDX,EBX,ESP,EBP,ESI,EDI%>
// * the ADD instruction
ADD={opcode:0x01 operande:modRM(0b11, R32_MAP.ECX, R32_MAP.EAX)}

// THE CONSOLE
console=*(0xb8000) [{color:#8 attr:#8} (80*25)]

// Just a RVA on a file to read/write a previously defined section muf
rva:#32 -> *(rva) muf

// a char[][] in C as the arv parameter of main function
[{ptr_level1:#32 -> *(ptr_level1)[#8(?)] } (?ptr_level1)]

// a link list
myList={content pointer (?pointer != 0 -> *(pointeur)myList)}


```