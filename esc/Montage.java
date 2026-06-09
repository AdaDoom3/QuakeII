import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;

// Tiles selected gameplay frames into a single captioned contact sheet.
public class Montage {
    public static void main(String[] a) throws Exception {
        String[] files = {
            "shots/frame_0008.png", "shots/frame_0014.png",
            "shots/frame_0020.png", "shots/frame_0030.png"
        };
        String[] caps = {"t = 0.56 s", "t = 0.98 s", "t = 1.40 s", "t = 2.10 s"};
        int n = files.length, pad = 14, label = 26, cols = 2;
        int rows = (n + cols - 1) / cols;
        BufferedImage first = ImageIO.read(new File(files[0]));
        int w = first.getWidth(), h = first.getHeight();
        int W = cols * w + (cols + 1) * pad;
        int H = rows * (h + label) + (rows + 1) * pad + 40;
        BufferedImage out = new BufferedImage(W, H, BufferedImage.TYPE_INT_RGB);
        Graphics2D g = out.createGraphics();
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
        g.setColor(new Color(14, 14, 22)); g.fillRect(0, 0, W, H);
        g.setColor(new Color(180, 220, 255)); g.setFont(new Font("SansSerif", Font.BOLD, 20));
        g.drawString("Extreme Space Combat — Scala 3 port, headless capture", pad, 28);
        for (int i = 0; i < n; i++) {
            int c = i % cols, r = i / cols;
            int x = pad + c * (w + pad);
            int y = 40 + pad + r * (h + label + pad);
            BufferedImage img = ImageIO.read(new File(files[i]));
            g.drawImage(img, x, y, null);
            g.setColor(new Color(60, 70, 90)); g.drawRect(x, y, w - 1, h - 1);
            g.setColor(new Color(220, 230, 245)); g.setFont(new Font("Monospaced", Font.PLAIN, 14));
            g.drawString(caps[i], x + 4, y + h + 18);
        }
        g.dispose();
        ImageIO.write(out, "png", new File("shots/contact_sheet.png"));
        System.out.println("wrote shots/contact_sheet.png " + W + "x" + H);
    }
}
