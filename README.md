Architecture 2016 Coursework
================================
There are three central aims of this coursework:

- Solidify your understanding of how an instruction
  processor actually functions. The overall functionality
  of how a processor works is relatively easy to grasp,
  but there is lots of interesting detail which gives
  you some insight (both into CPUs, but also into
  software and digital design).
  
- Understand the importance of having good specifications,
  in terms of functionality, APIs, and requirements. This
  is fundamental to CPU design and implementation, but is
  also true in the wider world (again) of software and
  digital design.
 
- Develop your skills in coding from scratch. There is
  not much scaffolding here, I am genuinely asking you
  to create your own CPU simulator from scratch. You
  will also hopefully learn some important lessons about
  reducing code repetition and automation.
  
Coursework Deliverables
-----------------------

There are two C++ [deliverables](https://en.wikipedia.org/wiki/Deliverable) for this coursework:

1. Create a MIPS software simulator

2. Develop a test suite for your MIPS simulator

The API for the simulator is given as a bare C header file,
defining the functions and data-types involved, along
with a functional specification of what a simulator
should do.
 
The first part is the implementation of a mips simulator, and
is essentially a library that implements the api found in [`include/mips_cpu.h`](include/mips_cpu.h).

The second part is the test suite which will drive the
simulator and make it do things. This is a C++ program in a file called `src/[your_login]/test_mips.cpp`.

Assessment criteria
-------------------

This is an exercise in both implementing specifications,
and in testing specifications, so the assessment
is weighted as follows:

- 20% Compilation and robustness: How much (manual) work
  was needed to make the submission compile in the target
  environment and run without crashing? The expectation
  is that everyone can get full marks here.

- 50% Functionality: What proportion of the CPU simulator's
  functions operate correctly? It is unlikely that many submissions
  will get all instructions working perfectly, but there is a core
  subset that everyone will be able to get fully working, a larger
  set that most people will be able to get partially working, and some
  instructions that are really quite difficult to get right and many
  people won't attempt. Some hints on that will appear later.

- 30% Testing: What proportion of the CPU simulator is
  exercised and tested by the submitted test suite? So _if_ an
  instruction is implemented, is it a) exercised, and b) is
  the result checked. You can still get a decent mark here
  even if you have a small number of instructions implemented,
  as long as they are tested well.
  
- (at most 10%) Bug reports: This specification will not be perfect, and
  I welcome bug reports. Things like spelling mistakes are
  welcome, but not quite as valuable. What is important
  are points of genuine ambiguity, or errors of implementation
  in the code spec. Bug reports should identify both the
  problem, and how to reproduce it, and particularly welcome are
  bug reports with suggested fixes. Note that "I don't know
  what to do" or "my program crashes" or "this is too hard" are
  not bugs, they need to be errors in the specification.

Compilation Environment
-----------------------

For most people it is sufficient to say: the target compilation
is plain C or C++, and the target environment is the C and/or
C++ standard library. It is easy to stay within platform independent
APIs, as no platform-specific interaction with the environment is
needed during execution. So it should work happily on both linux
and windows.

The actual target environment is the lab Linux environment,
and the version of gcc/g++ installed there. You can develop
in Windows, MacOS, or another linux, but you need to make sure
it works in that environment.

When running your test suite, the executable will be launched
with its working directory as `src/[your_login]`, so if you
wish to read files you can place them there (or in sub-directories).

When your CPU simulator is executing, you can make no assumptions
about the working directory, or the presence or absence of other
files.

Managing expectations
---------------------

You may think that it is always possible to get 90-100% in
coursework if you just work hard enough. That is not
true here, and will usually not be true in your future
courseworks. The grade distribution for this coursework
should be roughly the same as exam grade distributions,
and certainly was last year. So 70% is a good mark, 80%+
is a great mark, and anything above 60% shows that you're
doing ok.

Some students will have more programming experience,
and so will find this exercise easier, and may well
end up with a higher grade. That's life I'm afraid,
just like some people turned up last year knowing more
of the maths curriculum. This mark goes into Computing Lab,
which is intended to encourage and recognise ability in the
practical application of programming and computing concepts.
For those students who are less experienced in programming
this kind of exercise is much more valuable, and they
will get more out of it. But everyone, no matter their
programming ability, should find it helps clarify their
current understanding of instruction processors, and supports
their learning through the rest of this module.

Guidance on instructions
------------------------

We are going to look at the MIPS-1 (or MIPS-I) instruction
set, in big endian mode. There are 47 instructions eligible for implementation,
as certain instructions such as SYSCALL are too complex to
handle here. The following table gives the mnemonics, the description,
and a rough guide to how complex they are. Note that the complexity is
based on both how easy it is to implement the base functionality,
but also how easy it is to deal with, and try to test, corner cases.


Code  |   Meaning                                 | Complexity  
------|-------------------------------------------|-----------
ADD   |  Add (with overflow)                      | 2  XX       
ADDI  |  Add immediate (with overflow)            | 2  XX       
ADDIU |  Add immediate unsigned (no overflow)     | 2  XX       
ADDU  |  Add unsigned (no overflow)               | 1  X        
AND   |  Bitwise and                              | 1  X         
ANDI  |  Bitwise and immediate                    | 2  XX       
BEQ   |  Branch on equal                          | 3  XXX      
BGEZ  |  Branch on greater than or equal to zero  | 3  XXX      
BGEZAL|  Branch on non-negative (>=0) and link    | 4  XXXX     
BGTZ  |  Branch on greater than zero              | 3  XXX      
BLEZ  |  Branch on less than or equal to zero     | 3  XXX      
BLTZ  |  Branch on less than zero                 | 3  XXX      
BLTZAL|  Branch on less than zero and link        | 4  XXXX     
BNE   |  Branch on not equal                      | 3  XXX      
DIV   |  Divide                                   | 4  XXXX     
DIVU  |  Divide unsigned                          | 4  XXXX     
J     |  Jump                                     | 3  XXX      
JALR  |  Jump and link register                   | 4  XXXX     
JAL   |  Jump and link                            | 4  XXXX     
JR    |  Jump register                            | 3  XXX      
LB    |  Load byte                                | 3  XXX       
LBU   |  Load byte unsigned                       | 3  XXX      
LH    |  Load half-word                           | 3  XXX       
LHU   |  Load half-word unsigned                  | 3  XXX       
LUI   |  Load upper immediate                     | 2  XX       
LW    |  Load word                                | 2  XX       
LWL   |  Load word left                           | 5  XXXXX    
LWR   |  Load word right                          | 5  XXXXX    
MFHI  |  Move from HI                             | 3  XXX     
MFLO  |  Move from LO                             | 3  XXX     
MTHI  |  Move to HI                               | 3  XXX     
MTLO  |  Move to LO                               | 3  XXX     
MULT  |  Multiply                                 | 4  XXXX     
MULTU |  Multiply unsigned                        | 4  XXXX     
OR    |  Bitwise or                               | 1  X        
ORI   |  Bitwise or immediate                     | 2  XX       
SB    |  Store byte                               | 3  XXX      
SH    |  Store half-word                          | 3  XXX      
SLL   |  Shift left logical                       | 2  XX       
SLLV  |  Shift left logical variable              | 3  XXX       
SLT   |  Set on less than (signed)                | 2  XX       
SLTI  |  Set on less than immediate (signed)      | 3  XXX       
SLTIU |  Set on less than immediate unsigned      | 3  XXX      
SLTU  |  Set on less than unsigned                | 1  X        
SRA   |  Shift right arithmetic                   | 2  XX       
SRAV  |  Shift right arithmetic                   | 2  XX       
SRL   |  Shift right logical                      | 2  XX       
SRLV  |  Shift right logical variable             | 3  XXX       
SUB   |  Subtract                                 | 2  XX       
SUBU  |  Subtract unsigned                        | 1  X        
SW    |  Store word                               | 2  XX       
XOR   |  Bitwise exclusive or                     | 1  X        
XORI  |  Bitwise exclusive or immediate           | 2  XX       

This is a not-quite an exhaustive list of MIPS-1 instructions.
Any instruction not covered in this list will not be tested
as part of the assessment, and any implementation defined
behaviour is fine (return an error, or actually implement
the instruction).

There are many instructions, but there is a lot of commonality
between some instructions. Think about the underlying
digital data-path in a real processor, and use that to identify
where there are similarities. 

You may get to the point where things start getting very boring,
and you seem to be doing the same thing over and over, possibly
by copying and pasting. This is generally an indication that you
are missing an opportunity to abstract or automate. For example,
how different are "and", "or", and "xor"? What is the shared
functionality between "lb", "lw", and "lh"? You may want to
re-factor your code every once in a while as you work out better
ways of testing them - the purpose of your test-bench is to
give you the confidence to do that.

As you move through the instructions you should find that
you have to think carefully about the first instruction of each type,
deciding how to implement and test it. The next of the same
type should be much quicker, then the next almost mechanical, 
and you'll probably find two-thirds of the instructions are
done in a minute each. However, you should still expect this to
take a substantial amount of time, particularly if you plan to do
all instructions.

I would expect most people to be able to implement and test all
the 1s and 2s fairly easily. Implementing the 3s is not so
difficult, but testing them can be more complex. The 4s are doable,
but have some complexity in implementation and testing. Implementing
the 5s correctly is really quite challenging.

Getting Started
---------------

### Read this document

You have got to this point already. If you skipped to here, go back
and read the entire thing again.

### Get the source code

You can get the source code either by:

1. Downloading the zip file (see the link on the right hand side),
   which gives you a snapshot of the files in the repository.
 
2. Cloning the source code to your local directory, keeping the
   git information intact. You don't need a github account to do
   this, and your repository will be private.

3. Fork the code into your own repository. This assumes that you
   have your own account, and presumably the ability to keep
   that repository private. See the [Student Pack](https://education.github.com/pack)
   if you're interested in that direction.

While it is not required, I would highly recommend that you try
option 2 (I will use this route in class), and option 3 is even better.
It is good to get some experience of how source control works,
and acting as a consumer is a good way of understanding what is going on.
There are a number of GUI tools available which make things easier:

 - The github GUI is available for [Windows](https://windows.github.com/),
    [Mac](https://mac.github.com/).
    
 - There are third party GUI tools like [TortoiseGIT](https://code.google.com/p/tortoisegit/)
 
 - There is a default GUI called [git gui](https://www.kernel.org/pub/software/scm/git/docs/git-gui.html)
   from the main git people, that should be cross platform.

 - Or you can just use the command line. If you are only using git
   to get and update the code, then "git clone" and "git pull" are
   easy to use.

The submission itself is through blackboard as a zip, so there
is no requirement to use git. Even if I update the repository,
you can still just download the zip again and copy your current
work in - it is deliberately designed so that copying your
`src/<login>` directory into the updated source code will work.

### Read the source code

The source code is part of the specification, and is heavily
documented (it is much more important to document APIs than
it is to document implementation). Suggested reading order is:

 - `include/mips.h`
 - `include/mips_mem.h`
 - `include/mips_cpu.h`
 - `include/mips_test.h`

*Optional*: The comments follow the format for a well-known tool
called [doxygen](http://www.stack.nl/~dimitri/doxygen/). If you
apply doxygen to the file `doc/mips.doxygen`, then it will generate
some nice html formatted API documentation for you.

### Check you understand MIPS

The ISA we are using is a subset of the MIPS-1 (or MIPS-I)
instruction set in big endian mode. There is a lot of
discussion of MIPS in the course text book, and the MIPS
specification itself is available online, for example:
http://math-atlas.sourceforge.net/devel/assembly/mips-iv.pdf
There are multiple revisions or extensions of the instruction set,
so remember that we are only considering MIPS-1.

I think this is quite a nice break-down of the instructions,
but be careful about the details:
http://www.mrc.uidaho.edu/mrc/people/jff/digital/MIPSir.html

Another summarised version is the [QuickReference](https://www.lri.fr/~de/MIPS.pdf).
This one is particularly useful as it highlights the commonalities
between instructions, which both makes implementation clearer, and
helps to visualise how the hardware works.
