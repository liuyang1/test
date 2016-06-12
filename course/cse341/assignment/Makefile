dep:
	wget https://courses.cs.washington.edu/courses/cse341/15au/assignments/OctopusInterpreter-starter.hs
	wget https://courses.cs.washington.edu/courses/cse341/15au/assignments/OctoParser.y
	echo "install happy for compiling *.y file"

pre:
	mv OctopusInterpreter-starter.hs OctopusInterpreter.hs
	happy OctoParser.y

clean:
	rm -rf OctoParser.hs
