# pedtransposer

transpose plink's ped file.
see ped file information: http://pngu.mgh.harvard.edu/~purcell/plink/

this program aim NFS & low memory environment.
when using --buf option, limit buffer size and use temporary files.

## example

```
% cat test.ped

FAM001  1  0 0  1  1  A A  G G  A C
FAM002  2  1 2  2  2  T T  A C  0 1
FAM003 3 3 4 3 3 G G C T 1 0

% ./pt --file test.ped --out hoge.ped
% cat hoge.ped

FAM001  FAM002  FAM003
1       2       3
0       1       3
0       2       4
1       2       3
1       2       3
A A     T T     G G
G G     A C     C T
A C     0 1     1 0
```

## option

```
./pt: --file --out [--buf]  [--space]
	--file	input ped file path
	--out	output ped file path
	--buf	limit buffer size(MiB). default: allocate input file size
	--space	output delimite space. default: tab
```
