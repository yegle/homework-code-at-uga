all: hw1.pdf hw2.pdf

%.pdf: %.tex
	xelatex $<
	xelatex $<
	open $(<:.tex=.pdf)
