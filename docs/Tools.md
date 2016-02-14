We try to provide tools that work well together for every phase of the software
development process:

* Builds

Building software across different toolsets is hard, from keeping track of all
the build flags under different build environments.

Different users like different build environments,
mostly their platform supported one and keeping projects in sync between
different build tools is an exercise in frustration, unless you have a very
high number of developers in your project.

* Source Analysis

In this day and age everyone should be running a static code analyzer as part
of their software development process though in practice few seem to bother to
actually make them a part of their usual development cycle. Maybe the reason is
that these tools are not usually provided as part of the default build process
setup, and unless automate them you will not bother to run them too often.

* Code formatting & beautifying

The amount of time that is spent fixing and correcting formatting mistakes in
contributions to a project is enormous. There is no reason we should be wasting
valuable development time on fixing things that machines can already do for us.

We help you by providing support for invoking these tools either manually,
when committing or as a repository hook.

*C++* clang-format
http://clang.llvm.org/docs/ClangFormat.html

*C#* NArrange / NRefactory

* Documentation

Documentation is one of the most important parts of any software project yet
one that is usually neglected, especially in open source projects. There are
multiple reasons for this of course, but probably the reason this happens is
because actually setting up an automated generation system is hard work. You
need to setup your own server which is an extra step that is not trivial.

We provide an easy way to generate 

* Testing

* Packaging

* Publishing

* Deployment

Developing software across different platforms is a really challenging activity.
We help you by having a standardized way to deploy your application.


We built this framework over the years out of frustration for the sheer
complexity of developing cross-platform native applications across different
environments.

Why focus on native code in this day and age of 