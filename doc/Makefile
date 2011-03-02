#
# Makefile pour compiler la doc et autre tools pour kopt
#

DOC = kopul_specification

doc: ${DOC}.tex
	latex ${DOC}.tex
	dvipdf ${DOC}.dvi

clean:
	rm -rf *~ ${DOC}.dvi