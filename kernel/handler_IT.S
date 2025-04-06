.macro HANDLER_INTERRUPT id
    .text
    .globl handler_IT_\id
handler_IT_\id:
    pushl %eax
    pushl %edx
    pushl %ecx
    pushl $\id                 # passer l'ID au handler C
    call handler_en_C
    addl $4, %esp              # nettoyer la pile
    popl %ecx
    popl %edx
    popl %eax
    iret
.endm

# Génération des handlers avec la macro
HANDLER_INTERRUPT 14
HANDLER_INTERRUPT 15
HANDLER_INTERRUPT 16
HANDLER_INTERRUPT 17
HANDLER_INTERRUPT 18
HANDLER_INTERRUPT 19
HANDLER_INTERRUPT 20
HANDLER_INTERRUPT 21
HANDLER_INTERRUPT 22
HANDLER_INTERRUPT 23
HANDLER_INTERRUPT 24
HANDLER_INTERRUPT 25
HANDLER_INTERRUPT 26
HANDLER_INTERRUPT 27
HANDLER_INTERRUPT 28
HANDLER_INTERRUPT 29
HANDLER_INTERRUPT 30
HANDLER_INTERRUPT 31
HANDLER_INTERRUPT 32
HANDLER_INTERRUPT 33
HANDLER_INTERRUPT 34
HANDLER_INTERRUPT 35
HANDLER_INTERRUPT 36
HANDLER_INTERRUPT 37
HANDLER_INTERRUPT 38
HANDLER_INTERRUPT 39
HANDLER_INTERRUPT 40
HANDLER_INTERRUPT 41
HANDLER_INTERRUPT 42
HANDLER_INTERRUPT 43
HANDLER_INTERRUPT 44
HANDLER_INTERRUPT 45
HANDLER_INTERRUPT 46
HANDLER_INTERRUPT 47
HANDLER_INTERRUPT 48
HANDLER_INTERRUPT 49
HANDLER_INTERRUPT 50
HANDLER_INTERRUPT 51
HANDLER_INTERRUPT 52
HANDLER_INTERRUPT 53
HANDLER_INTERRUPT 54
HANDLER_INTERRUPT 55
HANDLER_INTERRUPT 56
HANDLER_INTERRUPT 57
HANDLER_INTERRUPT 58
HANDLER_INTERRUPT 59
HANDLER_INTERRUPT 60

.data
.globl handlers_IT
handlers_IT:
    .long handler_IT_14
    .long handler_IT_15
    .long handler_IT_16
    .long handler_IT_17
    .long handler_IT_18
    .long handler_IT_19
    .long handler_IT_20
    .long handler_IT_21
    .long handler_IT_22
    .long handler_IT_23
    .long handler_IT_24
    .long handler_IT_25
    .long handler_IT_26
    .long handler_IT_27
    .long handler_IT_28
    .long handler_IT_29
    .long handler_IT_30
    .long handler_IT_31
    .long handler_IT_32
    .long handler_IT_33
    .long handler_IT_34
    .long handler_IT_35
    .long handler_IT_36
    .long handler_IT_37
    .long handler_IT_38
    .long handler_IT_39
    .long handler_IT_40
    .long handler_IT_41
    .long handler_IT_42
    .long handler_IT_43
    .long handler_IT_44
    .long handler_IT_45
    .long handler_IT_46
    .long handler_IT_47
    .long handler_IT_48
    .long handler_IT_49
    .long handler_IT_50
    .long handler_IT_51
    .long handler_IT_52
    .long handler_IT_53
    .long handler_IT_54
    .long handler_IT_55
    .long handler_IT_56
    .long handler_IT_57
    .long handler_IT_58
    .long handler_IT_59
    .long handler_IT_60
