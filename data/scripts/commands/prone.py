
import anh

import anh.object.creature.postures.prone as _prone

class ProneCommand(anh.base.Command):
    label = "prone"
    
    def execute(self, context):
        obj = context.object
        
        obj.set_posture(_prone)
        
        anh.messages.broadcast_update_posture_message(obj)
        
