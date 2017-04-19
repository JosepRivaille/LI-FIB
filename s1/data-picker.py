from os import system, listdir
from os.path import isfile, join
from subprocess import call

vars_path = 'vars'
vars_files = [f for f in listdir(vars_path) if isfile(join(vars_path, f))]
for f in vars_files:
	command = ''.join(['./sat < ', vars_path, '/', f])
	system(command)