# marktab [![travis](https://travis-ci.org/cknadler/marktab.png "travis")](https://travis-ci.org/cknadler/marktab)

Concise, machine readable, guitar tabs.

## Development

MarkTab is actively under development. Contributions are more than welcome.

#### Build

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

## Docs

The [MarkTab grammar](https://github.com/cknadler/marktab/blob/master/docs/grammar.md) is fairly solid at this point. Until the first implementation is finished, this is the only real source of documentation for the language. If you have any questions or comments, feel free to open an issue.

## Contributing

Issues and, better yet, pull requests are more than welcome. See [CONTRIBUTING](https://github.com/cknadler/marktab/blob/master/CONTRIBUTING.md) for details.

## Copyright

Copyright (c) 2012 Chris Knadler. See LICENSE for details.
