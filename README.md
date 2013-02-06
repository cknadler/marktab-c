# marktab [![travis](https://travis-ci.org/cknadler/marktab.png "travis")](https://travis-ci.org/cknadler/marktab)

Concise, machine readable, guitar tabs.

## Usage

As of now, MarkTab is incomplete. If you want to play with it anyway, here is a quick guide.

```bash
git clone https://github.com/cknadler/marktab.git
cd marktab
make
```

The build process creates a bin directory where it places the MarkTab executable.

The following will make a basic tab:

```bash
cd bin
echo "1:1" > tab.mt
./marktab tab.mt
```

## Development

MarkTab is actively under development. Contributions are more than welcome.

#### Build

No dependencies, just run:

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
make test
```

## Docs

The [MarkTab grammar](https://github.com/cknadler/marktab/blob/master/docs/grammar.md) is fairly solid at this point. Until the first implementation is finished, this is the only real source of documentation for the language. If you have any questions or comments, feel free to open an issue.

## Contributing

Issues and, better yet, pull requests are more than welcome. See [CONTRIBUTING](https://github.com/cknadler/marktab/blob/master/CONTRIBUTING.md) for details.

## Copyright

Copyright (c) 2013 Chris Knadler. See [LICENSE](https://github.com/cknadler/marktab/blob/master/LICENSE) for details.
