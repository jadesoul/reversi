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
		return my_rice < r.my_rice or op_rice < r.op_rice or pos < r.pos;
	}
};

struct eat_val {
	mask_t 		eat_mask;
	uint		total;
	hash_t		eat_zobbrist_hash;
	//...

	eat_val():eat_mask(0), total(0), eat_zobbrist_hash(0) {}
};

class MoveDatabase {
public:
	typedef map<eat_key, uint> eat_table_t;
	//typedef hash_map<eat_key, uint> eat_table_t;//TODO

public:
	mask_t 				heng[8], shu[8], pie[15], nar[15];//for generating corss
	mask_t 				cross_table[64];//cross for each eating position
	eat_table_t 		eat_table;//eating database: store the index to eat_val_list for each eat_key
	vector<eat_val>		eat_val_list;//store all kinds of eating situation

public:
	//dynamic: use pre-generated database
	MoveDatabase(bool dynamic=true) {
		init_heng_shu_pie_nar();
		init_cross_table();
//		init_eat_table();
	}

	void test() {
//		for_n(i, 15) print(pie[i]);
//		for_n(i, 15) print(nar[i]);

		for_n(i, 64) {
			system("clear");
			cout<<"i="<<i<<", rice mask="<<endl;
			print(cross_table[i]);
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

	bool find(const mask_t& my, const mask_t& op, const pos_t& pos, eat_val& val) {
		mask_t cross=cross_table[pos];
		eat_key key(my & cross, op & cross, pos);
		eat_table_t::iterator it = eat_table.find(key);
		if (it == eat_table.end()) return false;
		val = eat_val_list[it->second];//use index in list
		return true;
	}

	void add(const mask_t& my, const mask_t& op, const pos_t& pos, const eat_val& val) {
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

		//for debug
//		mask_t cross=cross_table[pos];
//		print(cross);

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
//				print(pms[i][j]);
			}
//			system("clear");
//			print(m);
//			getchar();
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

class MoveMaker {
public:
	MoveMaker() {}

	bool make_move(const mask_t& my, const mask_t& op, const pos_t& pos, eat_val& val) {

	}
};

MoveMaker move_maker;

#endif /* BITBOARD_MAKE_MOVE_H_ */




