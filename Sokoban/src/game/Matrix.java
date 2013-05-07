package game;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.HashSet;

public class Matrix <T> {
	
	private int width;
	private int height;
	
	private HashMap<Point, T> elements = new HashMap<Point, T>();
	
	public Matrix() {}
	
	public Matrix(T[][] m) {
		for(int row = 0; row < m.length; row++) {
			for(int column = 0; column < m[row].length; column++) {
				set(row, column, m[row][column]);
			}
		}
	}
	
	public Matrix(Matrix<T> m) {
		for(int row = 0; row < m.getHeight(); row++) {
			for(int column = 0; column < m.getWidth(); column++) {
				Point p = new Point(row, column);
				if(m.get(p) != null) {
					set(p, m.get(p));
				}
			}
		}
	}

	public void set(int row, int column, T element) {
		set(new Point(row, column), element);
	}

	public void set(Point p, T element) {
		elements.put(p, element);
		width = Math.max(width, p.column+1);
		height = Math.max(height, p.row+1);
	}
	
	public T get(int row, int column) {
		return get(new Point(row, column));
	}

	public T get(Point p) {
		return elements.get(p);
	}
	
	public boolean withinBounds(Point p) {
		return p.row >= 0 && p.column >= 0 && p.row < height && p.column < width;
	}

	public int getWidth() {
		return width;
	}
	
	public int getHeight() {
		return height;
	}

	public Point findFirstMatchInSet(HashSet<T> elementSet) {
		for(int row = 0; row < height; row++) {
			for(int column = 0; column < width; column++) {
				Point p = new Point(row, column);
				if(elementSet.contains(get(p))) {
					return p;
				}
			}
		}
		return null;
	}
	
	
	public void fillSpacesWithElement(T element) {
		for(int row = 0; row < height; row++) {
			for(int column = 0; column < width; column++) {
				Point p = new Point(row, column);
				if(get(p) == null) set(p, element);
			}
		}
	}
	
	public static Matrix<Tile> tileMatrixFromBufferedReader(BufferedReader reader, int numberOfRows) {
		
		Matrix<Tile> tileMatrix = new Matrix<Tile>();
		
		for(int row = 0; row < numberOfRows; row++)
		{
		    String line = null;
			try {
				line = reader.readLine();
			} catch (IOException e) {
				e.printStackTrace();
			}
		    
		    for(int column = 0; column < line.length(); column++) {
		    	tileMatrix.set(row, column, Tile.charAsEnum(line.charAt(column)));
		    }
		}
		
		tileMatrix.fillSpacesWithElement(Tile.Wall);
		
		return tileMatrix;
	}
	
	public HashSet<T> adjacentElements(Point p) {
		HashSet<T> elements = new HashSet<T>();
		for(int dy = -1; dy < 2; dy++) {
			for(int dx = -1; dx < 2; dx++) {
				if(!(dx == 0 && dy == 0)) {
					Point np = new Point(p.row + dy, p.column + dx);
					T e = get(np);
					if(e != null) elements.add(e);
				}
			}
		}
		return elements;
	}
}
