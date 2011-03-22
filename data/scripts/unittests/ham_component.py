from embedded_component import *
class HamComponent(HAMComponentInterface):
    def Init(self,pt):
        self.ham = HAM()
        print(self.ham.mind)
    def Deinit(self):
        print(self.ham.health)
        self.ham.health = 0
        self.ham.action = 0
        self.ham.mind = 0
        self.ObjectId = 0
    def Update(self,update):
        self.update = update
        print (self.update)
    def HandleMessage(self,message):
        self.message = message
    def component_info(self):
        return self.ComponentInfo
    def ham(self):
        print(self.ham)
        return self.ham