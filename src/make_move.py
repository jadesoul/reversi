print ('''
#include "common.h"
''')

'''
mask_t         eat_mask;//holding the bits for fliping
mask_t         gard_mask;//my border stones that make the eating possible
uint        total_eat;//total eating stones
hash_t        eat_zobbrist_hash;

template<class PosMaskChanger, class NextMaker>
class MoveFromPosMaker<PosMaskChanger, 2, NextMaker> {
public:
    inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
        PosMaskChanger next_pos_mask;
        mask_t pm;
        mask_t eat_mask=0;//tmp eat mask for this direction
        
        pm = next_pos_mask(pmask);
        if (pm & op) {
            eat_mask |= pm;
            pm = next_pos_mask(pm);
            if ((pm & my)) {
                val.eat_mask |= eat_mask;
                val.gard_mask |= pm;
                val.total_eat +=1;
            }
        }
        return NextMaker()(my, op, pmask, val);
    }
};

template<class PosMaskChanger, class NextMaker>
class MoveFromPosMaker<PosMaskChanger, 3, NextMaker> {
public:
    inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
        PosMaskChanger next_pos_mask;
        mask_t pm;
        mask_t eat_mask=0;//tmp eat mask for this direction

        pm = next_pos_mask(pmask);
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


#define make_move_look_2(my, op, pmask, next)
tmp=0;
pm=next(pmask);
if (pm & op) {
    tmp |= pm;
    pm=next(pm);
    if (pm & my) {
        eat |= tmp;
    }
}

#define make_move_look_3(my, op, pmask, next)
tmp=0;
pm=next(pmask);
if (pm & op) {
    tmp |= pm;
    pm=next(pm);
    if (pm & my) {
        eat |= tmp;
    } else if (pm & op) {
        tmp |= pm;
        pm=next(pm);
        if (pm & my) {
           eat |= tmp;
        } else ...
    }
}
'''

def gen_make_move_look_n(n):
    define='#define make_move_look_%d(next)' % n
    ss=[define]
    
    def level_code(n, l, tab=''):
        if l==1:
            tmp='tmp |= pm;'
            my='if (next(pm) & my) {'
            ret=tab+'flip |= tmp;'
            end='}'
            ss=tmp, my, ret, end
        elif l==n:
            tmp='tmp=0;'
            next='pm=next(pmask);'
            op='if (pm & op) {'
            sub=level_code(n, l-1, ' ' * 4)
            end='}'
            ss=tmp, next, op, sub, end
        else:
            tmp='tmp |= pm;'
            next='pm=next(pm);'
            my='if (pm & my) {'
            ret=tab+'flip |= tmp;'
            op='} else if (pm & op) {'
            sub=level_code(n, l-1, ' ' * 4)
            end='}'
            ss=tmp, next, my, ret, op, sub, end
        ss='\n'.join(ss).split('\n')
        ss=[tab+s for s in ss]
        return '\n'.join(ss)
    
    if n>=2:
        s=level_code(n, n)
        ss.extend(s.split('\n'))    
    ss=['%-80s\\' % s for s in ss]
    return '\n'.join(ss)+'\n'
    
for i in range(8):
    print (gen_make_move_look_n(i))
    
'''
//make move from grid(x, y), pos(x-1, y-1)
//i, j starts from 0
//x, y starts from 1
template<uint x, uint y>
bool make_move_from_pos(const ulong& my, const ulong& op, const mask_t& pmask, eat_val& val) {
    typedef MoveFromPosMaker<Dummy, 0, Dummy>                     End;
    typedef MoveFromPosMaker<RightUp, MIN(8-y, x-1), End>         A;
    typedef MoveFromPosMaker<Up, x-1, A>                         B;
    typedef MoveFromPosMaker<LeftUp, MIN(y-1, x-1), B>             C;
    typedef MoveFromPosMaker<Left, y-1, C>                         D;
    typedef MoveFromPosMaker<LeftDown, MIN(y-1, 8-x), D>         E;
    typedef MoveFromPosMaker<Down, 8-x, E>                         F;
    typedef MoveFromPosMaker<RightDown, MIN(8-y, 8-x), F>         G;
    typedef MoveFromPosMaker<Right, 8-y, G>                     H;

    return H()(my, op, pmask, val);
}
'''

def text(i, j):
    return chr(ord('A')+j)+str(i+1)

def gen_valid_move_from_pos(x, y):
    tab=' '*4
    ss=[
        'ulong make_move_from_pos_%d%d(ulong my, ulong op) {' % (x, y),
        tab+'ulong pm, tmp, flip=0, pmask=(ONE << %d);' % ((x-1)*8+(y-1)),
        tab+'make_move_look_%d(MASK_EAST)' % (8-y),
        tab+'make_move_look_%d(MASK_SOUTH_EAST)' % min(8-y, 8-x),
        tab+'make_move_look_%d(MASK_SOUTH)' % (8-x),
        tab+'make_move_look_%d(MASK_SOUTH_WEST)' % min(y-1, 8-x),
        tab+'make_move_look_%d(MASK_WEST)' % (y-1),
        tab+'make_move_look_%d(MASK_NORTH_WEST)' % min(y-1, x-1),
        tab+'make_move_look_%d(MASK_NORTH)' % (x-1),
        tab+'make_move_look_%d(MASK_NORTH_EAST)' % min(8-y, x-1),
        tab+'return flip;',
        '}',
    ]
    return '\n'.join(ss)+'\n'

for i in range(8):
    for j in range(8):
        print (gen_valid_move_from_pos(i+1, j+1))

print( '''typedef ulong (* make_move_from_pos_func)(ulong my, ulong op);

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

''')
