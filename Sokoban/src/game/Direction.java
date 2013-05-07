package game;

import game.Point;

public enum Direction {
	Left(0,-1, 'L'),
	Right(0,1, 'R'),
	Up(-1,0, 'U'),
	Down(1,0, 'D'),
	Here(0,0, 'O');
	
	private int row;
	private int column;
	private char ascii; 
	
	Direction(int row, int column, char asciiCode){
		this.row = row;
		this.column = column;
		this.ascii = asciiCode;
	}
	
	public Direction getDirectionLeftOf(){
		switch(this){
			case Left:
				return Up;
			case Up:
				return Right;
			case Right:
				return Down;
			default:
				return Left;
		}
	}
	
	public Direction getDirectionRightOf(){
		switch(this){
			case Up:
				return Left;
			case Right:
				return Up;
			case Down:
				return Right;
			default:
				return Left;
		}
	}
	
	public char getAsciiCode(){
		return ascii;
	}
	
	public Point getPositionAdjustment(){
		return new Point(row, column);
	}
	
	public Point getNewPosition(int fromX, int fromY){
		return new Point(fromX + row, fromY + column);
	}
	
	public Point getNewPosition(Point from){
		return new Point(from.row + row, from.column + column);
	}
	
	/*Gets the direction for two neighbouring points*/
	public static Direction getDirection(Point from, Point to){
		
		int row = to.row - from.row;
		int column = to.column - from.column;

		for(Direction d : Direction.values()){
			if(d.row == row&&d.column == column){
				return d;
			}
		}
		
		throw new IllegalArgumentException("The two points are not neighbours");
	}
}
