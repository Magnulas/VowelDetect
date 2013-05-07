package game;

public class InvalidMoveException extends Exception {
	
	private static final long serialVersionUID = 1L;
	
	public final Point fromPos;
	public final Point toPos;
	
	public InvalidMoveException(Point fromPos, Point toPos) {
		this.fromPos = fromPos;
		this.toPos = toPos;
	}
	
	public String toString() {
		return "Invalid move from " + fromPos + " to " + toPos;
	}
}