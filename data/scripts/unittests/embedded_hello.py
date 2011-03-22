from embedded_hello import *
class PythonDerived(Base):
    def __init__(self):
        Base.__init__(self)
    def hello(self):
        self.x = 15.0
        self.x = "hi"
        return 'Hello from Python!'
