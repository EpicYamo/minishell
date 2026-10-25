# minishell

A small Bash-like shell written from scratch in C, as part of the
42 school curriculum (minishell).

It reads a line through GNU readline, tokenises it, builds a command list,
runs pipelines and redirections, and takes care of its own environment, exit
statuses, signals and memory.

The prompt is `🜁 Y-Shell>`; the banner prints the repository address.

## What it does

| Area | Implemented |
| --- | --- |
| Line editing | GNU readline: history, arrow keys, Ctrl+R search |
| Tokenising | quotes (single and double), escapes, `$VAR` and `$?` expansion |
| Pipelines | any number of commands joined with `|` |
| Redirections | `<`, `>`, `>>`, `<<` (heredoc) |
| Heredocs | optional variable expansion, interrupted cleanly with Ctrl+C |
| Environment | own linked list, `SHLVL`, `export`, `unset`, lookup for `$VAR` |
| Builtins | `echo` (with `-n`), `cd`, `pwd`, `export`, `unset`, `env`, `exit` |
| Exit status | stored per command, `$?` reflects the last pipeline |
| Signals | Ctrl+C at the prompt clears the line; SIGQUIT ignored |
| Memory | every allocation goes through a small garbage collector |

### Not implemented

These are outside the 42 subject and deliberately absent:

- `;`, `&&`, `||` and command grouping
- job control (`jobs`, `fg`, `bg`) and `&`
- globbing (`*`, `?`, `[...]`)
- command substitution (`$(...)`, backticks)
- running a script file: the shell rejects arguments

## Build

```bash
git clone https://github.com/EpicYamo/minishell.git
cd minishell
make
```

Requires a C compiler and GNU readline:

```bash
sudo apt install libreadline-dev
```

## Run

```bash
./minishell
```

```
🜁 Y-Shell> echo "hello" | tr a-z A-Z
HELLO
🜁 Y-Shell> cat < Makefile | grep SRC
SRC		:= minishell.c \
🜁 Y-Shell> export NAME=world
🜁 Y-Shell> echo "hello $NAME"
hello world
🜁 Y-Shell> echo $?
0
🜁 Y-Shell> cat << EOF | wc -l
> one
> two
> EOF
2
```

Quotes behave as expected: single quotes keep everything literal, double
quotes expand variables but keep spaces together.

## How it works

```
readline ──► lexer ──► parser ──► executor ──► processes / builtins
              │          │            │
              │          │            └─ pipes, redirections, wait, $?
              │          └─ t_command list, heredocs
              └─ tokens: words, quotes, operators, $ expansions
```

| Directory | Responsibility |
| --- | --- |
| `minishell.c` | prompt, readline loop, signal setup, top-level flow |
| `lexer/` | splits the line into tokens, handles quotes and `$` |
| `parser/` | builds `t_command` nodes, redirections, heredocs, environment parsing |
| `executor/` | forks, connects pipes, applies redirections, runs builtins, waits |
| `built_ins/` | `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` |
| `utils/` | garbage collector, environment list, signals, banner, helpers |

Two details worth knowing when reading the code:

- **Placeholder tokens.** `$?` and a lone `$` are replaced by unique sentinel
  strings before tokenising and turned back into values afterwards, which
  keeps the lexer from treating their characters as operators.
- **`cd` changes the directory, not the variable.** It calls `chdir` and
  `pwd` reads the result with `getcwd`; the inherited `PWD` / `OLDPWD`
  variables are left untouched, which is a known difference from Bash.
- **Garbage collector.** Pointers are registered in `t_gc` and released once
  per input line, so error paths do not have to free everything by hand.

## Signals and exit status

- `SIGINT` at the prompt prints a newline and redraws the prompt, like Bash.
- `SIGINT` during a heredoc removes the temporary file and exits with 130.
- `SIGQUIT` is ignored in the shell and inherited as default by children.
- `$?` is updated after every pipeline, including builtins and failures.

## Checking for leaks

readline keeps allocations alive on purpose, so the repository ships
suppressions:

```bash
cd ignore_readline_leaks
./run.sh              # valgrind, with child processes traced
./run_wo_childproc.sh # valgrind, shell only
```

## About

Built as part of the 42 school curriculum by
**Arda Aycan** ([@EpicYamo](https://github.com/EpicYamo)).

The subject asks for a shell that behaves like Bash for a defined set of
features; anything beyond that list was left out on purpose rather than
half-implemented.
