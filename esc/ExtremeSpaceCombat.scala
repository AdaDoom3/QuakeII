//  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//  Extreme_Space_Combat  —  a grid skirmish of players, bots, bombs and bullets
//
//  A distillation of the original Java arcade into Haskell-flavoured Scala 3:
//  expression-oriented, comprehension-driven, immutable where the world allows.
//  The *spelling*, however, is unapologetically Ada — every name is written in
//  full, declarations are colon-aligned, and each construct is sealed with an
//  `end` marker so the reader is never in doubt where a thought concludes.
//
//  The literate ordering proceeds outside-in: first the settings that name the
//  universe, then the geometry of the compass and the grid, then the living
//  Sprites that inhabit it, and finally the projector loop that animates them.
//  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░

import java.io.File
import java.util.ResourceBundle
import java.awt.{Color, Insets, KeyEventDispatcher, KeyboardFocusManager}
import java.awt.event.KeyEvent
import java.awt.image.BufferedImage
import java.awt.geom.AffineTransform
import javax.swing.{ImageIcon, JFrame, JLabel, JOptionPane, KeyStroke, WindowConstants}
import javax.sound.sampled.{AudioSystem, Clip, Line}
import javax.imageio.ImageIO
import scala.collection.mutable
import scala.math.{atan2, hypot, toDegrees, toRadians}
import scala.util.{boundary, Random, Try}

object Extreme_Space_Combat:

  //  ══════════════════════════════════════════════════════════════════════════
  //  Configuration  —  the single source of every tunable, drawn from a bundle.
  //  A missing key is a programming error, so we let it speak as an exception.
  //  ══════════════════════════════════════════════════════════════════════════
  object Configuration:
    private val Bundle : ResourceBundle = ResourceBundle.getBundle("Settings")

    def Text  (Identifier : String) : String = Bundle.getString(Identifier.toLowerCase)
    def Number(Identifier : String) : Int    = Text(Identifier).toInt

    val Frame_Delay     : Int    = Number("level.speed")
    val Level_Size      : Int    = Number("level.size")
    val Time_Limit      : Int    = Number("level.time.limit")
    val Tile_Size       : Int    = Number("level.tile.size")
    val Title           : String = Text  ("level.title")
    val Icon_Name       : String = Text  ("level.icon")
    val Background_Name : String = Text  ("level.background")
    val Image_Extension : String = Text  ("extension.image")
    val Audio_Extension : String = Text  ("extension.audio")

    val Text_Versus_Win  : String = Text("text.vs.win")
    val Text_Versus_Tie  : String = Text("text.vs.tie")
    val Text_Coop_Die    : String = Text("text.coop.die")
    val Text_Coop_Win    : String = Text("text.coop.win")
    val Text_Coop_Lose   : String = Text("text.coop.lose")

    val Song_Sound           : String = Text("sound.song")
    val End_Sound            : String = Text("sound.end")
    val Explode_Bomb_Sound   : String = Text("sound.explode.bomb")
    val Explode_Player_Sound : String = Text("sound.explode.player")
    val Explode_Bullet_Sound : String = Text("sound.explode.bullet")

    val Player_Health     : Int = Number("player.health")
    val Bomb_Health       : Int = Number("bomb.health")
    val Bomb_Fade         : Int = Number("bomb.fade")
    val Bot_Health        : Int = Number("bot.health")
    val Bot_Move_Timing   : Int = Number("bot.timing.move")
    val Bot_Shoot_Timing  : Int = Number("bot.timing.shoot")
    val Bullet_Timing     : Int = Number("bullet.timing")

    val Player_Count : Int = Number("player.count")
    val Shield_Count : Int = Number("shield.count")
    val Bomb_Count   : Int = Number("bomb.count")
    val Bot_Count    : Int = Number("bot.count")
  end Configuration

  import Configuration.*

  //  A wall-clock reading, named once so the rest of the program reads as prose.
  inline def Now : Long = System.currentTimeMillis

  //  ══════════════════════════════════════════════════════════════════════════
  //  Compass  —  every heading is a degree on the 360° rose; movement is octant
  //  arithmetic.  North is "up the screen", so a step North decreases Y.
  //  ══════════════════════════════════════════════════════════════════════════
  object Compass:
    val North      : Int =   0
    val East       : Int =  90
    val South      : Int = 180
    val West       : Int = 270
    val North_East : Int =  45
    val South_East : Int = 135
    val South_West : Int = 225
    val North_West : Int = 315
    val Right      : Int =  90
    val Half_Right : Int =  45
    val Full_Circle: Int = 360
    val Half_Circle: Int = 180

    //  Round an arbitrary heading to the nearest of the eight octant points.
    def Snap_To_Octant(Heading : Int) : Int =
      val Centred = ((Heading + Half_Right / 2) % Full_Circle + Full_Circle) % Full_Circle
      (Centred / Half_Right) * Half_Right
    end Snap_To_Octant

    //  The unit step that an octant heading implies, as a (Delta_X, Delta_Y).
    def Octant_Delta(Heading : Int) : (Int, Int) = Snap_To_Octant(Heading) match
      case `North`      => ( 0, -1)
      case `North_East` => ( 1, -1)
      case `East`       => ( 1,  0)
      case `South_East` => ( 1,  1)
      case `South`      => ( 0,  1)
      case `South_West` => (-1,  1)
      case `West`       => (-1,  0)
      case `North_West` => (-1, -1)
      case _            => ( 0,  0)
    end Octant_Delta

    //  A uniformly chosen cardinal heading, for the indifferent drifting Shield.
    def Random_Cardinal : Int = Vector(North, East, South, West)(Random.nextInt(4))
  end Compass

  import Compass.*

  //  ══════════════════════════════════════════════════════════════════════════
  //  Location  —  an immutable lattice point; all spatial questions are pure.
  //  ══════════════════════════════════════════════════════════════════════════
  final case class Location(X : Int, Y : Int):
    def Distance_To(Other : Location) : Double =
      hypot((Other.X - X).toDouble, (Other.Y - Y).toDouble).abs

    def Adjacent(Heading : Int) : Location =
      val Delta = Compass.Octant_Delta(Heading)
      Location(X + Delta._1, Y + Delta._2)
  end Location

  //  ══════════════════════════════════════════════════════════════════════════
  //  Grid  —  a sparse board keyed by Location.  An in-bounds empty square reads
  //  as null; an out-of-bounds query *raises*, which is precisely the sentinel
  //  the bot's ray-casts lean on to know they have struck the edge of the world.
  //  ══════════════════════════════════════════════════════════════════════════
  final class Grid[Element >: Null <: AnyRef](val Rows : Int, val Columns : Int):
    private val Cells : mutable.Map[Location, Element] = mutable.Map.empty

    def Invalid(Where : Location) : Boolean =
      Where.X < 0 || Where.X >= Rows || Where.Y < 0 || Where.Y >= Columns

    def Get(Where : Location) : Element =
      if Invalid(Where) then throw IllegalStateException("off the board")
      else Cells.getOrElse(Where, null)

    def Add   (Where : Location, What : Element) : Unit = Cells.update(Where, What)
    def Remove(Where : Location)                 : Unit = Cells.remove(Where)

    def Everything                  : List[Element] = Cells.values.toList
    def Of_Kind(Kind : Class[?])    : List[Element] = Everything.filter(_.getClass == Kind)
    def Count  (Kind : Class[?])    : Int           = Of_Kind(Kind).length
    def Any_Of (Kind : Class[?])    : Element        = Everything.find(_.getClass == Kind).orNull

    def Random_Empty : Location =
      val Vacancies =
        for
          X <- 0 until Rows
          Y <- 0 until Columns
          if !Cells.contains(Location(X, Y))
        yield Location(X, Y)
      if Vacancies.isEmpty then throw IllegalArgumentException("no room left")
      else Vacancies(Random.nextInt(Vacancies.length))
    end Random_Empty
  end Grid

  //  ══════════════════════════════════════════════════════════════════════════
  //  Sprite  —  the living cell.  Every actor on the board is one of these; the
  //  base supplies placement, motion, health and the projector's view of it,
  //  while `Act` and `Die` are the two hooks subclasses override to come alive.
  //  ══════════════════════════════════════════════════════════════════════════
  class Sprite:
    var Position          : Location              = null
    var Home_Grid         : Grid[Sprite]          = null
    var Facing_Direction  : Int                   = North
    var Tint              : Color                  = null
    private var Picture       : Option[BufferedImage] = None
    private var Current_Health: Int                    = -1
    private var Stored_Health : Int                    =  0
    private var Fuse          : Int                    =  0

    //  Subclass hooks; the base actor merely exists.
    def Act() : Unit = ()
    def Die() : Unit = ()

    //  The file from which this kind of Sprite paints itself.
    def Artwork_Name : String = getClass.getSimpleName.toLowerCase

    def Image_Or_Null : BufferedImage = Picture.orNull

    //  Invincibility is a health of "below zero, yet placed"; the Shield lives
    //  here permanently, the explosions transiently.
    def Is_Invincible : Boolean = Home_Grid != null && Current_Health <= 0

    def Set_Invincible(Value : Boolean) : Unit =
      if Value then
        Stored_Health  = Current_Health
        Current_Health = -1
      else Current_Health = Stored_Health

    def Set_Health(Amount : Int) : Unit =
      if Amount < 0 || Is_Invincible then throw IllegalArgumentException("ill health")
      else Current_Health = Amount

    def Damage(Amount : Int) : Unit =
      if Current_Health > 0 then
        Current_Health -= Amount
        if Current_Health <= 0 then Remove()

    //  Schedule self-erasure after `Lifespan` further steps (the explosions).
    def Fade(Lifespan : Int) : Unit = Fuse = Lifespan

    def Set_Direction(Heading : Int) : Unit =
      Facing_Direction = ((Heading % Full_Circle) + Full_Circle) % Full_Circle

    //  One tick of life: age the fuse, then act.
    def Step() : Unit =
      if Fuse > 0 then
        Fuse -= 1
        if Fuse == 0 then Remove()
      Act()

    def Remove() : Unit =
      if Home_Grid != null && (Home_Grid.Get(Position) ne this) then
        throw IllegalStateException("phantom removal")
      Home_Grid.Remove(Position)
      Die()
      Home_Grid = null
      Position  = null

    //  Place onto a board at a chosen square, evicting any prior occupant; the
    //  artwork is loaded lazily on first placement.
    def Put(Level : Grid[Sprite], Square : Location) : Unit =
      if Level != null && Level.Invalid(Square) then throw IllegalStateException("off the board")
      val Occupant = Level.Get(Square)
      if Occupant != null then Occupant.Remove()
      Level.Add(Square, this)
      Position  = Square
      Home_Grid = Level
      if Picture.isEmpty then
        Picture = Try(Option(ImageIO.read(new File(Artwork_Name + Image_Extension)))).toOption.flatten

    //  Place onto a board at any free square.
    def Put(Level : Grid[Sprite]) : Unit = Put(Level, Level.Random_Empty)

    //  Slide to an adjacent (already-validated) square, evicting any occupant.
    def Move(Square : Location) : Unit =
      if Home_Grid == null || Home_Grid.Invalid(Square) then throw IllegalStateException("homeless move")
      Home_Grid.Remove(Position)
      val Occupant = Home_Grid.Get(Square)
      if Occupant != null then Occupant.Remove()
      Put(Home_Grid, Square)

    //  The square one step along `Heading`, raising if it leaves the board.
    def Adjacent_Square(Heading : Int) : Location =
      val Result = Position.Adjacent(Heading)
      if Home_Grid.Invalid(Result) then throw IllegalStateException("off the board")
      else Result

    def Adjacent_Square() : Location = Adjacent_Square(Facing_Direction)

    //  The octant heading that points from here toward `Target`.
    def Direction_Toward(Target : Location) : Int =
      val Delta_X  = Target.X - Position.X
      val Delta_Y  = Target.Y - Position.Y
      val Angle    = toDegrees(atan2(-Delta_Y.toDouble, Delta_X.toDouble)).toInt
      val Bearing  = ((Right - Angle + Half_Right / 2) % Full_Circle + Full_Circle) % Full_Circle
      (Bearing / Half_Right) * Half_Right
  end Sprite

  //  ── The fleeting marks left by detonations; distinct types so that bullets
  //     may recognise (and pass harmlessly through) one another's wreckage. ──
  final class Bullet_Explosion       extends Sprite
  final class Bomb_Explosion_Middle  extends Sprite
  final class Bomb_Explosion_Corner  extends Sprite
  final class Bomb_Explosion_Side    extends Sprite

  //  ══════════════════════════════════════════════════════════════════════════
  //  Bullet  —  a self-propelled cell that advances on its own clock, passing
  //  through kindred fire and detonating against anything solid.
  //  ══════════════════════════════════════════════════════════════════════════
  final class Bullet(Initial_Direction : Int) extends Sprite:
    Set_Direction(Initial_Direction)
    private var Last_Advance : Long = Now - Bullet_Timing

    override def Put(Level : Grid[Sprite], Square : Location) : Unit =
      super.Put(Level, Square)

    private def Explode() : Unit =
      val Saved_Grid   = Home_Grid
      val Saved_Square = Position
      val Wreckage     = new Bullet_Explosion
      Wreckage.Set_Direction(Facing_Direction)
      Remove()
      Wreckage.Put(Saved_Grid, Saved_Square)
      Wreckage.Fade(3)

    override def Act() : Unit =
      if Last_Advance + Bullet_Timing < Now then
        try
          val Ahead    = Adjacent_Square()
          val Occupant = Home_Grid.Get(Ahead)
          Occupant match
            case null | (_ : Bullet) | (_ : Bullet_Explosion) =>
              Move(Ahead)
              Last_Advance = Now
            case _ =>
              val Victim = Occupant
              if !Victim.Is_Invincible then
                Sound.Play(Explode_Bullet_Sound)
                Victim.Damage(1)
              Explode()
        catch case _ : Throwable => Explode()
  end Bullet

  //  ══════════════════════════════════════════════════════════════════════════
  //  Bomb  —  inert until killed, whereupon it blooms into a cross of sides,
  //  corners and a central flash, each scheduled to fade.
  //  ══════════════════════════════════════════════════════════════════════════
  final class Bomb extends Sprite:
    Set_Health(Bomb_Health)

    override def Die() : Unit =
      Sound.Play(Explode_Bomb_Sound)
      for Quadrant <- North to West by Right do
        val Side   = new Bomb_Explosion_Side
        val Corner = new Bomb_Explosion_Corner
        Side  .Set_Direction(Quadrant + Right)
        Corner.Set_Direction(Quadrant + Right)
        Try(Side  .Put(Home_Grid, Adjacent_Square(Right      + Quadrant)))
        Try(Corner.Put(Home_Grid, Adjacent_Square(Half_Right + Quadrant)))
        Side  .Fade(Bomb_Fade)
        Corner.Fade(Bomb_Fade)
      val Middle = new Bomb_Explosion_Middle
      Middle.Put(Home_Grid, Position)
      Middle.Fade(Bomb_Fade)
  end Bomb

  //  ══════════════════════════════════════════════════════════════════════════
  //  Bot  —  the autonomous hunter.  On its own move- and shoot-clocks it first
  //  tries to `Attack` a player it can already line up against, and failing that
  //  `Manoeuvre`s toward the most promising firing square.  Both routines cast
  //  rays outward from each player and lean on the grid's edge-raising sentinel
  //  to terminate cleanly.
  //  ══════════════════════════════════════════════════════════════════════════
  final class Bot extends Sprite:
    Set_Health(Bot_Health)
    private var May_Move      : Boolean = false
    private var May_Shoot     : Boolean = false
    private var Last_Move_At  : Long    = Now - Bot_Move_Timing
    private var Last_Shoot_At : Long    = Now - Bot_Shoot_Timing

    override def Die() : Unit = Sound.Play(Explode_Player_Sound)

    //  A private break used to abandon a ray the moment it meets an obstacle.
    private final class Blocked extends RuntimeException

    private def Attack() : Boolean = boundary:
      for Target <- Home_Grid.Of_Kind(classOf[Player]) do
        var Heading  = (Target.Facing_Direction + Right) % Full_Circle
        var Sweeping = true
        while Sweeping do
          var Current = Target.Position
          var Range   = 0
          try
            while true do
              Current = Current.Adjacent(Heading)
              Range  += 1
              if Range > 1 && Position == Current then
                if Facing_Direction == Direction_Toward(Target.Position) then
                  if May_Shoot then
                    new Bullet(Facing_Direction).Put(Home_Grid, Adjacent_Square())
                    boundary.break(true)
                else if Range > 2 then
                  if May_Move then
                    Set_Direction(Direction_Toward(Target.Position))
                    Move(Adjacent_Square())
                  boundary.break(true)
              else if Home_Grid.Get(Current) != null then throw new Blocked
          catch case _ : Throwable => ()
          if Heading == Target.Facing_Direction then Sweeping = false
          else Heading = (Heading + Right) % Full_Circle
      false
    end Attack

    private def Manoeuvre() : Boolean =
      if May_Move then
        var Nearest : Location = null
        var Least   : Double   = Level_Size.toDouble
        for Target <- Home_Grid.Of_Kind(classOf[Player]) do
          var Heading = (Target.Facing_Direction + Right) % Full_Circle
          while Heading != Target.Facing_Direction do
            var Current = Target.Position
            var Range   = 0
            try
              while true do
                Current = Current.Adjacent(Heading)
                if (Home_Grid.Get(Current) ne this) && Home_Grid.Get(Current) != null then throw new Blocked
                Range += 1
                if Range > 2 && Current.Distance_To(Position) < Least then
                  Least   = Current.Distance_To(Position)
                  Nearest = Current
            catch case _ : Throwable => ()
            Heading = (Heading + Right) % Full_Circle
        if Nearest != null then
          Least = Level_Size.toDouble
          var Step_Square : Location = null
          var Octant      = North
          while Octant != Full_Circle do
            try
              val Candidate = Adjacent_Square(Octant)
              if Home_Grid.Get(Candidate) == null && Nearest.Distance_To(Candidate) < Least then
                Least       = Nearest.Distance_To(Candidate)
                Step_Square = Candidate
            catch case _ : Throwable => ()
            Octant += Half_Right
          if Step_Square != null then
            val Bearing = Direction_Toward(Step_Square)
            Set_Direction(if Bearing % Right != 0 then Bearing - Half_Right else Bearing)
            Move(Step_Square)
            return true
      false
    end Manoeuvre

    override def Act() : Unit =
      if Last_Move_At  + Bot_Move_Timing  < Now then { May_Move  = true; Last_Move_At  = Now }
      if Last_Shoot_At + Bot_Shoot_Timing < Now then { May_Shoot = true; Last_Shoot_At = Now }
      if !Attack() then Manoeuvre()
      May_Move  = false
      May_Shoot = false
  end Bot

  //  ══════════════════════════════════════════════════════════════════════════
  //  Shield  —  an indestructible block of scenery, idly facing some direction.
  //  ══════════════════════════════════════════════════════════════════════════
  final class Shield extends Sprite:
    Set_Invincible(true)
    Set_Direction(Compass.Random_Cardinal)
  end Shield

  //  ══════════════════════════════════════════════════════════════════════════
  //  Player  —  a human-driven Sprite.  Its constructor reads its key bindings
  //  and tint from the bundle and installs a global dispatcher that walks the
  //  four headings and fires along the facing.  Pressing a direction both turns
  //  and (if the square is free) advances; the shoot key spawns a Bullet ahead.
  //  ══════════════════════════════════════════════════════════════════════════
  final class Player(val Identifier : Int) extends Sprite:
    override def Artwork_Name : String = "player"

    private val Move_Keys : Vector[String] = Vector("up", "right", "down", "left")
      .map(Suffix => Configuration.Text(s"player.$Identifier.$Suffix").toUpperCase)
    private val Shoot_Key : String         = Configuration.Text(s"player.$Identifier.shoot").toUpperCase
    private val Pressed   : Array[Boolean] = Array.fill(Move_Keys.length + 1)(false)

    Set_Health(Player_Health)
    Tint = Try(classOf[Color]
      .getField(Configuration.Text(s"player.$Identifier.color").toUpperCase)
      .get(null).asInstanceOf[Color]).getOrElse(null)

    private val Dispatcher : KeyEventDispatcher = (Event : KeyEvent) =>
      val Key = KeyStroke.getKeyStrokeForEvent(Event).toString
      var Heading = North
      for Index <- Move_Keys.indices do
        if Key == "pressed " + Move_Keys(Index) then
          if !Pressed(Index) then
            Pressed(Index) = true
            val Next = Try(Adjacent_Square(Heading)).toOption.orNull
            if Next != null && !Home_Grid.Invalid(Next) && Home_Grid.Get(Next) == null then Move(Next)
            Set_Direction(Heading)
        else if Key == "released " + Move_Keys(Index) then Pressed(Index) = false
        Heading = (Right + Heading) % Full_Circle
      if Key == "pressed " + Shoot_Key then
        if !Pressed(Move_Keys.length) then
          Pressed(Move_Keys.length) = true
          try
            val Square = Adjacent_Square()
            if Home_Grid.Get(Square) == null then new Bullet(Facing_Direction).Put(Home_Grid, Square)
          catch case _ : Throwable => ()
      else if Key == "released " + Shoot_Key then Pressed(Move_Keys.length) = false
      false

    KeyboardFocusManager.getCurrentKeyboardFocusManager.addKeyEventDispatcher(Dispatcher)

    override def Die() : Unit =
      Sound.Play(Explode_Player_Sound)
      KeyboardFocusManager.getCurrentKeyboardFocusManager.removeKeyEventDispatcher(Dispatcher)
  end Player

  //  ══════════════════════════════════════════════════════════════════════════
  //  Sound  —  audio is a courtesy, never a contract: every failure (no device,
  //  no file, no mixer) collapses silently to None so the game plays on in mute.
  //  ══════════════════════════════════════════════════════════════════════════
  object Sound:
    def Open(Path : String) : Option[Clip] =
      Try {
        val Channel = AudioSystem.getLine(new Line.Info(classOf[Clip])).asInstanceOf[Clip]
        Channel.open(AudioSystem.getAudioInputStream(new File(Path + Audio_Extension)))
        Channel
      }.toOption

    def Play(Path : String) : Unit = Open(Path).foreach(_.start())
  end Sound

  //  ══════════════════════════════════════════════════════════════════════════
  //  Demonstration  —  a thin, env-driven harness so the match can be filmed
  //  without a human at the keys.  When ESC_CAPTURE_DIR is set, the projector
  //  writes PNG frames there, stops after ESC_MAX_FRAMES, and skips the closing
  //  dialog.  When it is unset, the program behaves exactly as the original.
  //  ══════════════════════════════════════════════════════════════════════════
  object Demonstration:
    val Capture_Directory : Option[File] =
      Option(System.getenv("ESC_CAPTURE_DIR")).map { Name =>
        val Folder = new File(Name); Folder.mkdirs(); Folder
      }
    val Capture_Stride : Int         = Option(System.getenv("ESC_CAPTURE_EVERY")).map(_.toInt).getOrElse(5)
    val Capture_Limit  : Option[Int] = Option(System.getenv("ESC_MAX_FRAMES")).map(_.toInt)

    def Capture(Buffer : BufferedImage, Frame : Int) : Unit =
      for Folder <- Capture_Directory if Frame % Capture_Stride == 0 do
        ImageIO.write(Buffer, "png", new File(Folder, f"frame_$Frame%04d.png"))

    def Filming : Boolean = Capture_Directory.isDefined
  end Demonstration

  //  ══════════════════════════════════════════════════════════════════════════
  //  Main  —  raise the window, populate the board, then run the projector: each
  //  frame steps every Sprite, paints them (tinted by XOR) onto the back buffer
  //  over the background, and tests the four ways a match can end.
  //  ══════════════════════════════════════════════════════════════════════════
  def main(Arguments : Array[String]) : Unit =
    val Level      = new Grid[Sprite](Level_Size, Level_Size)
    val Background = ImageIO.read(new File(Background_Name + Image_Extension))
    val Buffer     = new BufferedImage(Level_Size * Tile_Size, Level_Size * Tile_Size, Background.getType)
    val Canvas     = Buffer.createGraphics()
    val Window     = new JFrame(Title)

    Window.setVisible(true)
    val Border : Insets = Window.getInsets
    Window.setSize(Level_Size * Tile_Size + Border.left + Border.right, Level_Size * Tile_Size + Border.top)
    Window.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE)
    Window.setResizable(false)
    Try(Window.setIconImage(new ImageIcon(ImageIO.read(new File(Icon_Name + Image_Extension))).getImage))

    for _ <- 1 to Player_Count do new Player({ Spawned_Players += 1; Spawned_Players }).Put(Level)
    for _ <- 1 to Shield_Count do new Shield().Put(Level)
    for _ <- 1 to Bomb_Count   do new Bomb().Put(Level)
    for _ <- 1 to Bot_Count    do new Bot().Put(Level)

    val Song              = Sound.Open(Song_Sound)
    val Initial_Bot_Count = Bot_Count
    Song.foreach(_.start())

    val Label = new JLabel(new ImageIcon(Buffer))
    Window.add(Label)

    var Outcome  = ""
    var Frames   = 0
    var Running  = true
    val Started  = Now
    var Last     = Now - Frame_Delay
    Canvas.drawImage(Background, 0, 0, null)

    while Running do
      if Last + Frame_Delay < Now then
        if (Now - Started) / 1000 > Time_Limit then
          Outcome = if Level.Count(classOf[Bot]) > 0 then Text_Coop_Lose else Text_Versus_Tie
          Running = false
        else if Initial_Bot_Count > 0 && Level.Count(classOf[Bot]) <= 0 then
          Outcome = Text_Coop_Win + (Now - Started) / 1000 + " seconds"
          Running = false
        else if Initial_Bot_Count <= 0 && Level.Count(classOf[Player]) < 2 then
          val Winner = Level.Any_Of(classOf[Player]).asInstanceOf[Player]
          Outcome = Text_Versus_Win + Winner.Identifier
          Running = false
        else if Initial_Bot_Count > 0 && Level.Count(classOf[Player]) <= 0 then
          Outcome = Text_Coop_Die
          Running = false
        else
          for Actor <- Level.Everything do Try(Actor.Step())
          Last = Now
          for Actor <- Level.Everything do
            if Actor.Tint != null then Canvas.setXORMode(Actor.Tint)
            val Transform = AffineTransform.getTranslateInstance(
              Actor.Position.X * Tile_Size, Actor.Position.Y * Tile_Size)
            Transform.rotate(toRadians(Actor.Facing_Direction), Tile_Size / 2.0, Tile_Size / 2.0)
            Canvas.drawImage(Actor.Image_Or_Null, Transform, null)
            if Actor.Tint != null then Canvas.setPaintMode()
          Frames += 1
          Demonstration.Capture(Buffer, Frames)
          Label.repaint(0, 0, 0, Window.getWidth, Window.getHeight)
          Window.setVisible(true)
          Canvas.drawImage(Background, 0, 0, null)
          if Demonstration.Capture_Limit.exists(Frames >= _) then Running = false
    end while

    Song.foreach(_.stop())
    Sound.Play(End_Sound)
    if !Demonstration.Filming then
      JOptionPane.showMessageDialog(null, Outcome, Title, JOptionPane.PLAIN_MESSAGE, null)
    Window.dispose()
    if Demonstration.Filming then
      println(s"[demonstration] outcome: $Outcome  (frames rendered: $Frames)")
      System.exit(0)
  end main

  //  A monotonic counter so each Player is born with the next ordinal identity.
  private var Spawned_Players : Int = 0

end Extreme_Space_Combat
