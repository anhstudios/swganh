
import anh
import re

class TipCommand(anh.base.Command):
    label = "tip"
    crc = "0xC64D8CB0"
    regex = r'(\w+\s)?(\d+)(\sbank)?'
    
    def execute(self, context):
        obj = context.object;
        target = context.target;
        
        match = re.search(regex, context.command_string)
        
        if not match:
            anh.messages.send_chat_system_message("@base_player:tip_error")
            return
            
        amount = match.group(2)    
        
        if match.group(1) == None:
            target = context.target
        else:
            target = anh.simulation.find_object_by_name(match.group(1))
                            
        if match.group(3) != None:
            anh.treasury.tip_bank(obj, target, amount)
        else:
            anh.treasury.tip_cash(obj, target, amount)
        
        
        