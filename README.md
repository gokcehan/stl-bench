STL Bench
=========

This repo contains implementations of queue and deque using dynamic arrays with different memory management strategies.
Simple benchmarks are provided comparing the performance with a few C++ STL containers, namely list, vector and deque.
A sample result page from my machine can be found [here][].

[here]: https://cdn.rawgit.com/gokcehan/stl-bench/master/charts.html

Quickstart
----------

You need libjson development library to compile the code.
Simply run `make` to compile and then run the executable `bench` to run the benchmark.
This will output a `charts.html` file in the current directory with the results plotted using google charts library.

Queue
-----

### push back (amortized O(1))

    | |
    |1|
    |1|2|
    |1|2|3| |
    |1|2|3|4|
    |1|2|3|4|5| | | |
    |1|2|3|4|5|6| | |
    |1|2|3|4|5|6|7| |
    |1|2|3|4|5|6|7|8|
    |1|2|3|4|5|6|7|8|9| | | | | | | |

### pop back (O(1))

    |1|2|3|4|5|6|7|8|9| | | | | | | |
    |1|2|3|4|5|6|7|8| | | | | | | | |
    |1|2|3|4|5|6|7| | | | | | | | | |
    |1|2|3|4|5|6| | | | | | | | | | |
    |1|2|3|4|5| | | | | | | | | | | |
    |1|2|3|4| | | | | | | | | | | | |
    |1|2|3| | | | | | | | | | | | | |
    |1|2| | | | | | | | | | | | | | |
    |1| | | | | | | | | | | | | | | |
    | | | | | | | | | | | | | | | | |

### push front (either O(n) or O(1))

    | |
    |1|
    |2|1|
    |3|2|1| |
    |4|3|2|1|
    |5|4|3|2|1| | | |
    |6|5|4|3|2|1| | |
    |7|6|5|4|3|2|1| |
    |8|7|6|5|4|3|2|1|
    |9|8|7|6|5|4|3|2|1| | | | | | | |


    | | | | .2|1| | |  (arbitrary state)
    | | | .3|2|1| | |
    | | .4|3|2|1| | |
    | .5|4|3|2|1| | |
    |6|5|4|3|2|1| | |
    |7|6|5|4|3|2|1| |
    |8|7|6|5|4|3|2|1|
    |9|8|7|6|5|4|3|2|1| | | | | | | |

### pop front (O(1))

    |9|8|7|6|5|4|3|2|1| | | | | | | |
    | .8|7|6|5|4|3|2|1| | | | | | | |
    | | .7|6|5|4|3|2|1| | | | | | | |
    | | | .6|5|4|3|2|1| | | | | | | |
    | | | | .5|4|3|2|1| | | | | | | |
    | | | | | .4|3|2|1| | | | | | | |
    | | | | | | .3|2|1| | | | | | | |
    | | | | | | | .2|1| | | | | | | |
    | | | | | | | | .1| | | | | | | |
    | | | | | | | | | . | | | | | | |

Deque
-----

### push back (amortized O(1))

                                  | : |
                                  | :1|
                                  | :1|2|
                                  | :1|2|3| |
                                  | :1|2|3|4|
                                  | :1|2|3|4|5| | | |
                                  | :1|2|3|4|5|6| | |
                                  | :1|2|3|4|5|6|7| |
                                  | :1|2|3|4|5|6|7|8|
                                  | :1|2|3|4|5|6|7|8|9| | | | | | | |

### pop back (O(1))

                                  | :1|2|3|4|5|6|7|8|9| | | | | | | |
                                  | :1|2|3|4|5|6|7|8| | | | | | | | |
                                  | :1|2|3|4|5|6|7| | | | | | | | | |
                                  | :1|2|3|4|5|6| | | | | | | | | | |
                                  | :1|2|3|4|5| | | | | | | | | | | |
                                  | :1|2|3|4| | | | | | | | | | | | |
                                  | :1|2|3| | | | | | | | | | | | | |
                                  | :1|2| | | | | | | | | | | | | | |
                                  | :1| | | | | | | | | | | | | | | |
                                  | : | | | | | | | | | | | | | | | |

### push front (amortized O(1))

                                  | : |
                                  |1: |
                                |2|1: |
                            | |3|2|1: |
                            |4|3|2|1: |
                    | | | |5|4|3|2|1: |
                    | | |6|5|4|3|2|1: |
                    | |7|6|5|4|3|2|1: |
                    |8|7|6|5|4|3|2|1: |
    | | | | | | | |9|8|7|6|5|4|3|2|1: |

### pop front (O(1))

    | | | | | | | |9|8|7|6|5|4|3|2|1: |
    | | | | | | | | |8|7|6|5|4|3|2|1: |
    | | | | | | | | | |7|6|5|4|3|2|1: |
    | | | | | | | | | | |6|5|4|3|2|1: |
    | | | | | | | | | | | |5|4|3|2|1: |
    | | | | | | | | | | | | |4|3|2|1: |
    | | | | | | | | | | | | | |3|2|1: |
    | | | | | | | | | | | | | | |2|1: |
    | | | | | | | | | | | | | | | |1: |
    | | | | | | | | | | | | | | | | : |

### all in one (amortized O(1))

                                  | : |
                                  | :o|
                                  | :x|o|
                                  | :x|x|o| |
                                  | :x|x|x|o|
                                  | :x|x|x|x|o| | | |
                                  |o:x|x|x|x|x| | | |
                                |o|x:x|x|x|x|x| | | |
                            | |o|x|x:x|x|x|x|x| | | |
                            |o|x|x|x:x|x|x|x|x| | | |
                    | | | |o|x|x|x|x:x|x|x|x|x| | | |
                    | | | | |x|x|x|x:x|x|x|x|x| | | |
                    | | | | | |x|x|x:x|x|x|x|x| | | |
                    | | | | | | |x|x:x|x|x|x|x| | | |
                    | | | | | | | |x:x|x|x|x|x| | | |
                    | | | | | | | | :x|x|x|x|x| | | |
                    | | | | | | | | : .x|x|x|x| | | |
                    | | | | | | | | : | .x|x|x| | | |
                    | | | | | | | | : | | .x|x| | | |
                    | | | | | | | | : | | .x|x|o| | |
                    | | | | | | | | : | | .x|x|x|o| |
                    | | | | | | | | : | | .x|x|x|x|o|
       (see below)  | | | | | | | | :x|x|x|x|x|o| | | | | | | | | | |
                    | | | | | | | | :x|x|x|x|x|x|o| | | | | | | | | |

Memory Management Strategies
----------------------------

### Naive (amortized O(1))

     i   s   c
    --- --- ---
     0   1   0  | |              i: step
     1   1   1  |1|              s: size
     2   2   2  |1|2|            c: cost
     3   2   1  | .2|
     4   4   2  | .2|3| |        growth factor      = 2
     5   4   1  | | .3| |        number of elements = 2
     6   4   1  | | .3|4|
     7   4   1  | | | .4|
     8   8   2  | | | .4|5| | | |
     9   8   1  | | | | .5| | | |
    10   8   1  | | | | .5|6| | |
    11   8   1  | | | | | .6| | |
    12   8   1  | | | | | .6|7| |
    13   8   1  | | | | | | .7| |
    14   8   1  | | | | | | .7|8|
    15   8   1  | | | | | | | .8|
    16  16   2  | | | | | | | .8|9| | | | | | | |

### Reclaiming (amortized O(1))

     i   s   c
    --- --- ---
     0   1   0  | |              i: step
     1   1   1  |1|              s: size
     2   2   2  |1|2|            c: cost
     3   2   1  | .2|
     4   4   2  |2|3| | |        growth factor      = 2
     5   4   1  | .3| | |        number of elements = 2
     6   4   1  | .3|4| |
     7   4   1  | | .4| |
     8   4   1  | | .4|5|
     9   4   1  | | | .5|
    10   8   2  |5|6| | | | | | |
    11   8   1  | .6| | | | | | |
    12   8   1  | .6|7| | | | | |
    13   8   1  | | .7| | | | | |
    14   8   1  | | .7|8| | | | |
    15   8   1  | | | .8| | | | |
    16   8   1  | | | .8|9| | | |

### Conservative (amortized O(1))

     i   s   c
    --- --- ---
     0   1   0  | |              i: step
     1   1   1  |1|              s: size
     2   2   2  |1|2|            c: cost
     3   2   1  | .2|
     4   4   2  |2|3| | |        growth factor      = 2
     5   4   1  | .3| | |        number of elements = 2
     6   4   1  | .3|4| |        load factor limit  = 0.5
     7   4   1  | | .4| |
     8   4   1  | | .4|5|
     9   4   1  | | | .5|
    10   4   2  |5|6| | |
    11   4   1  | .6| | |
    12   4   1  | .6|7| |
    13   4   1  | | .7| |
    14   4   1  | | .7|8|
    15   4   1  | | | .8|
    16   4   2  |8|9| | |

Experiments
-----------

### Fill Back Test

Fill back test is simply pushing n elements to the container in sequence using `push_back`.

### Fill Back Reserved Test

Fill back reserved test is just like fill back test but it calls `reserve_back` to prevent unnecessary reallocations during pushes.
Since there are no such methods in `StdDeque` and `StdList` we just use values from regular fill back test for these containers.
`StdVector`'s `reserve_back` is just a wrapper for the `reserve` method.

### Fill Front Test

Fill back test is simply pushing n elements to the container using `push_front`.
Note that `StdVector` and `queue`'s are not run in these tests as they have high complexities.
This is denoted with '(NR)' prefix in the legend.
These structures should be expected to behave much slower than the ones that are shown in the graph.

### Fill Front Reserved Test

For the sake of completeness, these are the results for fill front test which calls `reserve_front` method before insertions.
`StdVector` is not run in this test as there is no way to implement it in vectors without an offset variable.

### Queue Test

Queue test is the case that is described in the memory reclaiming section.
In this test containers are used as a FIFO queue alternatingly `pushing_back` and `popping_front` while having a fixed number of element k which is selected to be 1000 in these runs.
`StdVector` is once again excluded from the test for having a high complexity.

### Zigzag Test

Zigzag test is a made-up test that make use of pushing and popping from back and front in a balanced fashion.
Specifically in this test:

- n / 4 elements are pushed back
- n / 4 elements are pushed front
- n / 2 elements are popped back
- n / 4 elements are pushed front
- n / 4 elements are pushed back
- n / 2 elements are popped front

`StdVector` and `queue`'s are excluded from this test.

### Traverse Test

Traverse test iterates over the container and simply increase the first integer in the underlying structure.
It is possible to iterate over `StdList` but it requires using either iterators or something quite different than others as it doesn't provide `[]` operator hence it is excluded from this test.
Since this test substantially measures the locality of containers one should expect `StdList` to be slower than others.

### Shuffle Test

Shuffle test uses the [Fisher-Yates][] algorithm to shuffle a pre-filled container.
`StdList` is excluded from the test for requiring a different shuffle algorithm than others.

[Fisher-Yates]: http://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle

### QSort Test

QSort test involves using [in-place quicksort][] on pre-filled and pre-shuffled containers.
Elements are compared to each other only by considering the value of their first integer in the structure.
`StdList` is once again excluded from the test for requiring a different algorithm than others.

[in-place quicksort]: http://en.wikipedia.org/wiki/Quicksort#In-place_version

### Fill Back Test (Memory)

Fill back memory test is the same as fill back test but this time we measure the average load factor between insertions.
`StdList` and `StdDeque` are excluded from the test for either having a load factor of 1 or very close to 1 at all times.

### Fill Front Test (Memory)

Just for the sake of completeness same test is run for fill front.

### Queue Test (Memory)

This test measures the memory usage of the case that is described in memory reclaiming section for k = 1000.

### Zigzag Test (Memory)

This test shows the average load factor in the zigzag test.

Others
------

- <http://www.gotw.ca/gotw/054.htm>
- <http://www.gotw.ca/publications/mill10.htm>
- <http://www.baptiste-wicht.com/2012/12/cpp-benchmark-vector-list-deque/>
- <http://www.codeproject.com/Articles/5425/An-In-Depth-Study-of-the-STL-Deque-Container>
