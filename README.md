# Document Search Engine

This project returns a ranked list of files from a specified directory based on the search query.

## Usage

Compile and run the program and then answer the program's prompts:
```console
$ make
$ ./search
```

(If the `make` command fails, try running `make clean` before.)

Example:
```console
$ make
$ ./search
How many buckets?
> 10
Enter a filename search string:
> docs/*.txt
Enter a search query (X to exit):
> computer architecture gw
docs/D1.txt
docs/D2.txt
Enter a search query (X to exit):
> X
```

After a search query is entered and the ranking is returned, the calculated scores used to rank the files are appended to `search_scores.txt`, like this:
```text
docs/D1.txt:	1.504077
docs/D2.txt:	0.810930
docs/D3.txt:	0.000000
```
