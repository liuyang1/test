#! /bin/python 
import cmd
import sys
class CLI(cmd.Cmd):
	def __init__(self):
		cmd.Cmd.__init__(self)
		self.prompt='> '
	def do_hi(self,arg):
		print "hi,cmd! \n",arg
	def do_quit(self,arg):
		sys.exit(0)
	do_q=do_quit

cli=CLI()
cli.cmdloop()
