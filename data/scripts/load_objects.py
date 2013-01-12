''' SWGANH 2012 '''
''' This script is used to generate a list of all the game objects'''
''' Ensure this is run under the expected working directory '''

import sys, os, io, glob, importlib
from swgpy.object import *
templates = TemplateMap()
prototypes = PrototypeMap()
tempInterface = BaseTemplate()

def addTemplate(template):
	if template.is_prototype:
		prototypes[template.name] = template.create({})
	else:
		templates[template.name] = template


template_files = []
try:
	for line in open('object_cache.tmp'):
		template_files.append(line.strip())
except IOError as e:
	#This is the first load, so we can't take advantage of caching.
	cache_out = open('object_cache.tmp', 'w')

	files = []
	subdirs = []
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
			template_files.append(file)
			cache_out.write(file+'\n');
			
for file in template_files:
	module = importlib.import_module(file)
	module.loadTemplates(addTemplate)