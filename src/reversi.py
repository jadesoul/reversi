import pyximport
pyximport.install(inplace=1)

import engine

r=engine.reversi('00000000000000000000000000021000000120000000000000000000000000001')
if r:
    print r
