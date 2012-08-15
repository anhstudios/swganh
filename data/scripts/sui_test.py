from swgpy.app import SWGKernel
from swgpy.sui import SUIService, SUIWindow, InputTrigger, ListBoxType

def sendListBox(kernel, player_id):
	sui_service = kernel.service_manager().sui_service()
	owner = kernel.service_manager().simulation_service().findObjectById(player_id)
	
	#Create the window
	window = sui_service.CreateListBox(1, 'List Title', 'This is a list', ['Woot', 'Lol', 'Omg'], owner)
	
	#Send it to the owner
	sui_service.OpenSUIWindow(window)
	
#sui_test.sendListBox(kernel, 8589934603)