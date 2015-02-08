/*
 * JavaScript Document For Web Reversi
 *
 * Copyright (c) 2010 Jadesoul (Home Page: http://jadesoul.org , Blog: http://www.jadesoul.org)
 * Licensed under the Apache License:
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * $Date: 2010-7-22$
 * $Author: Jadesoul $
 * 
 */

function alert() {
	return;
}

String.prototype.format = function() {
	var s=this.valueOf();
	for (var i=0; i<arguments.length; i++) {
		var re=new RegExp('\\\{'+i+'\\\}', 'g');
		s=s.replace(re, arguments[i]);	
	}
	return s;
}

var GridState = {
	EMPTY : 0,
	BLACK : 1,
	WHITE : 2,
	B_SEL : 3,
	W_SEL : 4
};

var Direction = {
	RIGHT 		: 0,
	RIGHT_DOWN 	: 1,
	DOWN 		: 2,
	LEFT_DOWN 	: 3,
	LEFT 		: 4,
	LEFT_UP 	: 5,
	UP 			: 6,
	RIGHT_UP 	: 7
}

var sys={
	GridState	: GridState,
	Direction	: Direction
};

var Grid=function(row, col, state, div) {
	this.state=state;
	this.row=row;
	this.col=col;
	this.div=div;
	
	this.setState=function(s) {
		this.setEmpty();
		this.state=s;
		if (s==GridState.BLACK) {
			this.div.addClass('chess-black');
		} else if (s==GridState.WHITE) {
			this.div.addClass('chess-white');
		} else if (s==GridState.B_SEL) {
			this.div.addClass('chess-black-select');
		} else if (s==GridState.W_SEL) {
			this.div.addClass('chess-white-select');
		}
	}
	
	this.setEmpty=function() {
		this.state=GridState.EMPTY;
		this.div.removeClass('chess-black').removeClass('chess-white').removeClass('chess-black-select').removeClass('chess-white-select');
	}
	
	this.nextGrid=function(d) {
		var i = this.row, j = this.col;
		
		if (d == Direction.RIGHT)
		{
			if (j == 8) return null;
			else return game.map.grid(i, j + 1);
		}
		else if (d == Direction.RIGHT_DOWN)
		{
			if (i == 8 || j == 8) return null;
			else return game.map.grid(i + 1, j + 1);
		}
		else if (d == Direction.DOWN)
		{
			if (i == 8) return null;
			else return game.map.grid(i + 1, j);
		}
		else if (d == Direction.LEFT_DOWN)
		{
			if (i == 8 || j == 1) return null;
			else return game.map.grid(i + 1, j - 1);
		}
		else if (d == Direction.LEFT)
		{
			if (j == 1) return null;
			else return game.map.grid(i, j - 1);
		}
		else if (d == Direction.LEFT_UP)
		{
			if (i == 1 || j == 1) return null;
			else return game.map.grid(i - 1, j - 1);
		}
		else if (d == Direction.UP)
		{
			if (i == 1) return null;
			else return game.map.grid(i - 1, j);
		}
		else if (d == Direction.RIGHT_UP)
		{
			if (i == 1 || j == 8) return null;
			else return game.map.grid(i - 1, j + 1);
		}
	}
	
}

var GameMap=function(m) {
	this.rows=[];
	
	for (var i=0; i<=8; i++) {
		var cols=[];
		for (var j=0; j<=8; j++) {
			if (i==0 || j==0) cols.push('^_^');
			else cols.push(new Grid(i, j, GridState.EMPTY, $('#board div.chess[id=chess_'+i+'_'+j+']')));	
		}
		this.rows.push(cols);	
	}
	
	this.grid=function(r, c) {
		return this.rows[r][c];
	}
	
	this.init=function(m) {
		if (!m) m='0000000000000000000000000002100000012000000000000000000000000000';
		for (var i=1; i<=8; i++) {
			for (var j=1; j<=8; j++) {
				var n=m.charAt((i-1)*8+j-1);
				this.grid(i, j).setState(parseInt('0'+n));
			}
		}
	}
	
	this.toString=function() {
		var s='';
		for (var i=1; i<=8; i++) {
			for (var j=1; j<=8; j++) {
				s+=this.rows[i][j].state;
			}
		}
		return s;
	}
	
	
	this.init(m);
}

var Player=function(name, color) {
	this.name='default name';
	this.color=color;
	this.oppo=(this.color==GridState.BLACK?GridState.WHITE:GridState.BLACK);
	this.guys=[];
	this.avails=[];
	
	this.findAvails=function() {
		this.avails=[];
		for (var i=0; i<this.guys.length; i++) {
			for (var d=0; d<8; d++) {
				var g=this.guys[i];
				g=g.nextGrid(d);
				if (!g || g.state!=this.oppo) continue;
				for (; g && g.state==this.oppo; g=g.nextGrid(d));
				if (g && g.state==GridState.EMPTY && $.inArray(g, this.avails)==-1) {
					g.setState(this.color+2);
					this.avails.push(g);
				}
			}
		}
		return this.avails.length;
	}

	this.refreshGuys=function() {
		var map=game.map;
		this.guys=[];
		for (var i=1; i<=8; i++) {
			for (var j=1; j<=8; j++) {
				if (map.grid(i, j).state==this.color) this.guys.push(map.grid(i, j));
			}
		}
		return this.guys.length;
	}
	
	this.eatFrom=function(r, c) {
		var g=game.map.grid(r, c);
		if (g.state!=this.color+2) return;
		if ($.inArray(g, this.avails)==-1) return;
		
		g.setState(this.color);
		var grid=g;
		var count=0;
		for (var d=0; d<8; ++d) {
			var tmp=[];
			g=grid;
			g=g.nextGrid(d);
			if (!g || g.state!=this.oppo) continue;	
			for (; g && g.state==this.oppo; g=g.nextGrid(d)) tmp.push(g);
			if (g && g.state==this.color) {
				count+=tmp.length;
				for (var i=0; i<tmp.length; i++) {
					tmp[i].setState(this.color);
				}
			}
		}
		return count;
	}
	
	this.putChess=function(r, c) {
		//alert('eat:'+this.eatFrom(r, c));
		this.eatFrom(r, c);
	}
	
	this.nextDirection=function(d) {
		if (d == Direction.RIGHT_UP) return Direction.RIGHT;
		return d+1;
	}
	
	this.toString=function() {
		return this.name+' (执'+(this.color==GridState.BLACK?'黑':'白')+'子,上次棋子数'+this.guys.length+',上次可下位置数'+this.avails.length+')'
	}
	return this;
}

Player.ComputerPlayer=function(name, color) {
	
	this.name=name;
	this.color=color;
	this.oppo=(this.color==GridState.BLACK?GridState.WHITE:GridState.BLACK);
	
	this.AI={
		url:'/reversi',
		version:1.0
	}
	
	this.play=function() {
		if (game.over) return game.gameOver('游戏已结束，重新再来一局吧！');
		game.clearSel();
		var url=this.AI.url+'?'+game.map.toString()+game.turn;
		
		game.np.refreshGuys();
		game.np.findAvails();
		
		$.get(url, this.realPlay);
	}
	
	this.realPlay=function(r) {
		game.clearSel();
		game.np.refreshGuys();
		
		if (game.np.findAvails()>0) {
			if (!(r && r.length==2 && r.charAt(0)>=1 && r.charAt(0)<=8 && r.charAt(1)>=1 && r.charAt(1)<=8)) alert('engine error, result:'+r);
			var g=game.map.grid(r.charAt(0), r.charAt(1));
			if (g.state!=game.np.color+2) alert('engine error:select');
			if ($.inArray(g, game.np.avails)==-1) alert('engine error:avails');
			game.np.putChess(r.charAt(0), r.charAt(1));
		} else {
			return alert('fatal error');
		}
		
		game.showChessCount();
		if (game.b+game.w==64) return game.gameOver('游戏已结束，棋盘被下满了！');

		game.changePlayer();
		game.clearSel();
		if (game.np.refreshGuys()==0) return game.gameOver('真悲惨，'+game.np+'的棋子被吃光了!');

		if (game.np.findAvails()==0) {
			if (game.onePassed && game.onePassed!=game.np)	 {
				return game.gameOver('双方都无子可下，游戏结束!');
			} else {
				game.onePassed=game.np;
				alert(game.np+' 得弃权啦，没有地方下子!');
				game.changePlayer();
				game.np.refreshGuys();
				if (game.np.findAvails()==0) {
					return game.gameOver('双方都无子可下，游戏结束!');
				}
			}
		}
		game.onePassed=null;
		setTimeout('game.np.play()', 510);
	}
	
	return this;
}

Player.HumanPlayer=function(name, color) {
	
	this.name=name;
	this.color=color;
	this.oppo=(this.color==GridState.BLACK?GridState.WHITE:GridState.BLACK);
	
	
	this.play=function() {
		if (game.over) return game.gameOver('游戏已结束，重新再来一局吧！');
	}
	
	this.realPlay=function(r, c) {
		if (game.over) return game.gameOver('游戏已结束，重新再来一局吧！');
		game.clearSel();
		
		game.np.refreshGuys();
		if (game.np.findAvails()>0) {
			if (!(r && c && r>=1 && r<=8 && c>=1 && c<=8)) alert('do not try to cheat ^_^ : r='+r);
			var g=game.map.grid(r, c);
			if (g.state!=game.np.color+2) return;
			if ($.inArray(g, game.np.avails)==-1) return;
			game.np.putChess(r, c);
		} else {
			return alert('fatal error');
		}
		
		game.showChessCount();
		if (game.b+game.w==64) return game.gameOver('游戏已结束，棋盘被下满了！');

		game.changePlayer();
		game.clearSel();
		if (game.np.refreshGuys()==0) return game.gameOver('真悲惨，'+game.np+'的棋子被吃光了!');

		if (game.np.findAvails()==0) {
			if (game.onePassed && game.onePassed!=game.np)	 {
				return game.gameOver('双方都无子可下，游戏结束!');
			} else {
				game.onePassed=game.np;
				alert(game.np+' 得弃权啦，没有地方下子!');
				game.changePlayer();
				game.clearSel();
				game.np.refreshGuys();
				if (game.np.findAvails()==0) {
					return game.gameOver('双方都无子可下，游戏结束!');
				}
			}
		}
		game.onePassed=null;
		setTimeout('game.np.play()', 100);
	}
	
	return this;
}

Player.ComputerPlayer.prototype=new Player();
Player.HumanPlayer.prototype=new Player();
	
function Game() {
	this.author='jadesoul';
	this.version=1.0;
	
	//Game.Reversi.prototype=this;
}

Game.Reversi=function(blackPlayer, whitePlayer, initialMap) {

	this.turn=GridState.BLACK;
	
	this.bp=blackPlayer;
	this.wp=whitePlayer;
	this.np=blackPlayer;
	this.onePassed=null;
	this.over=false;
	this.b=0;
	this.w=0;
	this.e=0;
	
	this.map=new GameMap(initialMap);
	
	this.start=function() {
		game.clearSel();
		game.showChessCount();
		game.np.refreshGuys();
		game.np.findAvails();
		setTimeout('game.np.play()', 300);
	}
	
	this.reset=function() {
		this.clearSel();
		this.map.init();
	}
	
	this.clearSel=function() {
		for (var i=1; i<=8; i++) {
			for (var j=1; j<=8; j++) {
				var s=game.map.grid(i, j).state;
				if (s==GridState.B_SEL || s==GridState.W_SEL) {
					game.map.grid(i, j).setEmpty();
					//alert(i+','+j+','+s+','+game.map.grid(i, j).state);
				}
			}
		}
	}
	
	this.changePlayer=function() {
		if (this.np==this.bp) {
			this.np=this.wp;
			this.turn=GridState.WHITE;
		} else {
			this.np=this.bp;
			this.turn=GridState.BLACK;
		}
	}

	this.gameOver=function(reason) {
		this.over=true;
		this.clearSel();
		var b=0, w=0, e=0;
		for (var i=1; i<=8; i++) {
			for (var j=1; j<=8; j++) {
				if (this.map.grid(i, j).state==GridState.EMPTY) e++;
				else if (this.map.grid(i, j).state==GridState.BLACK) b++;
				else if (this.map.grid(i, j).state==GridState.WHITE) w++;
			}
		}
		alert('=========== game over =============\n\n'+reason+'\n'+game.bp.name+'执黑先下以 '+b+':'+w+' '+(b>w?'战胜':(b<w?'败给':'打平'))+game.wp.name+'剩余空格数: '+e);
	}
	
	this.showChessCount=function() {
		game.clearSel();
		this.b=this.w=this.e=0;
		for (var i=1; i<=8; i++) {
			for (var j=1; j<=8; j++) {
				if (this.map.grid(i, j).state==GridState.EMPTY) this.e++;
				else if (this.map.grid(i, j).state==GridState.BLACK) this.b++;
				else if (this.map.grid(i, j).state==GridState.WHITE) this.w++;
			}
		}
		$('#chess-black-count').html('<center style="margin-top:10px;">'+this.bp.name+'<br/>'+this.b+'</center>');
		$('#chess-white-count').html('<center style="margin-top:10px;">'+this.wp.name+'<br/>'+this.w+'</center>');
		if (this.b+this.w==64 || this.b==0 || this.w==0) game.over=true;
	}
	
	return this;
}

Game.Reversi.prototype=new Game();

function initGUI() {
	var i=r=c=0;
	$('#board td').each(function() {
		r=parseInt(i/8) + 1;
		c=i%8 + 1;
		i++;
		$(this).empty().append('<div id="chess_'+r+'_'+c+'" class="chess chess-white"></div>');
	})
	
	$('#board div.chess').click(function() {
		var arr=this.id.split('_');
		if (game.np.name=='游客') game.np.realPlay(arr[1], arr[2]);
	})
}

var game;

$(function (){
	initGUI();
	
//	game=new Game.Reversi(
//		new Player.ComputerPlayer('深蓝', GridState.BLACK), 
//		new Player.HumanPlayer('游客', GridState.WHITE)
//	);

	game=new Game.Reversi(
		new Player.ComputerPlayer('深蓝', GridState.BLACK), 
		new Player.ComputerPlayer('浅绿', GridState.WHITE)
	);
	
//	game=new Game.Reversi(
//		new Player.HumanPlayer('游客', GridState.BLACK), 
//		new Player.ComputerPlayer('深蓝', GridState.WHITE)
//	);
	
	game.start();
})
