print '''
#include "common.h"
'''

'''
#define valid_move_look_2(my, op, pmask, next)
pm=next(pmask);
if (pm & op) {
    pm=next(pm);
    if (pm & my) {
        return 1;
    }
}

#define valid_move_look_3(my, op, pmask, next)
pm=next(pmask);
if (pm & op) {
    pm=next(pm);
    if (pm & my) {
        return 1;
    } else if (pm & op) {
        pm=next(pm);
        if (pm & my) {
            return 1;
        } else ...
    }
}
'''

def gen_valid_move_look_n(n):
    define='#define valid_move_look_%d(next)' % n
    ss=[define]
    
    def level_code(n, l, tab=''):
        if l==1:
            my='if (next(pm) & my) {'
            ret=tab+'return 1;'
            end='}'
            ss=my, ret, end
        elif l==n:
            next='pm=next(pmask);'
            op='if (pm & op) {'
            sub=level_code(n, l-1, ' ' * 4)
            end='}'
            ss=next, op, sub, end
        else:
            next='pm=next(pm);'
            my='if (pm & my) {'
            ret=tab+'return 1;'
            op='} else if (pm & op) {'
            sub=level_code(n, l-1, ' ' * 4)
            end='}'
            ss=next, my, ret, op, sub, end
        ss='\n'.join(ss).split('\n')
        ss=[tab+s for s in ss]
        return '\n'.join(ss)
    
    if n>=2:
        s=level_code(n, n)
        ss.extend(s.split('\n'))    
    ss=['%-80s\\' % s for s in ss]
    return '\n'.join(ss)+'\n'
    
for i in xrange(8):
    print gen_valid_move_look_n(i)
    
'''
//check valid move from grid(x, y), pos(x-1, y-1)
//i, j starts from 0
//x, y starts from 1
template<uint x, uint y>
bool valid_move_checker(const ulong& my, const ulong& op, const mask_t& pmask) {
    typedef ValidMoveFromPosChecker<Dummy, 0, Dummy>                     End;
    typedef ValidMoveFromPosChecker<RightUp, MIN(8-y, x-1), End>         A;
    typedef ValidMoveFromPosChecker<Up, x-1, A>                         B;
    typedef ValidMoveFromPosChecker<LeftUp, MIN(y-1, x-1), B>             C;
    typedef ValidMoveFromPosChecker<Left, y-1, C>                         D;
    typedef ValidMoveFromPosChecker<LeftDown, MIN(y-1, 8-x), D>         E;
    typedef ValidMoveFromPosChecker<Down, 8-x, E>                         F;
    typedef ValidMoveFromPosChecker<RightDown, MIN(8-y, 8-x), F>         G;
    typedef ValidMoveFromPosChecker<Right, 8-y, G>                         H;
    return H()(my, op, pmask);
}
'''

def text(i, j):
    return chr(ord('A')+j)+str(i+1)

def gen_valid_move_from_pos(x, y):
    tab=' '*4
    ss=[
        'int valid_move_from_pos_%d%d(ulong my, ulong op) {' % (x, y),
        tab+'ulong pm, pmask=(ONE << %d);' % ((x-1)*8+(y-1)),
        tab+'valid_move_look_%d(MASK_EAST)' % (8-y),
        tab+'valid_move_look_%d(MASK_SOUTH_EAST)' % min(8-y, 8-x),
        tab+'valid_move_look_%d(MASK_SOUTH)' % (8-x),
        tab+'valid_move_look_%d(MASK_SOUTH_WEST)' % min(y-1, 8-x),
        tab+'valid_move_look_%d(MASK_WEST)' % (y-1),
        tab+'valid_move_look_%d(MASK_NORTH_WEST)' % min(y-1, x-1),
        tab+'valid_move_look_%d(MASK_NORTH)' % (x-1),
        tab+'valid_move_look_%d(MASK_NORTH_EAST)' % min(8-y, x-1),
        tab+'return 0;',
        '}',
    ]
    return '\n'.join(ss)+'\n'

for i in xrange(8):
    for j in xrange(8):
        print gen_valid_move_from_pos(i+1, j+1)

print '''typedef int (*valid_move_from_pos_func)(ulong my, ulong op);

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

'''
