.PHONY: build scan html

rescan: build scan html

build:
	cov-build --dir im gcc 1.c

scan:
	cov-analyze --dir im --all --aggressiveness-level high

html:
	cov-format-errors --dir im --html-output html --filesort -x

web:
	python -m SimpleHTTPServer
