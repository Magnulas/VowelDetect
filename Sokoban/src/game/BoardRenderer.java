package game;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;

import javax.imageio.ImageIO;
import javax.swing.JPanel;


public class BoardRenderer extends JPanel {

	private static final long serialVersionUID = 4966068621307851773L;
	
	
	// Rendering
	BufferedImage bufferedImage = new BufferedImage(5, 5, BufferedImage.TYPE_INT_RGB);
	Graphics2D bufferedGraphics;
	Dimension size;
	
	// Content
	Matrix<Tile> tileMatrix;
	HashMap<Tile, Image> boardImages;
	
	public static final int TILE_SIZE = 20;
	
	
	public BoardRenderer(Matrix<Tile> tileMatrix) {
		this.tileMatrix = tileMatrix;
		boardImages = new HashMap<Tile, Image>();
		
		try {
			boardImages.put(Tile.Box, ImageIO.read(new File(Sokoban.workingDirectory + "/resources/images/box.png")));
			boardImages.put(Tile.BoxOnGoal, ImageIO.read(new File(Sokoban.workingDirectory + "/resources/images/box_on_goal.png")));
			boardImages.put(Tile.Goal, ImageIO.read(new File(Sokoban.workingDirectory + "/resources/images/goal.png")));
			boardImages.put(Tile.Player, ImageIO.read(new File(Sokoban.workingDirectory + "/resources/images/player.png")));
			boardImages.put(Tile.PlayerOnGoal, ImageIO.read(new File(Sokoban.workingDirectory + "/resources/images/player.png")));
			boardImages.put(Tile.Empty, ImageIO.read(new File(Sokoban.workingDirectory + "/resources/images/space.png")));
			boardImages.put(Tile.Wall, ImageIO.read(new File(Sokoban.workingDirectory + "/resources/images/wall.png")));
		} catch (IOException e) {
			System.out.println("error while loading sokoban image files: " + e);
		}
		

		size = new Dimension(TILE_SIZE * tileMatrix.getWidth(), TILE_SIZE * tileMatrix.getHeight());
		bufferedImage = new BufferedImage(size.width, size.height, BufferedImage.TYPE_INT_RGB);
		bufferedGraphics = bufferedImage.createGraphics();
        this.setPreferredSize(size);
	}

	public void paint(Graphics g) {
		update(g);
	}

	public void update(Graphics g) {
		
		bufferedGraphics.clearRect(0, 0, size.width, size.height);
		draw(bufferedGraphics);
		((Graphics2D)g).drawImage(bufferedImage, 0, 0, this);
	}
	
	public void draw(Graphics2D g) {
		for(int y = 0; y < tileMatrix.getHeight(); y++) {
			for(int x = 0; x < tileMatrix.getWidth(); x++) {
				Tile tile = tileMatrix.get(y, x);
				g.drawImage(boardImages.get(tile), TILE_SIZE * x, TILE_SIZE * y, null);
			}
		}
	}
}
