default: pdf

pdf:
	pdflatex *.tex
	bibtex *.aux
	pdflatex *.tex
	pdflatex *.tex

word:
	detex *.tex | wc -w

clean:
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.log
	rm -f *.out
	rm -f *.toc
	rm -f *~
	rm -f .DS_Store
