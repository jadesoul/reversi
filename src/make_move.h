
#include "common.h"

#define make_move_look_0(next)                                                  \

#define make_move_look_1(next)                                                  \

#define make_move_look_2(next)                                                  \
tmp=0;                                                                          \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    tmp |= pm;                                                                  \
    if (next(pm) & my) {                                                        \
        flip |= tmp;                                                            \
    }                                                                           \
}                                                                               \

#define make_move_look_3(next)                                                  \
tmp=0;                                                                          \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    tmp |= pm;                                                                  \
    pm=next(pm);                                                                \
    if (pm & my) {                                                              \
        flip |= tmp;                                                            \
    } else if (pm & op) {                                                       \
        tmp |= pm;                                                              \
        if (next(pm) & my) {                                                    \
            flip |= tmp;                                                        \
        }                                                                       \
    }                                                                           \
}                                                                               \

#define make_move_look_4(next)                                                  \
tmp=0;                                                                          \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    tmp |= pm;                                                                  \
    pm=next(pm);                                                                \
    if (pm & my) {                                                              \
        flip |= tmp;                                                            \
    } else if (pm & op) {                                                       \
        tmp |= pm;                                                              \
        pm=next(pm);                                                            \
        if (pm & my) {                                                          \
            flip |= tmp;                                                        \
        } else if (pm & op) {                                                   \
            tmp |= pm;                                                          \
            if (next(pm) & my) {                                                \
                flip |= tmp;                                                    \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}                                                                               \

#define make_move_look_5(next)                                                  \
tmp=0;                                                                          \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    tmp |= pm;                                                                  \
    pm=next(pm);                                                                \
    if (pm & my) {                                                              \
        flip |= tmp;                                                            \
    } else if (pm & op) {                                                       \
        tmp |= pm;                                                              \
        pm=next(pm);                                                            \
        if (pm & my) {                                                          \
            flip |= tmp;                                                        \
        } else if (pm & op) {                                                   \
            tmp |= pm;                                                          \
            pm=next(pm);                                                        \
            if (pm & my) {                                                      \
                flip |= tmp;                                                    \
            } else if (pm & op) {                                               \
                tmp |= pm;                                                      \
                if (next(pm) & my) {                                            \
                    flip |= tmp;                                                \
                }                                                               \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}                                                                               \

#define make_move_look_6(next)                                                  \
tmp=0;                                                                          \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    tmp |= pm;                                                                  \
    pm=next(pm);                                                                \
    if (pm & my) {                                                              \
        flip |= tmp;                                                            \
    } else if (pm & op) {                                                       \
        tmp |= pm;                                                              \
        pm=next(pm);                                                            \
        if (pm & my) {                                                          \
            flip |= tmp;                                                        \
        } else if (pm & op) {                                                   \
            tmp |= pm;                                                          \
            pm=next(pm);                                                        \
            if (pm & my) {                                                      \
                flip |= tmp;                                                    \
            } else if (pm & op) {                                               \
                tmp |= pm;                                                      \
                pm=next(pm);                                                    \
                if (pm & my) {                                                  \
                    flip |= tmp;                                                \
                } else if (pm & op) {                                           \
                    tmp |= pm;                                                  \
                    if (next(pm) & my) {                                        \
                        flip |= tmp;                                            \
                    }                                                           \
                }                                                               \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}                                                                               \

#define make_move_look_7(next)                                                  \
tmp=0;                                                                          \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    tmp |= pm;                                                                  \
    pm=next(pm);                                                                \
    if (pm & my) {                                                              \
        flip |= tmp;                                                            \
    } else if (pm & op) {                                                       \
        tmp |= pm;                                                              \
        pm=next(pm);                                                            \
        if (pm & my) {                                                          \
            flip |= tmp;                                                        \
        } else if (pm & op) {                                                   \
            tmp |= pm;                                                          \
            pm=next(pm);                                                        \
            if (pm & my) {                                                      \
                flip |= tmp;                                                    \
            } else if (pm & op) {                                               \
                tmp |= pm;                                                      \
                pm=next(pm);                                                    \
                if (pm & my) {                                                  \
                    flip |= tmp;                                                \
                } else if (pm & op) {                                           \
                    tmp |= pm;                                                  \
                    pm=next(pm);                                                \
                    if (pm & my) {                                              \
                        flip |= tmp;                                            \
                    } else if (pm & op) {                                       \
                        tmp |= pm;                                              \
                        if (next(pm) & my) {                                    \
                            flip |= tmp;                                        \
                        }                                                       \
                    }                                                           \
                }                                                               \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}                                                                               \

ulong make_move_from_pos_11(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 0);
    make_move_look_7(MASK_EAST)
    make_move_look_7(MASK_SOUTH_EAST)
    make_move_look_7(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_0(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_0(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_12(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 1);
    make_move_look_6(MASK_EAST)
    make_move_look_6(MASK_SOUTH_EAST)
    make_move_look_7(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_1(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_0(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_13(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 2);
    make_move_look_5(MASK_EAST)
    make_move_look_5(MASK_SOUTH_EAST)
    make_move_look_7(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_2(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_0(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_14(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 3);
    make_move_look_4(MASK_EAST)
    make_move_look_4(MASK_SOUTH_EAST)
    make_move_look_7(MASK_SOUTH)
    make_move_look_3(MASK_SOUTH_WEST)
    make_move_look_3(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_0(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_15(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 4);
    make_move_look_3(MASK_EAST)
    make_move_look_3(MASK_SOUTH_EAST)
    make_move_look_7(MASK_SOUTH)
    make_move_look_4(MASK_SOUTH_WEST)
    make_move_look_4(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_0(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_16(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 5);
    make_move_look_2(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_7(MASK_SOUTH)
    make_move_look_5(MASK_SOUTH_WEST)
    make_move_look_5(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_0(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_17(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 6);
    make_move_look_1(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_7(MASK_SOUTH)
    make_move_look_6(MASK_SOUTH_WEST)
    make_move_look_6(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_0(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_18(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 7);
    make_move_look_0(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_7(MASK_SOUTH)
    make_move_look_7(MASK_SOUTH_WEST)
    make_move_look_7(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_0(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_21(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 8);
    make_move_look_7(MASK_EAST)
    make_move_look_6(MASK_SOUTH_EAST)
    make_move_look_6(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_0(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_1(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_22(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 9);
    make_move_look_6(MASK_EAST)
    make_move_look_6(MASK_SOUTH_EAST)
    make_move_look_6(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_1(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_1(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_23(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 10);
    make_move_look_5(MASK_EAST)
    make_move_look_5(MASK_SOUTH_EAST)
    make_move_look_6(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_2(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_1(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_24(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 11);
    make_move_look_4(MASK_EAST)
    make_move_look_4(MASK_SOUTH_EAST)
    make_move_look_6(MASK_SOUTH)
    make_move_look_3(MASK_SOUTH_WEST)
    make_move_look_3(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_1(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_25(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 12);
    make_move_look_3(MASK_EAST)
    make_move_look_3(MASK_SOUTH_EAST)
    make_move_look_6(MASK_SOUTH)
    make_move_look_4(MASK_SOUTH_WEST)
    make_move_look_4(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_1(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_26(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 13);
    make_move_look_2(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_6(MASK_SOUTH)
    make_move_look_5(MASK_SOUTH_WEST)
    make_move_look_5(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_1(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_27(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 14);
    make_move_look_1(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_6(MASK_SOUTH)
    make_move_look_6(MASK_SOUTH_WEST)
    make_move_look_6(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_1(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_28(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 15);
    make_move_look_0(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_6(MASK_SOUTH)
    make_move_look_6(MASK_SOUTH_WEST)
    make_move_look_7(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_1(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_31(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 16);
    make_move_look_7(MASK_EAST)
    make_move_look_5(MASK_SOUTH_EAST)
    make_move_look_5(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_0(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_2(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_32(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 17);
    make_move_look_6(MASK_EAST)
    make_move_look_5(MASK_SOUTH_EAST)
    make_move_look_5(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_1(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_2(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_33(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 18);
    make_move_look_5(MASK_EAST)
    make_move_look_5(MASK_SOUTH_EAST)
    make_move_look_5(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_2(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_2(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_34(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 19);
    make_move_look_4(MASK_EAST)
    make_move_look_4(MASK_SOUTH_EAST)
    make_move_look_5(MASK_SOUTH)
    make_move_look_3(MASK_SOUTH_WEST)
    make_move_look_3(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_2(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_35(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 20);
    make_move_look_3(MASK_EAST)
    make_move_look_3(MASK_SOUTH_EAST)
    make_move_look_5(MASK_SOUTH)
    make_move_look_4(MASK_SOUTH_WEST)
    make_move_look_4(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_2(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_36(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 21);
    make_move_look_2(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_5(MASK_SOUTH)
    make_move_look_5(MASK_SOUTH_WEST)
    make_move_look_5(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_2(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_37(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 22);
    make_move_look_1(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_5(MASK_SOUTH)
    make_move_look_5(MASK_SOUTH_WEST)
    make_move_look_6(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_2(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_38(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 23);
    make_move_look_0(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_5(MASK_SOUTH)
    make_move_look_5(MASK_SOUTH_WEST)
    make_move_look_7(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_2(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_41(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 24);
    make_move_look_7(MASK_EAST)
    make_move_look_4(MASK_SOUTH_EAST)
    make_move_look_4(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_0(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_3(MASK_NORTH)
    make_move_look_3(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_42(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 25);
    make_move_look_6(MASK_EAST)
    make_move_look_4(MASK_SOUTH_EAST)
    make_move_look_4(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_1(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_3(MASK_NORTH)
    make_move_look_3(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_43(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 26);
    make_move_look_5(MASK_EAST)
    make_move_look_4(MASK_SOUTH_EAST)
    make_move_look_4(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_2(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_3(MASK_NORTH)
    make_move_look_3(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_44(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 27);
    make_move_look_4(MASK_EAST)
    make_move_look_4(MASK_SOUTH_EAST)
    make_move_look_4(MASK_SOUTH)
    make_move_look_3(MASK_SOUTH_WEST)
    make_move_look_3(MASK_WEST)
    make_move_look_3(MASK_NORTH_WEST)
    make_move_look_3(MASK_NORTH)
    make_move_look_3(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_45(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 28);
    make_move_look_3(MASK_EAST)
    make_move_look_3(MASK_SOUTH_EAST)
    make_move_look_4(MASK_SOUTH)
    make_move_look_4(MASK_SOUTH_WEST)
    make_move_look_4(MASK_WEST)
    make_move_look_3(MASK_NORTH_WEST)
    make_move_look_3(MASK_NORTH)
    make_move_look_3(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_46(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 29);
    make_move_look_2(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_4(MASK_SOUTH)
    make_move_look_4(MASK_SOUTH_WEST)
    make_move_look_5(MASK_WEST)
    make_move_look_3(MASK_NORTH_WEST)
    make_move_look_3(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_47(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 30);
    make_move_look_1(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_4(MASK_SOUTH)
    make_move_look_4(MASK_SOUTH_WEST)
    make_move_look_6(MASK_WEST)
    make_move_look_3(MASK_NORTH_WEST)
    make_move_look_3(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_48(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 31);
    make_move_look_0(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_4(MASK_SOUTH)
    make_move_look_4(MASK_SOUTH_WEST)
    make_move_look_7(MASK_WEST)
    make_move_look_3(MASK_NORTH_WEST)
    make_move_look_3(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_51(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 32);
    make_move_look_7(MASK_EAST)
    make_move_look_3(MASK_SOUTH_EAST)
    make_move_look_3(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_0(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_4(MASK_NORTH)
    make_move_look_4(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_52(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 33);
    make_move_look_6(MASK_EAST)
    make_move_look_3(MASK_SOUTH_EAST)
    make_move_look_3(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_1(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_4(MASK_NORTH)
    make_move_look_4(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_53(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 34);
    make_move_look_5(MASK_EAST)
    make_move_look_3(MASK_SOUTH_EAST)
    make_move_look_3(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_2(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_4(MASK_NORTH)
    make_move_look_4(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_54(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 35);
    make_move_look_4(MASK_EAST)
    make_move_look_3(MASK_SOUTH_EAST)
    make_move_look_3(MASK_SOUTH)
    make_move_look_3(MASK_SOUTH_WEST)
    make_move_look_3(MASK_WEST)
    make_move_look_3(MASK_NORTH_WEST)
    make_move_look_4(MASK_NORTH)
    make_move_look_4(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_55(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 36);
    make_move_look_3(MASK_EAST)
    make_move_look_3(MASK_SOUTH_EAST)
    make_move_look_3(MASK_SOUTH)
    make_move_look_3(MASK_SOUTH_WEST)
    make_move_look_4(MASK_WEST)
    make_move_look_4(MASK_NORTH_WEST)
    make_move_look_4(MASK_NORTH)
    make_move_look_3(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_56(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 37);
    make_move_look_2(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_3(MASK_SOUTH)
    make_move_look_3(MASK_SOUTH_WEST)
    make_move_look_5(MASK_WEST)
    make_move_look_4(MASK_NORTH_WEST)
    make_move_look_4(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_57(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 38);
    make_move_look_1(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_3(MASK_SOUTH)
    make_move_look_3(MASK_SOUTH_WEST)
    make_move_look_6(MASK_WEST)
    make_move_look_4(MASK_NORTH_WEST)
    make_move_look_4(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_58(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 39);
    make_move_look_0(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_3(MASK_SOUTH)
    make_move_look_3(MASK_SOUTH_WEST)
    make_move_look_7(MASK_WEST)
    make_move_look_4(MASK_NORTH_WEST)
    make_move_look_4(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_61(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 40);
    make_move_look_7(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_2(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_0(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_5(MASK_NORTH)
    make_move_look_5(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_62(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 41);
    make_move_look_6(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_2(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_1(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_5(MASK_NORTH)
    make_move_look_5(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_63(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 42);
    make_move_look_5(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_2(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_2(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_5(MASK_NORTH)
    make_move_look_5(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_64(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 43);
    make_move_look_4(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_2(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_3(MASK_WEST)
    make_move_look_3(MASK_NORTH_WEST)
    make_move_look_5(MASK_NORTH)
    make_move_look_4(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_65(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 44);
    make_move_look_3(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_2(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_4(MASK_WEST)
    make_move_look_4(MASK_NORTH_WEST)
    make_move_look_5(MASK_NORTH)
    make_move_look_3(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_66(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 45);
    make_move_look_2(MASK_EAST)
    make_move_look_2(MASK_SOUTH_EAST)
    make_move_look_2(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_5(MASK_WEST)
    make_move_look_5(MASK_NORTH_WEST)
    make_move_look_5(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_67(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 46);
    make_move_look_1(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_2(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_6(MASK_WEST)
    make_move_look_5(MASK_NORTH_WEST)
    make_move_look_5(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_68(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 47);
    make_move_look_0(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_2(MASK_SOUTH)
    make_move_look_2(MASK_SOUTH_WEST)
    make_move_look_7(MASK_WEST)
    make_move_look_5(MASK_NORTH_WEST)
    make_move_look_5(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_71(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 48);
    make_move_look_7(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_1(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_0(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_6(MASK_NORTH)
    make_move_look_6(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_72(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 49);
    make_move_look_6(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_1(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_1(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_6(MASK_NORTH)
    make_move_look_6(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_73(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 50);
    make_move_look_5(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_1(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_2(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_6(MASK_NORTH)
    make_move_look_5(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_74(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 51);
    make_move_look_4(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_1(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_3(MASK_WEST)
    make_move_look_3(MASK_NORTH_WEST)
    make_move_look_6(MASK_NORTH)
    make_move_look_4(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_75(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 52);
    make_move_look_3(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_1(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_4(MASK_WEST)
    make_move_look_4(MASK_NORTH_WEST)
    make_move_look_6(MASK_NORTH)
    make_move_look_3(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_76(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 53);
    make_move_look_2(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_1(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_5(MASK_WEST)
    make_move_look_5(MASK_NORTH_WEST)
    make_move_look_6(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_77(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 54);
    make_move_look_1(MASK_EAST)
    make_move_look_1(MASK_SOUTH_EAST)
    make_move_look_1(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_6(MASK_WEST)
    make_move_look_6(MASK_NORTH_WEST)
    make_move_look_6(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_78(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 55);
    make_move_look_0(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_1(MASK_SOUTH)
    make_move_look_1(MASK_SOUTH_WEST)
    make_move_look_7(MASK_WEST)
    make_move_look_6(MASK_NORTH_WEST)
    make_move_look_6(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_81(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 56);
    make_move_look_7(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_0(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_0(MASK_WEST)
    make_move_look_0(MASK_NORTH_WEST)
    make_move_look_7(MASK_NORTH)
    make_move_look_7(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_82(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 57);
    make_move_look_6(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_0(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_1(MASK_WEST)
    make_move_look_1(MASK_NORTH_WEST)
    make_move_look_7(MASK_NORTH)
    make_move_look_6(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_83(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 58);
    make_move_look_5(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_0(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_2(MASK_WEST)
    make_move_look_2(MASK_NORTH_WEST)
    make_move_look_7(MASK_NORTH)
    make_move_look_5(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_84(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 59);
    make_move_look_4(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_0(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_3(MASK_WEST)
    make_move_look_3(MASK_NORTH_WEST)
    make_move_look_7(MASK_NORTH)
    make_move_look_4(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_85(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 60);
    make_move_look_3(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_0(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_4(MASK_WEST)
    make_move_look_4(MASK_NORTH_WEST)
    make_move_look_7(MASK_NORTH)
    make_move_look_3(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_86(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 61);
    make_move_look_2(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_0(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_5(MASK_WEST)
    make_move_look_5(MASK_NORTH_WEST)
    make_move_look_7(MASK_NORTH)
    make_move_look_2(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_87(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 62);
    make_move_look_1(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_0(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_6(MASK_WEST)
    make_move_look_6(MASK_NORTH_WEST)
    make_move_look_7(MASK_NORTH)
    make_move_look_1(MASK_NORTH_EAST)
    return flip;
}

ulong make_move_from_pos_88(ulong my, ulong op) {
    ulong pm, tmp, flip=0, pmask=(ONE << 63);
    make_move_look_0(MASK_EAST)
    make_move_look_0(MASK_SOUTH_EAST)
    make_move_look_0(MASK_SOUTH)
    make_move_look_0(MASK_SOUTH_WEST)
    make_move_look_7(MASK_WEST)
    make_move_look_7(MASK_NORTH_WEST)
    make_move_look_7(MASK_NORTH)
    make_move_look_0(MASK_NORTH_EAST)
    return flip;
}

typedef ulong (* make_move_from_pos_func)(ulong my, ulong op);

#define MM(x, y) make_move_from_pos_ ## x ## y

make_move_from_pos_func try_make_move[64]={
    MM(1,1), MM(1,2), MM(1,3), MM(1,4), MM(1,5), MM(1,6), MM(1,7), MM(1,8),
    MM(2,1), MM(2,2), MM(2,3), MM(2,4), MM(2,5), MM(2,6), MM(2,7), MM(2,8),
    MM(3,1), MM(3,2), MM(3,3), MM(3,4), MM(3,5), MM(3,6), MM(3,7), MM(3,8),
    MM(4,1), MM(4,2), MM(4,3), MM(4,4), MM(4,5), MM(4,6), MM(4,7), MM(4,8),
    MM(5,1), MM(5,2), MM(5,3), MM(5,4), MM(5,5), MM(5,6), MM(5,7), MM(5,8),
    MM(6,1), MM(6,2), MM(6,3), MM(6,4), MM(6,5), MM(6,6), MM(6,7), MM(6,8),
    MM(7,1), MM(7,2), MM(7,3), MM(7,4), MM(7,5), MM(7,6), MM(7,7), MM(7,8),
    MM(8,1), MM(8,2), MM(8,3), MM(8,4), MM(8,5), MM(8,6), MM(8,7), MM(8,8),
};


