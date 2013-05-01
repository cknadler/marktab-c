![marktab](docs/marktab.png)

[![Build Status](https://travis-ci.org/cknadler/marktab.png?branch=master)](https://travis-ci.org/cknadler/marktab)

Concise, machine readable, guitar tabs.

## Library

Marktab is a statically linked library written in flex, Bison and plain C99 (no
libraries). To get Marktab up and running in your project:

* build and install it
* link against Marktab with `-lmt`
* include Marktab's header in your project with `#include <marktab.h>`

## API

Currently, the API is pretty minimal (read terrible). More will be added soon.

```c
// Renders a tab from stdin to stdout
void marktab_rend_stdin();

// Renders a tab from a file to stdout
int marktab_rend_file(char* file);
```

## Docs

The [MarkTab grammar](https://github.com/cknadler/marktab/blob/master/docs/grammar.md) is fairly solid at this point. Until the first implementation is finished, this is the only real source of documentation for the language. If you have any questions or comments, feel free to open an issue.

There are several examples of Marktab in action located in `examples`.

## Contributing

Issues and, better yet, pull requests are more than welcome. See [CONTRIBUTING](https://github.com/cknadler/marktab/blob/master/CONTRIBUTING.md) for details.

Marktab would not be what it is without the help of some awesome contributors.

## Copyright

Copyright (c) 2013 Chris Knadler. See [LICENSE](https://github.com/cknadler/marktab/blob/master/LICENSE) for details.
