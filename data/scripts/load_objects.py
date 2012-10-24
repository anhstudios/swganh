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
			files += glob.glob('templates.' + subdirname + '/*.py')			
		for file in filenames:
			files.append('templates.' + file)

for file in files:
	file = file.replace('./','').replace(os.sep, '.').replace('.py', '')
	if file != '__init__.py':
		exec('import ' + file)
		file.loadTemplates(templates, prototypes)