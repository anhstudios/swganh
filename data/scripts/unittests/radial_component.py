from embedded_component import *
class RadialComponent(RadialComponentInterface):
    def Init(ptree):
        self.object_id = 8675309
    def Deinit(self):
        self.object_id = 0
    def Update(float):
        self.x = float+10
    def HandleMessage(message):
        self.message = message
    def component_info(self):
        return self.component_info