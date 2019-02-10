all: cpu.out

fmt:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -style=file -i

%.out: %.c
	gcc -Ofast $< -o $@
