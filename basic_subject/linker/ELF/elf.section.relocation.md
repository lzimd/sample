# ELF section relocation

## define

```c
typedef struct
{
  Elf64_Addr    r_offset;   /* Address */
  Elf64_Xword   r_info;     /* Relocation type and symbol index */
  Elf64_Sxword  r_addend;   /* Addend */
} Elf64_Rela;
```

## AMD x86-64 relocations

- R_X86_64_PC32, PC 相对引用
- R_X86_64_32, PC 绝对引用

### AMD x86-64 Relocation Types

> 0.99.6 - 2012 <http://refspecs.linuxfoundation.org/elf/x86_64-abi-0.99.pdf>
> 1.0 - 2018 <https://github.com/hjl-tools/x86-psABI/wiki/x86-64-psABI-1.0.pdf>
>
> Table 4.10: Relocation Types

| notations | Represents  |
|:---:|:-----------------------------------------------------------------------------------------------------------------------|
| A   | Represents the addend used to compute the value of the relocatable field.
| B   | Represents the base address at which a shared object has been loaded into mem-ory during execution.
| G   | Represents the offset into the global offset table at which the relocation entry’ssymbol will reside during execution.
| GOT | Represents the address of the global offset table.
| L   | Represents the place (section offset or address) of the Procedure Linkage Tableentry for a symbol.
| P   | Represents the place (section offset or address) of the storage unit being relo-cated (computed usingr_offset).
| S   | Represents the value of the symbol whose index resides in the relocation entry.
| Z   | Represents the size of the symbol whose index resides in the relocation entry.

| Relocation Name       | Value | Field    | Calculation      | desc                                            | additional   |
|:------------------------ |:--:|:---------|:-----------------|:------------------------------------------------|:-------------|
| R_X86_64_NONE            | 0  | none     | none             | No reloc                                        |              |
| R_X86_64_COPY            | 5  | none     | none             | Copy symbol at runtime                          |              |
| R_X86_64_RELATIVE        | 8  | word64   | B + A            | Adjust by program base                          |              |
| R_X86_64_IRELATIVE       | 37 | word64   | indirect (B + A) | Adjust indirectly by program base               |              |
| R_X86_64_GLOB_DAT        | 6  | word64   | S                | Create GOT entry                                |              |
| R_X86_64_JUMP_SLOT       | 7  | word64   | S                | Create PLT entry                                |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_GOT32           | 3  | word32   | G + A            | 32 bit GOT entry                                |              |
| R_X86_64_GOT64           | 27 | word64   | G + A            | 64-bit GOT entry offset                         | Large Models |
| R_X86_64_GOTPCREL        | 9  | word32   | G + GOT + A - P  | 32 bit signed PC relative offset to GOT         |              |
| R_X86_64_GOTPCREL64      | 28 | word64   | G + GOT - P + A  | 64-bit PC relative offset to GOT entry          | Large Models |
| R_X86_64_GOTPLT64        | 30 | word64   | G + A            | like GOT64                                      |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_GOTOFF64        | 25 | word64   | S + A - GOT      | 64 bit offset to GOT                            |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_GOTPC32         | 26 | word32   | GOT + A - P      | 32 bit signed pc relative offset to GOT         |              |
| R_X86_64_GOTPC64         | 29 | word64   | GOT + A - P      | 64-bit PC relative offset to GOT                | Large Models |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_PLTOFF64        | 31 | word64   | L - GOT + A      | 64-bit GOT relative offset to PLT entry         | Large Models |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_PLT32           | 4  | word32   | L + A - P        | 32 bit PLT address                              |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_SIZE32          | 32 | word32   | Z + A            | Size of symbol plus 32-bit addend               |              |
| R_X86_64_SIZE64          | 33 | word64   | Z + A            | Size of symbol plus 64-bit addend               |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_PC8             | 15 | word8    | S + A - P        | 8 bit sign extended pc relative                 |              |
| R_X86_64_PC16            | 13 | word16   | S + A - P        | 16 bit sign extended pc relative                |              |
| R_X86_64_PC32            | 2  | word32   | S + A - P        | PC relative 32 bit signed                       |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_8               | 14 | word8    | S + A            | Direct 8 bit sign extended                      |              |
| R_X86_64_16              | 12 | word16   | S + A            | Direct 16 bit zero extended                     |              |
| R_X86_64_32              | 10 | word32   | S + A            | Direct 32 bit zero extended                     |              |
| R_X86_64_32S             | 11 | word32   | S + A            | Direct 32 bit sign extended                     |              |
| R_X86_64_PC64            | 24 | word64   | S + A - P        | PC relative 64 bit                              |              |
| R_X86_64_64              | 1  | word64   | S + A            | Direct 64 bit                                   |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_TLSGD           | 19 | word32   |                  | 32 bit signed PC relative offset to two GOT entries for GD symbol
| R_X86_64_GOTPC32_TLSDESC | 34 | word32   |                  | GOT offset for TLS descriptor                   |              |
| R_X86_64_TLSDESC_CALL    | 35 | none     |                  | Marker for call through TLS descriptor          |              |
| R_X86_64_GOTTPOFF        | 22 | word32   |                  | 32 bit signed PC relative offset to GOT entry for IE symbol    |
| R_X86_64_TLSDESC         | 36 | word64×2 |                  | TLS descriptor                                  |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_TLSLD           | 20 | word32   |                  | 32 bit signed PC relative offset to two GOT entries for LD symbol
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_DTPMOD64        | 16 | word64   |                  | ID of module containing symbol                  |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_DTPOFF32        | 21 | word32   |                  | Offset in TLS block                             |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_TPOFF32         | 23 | word32   |                  | Offset in initial TLS block                     |              |
| R_X86_64_TPOFF64         | 18 | word64   |                  | Offset in initial TLS block                     |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
| R_X86_64_DTPOFF64        | 17 | word64   |                  | Offset in module's TLS block                    |              |
|          --              | -- |    --    |        --        |                        -                        |      --      |
