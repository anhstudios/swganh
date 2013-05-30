'''
    This file is part of SWGANH which is released under the MIT license.
    See file LICENSE or go to http://swganh.com/LICENSE
    
    Run example:
        python3 setup.py [input value 1] [input value 2] [input value 3]
    
    Description:
        Before displaying the menu the menu runs all of the given input values.
        If one of the input values is q the program will quit. Otherwise it will prompt.
'''

import os, sys, datetime
from configparser import RawConfigParser

#### CONSTANTS ####

defaults = {'username': 'swganh', 'password': 'swganh', 'host': 'localhost', 'pause': 'on', 'clear': 'off'}

affirmatives = ['yes', 'on', '1', 'true', 'y']

#### Internationalization (I18N) Strings ####
I18N = {
    'PROGRAM_TITLE': ' SWGANH Database Install Script                              ',
    'PROGRAM_VERSION': '(v0.5.1) ',
    'INSTALL_ALL_MSG': 'Install All DB',
    'INSTALL_MSG': 'Install ',
    'BACKUP_MSG': 'Backup DB',
    'RESTORE_MSG': 'Restore DB',
    'RECONFIG_MSG': 'Reconfigure Setup.py',
    'PATIENCE_MSG': 'Operation started. Please be patient...',
    'NO_BACKUPS_MSG': 'No backup files found.',
    'FOUND_ONE_MSG': ('Found only one file (', ') should this file be used? '),
    'WHICH_BACKUP_MSG': 'Which backup should be installed? ',
    'INVALID_CHOICE_MSG': 'Invalid choice.',
    'RESTORE_FAILED_MSG': 'Restore failed.',
    'CONFIG_MISSING_MSG': 'Config missing or invalid. Is this the first run?',
    'MAKE_CHOICE_MSG': 'Make a choice or (q)uit: ',
    'OPERATION_COMPLETE_MSG': 'Operation Complete. Press any key to continue...',
    'INSTALLING_MSG': 'Installing',
    'DONE_MSG': '[DONE]',
    'USERNAME_PROMPT_MSG': 'Mysql Username (default=%(username)s): ' % defaults,
    'PASSWORD_PROMPT_MSG': 'Mysql Password (default=%(password)s): ' % defaults,
    'HOST_PROMPT_MSG': 'Mysql Host (default=%(host)s): ' % defaults,
    'PAUSE_PROMPT_MSG': 'Pause after each operation? (default=%(pause)s): ' % defaults,
    'CLEAR_PROMPT_MSG': 'Clear screen before printing menu? (default=%(clear)s): ' % defaults,
}

#### MAIN FUNCTION ####

def main(args):
    '''
        The main method.
        
        Parameters:
            args - the system arguments (sys.argv)
    '''
    #Load all configuration data
    cfg = getCfg()
    cfg['cwd'] = os.getcwd()
    cfg['databases'] = getImmediateSubdirectories(cfg['cwd'])
    
    #Defer processing to the main menu
    Menu(cfg).run(args[1:])

#### OPERATIONS ####

def installDatabase(cfg, database):
    '''
        Installs the given database.
        
        Parameters:
        cfg - the dictionary of config values
        database - the name of the database to install.
    '''
    for dirpath, _, filenames in os.walk(os.path.join(cfg['cwd'], database)):
        cfg['database'] = database
        for filename in filenames:
            callMysql(cfg, os.path.join(dirpath, filename))

def backupDB(cfg):
    '''
        Backs up the db to a timestampped file.
        
        Parameters:
        cfg - the dictionary of config values
    '''
    print(I18N['PATIENCE_MSG'])
    timestamp = str(datetime.datetime.now()).replace(' ', '_').replace(':', '_')
    os.system(("mysqldump --password="+cfg['password']+" --host="+cfg['host']+" --user="+cfg['username']+" "+
        "--databases "+(" ".join(cfg['databases']))+" --create-options --extended-insert --routines "+
        "--dump-date --triggers --comments > swganh_"+timestamp+".sql"))

def selectFile(files):
    '''
        Prompts the user to select a file from the given files list.
        
        Parameters:
        files - a list of files to display to the user. Can be empty.
        
        Return:
        The selected index, or None if a selection cannot be determined.
    '''
    selectedBackup = None
    
    #Handle the 3 cases separately.
    if len(files) == 0:
        print(I18N['NO_BACKUPS_MSG'])
    elif len(files) == 1:
        choice = input(I18N['FOUND_ONE_MSG'][0]+files[0]+I18N['FOUND_ONE_MSG'][1])
        if choice in affirmatives:
            selectedBackup = 0
    else:
        #Print the options for the user.
        for i in range(len(files)):
            print('\t'+str(i)+'. '+files[i])
            
        #Make sure the user enters a number in the range [0, len(files)).
        try:
            selectedBackup = int(input(I18N['WHICH_BACKUP_MSG']))
            if selectedBackup < 0 or selectedBackup >= len(files):
                raise ValueError
        except ValueError:
            print(I18N['INVALID_CHOICE_MSG'])
            selectedBackup = None
    #Return the result
    return selectedBackup
        
def restoreDB(cfg):
    '''
        Restores a copy of the db from an .sql dump.
        
        Parameters:
        cfg - the dictionary of config values.
    '''
    
    #Get the list of .sql files in the current directory
    files = getImmediateSqlFiles(cfg['cwd'])
    
    #Determine what file to use
    index = selectFile(files)
    
    #Make sure the index is valid.
    if index == None:
        print(I18N['RESTORE_FAILED_MSG'])
    else:
        print(I18N['PATIENCE_MSG'])
        callMysql(cfg, files[index], False)
    
#### MENU MANAGEMENT ####
class Menu(object):
    '''
        Encapsulates the main menu in a Menu object.
        Slots:
            cfg - the configuration dictionary
            operations - a list of operations. Each operation is a 2-tuple of (operation name, operation functor)
    '''
    __slots__ = ('cfg', 'operations')

    def __init__(self, cfg):
        '''
            Creates a new Menu Object
        '''
        self.cfg = cfg
        self.operations = []

        #Add some operations!
        self.operations.append((I18N['INSTALL_ALL_MSG'], lambda cfg: list(map((lambda db: installDatabase(cfg, db)), cfg['databases']))))
        for i in range(len(cfg['databases'])):
            self.operations.append((I18N['INSTALL_MSG']+cfg['databases'][i], lambda cfg, i=i: installDatabase(cfg, cfg['databases'][i])))
        self.operations.append((I18N['BACKUP_MSG'], backupDB)) 
        self.operations.append((I18N['RESTORE_MSG'], restoreDB))
        self.operations.append((I18N['RECONFIG_MSG'], rebuildCfg))

    def printMenu(self):
        '''
            Optionally clears the screen based upon config values, and then prints
            the menu.
        '''
        #Clear screen in a cross platform fashion
        if self.cfg['clear'].lower() in affirmatives:
            os.system( [ 'clear', 'cls' ][ os.name == 'nt' ] )

        print('----------------------------------------------------------------------')
        print(I18N['PROGRAM_TITLE']+I18N['PROGRAM_VERSION'])
        print('----------------------------------------------------------------------')

        for i in range(len(self.operations)):
            print("\t"+str(i)+". "+self.operations[i][0])

        print('----------------------------------------------------------------------')

    def run(self, buf=[]):
        '''
            Prints the screen and then loops over inputs until a 'q' is found.
            
            Parameters:
                buf - a list of operations to perform
        '''
        if len(buf) == 0:
            self.printMenu()
            buf = input(I18N['MAKE_CHOICE_MSG']).split()
        while(True):
            try:
                #Grab the next step
                step = buf.pop(0)
                
                #Check if the input is our exit condition
                if step == 'q':
                    break
                else:
                    #Check that the buffer is a valid entry.
                    step = int(step)
                    if step < 0 or step >= len(self.operations):
                        raise ValueError()
                    
                    #Attempt to execute the option, catching exceptions in case of problems.
                    try:
                        self.operations[step][1](self.cfg)

                        if self.cfg['pause'].lower() in affirmatives:
                            input(I18N['OPERATION_COMPLETE_MSG'])
                    except:
                        print(sys.exc_info()[0])
            except:
                print(I18N['INVALID_CHOICE_MSG'])
                
            #Check if we need more input
            if len(buf) == 0:
                self.printMenu()            
                buf = input(I18N['MAKE_CHOICE_MSG']).split()
            
            

#### CONFIGURATION MANAGEMENT ####

def rebuildCfg(oldCfg=None):
    '''
        Rebuilds the configuration file from user inputs.
        
        Optional Parameters:
            oldCfg - if defined values will be inserted into this dict instead of returned.
            
        Return:
            the new configuration dictionary.
    '''
    cfgParse = RawConfigParser()
    cfgParse['DEFAULT'] = defaults

    cfg = {}

    #Get user inputs
    cfg['username'] = input(I18N['USERNAME_PROMPT_MSG'])
    cfg['password'] = input(I18N['PASSWORD_PROMPT_MSG'])
    cfg['host'] = input(I18N['HOST_PROMPT_MSG'])
    cfg['pause'] = input(I18N['PAUSE_PROMPT_MSG'])
    cfg['clear'] = input(I18N['CLEAR_PROMPT_MSG'])

    #Set the default if the user chose to input nothing
    for k,v in defaults.items():
        if cfg[k] == '':
            cfg[k] = v

    #Write out data
    cfgParse['DEFAULT'] = cfg
    cfgParse.write(open('setup.cfg', 'w'))
    
    #Determine if we need to reset live values, or just return
    if oldCfg != None:
        for k,v in cfg.items():
            oldCfg[k] = v
    else:
        return cfg

def getCfg():
    '''
        Loads the config from disk or rebuilds it from user input.
        
        Return:
            the dictionary of user inputs
    '''
    try:
        #Attempt to load the file from the config
        with open('setup.cfg') as fp:
            cfgParse = RawConfigParser()
            cfgParse['DEFAULT'] = defaults            
            cfgParse.readfp(fp)
            return dict(cfgParse['DEFAULT'])
    except:
        #If we failed we'll rebuild the config.
        print(I18N['CONFIG_MISSING_MSG'])
        return rebuildCfg()

#### MYSQL MANAGEMENT ####

def getImmediateSubdirectories(path):
    '''
        Return:
            a list of immediate subdirectories
    '''
    return [subdir for subdir in os.listdir(path) 
        if os.path.isdir(os.path.join(path, subdir))]

def getImmediateSqlFiles(path):
    return [file for file in os.listdir(path) 
        if os.path.isfile(os.path.join(path, file)) and os.path.splitext(file)[1] == ('.sql')]
        
def callMysql(cfg, filename, needsDb=True):
    '''
        Installs the given filename into the proper database.
        
        Parameters:
            cfg - the configuration dictionary
            filename - the file to install.
    '''    
    if os.path.splitext(filename)[1] == '.sql':
        cfg['filename'] = filename
        
        #Find the short name
        shortname = os.path.split(filename)[1]
    
        #Add databases unless it's a create.sql script.
        dbs = ''
        if shortname != 'create.sql' and needsDb:
            dbs = '--database=%(database)s ' % cfg
        
        print(I18N['INSTALLING_MSG']+" {} ".format(shortname), end="")
        os.system("mysql --password=%(password)s --host=%(host)s --user=%(username)s " % cfg + dbs + "--default-character-set=utf8 < \"%(filename)s\"" % cfg)
        print(I18N['DONE_MSG'])

main(sys.argv)
