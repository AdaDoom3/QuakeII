# Extreme Space Combat — Scala 3 port

A distillation of the original single-file Java arcade into **Haskell-flavoured
Scala 3**, spelled in an **Ada literate style**: every identifier is written out
in full `Mixed_Case_With_Underscores`, declarations are colon-aligned, and each
construct is sealed with an `end` marker.

The irony the brief asked for: the *logic* is golfed down to comprehensions,
pattern matches and combinators, while the *surface* is as verbose and explicit
as an Ada package spec.

## What changed from the Java original

| Java                                            | Scala port |
|-------------------------------------------------|------------|
| Getter/setter pairs on every field              | public `var`s; no ceremony |
| `Object[][]` grid + manual loops                | sparse `mutable.Map[Location, Sprite]` + `for`-comprehensions |
| `null`-driven control flow                      | `Option`/`Try` for image & audio; null kept only where the bot AI's edge-sentinel needs it |
| `switch` fall-through for octant deltas         | exhaustive `match` returning a `(Δx, Δy)` tuple |
| Mutable `Location` with getters                 | immutable `final case class Location` |
| Non-local `return` inside loops                 | `scala.util.boundary` / `boundary.break` |
| `getSound(...).start()` (NPE if no device)      | `Sound.Open` returns `Option[Clip]`; missing audio degrades to silence |
| Anonymous `KeyEventDispatcher` class            | SAM lambda |

Behaviour is otherwise faithful: the same headings, the same bullet/bomb/bot
rules, the same four win/lose conditions, the same XOR-tinted blitting loop.

## Build & run

Requires a JDK and [`scala-cli`](https://scala-cli.virtuslab.org/).

```bash
# 1. generate the placeholder artwork the engine loads by class name
javac GenerateAssets.java && java GenerateAssets run

# 2. play (needs a display for the Swing window)
cd run
scala-cli run ../ExtremeSpaceCombat.scala --resource-dir .
```

`run/Settings.properties` is the `ResourceBundle` the engine reads every tunable
from (grid size, counts, healths, timings, key bindings, colours).

## Headless demonstration / screenshots

The `Demonstration` object adds an env-driven film harness — when
`ESC_CAPTURE_DIR` is unset the program behaves exactly like the original;
when set it writes PNG frames, stops after `ESC_MAX_FRAMES`, and skips the
closing dialog so it can run unattended:

```bash
ESC_CAPTURE_DIR=../shots ESC_CAPTURE_EVERY=2 ESC_MAX_FRAMES=70 \
  xvfb-run -a -s "-screen 0 800x800x24" \
  scala-cli run ../ExtremeSpaceCombat.scala --resource-dir .
```

`shots/contact_sheet.png` (built by `Montage.java`) is a sample capture of a
match in progress.
