# marktab [![travis](https://travis-ci.org/cknadler/marktab.png "travis")](https://travis-ci.org/cknadler/marktab)

Concise, machine readable, guitar tabs.

## Development

marktab is actively under development. Contributions are more than welcome.

#### Compile

Requirements:

* automake
* autoconf
* libtool

To build run (only the first time):

```bash
bundle install
rake
```

and then, as usual:

```bash
make
```

#### Testing

Requirements:

* ruby v1.9.3 or greater
* bundler

Before testing (only the first time) run: 

```bash
bundle install
```

And then, to run all tests:

```bash
rcomp test
```

## Copyright

Copyright (c) 2012 Chris Knadler. See LICENSE for details.
