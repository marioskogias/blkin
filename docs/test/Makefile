#######################################################
#
# Makefile
#  
#######################################################

XELATEX ?= xelatex
DVIPS ?= dvips
PS2PDF ?= ps2pdf
BIBTEX ?= bibtex
SED ?= /bin/sed
WC ?= /usr/bin/wc
DETEX ?= detex

all: article.pdf

clean:
	rm -f *log *aux *blg *bbl *dvi article.out article.pdf *toc *ps *~

article.pdf: *.tex article.bbl
	$(XELATEX) article.tex
	$(XELATEX) article.tex

article.aux: article.tex
	$(XELATEX) article.tex

article.bbl: article.aux
	$(BIBTEX) article.aux

article.ps: article.dvi
	$(DVIPS) -o article.ps article.dvi

wc: article.tex
	$(DETEX) -l article.tex|$(WC) -w

aspell: article.tex
	for i in *tex; do aspell --lang=en_US.UTF8 check $$i;done
