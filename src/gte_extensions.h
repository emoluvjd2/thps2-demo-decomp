// Game-specific GTE register-transfer helpers not supplied by Psy-Q.
#ifndef THPS2_GTE_EXTENSIONS_H
#define THPS2_GTE_EXTENSIONS_H

#define gte_ZeroTransVector() \
    __asm__ volatile ( \
        "ctc2 $zero, $5\n" \
        "ctc2 $zero, $6\n" \
        "ctc2 $zero, $7")

#define gte_ldir1(value) __asm__ volatile ("mtc2 %0, $9" : : "r"(value))
#define gte_ldir2(value) __asm__ volatile ("mtc2 %0, $10" : : "r"(value))
#define gte_ldir3(value) __asm__ volatile ("mtc2 %0, $11" : : "r"(value))

#define gte_ldlvnl(v) \
    __asm__ volatile ( \
        "lwc2 $25, 0(%0)\n" \
        "lwc2 $26, 4(%0)\n" \
        "lwc2 $27, 8(%0)" \
        : : "r"(v))

#define gte_ldrgb_word(value) __asm__ volatile ("mtc2 %0, $6" : : "r"(value))

#define gte_ldrot0(value) __asm__ volatile ("ctc2 %0, $0" : : "r"(value))
#define gte_ldrot1(value) __asm__ volatile ("ctc2 %0, $1" : : "r"(value))
#define gte_ldrot2(value) __asm__ volatile ("ctc2 %0, $2" : : "r"(value))
#define gte_ldrot3(value) __asm__ volatile ("ctc2 %0, $3" : : "r"(value))
#define gte_ldrot4(value) __asm__ volatile ("ctc2 %0, $4" : : "r"(value))

#define gte_ldvxy0(value) __asm__ volatile ("mtc2 %0, $0" : : "r"(value))
#define gte_ldvz0(value)  __asm__ volatile ("mtc2 %0, $1" : : "r"(value))
#define gte_ldvxy1(value) __asm__ volatile ("mtc2 %0, $2" : : "r"(value))
#define gte_ldvz1(value)  __asm__ volatile ("mtc2 %0, $3" : : "r"(value))
#define gte_ldvxy2(value) __asm__ volatile ("mtc2 %0, $4" : : "r"(value))
#define gte_ldvz2(value)  __asm__ volatile ("mtc2 %0, $5" : : "r"(value))

#endif // THPS2_GTE_EXTENSIONS_H
