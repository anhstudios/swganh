''' SWGANH 2012 
	This script is used on start up to prepare the list of templates
	for the server. It uses lazy loading to avoid loading unnecessary
	modules.
'''

import sys, os, glob, importlib
from swgpy.object import *

class TemplateProxy(object):
	'''
		A class which uses the proxy pattern to delay loading of
		python modules until they are used.
	'''
	__slots__ = ('file', 'create')
	
	def __init__(self, file):
		'''
			Constructor
			@param file the path to the module to lazy load
		'''
		self.file = file
		self.create = None
	
	def __call__(self, kernel):
		'''
			This method allows the TemplateProxy class to masquerade as a
			function in order to lazy load the module with the real function
		'''
		if self.create == None:
			module = importlib.import_module(self.file)
			self.create = module.create
		return self.create(kernel)

templates = TemplateMap()
template_files = []

#Attempt to load the cache. If an exception is thrown we will catch it and
#assume that it is the first run and that the cache needs to be generated
try:
	#For each line in the file put the (name, file) pairs into the 
	#template_files list for processing
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

	#For each of the files we found in the proper folders
	for name in files:
		#Do some work on the name to get the iff name and the module filename
		file = name.replace('..' + os.sep, '').replace('./','').replace(os.sep, '.').replace('.py', '').replace('data.scripts.', '')
		name = name[name.find('object'):].replace(os.sep, '/').replace('.py', '.iff')
		
		#We do not want to work with the necessary, but useless __init__.py files		
		if '__init__' in file:
			continue
		
		#Place the result into the list, and cache it
		template_files.append((name, file))
		cache_out.write(name+'\0'+file+'\n');

#We've got our pairs (either from caching or generation)		
for name, file in template_files:
	#Make the proxies and put them into the proper dictionary
	templates[name] = TemplateProxy(file)