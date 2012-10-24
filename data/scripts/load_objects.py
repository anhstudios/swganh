''' SWGANH 2012 '''
''' This script is used to generate a list of all the game objects'''
''' Ensure this is run under the expected working directory '''

import sys, os, io, glob
from swgpy.object import *
templates = TemplateMap()
prototypes = PrototypeMap() 

subdirs = []
files = []
for p in sys.path:
	for dirname, dirnames, filenames in os.walk(p + '/templates'):
		for subdirname in dirnames:
			for f in glob.glob(dirname + os.sep + subdirname+ os.sep + '*.py'):
				files.append(os.path.relpath(f))
		for fil in glob.glob(dirname + os.sep + '*.py'):
			files.append(os.path.relpath(fil))
print(files)
for file in files:
	file = file.replace('..' + os.sep, '').replace('./','').replace(os.sep, '.').replace('.py', '').replace('data.scripts.', '')
	if not '__init__' in file:
		exec('import ' + file + ' as module')
		module.loadTemplates(templates, prototypes)