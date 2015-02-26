#ifndef BITBOARD_FUNCTOR_H_
#define BITBOARD_FUNCTOR_H_
/*
 * File: functor.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月26日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "mask.h"
class Dummy;

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		return false;
	}
};

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker<Dummy, 0, Dummy> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		return false;
	}
};

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 0, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 1, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 2, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);

		if ((pm & op) and (next_pos_mask(pm) & my)) return true;
		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 3, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);

		if (pm & op) {
			pm = next_pos_mask(pm);
			if (pm & my) return true;
			if ((pm & op) and (next_pos_mask(pm) & my)) return true;
		}

		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 4, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);

		if (pm & op) {
			pm = next_pos_mask(pm);
			if (pm & my) return true;
			if (pm & op) {
				pm = next_pos_mask(pm);
				if (pm & my) return true;
				if ((pm & op) and (next_pos_mask(pm) & my)) return true;
			}
		}

		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 5, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);

		if (pm & op) {
			pm = next_pos_mask(pm);
			if (pm & my) return true;
			if (pm & op) {
				pm = next_pos_mask(pm);
				if (pm & my) return true;
				if (pm & op) {
					pm = next_pos_mask(pm);
					if (pm & my) return true;
					if ((pm & op) and (next_pos_mask(pm) & my)) return true;
				}
			}
		}

		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 6, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);

		if (pm & op) {
			pm = next_pos_mask(pm);
			if (pm & my) return true;
			if (pm & op) {
				pm = next_pos_mask(pm);
				if (pm & my) return true;
				if (pm & op) {
					pm = next_pos_mask(pm);
					if (pm & my) return true;
					if (pm & op) {
						pm = next_pos_mask(pm);
						if (pm & my) return true;
						if ((pm & op) and (next_pos_mask(pm) & my)) return true;
					}
				}
			}
		}

		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 7, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);

		if (pm & op) {
			pm = next_pos_mask(pm);
			if (pm & my) return true;
			if (pm & op) {
				pm = next_pos_mask(pm);
				if (pm & my) return true;
				if (pm & op) {
					pm = next_pos_mask(pm);
					if (pm & my) return true;
					if (pm & op) {
						pm = next_pos_mask(pm);
						if (pm & my) return true;
						if (pm & op) {
							pm = next_pos_mask(pm);
							if (pm & my) return true;
							if ((pm & op) and (next_pos_mask(pm) & my)) return true;
						}
					}
				}
			}
		}

		return NextChecker()(my, op, pmask);
	}
};


#endif /* BITBOARD_FUNCTOR_H_ */
