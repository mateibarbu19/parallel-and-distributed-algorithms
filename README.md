# Parallel-and-distributed-algorithms

**Disclaimer**: The majority of the code in this repository was written on top
of the infrastructure provided by the
[APD-UPB team](https://github.com/APD-UPB). **Be aware that some of this**
**code is their intellectual property.** Because of initial lack of skill using
Git, I wasn't able to import their original commits. But I do encourage you to
check their [Github repo](https://github.com/APD-UPB/APD).

<img align="center" src="https://upload.wikimedia.org/wikipedia/commons/thumb/f/f1/Fork_join.svg/1920px-Fork_join.svg.png">

## 1. Repository description

This project aims to provide a improvements to the original
[Open CourseWare](https://ocw.cs.pub.ro/courses/apd) Parallel and Distributed
Algorithms Laboratories taught at
[Politehnica University of Bucharest](https://upb.ro).

Here is the list of the improvements I brought (although not limited to):

- For POSIX labs:
    - improved *all* checkers
    - improved all Makefiles with more generic rules
    - support for scalable build with the use of macro directives
    - clangd configuration files
    - READMEs
    - bonuses like matrix multiplication with cache tiling
    - Mergesort for any array and number of threds
    - removed uses of global variables
    - removed uses of unjustified signed variables instead of unsigned
- For Java labs:
    - improved infrastructures (e.g. for (laboratories/lab-05))
    - READMEs
    - added a [checker](laboratories/lab-07/check.sh) where none existed and was
    needed
    - improved tests for [Floyd's Alg.](laboratories/lab-04/shortestPathsFloyd_Warshall)
    - coding style (`final` attribute added where possible, ...)

For more information or a syllabus check out the course description
[page here](https://cs.pub.ro/index.php/education/courses/60-under/an3underg/122-parallel-and-distributed-algorithms)
. This course was taught by Ciprian Mihai Dobre.

## 2. Requirements

If you are interested in a IDE, I would recommend
[VSCodium](https://vscodium.com/). Install the following extensions:

- `clangd`: for both POSIX and MPI labs
- `Language Support for Java(TM) by Red Hat` + `Debugger for Java`: for Java
    labs
- (optional) `Native Debug`: for GDB debugging

Before doing so I recommend you install some requirements *globally*. If you are
using a Debian-based system (tested on Debian 11), run these commands:

```bash
sudo apt install clangd
sudo apt install default-jdk

# optional, for I prefer gcc
# sudo apt install clang

# optional, if you want to see Java libraries implementation
# dependis on what version of openjdk, was installed in a previous step
# sudo apt install openjdk-11-source
```

Note: (Optional) If you took my advice and installed VSCodium, you'll may need to enable
the VSCode Marketplace using these
[instructions](https://github.com/VSCodium/vscodium/blob/master/DOCS.md#extensions-marketplace)
.

## 3. Usage

Open the directory you want to test. You'll either have a `Makefile` or Java
sources that could be run with the `Debugger for Java` extension.

All folders (I remember), include a `README` for further clarification.

## 4. Other projects

Check out Florin's bonus repositories for this course:

- [Pthreads workout](https://github.com/florinrm/apd-pregatire-pthreads)
- [Java workout](https://github.com/florinrm/apd-pregatire-java)

## 5. Acknowledgments

I would like to thank our teaching assistant
[Florin-Răzvan Mihalache](https://github.com/florinrm)
for all his support, this repository would not be possible without him.
