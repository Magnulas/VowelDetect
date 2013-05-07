package game;

import java.io.BufferedReader;
import java.io.IOException;

public enum Tile {
	Empty (' '),
	Goal ('.'),
	Player ('@'),
	PlayerOnGoal ('+'),
	Box ('$'),
	BoxOnGoal ('*'),
	Wall ('#');

	private char identifer;
	
	Tile(char identifer){
		this.identifer = identifer;
	}
	
	public char identifer(){
		return identifer;
	}
	
	public static Tile charAsEnum(char data){
		
		for(Tile t : Tile.values()){
			if(t.identifer()==data) return t;
		}
		
		throw new IllegalArgumentException("Given data '" + data + "' is not a valid tile");
	}
	
	public static Tile[][] parseTileMatrix(BufferedReader reader, int numberOfRows) {

		/*Build the puzzle*/
		Tile[][] tiles = new Tile[numberOfRows][];
		
		for(int i=0;i<numberOfRows;i++)
		{
		    String line;
			try {
				line = reader.readLine();
			} catch (IOException e) {
				e.printStackTrace();
				return null;
			}
			
		    tiles[i] = new Tile[line.length()];
		    
		    for(int j = 0;j<line.length();j++) {
		    	tiles[i][j] = Tile.charAsEnum(line.charAt(j));
		    }
		}
		
		return tiles;
	}
	
}