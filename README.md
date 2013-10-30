[![Build Status](https://travis-ci.org/haneefmubarak/sfDB2.png?branch=develop)](https://travis-ci.org/haneefmubarak/sfDB2)

sfDB2
=====

`sfDB2` aims to provide a simple database that has support for the bare minumum. The primary
design goal is to be as fast as possible, while still being portable and highly scalable.

Inspired by Redis and licensed the same.

Features and Implementation
===========================

Some of the features and/or implementations that `sfDB2` looks to support include:

 - Use a tree using the SHA1 hash to provide high speed lookups with virtually no collisions
 - Process requests extremely quickly by allowing only simple requests
 - Process requests even faster by interlacing requests as they come in and are processed
 - Reorder processing on the fly to ensure that one slow request doesn't hold up many others
 - Use various techniques to allow the kernel to swap pages that are not being used out and
 quickly load them back in
 - Make writes extremely fast by only allowing rows to be added or removed, not edited, 
 - Make read requests much much faster than write requests via not using mutexes in reads
 - Ensure that the maximum performance is pulled out of a single machine by scaling efficiently
 across threads, cores, memory, and also swap so that, even on a multi-GigE network pipe
 the network is the limiting factor
 - Prevent data loss by writing all incoming requests that alter data to a file
 - Allow DB reloads to occur quickly by periodically trimming the file
 - Prevent ambiguity by allocating a unique, incremented id for each row in a table, thus
 also eliminating the possibility of two rows being written to the coordinate

More may be added in the future

-Haneef Mubarak
