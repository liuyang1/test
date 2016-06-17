all: OctopusInterpreter

dep:
	wget https://courses.cs.washington.edu/courses/cse341/15au/assignments/OctopusInterpreter-starter.hs
	wget https://courses.cs.washington.edu/courses/cse341/15au/assignments/OctoParser.y
	echo "install happy for compiling *.y file"

pre:
	mv OctopusInterpreter-starter.hs OctopusInterpreter.hs

OctopusInterpreter: OctopusInterpreter.hs OctoParser.hs
	ghc OctopusInterpreter.hs

OctoParser.hs:OctoParser.y
	happy OctoParser.y

run: OctopusInterpreter
	echo run | ghci OctopusInterpreter.hs

try:
	ghci OctopusInterpreter.hs OctoParser.hs

clean:
	rm -rf OctoParser.hs *.hi *.o a.out

lint:OctoParser.hs
	ghc -Wall OctopusInterpreter.hs
	# hlint OctopusInterpreter.hs
