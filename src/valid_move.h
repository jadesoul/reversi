
#include "common.h"

#define valid_move_look_0(next)                                                 \

#define valid_move_look_1(next)                                                 \

#define valid_move_look_2(next)                                                 \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    if (next(pm) & my) {                                                        \
        return 1;                                                               \
    }                                                                           \
}                                                                               \

#define valid_move_look_3(next)                                                 \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    pm=next(pm);                                                                \
    if (pm & my) {                                                              \
        return 1;                                                               \
    } else if (pm & op) {                                                       \
        if (next(pm) & my) {                                                    \
            return 1;                                                           \
        }                                                                       \
    }                                                                           \
}                                                                               \

#define valid_move_look_4(next)                                                 \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    pm=next(pm);                                                                \
    if (pm & my) {                                                              \
        return 1;                                                               \
    } else if (pm & op) {                                                       \
        pm=next(pm);                                                            \
        if (pm & my) {                                                          \
            return 1;                                                           \
        } else if (pm & op) {                                                   \
            if (next(pm) & my) {                                                \
                return 1;                                                       \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}                                                                               \

#define valid_move_look_5(next)                                                 \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    pm=next(pm);                                                                \
    if (pm & my) {                                                              \
        return 1;                                                               \
    } else if (pm & op) {                                                       \
        pm=next(pm);                                                            \
        if (pm & my) {                                                          \
            return 1;                                                           \
        } else if (pm & op) {                                                   \
            pm=next(pm);                                                        \
            if (pm & my) {                                                      \
                return 1;                                                       \
            } else if (pm & op) {                                               \
                if (next(pm) & my) {                                            \
                    return 1;                                                   \
                }                                                               \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}                                                                               \

#define valid_move_look_6(next)                                                 \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    pm=next(pm);                                                                \
    if (pm & my) {                                                              \
        return 1;                                                               \
    } else if (pm & op) {                                                       \
        pm=next(pm);                                                            \
        if (pm & my) {                                                          \
            return 1;                                                           \
        } else if (pm & op) {                                                   \
            pm=next(pm);                                                        \
            if (pm & my) {                                                      \
                return 1;                                                       \
            } else if (pm & op) {                                               \
                pm=next(pm);                                                    \
                if (pm & my) {                                                  \
                    return 1;                                                   \
                } else if (pm & op) {                                           \
                    if (next(pm) & my) {                                        \
                        return 1;                                               \
                    }                                                           \
                }                                                               \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}                                                                               \

#define valid_move_look_7(next)                                                 \
pm=next(pmask);                                                                 \
if (pm & op) {                                                                  \
    pm=next(pm);                                                                \
    if (pm & my) {                                                              \
        return 1;                                                               \
    } else if (pm & op) {                                                       \
        pm=next(pm);                                                            \
        if (pm & my) {                                                          \
            return 1;                                                           \
        } else if (pm & op) {                                                   \
            pm=next(pm);                                                        \
            if (pm & my) {                                                      \
                return 1;                                                       \
            } else if (pm & op) {                                               \
                pm=next(pm);                                                    \
                if (pm & my) {                                                  \
                    return 1;                                                   \
                } else if (pm & op) {                                           \
                    pm=next(pm);                                                \
                    if (pm & my) {                                              \
                        return 1;                                               \
                    } else if (pm & op) {                                       \
                        if (next(pm) & my) {                                    \
                            return 1;                                           \
                        }                                                       \
                    }                                                           \
                }                                                               \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}                                                                               \

int valid_move_from_pos_11(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 0);
    valid_move_look_7(MASK_EAST)
    valid_move_look_7(MASK_SOUTH_EAST)
    valid_move_look_7(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_0(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_0(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_12(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 1);
    valid_move_look_6(MASK_EAST)
    valid_move_look_6(MASK_SOUTH_EAST)
    valid_move_look_7(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_1(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_0(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_13(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 2);
    valid_move_look_5(MASK_EAST)
    valid_move_look_5(MASK_SOUTH_EAST)
    valid_move_look_7(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_2(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_0(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_14(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 3);
    valid_move_look_4(MASK_EAST)
    valid_move_look_4(MASK_SOUTH_EAST)
    valid_move_look_7(MASK_SOUTH)
    valid_move_look_3(MASK_SOUTH_WEST)
    valid_move_look_3(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_0(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_15(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 4);
    valid_move_look_3(MASK_EAST)
    valid_move_look_3(MASK_SOUTH_EAST)
    valid_move_look_7(MASK_SOUTH)
    valid_move_look_4(MASK_SOUTH_WEST)
    valid_move_look_4(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_0(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_16(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 5);
    valid_move_look_2(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_7(MASK_SOUTH)
    valid_move_look_5(MASK_SOUTH_WEST)
    valid_move_look_5(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_0(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_17(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 6);
    valid_move_look_1(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_7(MASK_SOUTH)
    valid_move_look_6(MASK_SOUTH_WEST)
    valid_move_look_6(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_0(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_18(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 7);
    valid_move_look_0(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_7(MASK_SOUTH)
    valid_move_look_7(MASK_SOUTH_WEST)
    valid_move_look_7(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_0(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_21(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 8);
    valid_move_look_7(MASK_EAST)
    valid_move_look_6(MASK_SOUTH_EAST)
    valid_move_look_6(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_0(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_1(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_22(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 9);
    valid_move_look_6(MASK_EAST)
    valid_move_look_6(MASK_SOUTH_EAST)
    valid_move_look_6(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_1(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_1(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_23(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 10);
    valid_move_look_5(MASK_EAST)
    valid_move_look_5(MASK_SOUTH_EAST)
    valid_move_look_6(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_2(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_1(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_24(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 11);
    valid_move_look_4(MASK_EAST)
    valid_move_look_4(MASK_SOUTH_EAST)
    valid_move_look_6(MASK_SOUTH)
    valid_move_look_3(MASK_SOUTH_WEST)
    valid_move_look_3(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_1(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_25(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 12);
    valid_move_look_3(MASK_EAST)
    valid_move_look_3(MASK_SOUTH_EAST)
    valid_move_look_6(MASK_SOUTH)
    valid_move_look_4(MASK_SOUTH_WEST)
    valid_move_look_4(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_1(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_26(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 13);
    valid_move_look_2(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_6(MASK_SOUTH)
    valid_move_look_5(MASK_SOUTH_WEST)
    valid_move_look_5(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_1(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_27(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 14);
    valid_move_look_1(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_6(MASK_SOUTH)
    valid_move_look_6(MASK_SOUTH_WEST)
    valid_move_look_6(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_1(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_28(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 15);
    valid_move_look_0(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_6(MASK_SOUTH)
    valid_move_look_6(MASK_SOUTH_WEST)
    valid_move_look_7(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_1(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_31(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 16);
    valid_move_look_7(MASK_EAST)
    valid_move_look_5(MASK_SOUTH_EAST)
    valid_move_look_5(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_0(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_2(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_32(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 17);
    valid_move_look_6(MASK_EAST)
    valid_move_look_5(MASK_SOUTH_EAST)
    valid_move_look_5(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_1(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_2(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_33(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 18);
    valid_move_look_5(MASK_EAST)
    valid_move_look_5(MASK_SOUTH_EAST)
    valid_move_look_5(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_2(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_2(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_34(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 19);
    valid_move_look_4(MASK_EAST)
    valid_move_look_4(MASK_SOUTH_EAST)
    valid_move_look_5(MASK_SOUTH)
    valid_move_look_3(MASK_SOUTH_WEST)
    valid_move_look_3(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_2(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_35(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 20);
    valid_move_look_3(MASK_EAST)
    valid_move_look_3(MASK_SOUTH_EAST)
    valid_move_look_5(MASK_SOUTH)
    valid_move_look_4(MASK_SOUTH_WEST)
    valid_move_look_4(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_2(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_36(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 21);
    valid_move_look_2(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_5(MASK_SOUTH)
    valid_move_look_5(MASK_SOUTH_WEST)
    valid_move_look_5(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_2(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_37(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 22);
    valid_move_look_1(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_5(MASK_SOUTH)
    valid_move_look_5(MASK_SOUTH_WEST)
    valid_move_look_6(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_2(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_38(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 23);
    valid_move_look_0(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_5(MASK_SOUTH)
    valid_move_look_5(MASK_SOUTH_WEST)
    valid_move_look_7(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_2(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_41(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 24);
    valid_move_look_7(MASK_EAST)
    valid_move_look_4(MASK_SOUTH_EAST)
    valid_move_look_4(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_0(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_3(MASK_NORTH)
    valid_move_look_3(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_42(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 25);
    valid_move_look_6(MASK_EAST)
    valid_move_look_4(MASK_SOUTH_EAST)
    valid_move_look_4(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_1(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_3(MASK_NORTH)
    valid_move_look_3(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_43(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 26);
    valid_move_look_5(MASK_EAST)
    valid_move_look_4(MASK_SOUTH_EAST)
    valid_move_look_4(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_2(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_3(MASK_NORTH)
    valid_move_look_3(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_44(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 27);
    valid_move_look_4(MASK_EAST)
    valid_move_look_4(MASK_SOUTH_EAST)
    valid_move_look_4(MASK_SOUTH)
    valid_move_look_3(MASK_SOUTH_WEST)
    valid_move_look_3(MASK_WEST)
    valid_move_look_3(MASK_NORTH_WEST)
    valid_move_look_3(MASK_NORTH)
    valid_move_look_3(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_45(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 28);
    valid_move_look_3(MASK_EAST)
    valid_move_look_3(MASK_SOUTH_EAST)
    valid_move_look_4(MASK_SOUTH)
    valid_move_look_4(MASK_SOUTH_WEST)
    valid_move_look_4(MASK_WEST)
    valid_move_look_3(MASK_NORTH_WEST)
    valid_move_look_3(MASK_NORTH)
    valid_move_look_3(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_46(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 29);
    valid_move_look_2(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_4(MASK_SOUTH)
    valid_move_look_4(MASK_SOUTH_WEST)
    valid_move_look_5(MASK_WEST)
    valid_move_look_3(MASK_NORTH_WEST)
    valid_move_look_3(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_47(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 30);
    valid_move_look_1(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_4(MASK_SOUTH)
    valid_move_look_4(MASK_SOUTH_WEST)
    valid_move_look_6(MASK_WEST)
    valid_move_look_3(MASK_NORTH_WEST)
    valid_move_look_3(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_48(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 31);
    valid_move_look_0(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_4(MASK_SOUTH)
    valid_move_look_4(MASK_SOUTH_WEST)
    valid_move_look_7(MASK_WEST)
    valid_move_look_3(MASK_NORTH_WEST)
    valid_move_look_3(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_51(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 32);
    valid_move_look_7(MASK_EAST)
    valid_move_look_3(MASK_SOUTH_EAST)
    valid_move_look_3(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_0(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_4(MASK_NORTH)
    valid_move_look_4(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_52(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 33);
    valid_move_look_6(MASK_EAST)
    valid_move_look_3(MASK_SOUTH_EAST)
    valid_move_look_3(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_1(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_4(MASK_NORTH)
    valid_move_look_4(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_53(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 34);
    valid_move_look_5(MASK_EAST)
    valid_move_look_3(MASK_SOUTH_EAST)
    valid_move_look_3(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_2(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_4(MASK_NORTH)
    valid_move_look_4(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_54(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 35);
    valid_move_look_4(MASK_EAST)
    valid_move_look_3(MASK_SOUTH_EAST)
    valid_move_look_3(MASK_SOUTH)
    valid_move_look_3(MASK_SOUTH_WEST)
    valid_move_look_3(MASK_WEST)
    valid_move_look_3(MASK_NORTH_WEST)
    valid_move_look_4(MASK_NORTH)
    valid_move_look_4(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_55(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 36);
    valid_move_look_3(MASK_EAST)
    valid_move_look_3(MASK_SOUTH_EAST)
    valid_move_look_3(MASK_SOUTH)
    valid_move_look_3(MASK_SOUTH_WEST)
    valid_move_look_4(MASK_WEST)
    valid_move_look_4(MASK_NORTH_WEST)
    valid_move_look_4(MASK_NORTH)
    valid_move_look_3(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_56(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 37);
    valid_move_look_2(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_3(MASK_SOUTH)
    valid_move_look_3(MASK_SOUTH_WEST)
    valid_move_look_5(MASK_WEST)
    valid_move_look_4(MASK_NORTH_WEST)
    valid_move_look_4(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_57(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 38);
    valid_move_look_1(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_3(MASK_SOUTH)
    valid_move_look_3(MASK_SOUTH_WEST)
    valid_move_look_6(MASK_WEST)
    valid_move_look_4(MASK_NORTH_WEST)
    valid_move_look_4(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_58(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 39);
    valid_move_look_0(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_3(MASK_SOUTH)
    valid_move_look_3(MASK_SOUTH_WEST)
    valid_move_look_7(MASK_WEST)
    valid_move_look_4(MASK_NORTH_WEST)
    valid_move_look_4(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_61(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 40);
    valid_move_look_7(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_2(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_0(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_5(MASK_NORTH)
    valid_move_look_5(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_62(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 41);
    valid_move_look_6(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_2(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_1(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_5(MASK_NORTH)
    valid_move_look_5(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_63(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 42);
    valid_move_look_5(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_2(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_2(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_5(MASK_NORTH)
    valid_move_look_5(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_64(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 43);
    valid_move_look_4(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_2(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_3(MASK_WEST)
    valid_move_look_3(MASK_NORTH_WEST)
    valid_move_look_5(MASK_NORTH)
    valid_move_look_4(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_65(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 44);
    valid_move_look_3(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_2(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_4(MASK_WEST)
    valid_move_look_4(MASK_NORTH_WEST)
    valid_move_look_5(MASK_NORTH)
    valid_move_look_3(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_66(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 45);
    valid_move_look_2(MASK_EAST)
    valid_move_look_2(MASK_SOUTH_EAST)
    valid_move_look_2(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_5(MASK_WEST)
    valid_move_look_5(MASK_NORTH_WEST)
    valid_move_look_5(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_67(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 46);
    valid_move_look_1(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_2(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_6(MASK_WEST)
    valid_move_look_5(MASK_NORTH_WEST)
    valid_move_look_5(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_68(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 47);
    valid_move_look_0(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_2(MASK_SOUTH)
    valid_move_look_2(MASK_SOUTH_WEST)
    valid_move_look_7(MASK_WEST)
    valid_move_look_5(MASK_NORTH_WEST)
    valid_move_look_5(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_71(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 48);
    valid_move_look_7(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_1(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_0(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_6(MASK_NORTH)
    valid_move_look_6(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_72(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 49);
    valid_move_look_6(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_1(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_1(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_6(MASK_NORTH)
    valid_move_look_6(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_73(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 50);
    valid_move_look_5(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_1(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_2(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_6(MASK_NORTH)
    valid_move_look_5(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_74(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 51);
    valid_move_look_4(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_1(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_3(MASK_WEST)
    valid_move_look_3(MASK_NORTH_WEST)
    valid_move_look_6(MASK_NORTH)
    valid_move_look_4(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_75(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 52);
    valid_move_look_3(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_1(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_4(MASK_WEST)
    valid_move_look_4(MASK_NORTH_WEST)
    valid_move_look_6(MASK_NORTH)
    valid_move_look_3(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_76(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 53);
    valid_move_look_2(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_1(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_5(MASK_WEST)
    valid_move_look_5(MASK_NORTH_WEST)
    valid_move_look_6(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_77(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 54);
    valid_move_look_1(MASK_EAST)
    valid_move_look_1(MASK_SOUTH_EAST)
    valid_move_look_1(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_6(MASK_WEST)
    valid_move_look_6(MASK_NORTH_WEST)
    valid_move_look_6(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_78(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 55);
    valid_move_look_0(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_1(MASK_SOUTH)
    valid_move_look_1(MASK_SOUTH_WEST)
    valid_move_look_7(MASK_WEST)
    valid_move_look_6(MASK_NORTH_WEST)
    valid_move_look_6(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_81(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 56);
    valid_move_look_7(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_0(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_0(MASK_WEST)
    valid_move_look_0(MASK_NORTH_WEST)
    valid_move_look_7(MASK_NORTH)
    valid_move_look_7(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_82(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 57);
    valid_move_look_6(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_0(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_1(MASK_WEST)
    valid_move_look_1(MASK_NORTH_WEST)
    valid_move_look_7(MASK_NORTH)
    valid_move_look_6(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_83(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 58);
    valid_move_look_5(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_0(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_2(MASK_WEST)
    valid_move_look_2(MASK_NORTH_WEST)
    valid_move_look_7(MASK_NORTH)
    valid_move_look_5(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_84(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 59);
    valid_move_look_4(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_0(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_3(MASK_WEST)
    valid_move_look_3(MASK_NORTH_WEST)
    valid_move_look_7(MASK_NORTH)
    valid_move_look_4(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_85(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 60);
    valid_move_look_3(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_0(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_4(MASK_WEST)
    valid_move_look_4(MASK_NORTH_WEST)
    valid_move_look_7(MASK_NORTH)
    valid_move_look_3(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_86(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 61);
    valid_move_look_2(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_0(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_5(MASK_WEST)
    valid_move_look_5(MASK_NORTH_WEST)
    valid_move_look_7(MASK_NORTH)
    valid_move_look_2(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_87(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 62);
    valid_move_look_1(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_0(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_6(MASK_WEST)
    valid_move_look_6(MASK_NORTH_WEST)
    valid_move_look_7(MASK_NORTH)
    valid_move_look_1(MASK_NORTH_EAST)
    return 0;
}

int valid_move_from_pos_88(ulong my, ulong op) {
    ulong pm, pmask=(ONE << 63);
    valid_move_look_0(MASK_EAST)
    valid_move_look_0(MASK_SOUTH_EAST)
    valid_move_look_0(MASK_SOUTH)
    valid_move_look_0(MASK_SOUTH_WEST)
    valid_move_look_7(MASK_WEST)
    valid_move_look_7(MASK_NORTH_WEST)
    valid_move_look_7(MASK_NORTH)
    valid_move_look_0(MASK_NORTH_EAST)
    return 0;
}

typedef int (*valid_move_from_pos_func)(ulong my, ulong op);

#define VM(x, y) valid_move_from_pos_ ## x ## y

valid_move_from_pos_func check_valid_move[64]={
    VM(1,1), VM(1,2), VM(1,3), VM(1,4), VM(1,5), VM(1,6), VM(1,7), VM(1,8),
    VM(2,1), VM(2,2), VM(2,3), VM(2,4), VM(2,5), VM(2,6), VM(2,7), VM(2,8),
    VM(3,1), VM(3,2), VM(3,3), VM(3,4), VM(3,5), VM(3,6), VM(3,7), VM(3,8),
    VM(4,1), VM(4,2), VM(4,3), VM(4,4), VM(4,5), VM(4,6), VM(4,7), VM(4,8),
    VM(5,1), VM(5,2), VM(5,3), VM(5,4), VM(5,5), VM(5,6), VM(5,7), VM(5,8),
    VM(6,1), VM(6,2), VM(6,3), VM(6,4), VM(6,5), VM(6,6), VM(6,7), VM(6,8),
    VM(7,1), VM(7,2), VM(7,3), VM(7,4), VM(7,5), VM(7,6), VM(7,7), VM(7,8),
    VM(8,1), VM(8,2), VM(8,3), VM(8,4), VM(8,5), VM(8,6), VM(8,7), VM(8,8),
};

//test if we can make move at pos
#define my_valid_move(pos)        (is_empty(pos) && check_valid_move[pos](my, op))//TODO: no check empty
#define op_valid_move(pos)        (is_empty(pos) && check_valid_move[pos](op, my))
#define valid_move(pos)            my_valid_move(pos)


