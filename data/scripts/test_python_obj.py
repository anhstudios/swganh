import example_app_binding

class TestPythonObj(example_app_binding.ObjInterface):
    def DoWork(self):
        return "Test Python Script Works"
