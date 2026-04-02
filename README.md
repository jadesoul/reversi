[![zread](https://img.shields.io/badge/Ask_Zread-_.svg?style=flat&color=00b0aa&labelColor=000000&logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHZpZXdCb3g9IjAgMCAxNiAxNiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTQuOTYxNTYgMS42MDAxSDIuMjQxNTZDMS44ODgxIDEuNjAwMSAxLjYwMTU2IDEuODg2NjQgMS42MDE1NiAyLjI0MDFWNC45NjAxQzEuNjAxNTYgNS4zMTM1NiAxLjg4ODEgNS42MDAxIDIuMjQxNTYgNS42MDAxSDQuOTYxNTZDNS4zMTUwMiA1LjYwMDEgNS42MDE1NiA1LjMxMzU2IDUuNjAxNTYgNC45NjAxVjIuMjQwMUM1LjYwMTU2IDEuODg2NjQgNS4zMTUwMiAxLjYwMDEgNC45NjE1NiAxLjYwMDFaIiBmaWxsPSIjZmZmIi8%2BCjxwYXRoIGQ9Ik00Ljk2MTU2IDEwLjM5OTlIMi4yNDE1NkMxLjg4ODEgMTAuMzk5OSAxLjYwMTU2IDEwLjY4NjQgMS42MDE1NiAxMS4wMzk5VjEzLjc1OTlDMS42MDE1NiAxNC4xMTM0IDEuODg4MSAxNC4zOTk5IDIuMjQxNTYgMTQuMzk5OUg0Ljk2MTU2QzUuMzE1MDIgMTQuMzk5OSA1LjYwMTU2IDE0LjExMzQgNS42MDE1NiAxMy43NTk5VjExLjAzOTlDNS42MDE1NiAxMC42ODY0IDUuMzE1MDIgMTAuMzk5OSA0Ljk2MTU2IDEwLjM5OTlaIiBmaWxsPSIjZmZmIi8%2BCjxwYXRoIGQ9Ik0xMy43NTg0IDEuNjAwMUgxMS4wMzg0QzEwLjY4NSAxLjYwMDEgMTAuMzk4NCAxLjg4NjY0IDEwLjM5ODQgMi4yNDAxVjQuOTYwMUMxMC4zOTg0IDUuMzEzNTYgMTAuNjg1IDUuNjAwMSAxMS4wMzg0IDUuNjAwMUgxMy43NTg0QzE0LjExMTkgNS42MDAxIDE0LjM5ODQgNS4zMTM1NiAxNC4zOTg0IDQuOTYwMVYyLjI0MDFDMTQuMzk4NCAxLjg4NjY0IDE0LjExMTkgMS42MDAxIDEzLjc1ODQgMS42MDAxWiIgZmlsbD0iI2ZmZiIvPgo8cGF0aCBkPSJNNCAxMkwxMiA0TDQgMTJaIiBmaWxsPSIjZmZmIi8%2BCjxwYXRoIGQ9Ik00IDEyTDEyIDQiIHN0cm9rZT0iI2ZmZiIgc3Ryb2tlLXdpZHRoPSIxLjUiIHN0cm9rZS1saW5lY2FwPSJyb3VuZCIvPgo8L3N2Zz4K&logoColor=ffffff)](https://zread.ai/jadesoul/reversi)

## 基于机器学习的黑白棋游戏AI  reversi 1.0发布

* https://github.com/jadesoul/reversi
* 使用C++实现，完全开源
* 本项目的目的是研究机器学习在游戏AI中的应用

### 目前实现了

* 基本的局面表示，游戏对象，玩家对象，测试，日志，多线程
* 基于html+jquery的web界面，使用nginx代理cython包装的c++游戏引擎
* 简单的向后看1步AI，随机AI，基于状态空间树搜索的AI(广度优先、深度优先)，基于蒙特卡洛搜索的AI

### 待完成的

* 完美终局的搜索，作为训练数据
* 基于机器学习的AI 
* 开局库的支持 opening book support

欢迎开源界感兴趣的朋友一起参与，体验设计游戏AI的乐趣

### 参考文献
* Experiments with Monte Carlo Othello ，Evolutionary Computation, 2007. CEC 2007. IEEE Congress on
* Playing Othello Using Monte Carlo (J.A.M. Nijssen 2007)

### 更新历史
--------
* 2025年02月06日
    * 重温代码，完善了一些小细节，调整了开局黑子的位置和白子对调，打印棋盘更清晰的展示等
* 2015年01月26日
    * 完全重构了棋盘，借鉴了GNUGo中的棋盘技巧，使用特殊的墙帮助判断出界，使用宏来实现方向、位置变换
    * 棋盘位置由二维降到一维，使用大量的宏简化工作，老的代码在新棋盘上的移植和整理
    * 代码工程化方面大量的工作，包括声明和实现分离，基础库、引擎库、AI模块的分离，重新写CMakeLists文件
    * 最终搜索速度比之前的版本快了30%
* 2015年01月10日
    * 完成了开局库的设计和实现
    * 设计了一个使用开局库下棋的AI，其他AI只需要继承这个就可以使用开局库的功能
* 2013年06月17日
    * 逐步完成第一版研发
