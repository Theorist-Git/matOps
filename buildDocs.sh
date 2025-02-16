#!/usr/bin/sh

set -xe

doxygen Doxyfile

cd latex/

awk '/\\documentclass/ { 
    print; 
    print "\\let\\cleardoublepage\\clearpage"; 
    next 
} { 
    print 
}' refman.tex > tmp && mv tmp refman.tex

pdflatex refman.tex
pdflatex refman.tex
pdflatex refman.tex

cp refman.pdf ..
cd ..
mv refman.pdf documentation.pdf