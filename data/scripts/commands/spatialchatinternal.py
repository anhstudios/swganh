# All command scripts have a context that contains a reference to
# the object that initiated the command, the target of the command,
# a string containing any parameters, and finally a dictionary containing
# information about the command.
#
# anh.context.object
# anh.context.target
# anh.context.command_string
# anh.context.command_info

import re

def send_spatial_chat(object, target, message, chat_type, mood):
    print('Spatial Chat: ' + message)
    return

match = re.search(r'(\d+) (\d+) (\d+) (\d+) (\d+) (.*)', context['command_string'])
    
if not match: raise RuntimeError("Invalid spatial chat message")
    
send_spatial_chat(context['object'], context['target'], match.group(6), match.group(2), match.group(3))    
