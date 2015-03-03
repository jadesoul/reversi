#ifndef BITBOARD_MAKE_MOVE_H_
#define BITBOARD_MAKE_MOVE_H_
/*
 * File: make_move.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月26日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "common.h"
#include "valid_move.h"

/*

在位棋盘的基础上
构造一个米字型的mask

\    |    /
 \   |   /
  \  |  /
   \ | /
    \|/
-----+-----
    /|\
   / | \
  /  |  \
 /   |   \
/    |    \

下子点在mask的中心
是否能吃子，仅与这个mask有关

mask记作：rice

每种吃子的情况对应一个唯一哈希：64+64+6 = 134 bits
	key = ((my & rice) << 64 + (op & rice)) << 6 + (pos & 0x3F)

要求pos位置一定是空，可以减少6 bits
	key = (my & rice & pos_mask) << 64 + (op & rice)

事先穷举出所有可能的能吃子的key，value存放所有吃的子是1构成的eat_mask，得到一个hashmap
可能size会比较大

在最终下棋时，根据key去查找hashmap, O(1),如果能找不到，则不能吃子(valid_move)
如果能找到，则直接得到吃子eat_mask

//i=[0, 7]
mask_t heng[8]={
	0xFF,
	0xFF << 8,
	0xFF << 16,
	static_cast<mask_t>(0xFF) << 24,
	static_cast<mask_t>(0xFF) << 32,
	static_cast<mask_t>(0xFF) << 40,
	static_cast<mask_t>(0xFF) << 48,
	static_cast<mask_t>(0xFF) << 56,
};

//j=[0, 7]
mask_t shu[8]={
	0x0101010101010101,
	0x0101010101010101 << 1,
	0x0101010101010101 << 2,
	0x0101010101010101 << 3,
	0x0101010101010101 << 4,
	0x0101010101010101 << 5,
	0x0101010101010101 << 6,
	static_cast<mask_t>(0x0101010101010101) << 7,
};

//i=0:1, j=7:6, i+j=7 [0, 14]
mask_t pie[15]={
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//i=0:1, j=0:1, i-j=0 [-7, +7]
mask_t nar[15]={
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


*/

//仅与吃子相关的信息构成key
struct eat_key {
	mask_t 	my_rice;
	mask_t 	op_rice;
	pos_t	pos;

	eat_key(const mask_t& my, const mask_t& op, const pos_t& pos):
		my_rice(my), op_rice(op), pos(pos) {}

	bool operator < (const eat_key& r) const {
//		return my_rice < r.my_rice or op_rice < r.op_rice or pos < r.pos;
		return memcmp(this, &r, sizeof(*this))<0;
	}

	bool operator ==(const eat_key& r) const {
//		return my_rice == r.my_rice and op_rice == r.op_rice and pos == r.pos;
		return memcmp(this, &r, sizeof(*this))==0;
	}
};

template<>
struct std::hash<eat_key> {
	size_t operator() (const eat_key& key) const {
		return (key.my_rice * 13131 + key.op_rice) * 13131 + key.pos;
//		return key.my_rice ^ key.op_rice ^ key.pos;
	}
};

//typedef map<eat_key, uint> eat_table_t;
//typedef hash_map<eat_key, uint> eat_table_t;//TODO
typedef unordered_map<eat_key, uint> eat_table_t;//TODO

struct eat_val {
	mask_t 		eat_mask;//holding the bits for fliping
	mask_t 		gard_mask;//my border stones that make the eating possible
	uint		total_eat;//total eating stones
	hash_t		eat_zobbrist_hash;
	//...

	eat_val():eat_mask(0), gard_mask(0), total_eat(0), eat_zobbrist_hash(0) {}
};

class MoveDatabase {
public:
	mask_t 				heng[8], shu[8], pie[15], nar[15];//for generating corss
	mask_t 				cross_table[64];//cross for each eating position
	eat_table_t 		eat_table;//eating database: store the index to eat_val_list for each eat_key
	vector<eat_val>		eat_val_list;//store all kinds of eating situation

public:
	//dynamic: use pre-generated database
//	MoveDatabase(bool dynamic=true) {
	MoveDatabase() {
		init_heng_shu_pie_nar();
		init_cross_table();
//		const size_t n=UINT32_MAX/8;
		const size_t n=0x01 << 30;

//		eat_table.resize(n);
		eat_table.reserve(n);

		eat_val_list.reserve(2*n);
//		init_eat_table();
	}

	inline size_t table_size() const { return eat_table.size(); }
	inline size_t list_size() const { return eat_val_list.size(); }

	friend inline ostream& operator<<(ostream& o, const MoveDatabase& d) { d.dump(o); return o; }

	void dump(ostream& o) const {
		o<<"MoveDatabase { table="<<table_size()<<" list="<<list_size()<<" }";
	}

	void test() {
		for_n(i, 64) {
			system("clear");
			cout<<"i="<<i<<", rice mask="<<endl;
			cout<<Mask(cross_table[i])<<endl;
			getchar();
		}
	}

	void print(mask_t mask) {
		for_n(i, 64) {
			if (i>0 and i%8==0) cout<< "\n";
			cout<< ((mask & 0x01) ? '1' : '.');
			cout<<' ';
			mask >>= 1;
		}
		cout<< "\n\n";
	}

	bool find(const ulong& my, const ulong& op, const pos_t& pos, eat_val& val) {
//		return false;

		mask_t cross=cross_table[pos];
		eat_key key(my & cross, op & cross, pos);
		eat_table_t::iterator it = eat_table.find(key);
		if (it == eat_table.end()) return false;
		val = eat_val_list[it->second];//use index in list
//		log_status("found move in database: "<<BITS_TEXT(val.eat_mask));
		return true;
	}

	void add(const ulong& my, const ulong& op, const pos_t& pos, const eat_val& val) {
//		return;
		mask_t cross=cross_table[pos];
		eat_key key(my & cross, op & cross, pos);
		eat_table[key]=eat_val_list.size();
		eat_val_list.push_back(val);
	}
protected:
	void init_heng_shu_pie_nar() {
		for_n(i, 8) heng[i]=shu[i]=0;
		for_n(i, 15) pie[i]=nar[i]=0;

		for_n(i, 8) {
			for_n(j, 8) {
				pos_t p=POS(i, j);
				mask_t m= static_cast<mask_t>(0x01) << p;

				heng[i] |= m;
				shu[j] |= m;
				pie[i+j] |= m;
				nar[7+j-i] |= m;
			}
		}
	}

	void init_cross_table() {
		for_n(pos, 64) {
			mask_t pos_mask=static_cast<mask_t>(0x01) << pos;
			pos_t i=I(pos), j=J(pos);
			mask_t cross = (heng[i] | shu[j] | pie[i+j] | nar[7+j-i]);
			mask_t cross_without_center=cross ^ pos_mask;
			cross_table[pos] =  cross_without_center;
		}
	}

	void init_eat_table_from_pos(const pos_t& pos) {
		uint i=I(pos), j=J(pos);
		log_status("i="<<i<<", j="<<j);
		mask_t pos_mask = static_cast<mask_t>(0x01) << pos;
		mask_t m;

		mask_t pms[8][7];//at most 7 pms for each direction
		uint len[8];//len for each direction
		uint d;//current direction
		uint n;//how namy pms for d

		d=0;
		n=7-j;
		len[d]=n;
		m=MASK_EAST(pos_mask);
		for_n(k, n) {
			pms[d][k]=m;
			m=MASK_EAST(m);
		}

		d=1;
		n=MIN(7-i, 7-j);
		len[d]=n;
		m=MASK_SOUTH_EAST(pos_mask);
		for_n(k, n) {
			pms[d][k]=m;
			m=MASK_SOUTH_EAST(m);
		}

		d=2;
		n=7-i;
		len[d]=n;
		m=MASK_SOUTH(pos_mask);
		for_n(k, n) {
			pms[d][k]=m;
			m=MASK_SOUTH(m);
		}

		d=3;
		n=MIN(7-i, j);
		len[d]=n;
		m=MASK_SOUTH_WEST(pos_mask);
		for_n(k, n) {
			pms[d][k]=m;
			m=MASK_SOUTH_WEST(m);
		}

		d=4;
		n=j;
		len[d]=n;
		m=MASK_WEST(pos_mask);
		for_n(k, n) {
			pms[d][k]=m;
			m=MASK_WEST(m);
		}

		d=5;
		n=MIN(i, j);
		len[d]=n;
		m=MASK_NORTH_WEST(pos_mask);
		for_n(k, n) {
			pms[d][k]=m;
			m=MASK_NORTH_WEST(m);
		}

		d=6;
		n=i;
		len[d]=n;
		m=MASK_NORTH(pos_mask);
		for_n(k, n) {
			pms[d][k]=m;
			m=MASK_NORTH(m);
		}

		d=7;
		n=MIN(i, 7-j);
		len[d]=n;
		m=MASK_NORTH_EAST(pos_mask);
		for_n(k, n) {
			pms[d][k]=m;
			m=MASK_NORTH_EAST(m);
		}

		init_eat_table_from_pms(pms, len);
	}

	void init_eat_table_from_pms_from_start(mask_t pms[8][7], uint len[8], uint start[8], uint d) {
		if (d==0 and start[d]+1==len[d]) return;

		if (start[d]+1 < len[d]) {//can eat another more stone
			start[d] += 1;
		} else {// can not eat any more stone
			if (d==7) {
				//gen one
			} else {
				init_eat_table_from_pms_from_start(pms, len, start, d+1);
			}
		}

	}

	void init_eat_table_from_pms(mask_t pms[8][7], uint len[8]) {
/*
3 states
MY: my & pm
OP: op & pm
EM: do nothing

		for_n(i, 8) {
			mask_t m=0;
			uint n=len[i];

			log_status("n="<<n);
			for_n(j, n) {
				m |= pms[i][j];
			}
		}
*/

		uint start[8];
		for_n(i, 8) start[i]=0;
		uint d=0;//direction
		init_eat_table_from_pms_from_start(pms, len, start, d);
	}

	//TODO: mirror, save 4X
	inline void init_eat_table() {
		for_n(pos, 64) {
			init_eat_table_from_pos(pos);
		}
	}
};

MoveDatabase move_db;

template<class PosMaskChanger, int LOOK_N, class NextMaker>
class MoveFromPosMaker {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
		return false;
	}
};

template<>
class MoveFromPosMaker<Dummy, 0, Dummy> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
		return val.total_eat > 0;
	}
};

template<class PosMaskChanger, class NextMaker>
class MoveFromPosMaker<PosMaskChanger, 0, NextMaker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
		return NextMaker()(my, op, pmask, val);
	}
};

template<class PosMaskChanger, class NextMaker>
class MoveFromPosMaker<PosMaskChanger, 1, NextMaker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
		return NextMaker()(my, op, pmask, val);
	}
};

template<class PosMaskChanger, class NextMaker>
class MoveFromPosMaker<PosMaskChanger, 2, NextMaker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
		PosMaskChanger next_pos_mask;
		mask_t pm1 = next_pos_mask(pmask);
		mask_t pm2 = next_pos_mask(pm1);

		if ((pm1 & op) and (pm2 & my)) {
			val.eat_mask |= pm1;
			val.gard_mask |= pm2;
			val.total_eat +=1;
		}
		return NextMaker()(my, op, pmask, val);
	}
};

template<class PosMaskChanger, class NextMaker>
class MoveFromPosMaker<PosMaskChanger, 3, NextMaker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);
		mask_t eat_mask=0;//tmp eat mask for this direction

		if (pm & op) {
			eat_mask |= pm;
			pm = next_pos_mask(pm);
			if (pm & my) {
				val.eat_mask |= eat_mask;
				val.gard_mask |= pm;
				val.total_eat +=1;
			} else if (pm & op) {
				eat_mask |= pm;
				pm = next_pos_mask(pm);
				if (pm & my) {
					val.eat_mask |= eat_mask;
					val.gard_mask |= pm;
					val.total_eat +=2;
				}
			}
		}
		return NextMaker()(my, op, pmask, val);
	}
};


template<class PosMaskChanger, class NextMaker>
class MoveFromPosMaker<PosMaskChanger, 4, NextMaker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);
		mask_t eat_mask=0;//tmp eat mask for this direction

		if (pm & op) {
			eat_mask |= pm;
			pm = next_pos_mask(pm);
			if (pm & my) {
				val.eat_mask |= eat_mask;
				val.gard_mask |= pm;
				val.total_eat +=1;
			} else if (pm & op) {
				eat_mask |= pm;
				pm = next_pos_mask(pm);
				if (pm & my) {
					val.eat_mask |= eat_mask;
					val.gard_mask |= pm;
					val.total_eat +=2;
				} else if (pm & op) {
					eat_mask |= pm;
					pm = next_pos_mask(pm);
					if (pm & my) {
						val.eat_mask |= eat_mask;
						val.gard_mask |= pm;
						val.total_eat +=3;
					}
				}
			}
		}
		return NextMaker()(my, op, pmask, val);
	}
};


template<class PosMaskChanger, class NextMaker>
class MoveFromPosMaker<PosMaskChanger, 5, NextMaker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);
		mask_t eat_mask=0;//tmp eat mask for this direction

		if (pm & op) {
			eat_mask |= pm;
			pm = next_pos_mask(pm);
			if (pm & my) {
				val.eat_mask |= eat_mask;
				val.gard_mask |= pm;
				val.total_eat +=1;
			} else if (pm & op) {
				eat_mask |= pm;
				pm = next_pos_mask(pm);
				if (pm & my) {
					val.eat_mask |= eat_mask;
					val.gard_mask |= pm;
					val.total_eat +=2;
				} else if (pm & op) {
					eat_mask |= pm;
					pm = next_pos_mask(pm);
					if (pm & my) {
						val.eat_mask |= eat_mask;
						val.gard_mask |= pm;
						val.total_eat +=3;
					} else if (pm & op) {
						eat_mask |= pm;
						pm = next_pos_mask(pm);
						if (pm & my) {
							val.eat_mask |= eat_mask;
							val.gard_mask |= pm;
							val.total_eat +=4;
						}
					}
				}
			}
		}
		return NextMaker()(my, op, pmask, val);
	}
};

template<class PosMaskChanger, class NextMaker>
class MoveFromPosMaker<PosMaskChanger, 6, NextMaker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);
		mask_t eat_mask=0;//tmp eat mask for this direction

		if (pm & op) {
			eat_mask |= pm;
			pm = next_pos_mask(pm);
			if (pm & my) {
				val.eat_mask |= eat_mask;
				val.gard_mask |= pm;
				val.total_eat +=1;
			} else if (pm & op) {
				eat_mask |= pm;
				pm = next_pos_mask(pm);
				if (pm & my) {
					val.eat_mask |= eat_mask;
					val.gard_mask |= pm;
					val.total_eat +=2;
				} else if (pm & op) {
					eat_mask |= pm;
					pm = next_pos_mask(pm);
					if (pm & my) {
						val.eat_mask |= eat_mask;
						val.gard_mask |= pm;
						val.total_eat +=3;
					} else if (pm & op) {
						eat_mask |= pm;
						pm = next_pos_mask(pm);
						if (pm & my) {
							val.eat_mask |= eat_mask;
							val.gard_mask |= pm;
							val.total_eat +=4;
						} else if (pm & op) {
							eat_mask |= pm;
							pm = next_pos_mask(pm);
							if (pm & my) {
								val.eat_mask |= eat_mask;
								val.gard_mask |= pm;
								val.total_eat +=5;
							}
						}
					}
				}
			}
		}
		return NextMaker()(my, op, pmask, val);
	}
};

template<class PosMaskChanger, class NextMaker>
class MoveFromPosMaker<PosMaskChanger, 7, NextMaker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);
		mask_t eat_mask=0;//tmp eat mask for this direction

		if (pm & op) {
			eat_mask |= pm;
			pm = next_pos_mask(pm);
			if (pm & my) {
				val.eat_mask |= eat_mask;
				val.gard_mask |= pm;
				val.total_eat +=1;
			} else if (pm & op) {
				eat_mask |= pm;
				pm = next_pos_mask(pm);
				if (pm & my) {
					val.eat_mask |= eat_mask;
					val.gard_mask |= pm;
					val.total_eat +=2;
				} else if (pm & op) {
					eat_mask |= pm;
					pm = next_pos_mask(pm);
					if (pm & my) {
						val.eat_mask |= eat_mask;
						val.gard_mask |= pm;
						val.total_eat +=3;
					} else if (pm & op) {
						eat_mask |= pm;
						pm = next_pos_mask(pm);
						if (pm & my) {
							val.eat_mask |= eat_mask;
							val.gard_mask |= pm;
							val.total_eat +=4;
						} else if (pm & op) {
							eat_mask |= pm;
							pm = next_pos_mask(pm);
							if (pm & my) {
								val.eat_mask |= eat_mask;
								val.gard_mask |= pm;
								val.total_eat +=5;
							} else if (pm & op) {
								eat_mask |= pm;
								pm = next_pos_mask(pm);
								if (pm & my) {
									val.eat_mask |= eat_mask;
									val.gard_mask |= pm;
									val.total_eat +=6;
								}
							}
						}
					}
				}
			}
		}
		return NextMaker()(my, op, pmask, val);
	}
};

//make move from grid(x, y), pos(x-1, y-1)
//i, j starts from 0
//x, y starts from 1
template<uint x, uint y>
bool make_move_from_pos(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
	typedef MoveFromPosMaker<Dummy, 0, Dummy> 					End;
	typedef MoveFromPosMaker<RightUp, MIN(8-y, x-1), End> 		A;
	typedef MoveFromPosMaker<Up, x-1, A> 						B;
	typedef MoveFromPosMaker<LeftUp, MIN(y-1, x-1), B> 			C;
	typedef MoveFromPosMaker<Left, y-1, C> 						D;
	typedef MoveFromPosMaker<LeftDown, MIN(y-1, 8-x), D> 		E;
	typedef MoveFromPosMaker<Down, 8-x, E> 						F;
	typedef MoveFromPosMaker<RightDown, MIN(8-y, 8-x), F> 		G;
	typedef MoveFromPosMaker<Right, 8-y, G> 					H;

	return H()(my, op, pmask, val);
}

typedef bool (* MakeMoveFromPosFunction) \
		(const ulong& my_bits, const ulong& op_bits, const mask_t& pos_mask, eat_val& val);

#define MM(x, y) make_move_from_pos<x, y>

MakeMoveFromPosFunction make_move_from_pos_functions[64]={
	MM(1,1), MM(1,2), MM(1,3), MM(1,4), MM(1,5), MM(1,6), MM(1,7), MM(1,8),
	MM(2,1), MM(2,2), MM(2,3), MM(2,4), MM(2,5), MM(2,6), MM(2,7), MM(2,8),
	MM(3,1), MM(3,2), MM(3,3), MM(3,4), MM(3,5), MM(3,6), MM(3,7), MM(3,8),
	MM(4,1), MM(4,2), MM(4,3), MM(4,4), MM(4,5), MM(4,6), MM(4,7), MM(4,8),
	MM(5,1), MM(5,2), MM(5,3), MM(5,4), MM(5,5), MM(5,6), MM(5,7), MM(5,8),
	MM(6,1), MM(6,2), MM(6,3), MM(6,4), MM(6,5), MM(6,6), MM(6,7), MM(6,8),
	MM(7,1), MM(7,2), MM(7,3), MM(7,4), MM(7,5), MM(7,6), MM(7,7), MM(7,8),
	MM(8,1), MM(8,2), MM(8,3), MM(8,4), MM(8,5), MM(8,6), MM(8,7), MM(8,8),
};

class FastMoveMaker {
public:
	inline bool operator()(const ulong& my, const ulong& op, const pos_t& pos, eat_val& val) {
		assert(IS_EMPTY(my, op, pos));
		mask_t pos_mask= ONE << pos;
		return make_move_from_pos_functions[pos](my, op, pos_mask, val);
	}
};

FastMoveMaker try_make_move;



#endif /* BITBOARD_MAKE_MOVE_H_ */


