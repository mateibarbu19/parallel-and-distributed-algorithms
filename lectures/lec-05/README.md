# Lecture 05

## Prefix sum

This is a sequential implementation for the prefix sum algorithm presented in
the fifth lecture.

TODO:

- Parallelize this code.

### General formula for prefix sum

<!-- $$
\sum_{i=a}^{b}v(i) = \sum_{i=b - \lfloor log_2(b) \rfloor - 1}^{b}v(i) + 
\sum_{i=a}^{b - \lfloor log_2(b) \rfloor}v(i)
$$ --> 

<div align="center"><img style="background: white;" src="svg/3BuK4Hep4e.svg"></div>
