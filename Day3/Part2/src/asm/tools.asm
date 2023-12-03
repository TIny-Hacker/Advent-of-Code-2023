    assume adl=1

    section .text

include '../../include/ti84pceg.inc'

    public _getNumber

_getNumber:
    ld iy, ti.flags
    push ix
    ld ix, 0
    add ix, sp
    ld de, (ix + 6) ; pointer to start of number
    pop ix

.loopFind:
    ld a, (de)
    cp a, '0'
    inc de
    jr c, .loopFind
    cp a, ':'
    jr nc, .loopFind
    dec de
    ld bc, 0
    push bc

.loopNumber:
    ld a, (de)
    sub a, $30
    ld hl, 0
    ld l, a
    pop bc
    add hl, bc
    inc de
    ld a, (de)
    cp a, '0'
    ret c
    cp a, ':'
    ret nc
    push hl
    pop bc
    add hl, hl
    add hl, hl
    add hl, bc
    add hl, hl
    push hl
    jr .loopNumber
