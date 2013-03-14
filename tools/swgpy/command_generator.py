# This file is part of SWGANH which is released under the MIT license.
# See file LICENSE or go to http://swganh.com/LICENSE
#
# Author : rcythr (Richard Laughlin)
# Description: Generates object templates from an output csv file

# At the time of creation, the SQL command used to generate the csv was:
# SELECT iff.iff_template, iff.object_type, 0, iff.attribute_template_id, o.objectName_1, o.objectName_2
# FROM iff_templates iff
# INNER JOIN objects o ON o.object_string = iff.iff_template
# WHERE iff.object_type != 0;
#
# INTO OUTFILE 'C:\\templates.csv'
# FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
# ESCAPED BY '\\'
# LINES TERMINATED BY '\n'
# 

import os, sys

CSV_FILENAME = "commands.csv"
TEMPLATE_FILENAME = "command_template.txt"
CPP_OUT = "cpp_out.txt"

def main(force):
	#Load the template line by line
	template = []
	for line in open(TEMPLATE_FILENAME):
		template.append(line)
		
	saved_files = []
	
	with open(CPP_OUT, 'w') as cpp_out:
		#For every line in the csv
		for input in open(CSV_FILENAME):
			csv = []
			saved = False
			for i in map(lambda x: x.replace('\"', '').replace('\'', ''), input.strip().split(",")):
				csv.append(i)
		
			name, scriptHook, failScriptHook = csv
			if failScriptHook == 'failAdmin':
				continue
		
			lname = name.lower()
			cpp_out.write("//command_service->AddCommandCreator(\""+lname+"\", swganh::command::PythonCommandCreator(\"commands."+lname+"\", \""+commandClassName(name)+"\"));\n")
		
			#Ensure the folder exists
			out_file =  '../../data/scripts/commands/' +lname
			dir = os.path.dirname(out_file)
			if not os.path.exists(dir):
				os.makedirs(dir)
		
			#Convert entry at 0 to filename
			output_filename = "./" + out_file + ".py"
			
			replaces = []
			if os.path.isfile(output_filename):
				replace = None
				#Read in preserved targets sections
				for line in open(output_filename):
					if not force and line.find(stop_condition) != -1:
						saved = True
						saved_files.append(output_filename)
						break
					elif line.find(preserved_replace[0]) != -1:
						replace = ""
					elif line.find(preserved_replace[1]) != -1:
						replaces.append(replace)
						replace = None
					elif replace is not None:
						replace += line
			
			if saved:
				continue
			
			#Open output file for writing
			with open(output_filename, 'w') as output:
				#print('GENERATING', output_filename)
				for line in template:
					if line.find(preserved_replace[0]) != -1:
						output.write(line)
						if len(replaces) > 0:
							output.write(replaces[0])
							replaces = replaces[1:]
						continue
					elif line.find(preserved_replace[1]) != -1:
						output.write(line)
						continue
					else:
						#Replace CSV Targets
						for target, index, func in csv_replace_targets:
							if line.find(target) != -1:
								line = line.replace(target, str(func(csv[index])))
						output.write(line)
					
	## Create __init__.py for each subfolder if doesn't exist
	for dirname, subdirs, filenames in os.walk('.'):
		if not os.path.isfile(dirname + os.sep + '__init__.py'):
			with open(dirname + os.sep + '__init__.py', 'w') as init_py:
				init_py.write('')
	
	if len(saved_files) > 0:
		print("The following files were saved. You must run with -f, or remove the MODIFIED TAG")
		for file in saved_files:
			print(file)
	
def commandClassName(input):
	return input[0].upper() + input[1:] + "Command"
	
csv_replace_targets = []
csv_replace_targets.append(("@COMMAND_NAME@", 0, commandClassName))

preserved_replace = ("#### BEGIN MODIFICATIONS ####", "####  END MODIFICATIONS  ####")

stop_condition = "#### MODIFIED ####"

#Call to the main program
main((len(sys.argv) > 1 and sys.argv[1] == "-f"))