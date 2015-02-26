#ifndef BITBOARD_VALID_MOVE_H_
#define BITBOARD_VALID_MOVE_H_
/*
 * File: valid_move.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月25日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "mask.h"

#define VALID_MOVE_0(MASK_CONV, my, op, pmask)

#define VALID_MOVE_1(MASK_CONV, my, op, pmask)

#define VALID_MOVE_2(MASK_CONV, my, op, pmask) 	\
	pm=MASK_CONV(pmask);						\
	if (pm & op) {								\
		pm=MASK_CONV(pm);						\
		if (pm & my) {							\
			return true;						\
		}										\
	}

#define VALID_MOVE_3(MASK_CONV, my, op, pmask) 			\
	pm=MASK_CONV(pmask);								\
	if (pm & op) {										\
		pm=MASK_CONV(pm);								\
		if (pm & my) {									\
			return true;								\
		}												\
		if (pm & op) {									\
			pm=MASK_CONV(pm);							\
			if (pm & my) {								\
				return true;							\
			}											\
		}												\
	}

#define VALID_MOVE_4(MASK_CONV, my, op, pmask) 			\
	pm=MASK_CONV(pmask);								\
	if (pm & op) {										\
		pm=MASK_CONV(pm);								\
		if (pm & my) {									\
			return true;								\
		}												\
		if (pm & op) {									\
			pm=MASK_CONV(pm);							\
			if (pm & my) {								\
				return true;							\
			}											\
			if (pm & op) {								\
				pm=MASK_CONV(pm);						\
				if (pm & my) {							\
					return true;						\
				}										\
			}											\
		}												\
	}

#define VALID_MOVE_5(MASK_CONV, my, op, pmask)		 	\
	pm=MASK_CONV(pmask);								\
	if (pm & op) {										\
		pm=MASK_CONV(pm);								\
		if (pm & my) {									\
			return true;								\
		}												\
		if (pm & op) {									\
			pm=MASK_CONV(pm);							\
			if (pm & my) {								\
				return true;							\
			}											\
			if (pm & op) {								\
				pm=MASK_CONV(pm);						\
				if (pm & my) {							\
					return true;						\
				}										\
				if (pm & op) {							\
					pm=MASK_CONV(pm);					\
					if (pm & my) {						\
						return true;					\
					}									\
				}										\
			}											\
		}												\
	}

#define VALID_MOVE_6(MASK_CONV, my, op, pmask) 			\
	pm=MASK_CONV(pmask);								\
	if (pm & op) {										\
		pm=MASK_CONV(pm);								\
		if (pm & my) {									\
			return true;								\
		}												\
		if (pm & op) {									\
			pm=MASK_CONV(pm);							\
			if (pm & my) {								\
				return true;							\
			}											\
			if (pm & op) {								\
				pm=MASK_CONV(pm);						\
				if (pm & my) {							\
					return true;						\
				}										\
				if (pm & op) {							\
					pm=MASK_CONV(pm);					\
					if (pm & my) {						\
						return true;					\
					}									\
					if (pm & op) {						\
						pm=MASK_CONV(pm);				\
						if (pm & my) {					\
							return true;				\
						}								\
					}									\
				}										\
			}											\
		}												\
	}

#define VALID_MOVE_7(MASK_CONV, my, op, pmask) 			\
	pm=MASK_CONV(pmask);								\
	if (pm & op) {										\
		pm=MASK_CONV(pm);								\
		if (pm & my) {									\
			return true;								\
		}												\
		if (pm & op) {									\
			pm=MASK_CONV(pm);							\
			if (pm & my) {								\
				return true;							\
			}											\
			if (pm & op) {								\
				pm=MASK_CONV(pm);						\
				if (pm & my) {							\
					return true;						\
				}										\
				if (pm & op) {							\
					pm=MASK_CONV(pm);					\
					if (pm & my) {						\
						return true;					\
					}									\
					if (pm & op) {						\
						pm=MASK_CONV(pm);				\
						if (pm & my) {					\
							return true;				\
						}								\
						if (pm & op) {					\
							pm=MASK_CONV(pm);			\
							if (pm & my) {				\
								return true;			\
							}							\
						}								\
					}									\
				}										\
			}											\
		}												\
	}

//#define VALID_MOVE_EAST_2(my, op, pmask)	VALID_MOVE_2(MASK_EAST, my, op, pmask)
//#define VALID_MOVE_WEST_2(my, op, pmask)	VALID_MOVE_2(MASK_WEST, my, op, pmask)
//#define VALID_MOVE_NORTH_2(my, op, pmask)	VALID_MOVE_2(MASK_NORTH, my, op, pmask)
//#define VALID_MOVE_SOUTH_2(my, op, pmask)	VALID_MOVE_2(MASK_SOUTH, my, op, pmask)
//
//#define VALID_MOVE_NORTH_EAST_2(my, op, pmask)	VALID_MOVE_2(MASK_NORTH_EAST, my, op, pmask)
//#define VALID_MOVE_NORTH_WEST_2(my, op, pmask)	VALID_MOVE_2(MASK_NORTH_WEST, my, op, pmask)
//#define VALID_MOVE_SOUTH_EAST_2(my, op, pmask)	VALID_MOVE_2(MASK_SOUTH_EAST, my, op, pmask)
//#define VALID_MOVE_SOUTH_WEST_2(my, op, pmask)	VALID_MOVE_2(MASK_SOUTH_WEST, my, op, pmask)

#define VALID_MOVE_GRID_COL_1(my, op, pmask) \
	VALID_MOVE_7(MASK_EAST, my, op, pmask); \
	VALID_MOVE_0(MASK_WEST, my, op, pmask); \

#define VALID_MOVE_GRID_COL_2(my, op, pmask) \
	VALID_MOVE_6(MASK_EAST, my, op, pmask); \
	VALID_MOVE_1(MASK_WEST, my, op, pmask); \

#define VALID_MOVE_GRID_COL_3(my, op, pmask) \
	VALID_MOVE_5(MASK_EAST, my, op, pmask); \
	VALID_MOVE_2(MASK_WEST, my, op, pmask); \

#define VALID_MOVE_GRID_COL_4(my, op, pmask) \
	VALID_MOVE_4(MASK_EAST, my, op, pmask); \
	VALID_MOVE_3(MASK_WEST, my, op, pmask); \

#define VALID_MOVE_GRID_COL_5(my, op, pmask) \
	VALID_MOVE_3(MASK_EAST, my, op, pmask); \
	VALID_MOVE_4(MASK_WEST, my, op, pmask); \

#define VALID_MOVE_GRID_COL_6(my, op, pmask) \
	VALID_MOVE_2(MASK_EAST, my, op, pmask); \
	VALID_MOVE_5(MASK_WEST, my, op, pmask); \

#define VALID_MOVE_GRID_COL_7(my, op, pmask) \
	VALID_MOVE_1(MASK_EAST, my, op, pmask); \
	VALID_MOVE_6(MASK_WEST, my, op, pmask); \

#define VALID_MOVE_GRID_COL_8(my, op, pmask) \
	VALID_MOVE_0(MASK_EAST, my, op, pmask); \
	VALID_MOVE_7(MASK_WEST, my, op, pmask); \


bool vm_A1(ulong my, ulong op, mask_t pmask) {
	mask_t pm;
	VALID_MOVE_7(MASK_EAST, my, op, pmask);
	VALID_MOVE_0(MASK_WEST, my, op, pmask);

	VALID_MOVE_7(MASK_SOUTH, my, op, pmask);
	VALID_MOVE_7(MASK_SOUTH_EAST, my, op, pmask);
	return false;
}

bool vm_B1(ulong my, ulong op, mask_t pmask) {
	mask_t pm;
	VALID_MOVE_6(MASK_EAST, my, op, pmask);
	VALID_MOVE_7(MASK_SOUTH, my, op, pmask);
	VALID_MOVE_6(MASK_SOUTH_EAST, my, op, pmask);
	return false;
}

bool vm_C1(ulong my, ulong op, mask_t pmask) {
	mask_t pm;
	VALID_MOVE_5(MASK_EAST, my, op, pmask);
	VALID_MOVE_7(MASK_SOUTH, my, op, pmask);
	VALID_MOVE_5(MASK_SOUTH_EAST, my, op, pmask);
	VALID_MOVE_2(MASK_WEST, my, op, pmask);
	VALID_MOVE_2(MASK_SOUTH_WEST, my, op, pmask);
	return false;
}

bool vm_D1(ulong my, ulong op, mask_t pmask) {
	mask_t pm;
	VALID_MOVE_4(MASK_EAST, my, op, pmask);
	VALID_MOVE_7(MASK_SOUTH, my, op, pmask);
	VALID_MOVE_4(MASK_SOUTH_EAST, my, op, pmask);
	VALID_MOVE_3(MASK_WEST, my, op, pmask);
	VALID_MOVE_3(MASK_SOUTH_WEST, my, op, pmask);
	return false;
}

bool vm_E1(ulong my, ulong op, mask_t pmask) {
	mask_t pm;
	VALID_MOVE_3(MASK_EAST, my, op, pmask);
	VALID_MOVE_7(MASK_SOUTH, my, op, pmask);
	VALID_MOVE_3(MASK_SOUTH_EAST, my, op, pmask);
	VALID_MOVE_4(MASK_WEST, my, op, pmask);
	VALID_MOVE_4(MASK_SOUTH_WEST, my, op, pmask);
	return false;
}

bool vm_F1(ulong my, ulong op, mask_t pmask) {
	mask_t pm;
	VALID_MOVE_2(MASK_EAST, my, op, pmask);
	VALID_MOVE_5(MASK_WEST, my, op, pmask);

	VALID_MOVE_7(MASK_SOUTH, my, op, pmask);

	VALID_MOVE_2(MASK_SOUTH_EAST, my, op, pmask);

	VALID_MOVE_5(MASK_SOUTH_WEST, my, op, pmask);
	return false;
}

bool vm_G1(ulong my, ulong op, mask_t pmask) {
	mask_t pm;
	VALID_MOVE_6(MASK_WEST, my, op, pmask);

	VALID_MOVE_7(MASK_SOUTH, my, op, pmask);

	VALID_MOVE_6(MASK_SOUTH_WEST, my, op, pmask);
	return false;
}

bool vm_H1(ulong my, ulong op, mask_t pmask) {
	mask_t pm;
	VALID_MOVE_7(MASK_WEST, my, op, pmask);

	VALID_MOVE_7(MASK_SOUTH, my, op, pmask);

	VALID_MOVE_7(MASK_SOUTH_WEST, my, op, pmask);
	return false;
}

// ...

bool vm_H8(ulong my, ulong op, mask_t pmask) {
	mask_t pm;
	VALID_MOVE_7(MASK_WEST, my, op, pmask);
	VALID_MOVE_7(MASK_NORTH, my, op, pmask);
	VALID_MOVE_7(MASK_NORTH_WEST, my, op, pmask);
	return false;
}

#endif /* BITBOARD_VALID_MOVE_H_ */
