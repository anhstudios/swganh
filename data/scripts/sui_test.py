from swgpy.app import SWGKernel
from swgpy.sui import SUIService, SUIWindow, InputTrigger, ListBoxType, MessageBoxType, InputBoxType

def sendMessageBox(kernel, player_id):
	sui_service = kernel.service_manager().sui_service()
	owner = kernel.service_manager().simulation_service().findObjectById(player_id)
	
	#Create the window
	window = sui_service.CreateMessageBox(MessageBoxType.okcancel, 'WARNING OMG!', 'PROBLEMS, ISSUES, WTF!', owner)
	
	#Send it to the owner
	sui_service.OpenSUIWindow(window)

def listBoxCallback(object, event_type, result_list):
	pass
	
def sendListBox(kernel, player_id):
	sui_service = kernel.service_manager().sui_service()
	owner = kernel.service_manager().simulation_service().findObjectById(player_id)
	
	#Create the window
	window = sui_service.CreateListBox(ListBoxType.okcancel, 'List Title', 'This is a list', ['Woot', 'Lol', 'Omg'], owner)
	
	#Subscribe to some properties of the remote window
	returned_props = ["List.lstList:SelectedRow", "bg.caption.lblTitle:Text", "this:otherPressed"]
	window.SubscribeToEventCallback(1, "", InputTrigger.ok, returned_props, listBoxCallback)	
	window.SubscribeToEventCallback(2, "", InputTrigger.cancel, returned_props, listBoxCallback)
	
	#Send it to the owner
	sui_service.OpenSUIWindow(window)
	
def inputBoxCallback(object, event_type, result_list):
	pass
	
def sendInputBox(kernel, player_id):
	sui_service = kernel.service_manager().sui_service()
	owner = kernel.service_manager().simulation_service().findObjectById(player_id)
	
	#Create the window
	window = sui_service.CreateInputBox(InputBoxType.okcancel, 'Input Title', 'This is an input', 50, owner)
	
	#Subscribe to some properties of the remote window
	returned_props = ["txtInput:LocalText"]
	window.SubscribeToEventCallback(1, "", InputTrigger.ok, 
	
	#Send it to the owner
	sui_service.OpenSUIWindow(window)
	
#sui_test.sendMessageBox(kernel, 8589934603)
#sui_test.sendListBox(kernel, 8589934603)
#sui_test.sendInputBox(kernel, 8589934603)