# signalInput

Check Ctrl-C signal could interrupt stdin process or not.

## summary

- `Ctrl-D` make `fgets` return with `NULL` immediately.

### SIGINT handler

- default SIGINT handler, process will been interrupted, and quit.
- add user defined handler, it cannot return from `fgets` function, so won't quit.
    - However, as we setting `bExit` flag, it will quit next time.
