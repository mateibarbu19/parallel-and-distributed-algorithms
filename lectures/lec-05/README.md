# Lecture 05

## Prefix sum

This is a sequential implementation for the prefix sum algorithm presented in
the fifth lecture.

TODO:

- Parallelize this code.

### General formula for prefix sum

<!-- $$
\sum_{i=a}^{b}v(i) = \sum_{i=b - p - 1}^{b}v(i) + 
\sum_{i=a}^{b - p}v(i),
p = 2^{\lfloor log_2(b) \rfloor}
$$ --> 

<div align="center"><img style="background: white;" src="svg/TzyBMyhBse.svg"></div>

E.g.

<!-- $$
\sum_{i=0}^{8}v(i) = \sum_{i=1}^{8}v(i) + 
\sum_{i=0}^{0}v(i)
$$ --> 

<div align="center"><img style="background: white;" src="svg/PPxb4lE2XO.svg"></div>
