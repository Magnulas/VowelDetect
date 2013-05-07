package game;

import java.util.ArrayList;
import java.util.List;

public class Point { 
	
    public final int row;
    public final int column; 
   
    public Point(int row, int column) {
        this.row = row;
        this.column = column;
    }
    
    public int hashCode() { return row * 10000 + column; }
    public String toString() {  return "(" + row + ", " + column + ")"; }
    
    public boolean equals(Object obj) {
    	if(obj.getClass() != this.getClass()) return false;
    	Point otherPoint = (Point)obj;
    	return otherPoint.row == this.row && otherPoint.column == this.column;
    }

	public static List<Point> neighborPoints(Point p) {
		ArrayList<Point> points = new ArrayList<Point>();
		for(int dy = -1; dy < 2; dy++) {
			for(int dx = -1; dx < 2; dx++) {
				if(!(dx == 0 && dy == 0)) {
					points.add(new Point(p.row + dy, p.column + dx));
				}
			}
		}
		return points;
	}
}