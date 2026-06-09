import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.util.Random;
import javax.imageio.ImageIO;

// Generates the tile + background artwork the game loads by class name.
// Tiles point "up" (North) so the engine's rotation reads correctly.
public class GenerateAssets {
    static final int S = 28;          // tile size (matches level.tile.size)
    static final int GRID = 16;       // level.size

    public static void main(String[] a) throws Exception {
        File dir = new File(a.length > 0 ? a[0] : "run");
        dir.mkdirs();

        ship(dir, "player", new Color(235, 235, 245));   // tinted via XOR at runtime
        ship(dir, "bot",    new Color(220, 60, 60));

        disc(dir, "bullet", new Color(255, 230, 70), 0.30);
        bomb(dir, "bomb");
        block(dir, "shield", new Color(120, 150, 200));

        burst(dir, "bullet_explosion",      new Color(255, 170, 40), 0.45);
        burst(dir, "bomb_explosion_middle", new Color(255, 250, 200), 0.95);
        burst(dir, "bomb_explosion_corner", new Color(255, 140, 30), 0.70);
        burst(dir, "bomb_explosion_side",   new Color(255, 100, 20), 0.80);

        background(dir, "background");
        icon(dir, "icon");
        System.out.println("assets written to " + dir.getAbsolutePath());
    }

    static Graphics2D g2(BufferedImage img) {
        Graphics2D g = img.createGraphics();
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        return g;
    }
    static BufferedImage tile() { return new BufferedImage(S, S, BufferedImage.TYPE_INT_ARGB); }
    static void save(File dir, String name, BufferedImage img) throws Exception {
        ImageIO.write(img, "png", new File(dir, name + ".png"));
    }

    static void ship(File dir, String name, Color c) throws Exception {
        BufferedImage img = tile(); Graphics2D g = g2(img);
        Polygon p = new Polygon(new int[]{S/2, 3, S-3}, new int[]{2, S-4, S-4}, 3);
        g.setColor(c); g.fillPolygon(p);
        g.setColor(c.darker()); g.setStroke(new BasicStroke(1.6f)); g.drawPolygon(p);
        g.setColor(new Color(40, 40, 60)); g.fillOval(S/2 - 3, S/2 - 2, 6, 6); // cockpit
        g.dispose(); save(dir, name, img);
    }

    static void disc(File dir, String name, Color c, double r) throws Exception {
        BufferedImage img = tile(); Graphics2D g = g2(img);
        int d = (int)(S * r), o = (S - d) / 2;
        g.setColor(c); g.fillOval(o, o, d, d);
        g.setColor(Color.WHITE); g.fillOval(o + d/3, o + d/3, d/3, d/3);
        g.dispose(); save(dir, name, img);
    }

    static void bomb(File dir, String name) throws Exception {
        BufferedImage img = tile(); Graphics2D g = g2(img);
        g.setColor(new Color(35, 35, 40)); g.fillOval(4, 6, S-8, S-8);
        g.setColor(new Color(90, 90, 100)); g.setStroke(new BasicStroke(1.5f)); g.drawOval(4, 6, S-8, S-8);
        g.setColor(new Color(200, 80, 40)); g.setStroke(new BasicStroke(2f));
        g.drawLine(S/2, 6, S/2 + 4, 1);                          // fuse
        g.setColor(new Color(255, 200, 60)); g.fillOval(S/2 + 2, 0, 4, 4); // spark
        g.dispose(); save(dir, name, img);
    }

    static void block(File dir, String name, Color c) throws Exception {
        BufferedImage img = tile(); Graphics2D g = g2(img);
        g.setColor(c); g.fillRoundRect(2, 2, S-4, S-4, 6, 6);
        g.setColor(c.brighter()); g.setStroke(new BasicStroke(2f)); g.drawRoundRect(2, 2, S-4, S-4, 6, 6);
        g.setColor(c.darker());
        for (int i = 6; i < S - 4; i += 6) g.drawLine(i, 4, i, S - 5);     // hatching
        g.dispose(); save(dir, name, img);
    }

    static void burst(File dir, String name, Color c, double r) throws Exception {
        BufferedImage img = tile(); Graphics2D g = g2(img);
        int n = 8; double cx = S/2.0, cy = S/2.0, out = S/2.0 * r, in = out * 0.45;
        Polygon star = new Polygon();
        for (int i = 0; i < n * 2; i++) {
            double ang = Math.PI * i / n, rad = (i % 2 == 0) ? out : in;
            star.addPoint((int)(cx + Math.cos(ang) * rad), (int)(cy + Math.sin(ang) * rad));
        }
        g.setColor(c); g.fillPolygon(star);
        g.setColor(Color.WHITE); g.fillOval((int)(cx-2), (int)(cy-2), 4, 4);
        g.dispose(); save(dir, name, img);
    }

    static void background(File dir, String name) throws Exception {
        int W = GRID * S;
        BufferedImage img = new BufferedImage(W, W, BufferedImage.TYPE_INT_RGB);
        Graphics2D g = g2(img);
        g.setPaint(new GradientPaint(0, 0, new Color(8, 10, 26), 0, W, new Color(20, 8, 34)));
        g.fillRect(0, 0, W, W);
        Random rnd = new Random(42);
        for (int i = 0; i < 360; i++) {
            int x = rnd.nextInt(W), y = rnd.nextInt(W), b = 120 + rnd.nextInt(135);
            g.setColor(new Color(b, b, Math.min(255, b + 20)));
            g.fillOval(x, y, 1 + rnd.nextInt(2), 1 + rnd.nextInt(2));
        }
        g.setColor(new Color(255, 255, 255, 18));            // faint grid
        for (int i = 0; i <= GRID; i++) { g.drawLine(i*S, 0, i*S, W); g.drawLine(0, i*S, W, i*S); }
        g.dispose(); save(dir, name, img);
    }

    static void icon(File dir, String name) throws Exception {
        BufferedImage img = new BufferedImage(32, 32, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = g2(img);
        g.setColor(new Color(10, 12, 30)); g.fillRect(0, 0, 32, 32);
        g.setColor(new Color(120, 200, 255));
        g.fillPolygon(new int[]{16, 5, 27}, new int[]{3, 28, 28}, 3);
        g.dispose(); save(dir, name, img);
    }
}
