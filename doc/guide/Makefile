all: guide.pdf

%.pdf: %.tex hardware.tex software.tex troubleshooting.tex debugging.tex openocd.tex assessment.tex git.tex requirements.tex PCB.tex schematic.tex rework.tex schedule.tex
	pdflatex -shell-escape $<
	pdflatex -shell-escape $<

figs/%.schtex: figs/*.sch
	schtex $< $@

clean:
	-rm -rf *.out *.aux *.log

push:
	git push
