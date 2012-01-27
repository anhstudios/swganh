
import anh
import anh.messages.object_controller.send_spatial_chat as _send_spatial_chat
import re

class SpatialChatInternalCommand(anh.base.Command):
    label = "spatialchatinternal"
    regex = r'(\d+) (\d+) (\d+) (\d+) (\d+) (.*)'
    
    def execute(self, context):
        match = re.search(regex, context.command_string)
    
        if not match: raise RuntimeError("Invalid spatial chat message")
    
        _send_spatial_chat(
                context.object, context.target, 
                match.group(6), match.group(2), 
                match.group(3))    
