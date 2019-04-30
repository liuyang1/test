# Shadow symbol with linker

- use linker's `--wrap=symbol` option to introduce '__wrap_FUNC' and '__real_FUNC'
- gcc pass linker's option with `-Wl,OPTION,OPTION`
- `printf` with only one string, compiler will optimize it to `puts` function.
	- use `-ffreestanding` or `-fno-builtin` to forbidden this.
