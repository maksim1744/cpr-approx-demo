# `cpr approx` demo

https://github.com/user-attachments/assets/1e4fb0a0-27ff-4c1b-b6fc-9dcad7363855

Demo for [`cpr approx`](https://github.com/maksim1744/cpr) and [`remote-runner-server`](https://github.com/maksim1744/remote-runner-server). The repo contains an example of an optimization problem (just TSP in this case), where you are given 5 tests with some points on a plane and you need to visit all of them minimizing total distance.

### Setup

- Install [`cpr`](https://github.com/maksim1744/cpr) locally
- Create tests in the form of `tests/001.in`
- Implement a solution which will be run as `./main [input_file] [output_file]`
- Implement a scoring program which will be run as `./scorer [input_file] [solution_output_file]` and needs to print total score to the standard output. If the solution provided is incorrect it should just exit with error instead.
- Configure [`config.json`](./config.json)
- [optional] Write a script which converts `tests/*.ans` to the correct output format

You can obviously refer to the corresponding files in this repo for examples. Note that you normally won't have `gen.cpp` and `gen.sh`, since you will be just provided tests.

### Basic usage

```sh
cpr approx  # your best friend
cpr approx -n 10  # run each test 10 times
cpr approx --norun  # just merge *-001.out into *.ans, useful if you stopped cpr in the middle
                    # or copied *.out from elsewhere. Supports -n as well
```

Don't forget to compile `main` and `scorer`

### Debugging

Note that the provided [`main.cpp`](./main.cpp) example supports no args, in which case it reads from stdin. It is not required to use `cpr approx`, but might be convenient.

Otherwise, `i-th` run of `j-th` test will generate a bunch of `{j}-{i}.*` files. For example, if you run `cpr approx -n 3`, then the third (last) run of test 4 will have
- `004-003.out` as solution output
- `004-003.log` as solution stderr (this is a feature from [`main.cpp`](./main.cpp), you can change to whatever you want)
- `004-003.err` as scorer stderr
- `004.in` as input
- `004.ans` as the best output so far

### Remote runner

Of course you can just do the same on the remote host. But sometimes it is more convenient to develop and debug locally, and only use remote host for some heavy computing.

To do that
- Install and run [`remote-runner-server`](https://github.com/maksim1744/remote-runner-server) on the remote host (refer to the repo for detailed instructions)
- Start forwarding remote port (ex. 1234) to local port (ex. 5678):
    ```sh
    ssh -L 5678:localhost:1234 user@host  # optionally add -N to hide the shell
    ```
- Add `"remote"` section to `config.json`
- Run `cpr approx --remote`

#### Notes on remote runs
- It only runs the solution itself on the remote host, then copies `*.out` locally to run scorer and update answers
- Files to copy to remote host (from `"copy"`) will *not* be sent if they already exist on remote host (it checks md5 each time)
- `cpr approx` has *a lot* of retries and submits runs asynchronously, so you really need to try to lose any progress by disabling internet connection. For example, losing connection for an hour shouldn't break `cpr`. It will obviously block further execution, but then continue normally as soon as the connection is restored.
- But if you plan on running the solution overnight, it is recommended to run `cpr approx` directly on the remote host (under `screen` or `tmux`), and in the morning do `cpr approx --norun -n [same as on remote run]` locally
- It only copies `*.out` back, so if you need solution stderr from `*.log`, you will need to ssh yourself

### Config
```json
{
    "optimize": "min",  // "min" or "max"
    "result_func": "sum",  // "sum" or "avg"
    "tests": 5,  // total number of tests
    "skip_tests": [2],  // don't run solution for these tests
    "threads": 8,  // max number of threads for parallel execution
    "precision": 3,  // precision for printing results
    "finalize": ["python3", "finalize.py"],  // custom cmd for finalizing results
    // "main": ["./main"]  // defaults, but you can change to custom cmd, required input files will be passed as two last args
    // "scorer": ["./scorer"]
    "remote": {
        "local_port": 5678,  // local port with forwarded connection
        "workdir": "/home/maksim/test",  // remote working directory, must be full path
        "copy": [  // files to copy to remote
            "main",
            "tests/*.in"
        ],
        // "prerun": [  // additional commands to run on remote host after copying files
                        // for examples if your OS is different from remote one,
                        // sending the binary might not work, then you need to compile it from source:
        //     ["g++", "-O2", "main.cpp", "-o", "main"]
        // ],
        "threads": 96
    }
}
```
