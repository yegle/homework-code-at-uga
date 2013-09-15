SRCS = $(wildcard *.py)
TARGETS = $(SRCS:.py=.eps)

all: $(TARGETS)

%.eps: %.dot
	dot -Teps $< -o $(<:.dot=.eps)

%.dot: %.py
	python $<

.PHONY: clean

clean:
	rm -v *.dot *.eps
