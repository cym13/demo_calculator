===============
Demo Calculator
===============

What is this?
=============

This is a simple text-based calculator. It supports all four common
operations, integers and floating point numbers as well as variable
assignment to keep track of your results. It also has an history so you can
reference previous results and save your work in a file.

This is (really) not meant for production use. However it shows how one may
build a simple interpreter with basic features.

However the true reason why this was built is to showcase heap exploitation
techniques such as:

- heap buffer overflow

- use after free

- exploitation of uninitialized memory

- heap spraying

- return-oriented programming

Why?
====

Most tutorials on binary exploitation talk about stack-based issues: "Overflow
your buffer then overwrite the return address at the beginning of the stack
frame and take control of the intruction pointer".

But heap issues are also very common although they're harder to demonstrate
in a suitable environnement. I hope that this calculator is a step forward in
that direction.

I found a bug, can you fix it?
==============================

Good. You may open an issue here, but keep in mind that this is an
exploitation lab, it's supposed to be bugged in subtle ways to provide
vulnerabilities to exploit.

If your bug happens to be unrelated or blocking to demonstrate such
vulnerabilities I'll fix it.

I wouldn't have written this feature that way
=============================================

Again, although I tried to keep things readable I'm not trying to teach good
C: all design choices were made with vulnerability exploitation in mind. That
said, if you're looking for bugs and find some parts of the code strange you
way want to dig deeper and see where that leaves you ;)

I like that and would like to use it for a tutorial/a course/whatever
=====================================================================

That's what it's there for, take it and hack it!
