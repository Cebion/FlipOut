# FlipOut source archive

Recovered sources for FlipOut 1.1 (Mikko Rasa "The DataBeaver", GPL v2+; tdb.fi). The upstream SVN
and git servers are closed, so everything here comes from the Wayback Machine.

- `flipout/`: SVN trunk r124 (v1.1) from `svn.tdb.fi/flipout/trunk` (snapshot 2015): source, data, Build, compile-data.py.
- `libs/mspcore-1.1`, `mspdatafile-1.1.1`, `mspgbase-1.1`, `mspgl-1.1`, `mspgltk-1.1`, `mspal-0.10`, `builder-1.0`: release tarballs from `www.tdb.fi/files/` (LGPL).
- `libs/mspcore-3.0-HEAD-b116e16`: `libs/core.git` master snapshot (2019, Wayback 2021-11-27). Holds the later merged fs/io/strings.
  The 1.1-era fs/io/strings (tags fs-1.1, io-1.1, strings-1.1) are not archived, so 3.0 is the stand-in.

Known gap: mspcore 3.0 dropped `core/except.h` (`Msp::Exception`, `InvalidParameterValue`, `InvalidState`, `KeyError`, `throw_at`), which the 1.1-era libs and FlipOut use.
