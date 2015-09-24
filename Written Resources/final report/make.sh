#!/bin/bash
pdflatex AshleyMansonFinalReport.tex
bibtex AshleyMansonFinalReport.aux
pdflatex AshleyMansonFinalReport.tex
pdflatex AshleyMansonFinalReport.tex
detex AshleyMansonFinalReport.tex | wc -w
