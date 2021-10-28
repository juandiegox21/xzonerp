# XZone Roleplay

[![sampctl](https://img.shields.io/badge/sampctl-xzonerp-2f2f2f.svg?style=for-the-badge)](https://github.com/juandiegox21/xzonerp)

<!--
Short description of your library, why it's useful, some examples, pictures or
videos. Link to your forum release thread too.

Remember: You can use "forumfmt" to convert this readme to forum BBCode!

What the sections below should be used for:

`## Installation`: Leave this section un-edited unless you have some specific
additional installation procedure.

`## Testing`: Whether your library is tested with a simple `main()` and `print`,
unit-tested, or demonstrated via prompting the player to connect, you should
include some basic information for users to try out your code in some way.

And finally, maintaining your version number`:

* Follow [Semantic Versioning](https://semver.org/)
* When you release a new version, update `VERSION` and `git tag` it
* Versioning is important for sampctl to use the version control features

Happy Pawning!
-->

<img width="300" src="https://logodix.com/logo/304442.png"/>

XZRP is a GameMode for San Andreas Multi-Player using PAWN (Based on C).

In the past San Andreas Multi-Player gamemodes used to be written in one file, which made it really hard to mantain and to version it using git.

Fortunately this has changed over the years and we are now able to make our code more modular. This is a Roleplay GameMode using:

<img width="100" src="https://d1.awsstatic.com/asset-repository/products/amazon-rds/1024px-MySQL.ff87215b43fd7292af172e2a5d9b844217262571.png" alt="MySQL" title="MySQL" />
&nbsp;&nbsp;&nbsp;&nbsp;
<img width="20" src="https://upload.wikimedia.org/wikipedia/commons/7/71/Pawn_logo.png" alt="Pawn" title="Pawn" />

And the power of [PawnPlus][pawnplusref]

---

What this Gamemode Contains:

- Player information is stored, coordinates, passwords, etc.

- Player password is hashed

- Bikes rental (30m and you can extend it 30m more 5 min before it expires)

- Factions system with Vehicles, Leadership, and Member Ranks.

- Roleplay Channel Messages (/me, /do, /b)

- Basic Job system, with vehicles. (Right now only Trucker Job is available)

---

## Installation

### Just clone the repository and run

```
sampctl package install
```

## Usage

<!--
Write your code documentation or examples here. If your library is documented in
the source code, direct users there. If not, list your API and describe it well
in this section. If your library is passive and has no API, simply omit this
section.
-->

## Testing

<!--
Depending on whether your package is tested via in-game "demo tests" or
y_testing unit-tests, you should indicate to readers what to expect below here.
-->

To test, simply run the package:

```bash
sampctl package run
```

<!-- Definitions -->

[pawnplusref]: https://github.com/illidans4/pawnplus
