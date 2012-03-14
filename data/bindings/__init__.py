# Add paths
import sys, os
sys.path.insert(0, os.path.abspath('../'))
sys.path.append(os.path.abspath('bindings/app'))
sys.path.append(os.path.abspath('bindings/object'))
sys.path.append(os.path.abspath('bindings/combat'))
sys.path.append(os.path.abspath('bindings/utility'))
from bindings.app import *
from bindings.combat import *
from bindings.object import *
from bindings.utility import *