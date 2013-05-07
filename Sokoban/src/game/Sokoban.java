package game;

import java.awt.BorderLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import javax.swing.JFrame;



public class Sokoban {//implements KeyListener {
	
	Matrix<Tile> tileMatrix;
	Point playerPos;
	
	JFrame frame;
	
	ArrayList<Point> undoMovesList;
	ArrayList<Point> redoMovesList;
	
	private boolean showMoves = false;
	public static String workingDirectory; 
	
    public static void main(String[] args) {
    	if(args.length<2){
    		System.out.println("working direcoty and map please");
    		return;
    	}
    	
    	workingDirectory = args[0];
    	String map = args[1];
    	Matrix<Tile> level = createTestBoardFromFile(workingDirectory + "/resources/" + map);
    	
    	Sokoban game = new Sokoban(level);
		game.createAndShowGUI();
		
//		Create a socket which we read input from and send to the game or something
//		Direction[] vals = Direction.values();
//		while(true){
//			int i = rand.nextInt(vals.length);
//			Direction dir = vals[i];
//			game.tryToMovePlayerInDirection(dir.getPositionAdjustment());
//		}
		
		InputStreamReader isReader = new InputStreamReader(System.in);
		BufferedReader bufReader = new BufferedReader(isReader);
		while(true){
		    try {	       
		        if( bufReader.ready()) {
		        	
		        	char command = (char)bufReader.read();

		        	Direction dir;
		        	switch(command){
		        		case 'U':
		        			dir = Direction.Up;
		        			break;
		        		case 'L':
		        			dir = Direction.Left;
		        			break;
		        		case 'R':
		        			dir = Direction.Right;
		        			break;
		        		case 'D':
		        			dir = Direction.Down;
		        			break;
		        		default:
		        			dir = Direction.Here;
		        			break;
		        	}
	
		        	game.tryToMovePlayerInDirection(dir.getPositionAdjustment());
		        }
		    }
		    catch (Exception e) {
		       e.printStackTrace();
		    }
		}
    }

    public Sokoban(Matrix<Tile> tileMatrix) {
    	this.tileMatrix = tileMatrix;
    	playerPos = findPlayerPosition(tileMatrix);
    	
    	undoMovesList = new ArrayList<Point>();
    	redoMovesList = new ArrayList<Point>();
    }

	public Sokoban(String level) {
		this(createTestBoardFromFile(workingDirectory + "/resources/" + level));
	}
	
	private static Matrix<Tile> createTestBoardFromFile(String filePath) {
		Matrix<Tile> m = null;
		try {
			BufferedReader lIn = new BufferedReader(new FileReader(filePath));
            int lNumRows=Integer.parseInt(lIn.readLine());
            m = Matrix.tileMatrixFromBufferedReader(lIn, lNumRows);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return m;
	}
	
    public void createAndShowGUI() {
    	
    	//Create and set up the window.
    	frame = new JFrame("Sokoban");
    	frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    	// Add board renderer
    	frame.getContentPane().add(new BoardRenderer(tileMatrix), BorderLayout.CENTER);

    	//Display the window.
    	frame.pack();
    	frame.setVisible(true);
    	
    	//frame.addKeyListener(this);
    }
    
    public Matrix<Tile> getTileMatrix() {
    	return tileMatrix;
    }
    
    public void keyTyped(KeyEvent e) {}
    public void keyReleased(KeyEvent e) {}
    
    public void keyPressed(KeyEvent e) {
        Point direction = null;
        if(e.getKeyCode() == KeyEvent.VK_UP) direction = new Point(-1, 0);
        else if(e.getKeyCode() == KeyEvent.VK_DOWN) direction = new Point(1, 0);
        else if(e.getKeyCode() == KeyEvent.VK_LEFT) direction = new Point(0, -1);
        else if(e.getKeyCode() == KeyEvent.VK_RIGHT) direction = new Point(0, 1);
        
        if(direction != null) {
        	tryToMovePlayerInDirection(direction);
        }
        else if(e.getKeyCode() == KeyEvent.VK_BACK_SPACE || e.getKeyCode() == KeyEvent.VK_SPACE) {
        	ArrayList<Point> movesList = e.getKeyCode() == KeyEvent.VK_BACK_SPACE ? undoMovesList : redoMovesList;
        	if(movesList.size() > 1) {
        		Point lastToPos = movesList.get(movesList.size() - 1);
        		Point lastFromPos = movesList.get(movesList.size() - 2);
        		moveElement(lastToPos, lastFromPos, movesList == redoMovesList, false);
        		movesList.remove(movesList.size() - 1);
        		movesList.remove(movesList.size() - 1);
        		playerPos = findPlayerPosition(tileMatrix);
        		if(frame != null)
        			frame.repaint();
        	}
        }
    }
    
    private boolean tryToMovePlayerInDirection(Point direction) {
    	boolean moveIsValid = true;
    	
    	Point newPlayerPos = new Point(playerPos.row + direction.row, playerPos.column + direction.column);
    	Tile playerNewElement = tileMatrix.get(newPlayerPos);
    	
    	if(playerNewElement == Tile.Empty || playerNewElement == Tile.Goal) {
    		moveElement(playerPos, newPlayerPos);
    		playerPos = newPlayerPos;
    		if(frame != null)
    			frame.repaint();
    	}
    	else if(playerNewElement == Tile.Box || playerNewElement == Tile.BoxOnGoal) {
    		Point boxPos = newPlayerPos;
    		Point newBoxPos = new Point(boxPos.row + direction.row, boxPos.column + direction.column);
        	Tile boxNewElement = tileMatrix.get(newBoxPos);

        	if(boxNewElement == Tile.Empty || boxNewElement == Tile.Goal) {
        		moveElement(boxPos, newBoxPos);
        		moveElement(playerPos, newPlayerPos);
        		playerPos = newPlayerPos;
        		if(frame != null)
        			frame.repaint();
        	}
        	else {
        		moveIsValid = false;
        	}
    	}
    	else {
    		moveIsValid = false;
    	}
    	
    	if(moveIsValid && showMoves) {
        	if(direction.row == 1) System.out.print("D");
        	else if(direction.row == -1) System.out.print("U");
        	else if(direction.column == 1) System.out.print("R");
        	else if(direction.column == -1) System.out.print("L");
    	}
    	
    	return moveIsValid;
    }

    private void moveElement(Point fromPos, Point toPos, boolean addToUndoList, boolean clearRedoMovesList) {
        
    	if(clearRedoMovesList)
    		redoMovesList.clear();
    		
    	if(addToUndoList) {
    		undoMovesList.add(fromPos);
    		undoMovesList.add(toPos);
    	}
    	else {
    		redoMovesList.add(fromPos);
    		redoMovesList.add(toPos);
    	}
    	
		Tile fromElement = tileMatrix.get(fromPos);
		Tile toElement = tileMatrix.get(toPos);
		
		if(fromElement == Tile.PlayerOnGoal || fromElement == Tile.BoxOnGoal)
			tileMatrix.set(fromPos, Tile.Goal);
		else tileMatrix.set(fromPos,  Tile.Empty);
		
		if(toElement == Tile.Goal) {
			if(fromElement == Tile.Player || fromElement == Tile.PlayerOnGoal)
				tileMatrix.set(toPos,  Tile.PlayerOnGoal);
			else if(fromElement == Tile.Box || fromElement == Tile.BoxOnGoal)
				tileMatrix.set(toPos, Tile.BoxOnGoal);
		}
		else {
			if(fromElement == Tile.PlayerOnGoal) tileMatrix.set(toPos, Tile.Player);
			else if(fromElement == Tile.BoxOnGoal) tileMatrix.set(toPos,  Tile.Box);
			else tileMatrix.set(toPos, fromElement);
		}
    }
    
    private void moveElement(Point fromPos, Point toPos) {
    	moveElement(fromPos, toPos, true, true);
    }
    
    private Point findPlayerPosition(Matrix<Tile> tileMatrix) {

    	HashSet<Tile> playerTiles = new HashSet<Tile>();
    	playerTiles.add(Tile.Player);
    	playerTiles.add(Tile.PlayerOnGoal);
    	return tileMatrix.findFirstMatchInSet(playerTiles);
    }
    
	public void movePlayer(String movesString, final long moveTime) {

	    final ArrayList<Point>remainingMoves = moveStringToDirectionArrayList(movesString);
		final ScheduledExecutorService worker = Executors.newSingleThreadScheduledExecutor();
		
		final Runnable makeNextMove = new Runnable() {
			public void run() {
				if(remainingMoves.size() > 0) {
					tryToMovePlayerInDirection(remainingMoves.get(0));
					remainingMoves.remove(0);
					if(remainingMoves.size() > 0) {
						worker.schedule(this, moveTime, TimeUnit.MILLISECONDS);
					}
				}
			}
		};
		worker.schedule(makeNextMove, moveTime, TimeUnit.MILLISECONDS);
	}
	
	public void movePlayer(String movesString) throws InvalidMoveException {
		ArrayList<Point> moves = moveStringToDirectionArrayList(movesString);
		for(Point direction : moves) {
			boolean validMove = tryToMovePlayerInDirection(direction);
			if(!validMove) {
				Point fromPos = playerPos;
				Point toPos = new Point(playerPos.row + direction.row, playerPos.column + direction.column);
				throw new InvalidMoveException(fromPos, toPos);
			}
		}
	}

    private ArrayList<Point> moveStringToDirectionArrayList(String movesString) {
    	ArrayList<Point> moves = new ArrayList<Point>();
    	for(int i = 0; i < movesString.length(); i++) {
    		char c = movesString.charAt(i);
    		if(c == 'L') moves.add(new Point(0, -1));
    		else if(c == 'R') moves.add(new Point(0, 1));
    		else if(c == 'U') moves.add(new Point(-1, 0));
    		else if(c == 'D') moves.add(new Point(1, 0));
    	}
    	return moves;
    }
}
