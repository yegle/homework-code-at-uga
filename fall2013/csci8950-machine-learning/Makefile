TEX := $(wildcard *.tex)
PDF := $(TEX:.tex=.pdf)

all: $(PDF)

%.pdf: %.tex
	xelatex $<
	xelatex $<
	open $(<:.tex=.pdf)
