# marktab grammar

## note

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

## inline chord

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

#### modifiers

  * palm mute: `m`
  * muted string: `x`
  * bend: `b`
  * harmonic: `*`
  * vibrato: `~`

Modifiers are applied to a single note or chord.

#### transitions

  * slide up: `/`
  * slide down: `\`
  * hammer on: `h`
  * pull off: `p`

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

## comments

Basic, ruby style, comments.

#### format

```
# this is a comment, it will be ignored by the parser
```

## general formatting

This is still up in the air.

break: `end`

input:

```
Gmaj7: (6:3 4:4 3:4 2:3)

Gmaj7 Gmaj7 end (6:5 5:7 4:9) (6:5 5:7 4:7) (6:5 5:7)
```

output:

```
e|-----------------------|
B|-3-3-------------------|
G|-4-4-------------------|
D|-4-4-------------------|
A|-----------------------|
E|-3-3-------------------|

e|-----------------------|
B|-----------------------|
G|-----------------------|
D|-9-7-------------------|
A|-7-7-7-----------------|
E|-5-5-5-----------------|
```
