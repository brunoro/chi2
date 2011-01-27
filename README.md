Chi2
====
A pure-C implementation of the [chi2 feature selection](http://nlp.stanford.edu/IR-book/html/htmledition/feature-selectionchi2-feature-selection-1.html) that can handle about hierarchy, quite used on text classification.

Compiling
---------
This program should compile on any gcc version, but it was only tested on Linux and Mac OS X. 
A Makefile is included, so to compile, just go into the chi2 directory and:

    $ make

Running
-------
The program can be executed as:

        $ chi2 -i [input file] -o [output file] -n [number of features to be selected] -c [base class] -s [class separator]

The input file format is an [arff](http://www.cs.waikato.ac.nz/~ml/weka/arff.html) file, used by [Weka](www.cs.waikato.ac.nz/ml/weka/). 

There is a test file on 'test/0.arff ', and the 'make run' option should run this test against "comp.sys" selecting 80 features.

Roadmap
-------
Plans for any (but unlikely) future updates are the inclusion of other techniques (Information Gain) and support of other file formats (XML, CSV).


TODO
----
update all comments on code to English
