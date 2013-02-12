''' SWGANH 2012 '''
''' This script is used to generate a list of all the game objects'''
''' Ensure this is run under the expected working directory '''

import sys, os, io, glob, importlib
from swgpy.object import *
templates = TemplateMap()

class TemplateProxy(object):
	__slots__ = ('file', 'create')
	
	def __init__(self, file):
		self.file = file
		self.create = None
	
	def __call__(self, kernel):
		if self.create == None:
			module = importlib.import_module(self.file)
			self.create = module.create
		return self.create(kernel)

template_files = []
try:
	for line in open('object_cache.tmp'):
		template_files.append(line.strip().split('\0'))
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
				
	for name in files:
		file = name.replace('..' + os.sep, '').replace('./','').replace(os.sep, '.').replace('.py', '').replace('data.scripts.', '')
		name = name[name.find('object'):].replace(os.sep, '/').replace('.py', '.iff')
		if name.find('vehicle') != -1:
			print(name)
			
		if not '__init__' in file:
			template_files.append((name, file))
			cache_out.write(name+'\0'+file+'\n');
			
for name, file in template_files:
	templates[name] = TemplateProxy(file)