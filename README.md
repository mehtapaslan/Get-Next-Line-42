*This project has been created as part of the 42 curriculum by mehaslan.*

# get_next_line

## Description

`get_next_line` is a C function that returns **one line at a time** from a file descriptor. Calling it repeatedly in a loop reads a whole file (or standard input) line by line, without ever loading the entire file into memory.

```c
char *get_next_line(int fd);
```

**Return value**

| Situation | Result |
|---|---|
| A line was read | The line, including its terminating `\n` |
| Last line of a file that does not end with `\n` | The line, without `\n` |
| Nothing left to read, or an error occurred | `NULL` |

The caller owns the returned string and must `free` it.

**Constraints of the project**

- Allowed external functions: `read`, `malloc`, `free`.
- No `libft`, no `lseek()`, no global variables.
- The buffer size passed to `read` is set at compile time with `-D BUFFER_SIZE=n`. The code also compiles without that flag (default value: `42`).
- Must follow the 42 Norm, must not leak, and must not crash.

**Files**

| File | Content |
|---|---|
| `get_next_line.h` | Prototypes and the default `BUFFER_SIZE` |
| `get_next_line.c` | `get_next_line` and its three static helpers |
| `get_next_line_utils.c` | String helpers (`gnl_strlen`, `gnl_strjoin`, `gnl_strchr`, `gnl_free`) |

The bonus part (several file descriptors at once) is not implemented.

## Instructions

Compile the sources together with your own `main` (a `main` is not part of the repository):

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o gnl
```

Without the flag, the default `BUFFER_SIZE` of 42 is used:

```sh
cc -Wall -Wextra -Werror main.c get_next_line.c get_next_line_utils.c -o gnl
```

Minimal usage example:

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

It also works with standard input (`fd = 0`).

Useful checks: `norminette get_next_line.c get_next_line_utils.c get_next_line.h`, and `leaks --atExit -- ./gnl` on macOS (or `valgrind` on Linux). Try several buffer sizes, for example `1`, `42`, `9999` and `10000000`.

## Algorithm

### Why a `static` variable is needed

`read()` does not know where a line ends: it returns up to `BUFFER_SIZE` bytes, which may stop in the middle of a line or run past a `\n` into the next line. The extra bytes have already been consumed from the file descriptor and cannot be given back (`lseek` is forbidden, and a pipe or stdin cannot be rewound). They must be kept somewhere until the next call.

A `static` pointer, `remainder`, holds that data between calls. It keeps its value after the function returns, and it is local to `get_next_line`, so no global variable is needed.

### What one call does

Each call runs three steps, one helper per step:

1. **`join_line`**: while `remainder` contains no `\n` and `read` still returns data, read `BUFFER_SIZE` bytes and append them to `remainder`. It stops as soon as a `\n` is available, or at end of file.
2. **`get_line`**: copy the content of `remainder` up to and including the first `\n` into a newly allocated string. If there is no `\n` (last line), copy everything. If `remainder` is empty, return `NULL`.
3. **`next_line`**: replace `remainder` with what comes after that `\n`, and free the old buffer. If nothing follows, free `remainder` and set it to `NULL`.

The line is extracted **before** `remainder` is trimmed, because trimming discards it.

### Example

`BUFFER_SIZE = 4`, file content `ab\ncdefg\nh`:

| Call | `read` results | `remainder` after `join_line` | Returned | `remainder` after `next_line` |
|---|---|---|---|---|
| 1 | `ab\nc` | `ab\nc` | `ab\n` | `c` |
| 2 | `defg`, `\nh` | `cdefg\nh` | `cdefg\n` | `h` |
| 3 | `0` (EOF) | `h` | `h` | `NULL` |
| 4 | `0` (EOF) | empty string | `NULL` | `NULL` |

### Design choices and justification

- **Read as little as possible.** The reading loop stops at the first `\n`. The file is never read entirely before processing, which keeps memory use proportional to the line length plus `BUFFER_SIZE`, and makes the function usable on stdin and pipes.
- **Works for any `BUFFER_SIZE`.** The buffer lives on the heap (`malloc(BUFFER_SIZE + 1)`), so even a value like 10000000 cannot overflow the stack. With `BUFFER_SIZE = 1` the loop simply runs once per character.
- **Pointer to pointer (`char **remainder`).** `join_line` and `next_line` replace the string `remainder` points to (a new allocation each time). They therefore need the address of the pointer so the change is visible in `get_next_line`.
- **Single place for freeing.** `gnl_strjoin` frees its first argument (the old `remainder`), also when its own `malloc` fails, so no intermediate string is leaked. `gnl_free` frees a pointer and resets it to `NULL`, which makes every error path (`read` failure, `malloc` failure) leave `remainder` in a clean state for the next call.
- **`NULL`-safe helpers.** `gnl_strlen`, `gnl_strchr` and `gnl_strjoin` accept a `NULL` string, so the very first call (where `remainder` is still `NULL`) needs no special case.
- **Cost.** Appending with `gnl_strjoin` copies `remainder` each time, so a very long line read with a tiny `BUFFER_SIZE` costs more copying. This was accepted in exchange for simple, easy-to-verify code.

## Resources

**References**

- `man 2 read`, `man 3 malloc`, `man 3 free`
- [read(2), Linux manual page](https://man7.org/linux/man-pages/man2/read.2.html)
- [File descriptor, Wikipedia](https://en.wikipedia.org/wiki/File_descriptor)
- [Static variable, Wikipedia](https://en.wikipedia.org/wiki/Static_variable)
- The 42 Norm and the `get_next_line` subject (v14.3)

**Use of AI**

AI was used as a tutor and helper for these tasks only:

- **Concepts:** explaining `read`, `static` variables and pointers to pointers.
- **Review and testing:** pointing out bugs in my code and running tests (buffer sizes, edge cases, leaks).
- **Norm formatting:** fixing `norminette` errors without changing the logic.
- **Documentation:** drafting this README, which I reviewed.