
import anh
import re

class AddFriendCommand(anh.base.Command):
    label = "addfriend"
    regex = r'(*)'
    
    def execute(self, context):
        match = re.search(regex, context.command_string)
        
        context.object.add_friend(match[0])