''' SWGANH 2012 '''
''' This script is used to generate a list of all the game objects'''
''' Ensure this is run under the expected working directory '''

import sys, os, io, glob, importlib
from swgpy.object import *
templates = TemplateMap()
prototypes = PrototypeMap()

def addTemplate(template):
	#templates[template.name] = template
	#if template.is_prototype:
	prototypes[template.name] = template.create({})

subdirs = []
files = []
for p in sys.path:
	for dirname, dirnames, filenames in os.walk(p + '/templates'):
		for subdirname in dirnames:
			for f in glob.glob(dirname + os.sep + subdirname+ os.sep + '*.py'):
				files.append(os.path.relpath(f))
		for fil in glob.glob(dirname + os.sep + '*.py'):
			files.append(os.path.relpath(fil))
for file in files:
	file = file.replace('..' + os.sep, '').replace('./','').replace(os.sep, '.').replace('.py', '').replace('data.scripts.', '')
	if not '__init__' in file:
		module = importlib.import_module(file)
		module.loadTemplates(addTemplate)