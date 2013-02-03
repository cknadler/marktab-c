# Contributing

MarkTab is currently under development. There are two main ways to contribute:

## Browse the source

Searching the source for `TODO` tags is a simple way to find broken, unimplemented, or poorly implemented chunks of MarkTab.

From the root directory of the project, run:

```bash
grep -rn "TODO" src
```

## Browse the issues

MarkTab uses GitHub issues. Check them out [here](https://github.com/cknadler/marktab/issues?sort=updated&state=open). They are kept up to date with the state of the current implementation.

## Write some tests

There are definitely some holes in the test suite. Tests are currently written in two ways:

#### Unit Tests

For unit tests, MarkTab is currenlty using [clar](https://github.com/vmg/clar). This is the part of the test suite that probably needs the most attention. The current unit tests are located at [spec/clar](https://github.com/cknadler/marktab/tree/master/spec/clar).

#### Integration Tests

Basically just broad output tests. For these, MarkTab is using [RComp](https://github.com/cknadler/rcomp). You will probably have an easier time unit testing than writing any of these tests. Integration tests are located at [spec/rcomp](https://github.com/ckandler/marktab/tree/master/spec/rcomp).

## Doing work

If you are planning on tackling a `TODO` tag or an issue, let me know in one way or another. For `TODO` tags, open up an issue or send me an email. For issues that are already open, just leave a comment or send me an email.

## Testing

Before opening an issue, make sure you run the tests to make sure your changes didn't break anything.

```bash
make test
```

## Thanks

If you made it this far, thanks for even considering contributing to MarkTab. Seriously, you are awesome.
