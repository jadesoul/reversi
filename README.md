基于机器学习的黑白棋游戏AI  reversi 1.0发布：

* https://github.com/jadesoul/reversi
* 使用C++实现，完全开源
* 本项目的目的是研究机器学习在游戏AI中的应用


目前实现了：

* 基本的局面表示，游戏对象，玩家对象，测试，日志，多线程
* 基于html+jquery的web界面，使用nginx代理cython包装的c++游戏引擎
* 简单的向后看1步AI，随机AI，基于状态空间树搜索的AI(广度优先、深度优先)，基于蒙特卡洛搜索的AI

待完成的：

* 完美终局的搜索，作为训练数据
* 基于机器学习的AI 
* 开局库的支持 opening book support

欢迎开源界感兴趣的朋友一起参与，体验设计游戏AI的乐趣

参考文献：
* Experiments with Monte Carlo Othello ，Evolutionary Computation, 2007. CEC 2007. IEEE Congress on
* Playing Othello Using Monte Carlo (J.A.M. Nijssen 2007)

更新历史：

2015年01月26日23:11:27
* 完全重构了棋盘，借鉴了GNUGo中的棋盘技巧，使用特殊的墙帮助判断出界，使用宏来实现方向、位置变换
* 棋盘位置有二维降到一维，使用大量的宏简化工作，代码工程化，在新棋盘上的移植和整理
