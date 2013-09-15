all: p1.png

p1.png: p1.dot
	dot -Tpng p1.dot -o p1.png

p1.dot: p1.py
	python p1.py

.PHONY: clean

clean:
	rm -v *.dot *.png
