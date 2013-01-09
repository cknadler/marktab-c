# marktab grammar

The marktab grammar documentation. You can find the implemented version of this grammar, in bison, [here](https://github.com/cknadler/marktab/blob/master/src/y.tab.y).

## note

Notes represent a single not played on a single string.

#### format

```
4:1 3:1
```

#### output

```
e|-----|
B|-----|
G|---1-|
D|-1---|
A|-----|
E|-----|
```

#### muting

In addition to supplying a string and a note, you can represent a muted string as follows:

```
4:x 3:x
```
Goes to:

```
e|-----|
B|-----|
G|---x-|
D|-x---|
A|-----|
E|-----|
```

#### shorthand special case

When a note is defined, all notes following default to the string of that note if a string isn't specified. 

For example, the following are equivalent:

```
1:9 7 8 9
```
```
1:9 1:7 1:8 1:9
```

Another example:

```
1:9 7 2:8 9
```
```
1:9 1:7 2:8 2:9
```

## inline chord

Inline chords are groups of notes played at the same time on different strings. An inline chord, specifically, is the grouping of these notes inline.

#### format

```
(6:5 5:7 4:9) (6:5 5:7 4:7) (6:5 5:7)
```

#### output

```
e|-------|
B|-------|
G|-------|
D|-9-7---|
A|-7-7-7-|
E|-5-5-5-|
```

#### shorthand

Shorthand pattern for notes in a chord.

The following are equivalent:

```
(6:5 5:7 4:7)
```

```
(6:5 5,4:7)
```

## chord definition

A chord definitions is defining a chord and assigning it to a symbol for reuse.

#### format

Same chord syntax as inline chord.

```
Gmaj7: (6:3 4:4 3:4 2:3)

Gmaj7 Gmaj7
```

#### output

```
e|-----|
B|-3-3-|
G|-4-4-|
D|-4-4-|
A|-----|
E|-3-3-|
```

## note modifier

Modifiers and transitions modify notes.

#### modifiers

  * palm mute: `m`
  * harmonic: `*`
  * vibrato: `~`

Modifiers are applied to a single note or chord.

#### transitions

  * slide up: `/`
  * slide down: `\`
  * hammer on: `h`
  * pull off: `p`
  * bend: `b`


Transitions link two notes or chords together.

#### format

```
(1:5 2:5 3:5) / 1:9 \ 1:5* h 1:9
```

#### output

```
e|-5-/-9-\-5*-h-9-|
B|-5--------------|
G|-5--------------|
D|----------------|
A|----------------|
E|----------------|
```

#### shorthand

This is an abreviated pattern for transitions. This allows you to write strings of notes, linked with transitions, without having to re-specify the string number for each note. 

For example, the following are equivalent:

```
1:9 / 1:11 \ 1:1 h 1:2
```

```
1:9 / 11 \ 1 h 2
```

```
1:9/11\1h2
```

## rests

Rests add a single note worth of blank space in a tab.

#### format

```
(1:5 2:5) r 1:5
```

#### output

```
e|-5---5-|
B|-5-----|
G|-------|
D|-------|
A|-------|
E|-------|
```

## inline sequence

Sequences add a way encapsulate groups of notes and chords.

#### format

```
[(1:5 2:5) r 1:5] ^ 2
```

#### output

```
e|-5---5-5---5-|
B|-5-----5-----|
G|-------------|
D|-------------|
A|-------------|
E|-------------|
```

## sequences definition

Sequence definitions are a way to store a group of notes and chords for reuse.

#### format

```
symbol_name: [sequence of notes and chords]
```

```
OpenG: (6:3 5:2 4:0 3:0 2:2 1:2)
BridgeRiff: [1:2 / 1:5 b 1:7 1:7 \ 1:2 r OpenG OpenG r]

(1:5 2:5) BridgeRiff 1:5
```

#### additional notes

The only thing invalid in sequences are chord definitions and sequence definitions. This means you can use sequences inside of sequences, multipliers, anything. For example:

```
OpenG: (6:3 5:2 4:0 3:0 2:2 1:2)
OpenC: (5:3 4:2 3:0 2:1 1:0)
OpenEm: (6:0 5:2 4:2 3:0 2:0 1:0)
Riff: [5:3 4:0 4:2 3:0 2:0]
Chorus: [OpenEm [OpenG Riff] ^ 4 OpenG OpenC]

Chorus ^ 4
```

## repeating

Follow any object with `^ SomeNumber` to repeat it that many times.

You can repeat the following:

* notes
* chords (inline or defined)
* rests
* sequences (inline or defined)

## inline strings

#### format

```
= some string
1:9
```

#### output

```
some string

e|-9-|
B|---|
G|---|
D|---|
A|---|
E|---|
```

#### additional notes

The symbol alone, with no following characters will just print a newline. Similar to `puts` in ruby.

```
=
```

For now, this will serve as the way tabs are broken into sections.

## comments

Basic, ruby style, comments.

#### format

```
# this is a comment, it will be ignored by the parser
```

```
1:9 # everything after the hash will be ignored by the parser
```
