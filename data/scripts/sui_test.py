from swgpy.app import SWGKernel
from swgpy.sui import SUIService, SUIWindow, InputTrigger

def handleUIEvent(owner, event_type, result_text_list):
	pass

def sendMessageBox(kernel, player_id, title, text):
	sui_service = kernel.service_manager().sui_service()
	owner = kernel.service_manager().simulation_service().findObjectById(player_id)
	
	window = sui_service.CreateSUIWindow("Script.messageBox", owner)
	
	#Add in a couple event listeners for ok and cancel
	window.SubscribeToEventCallback(0, "", InputTrigger.ok, [], handleUIEvent)
	window.SubscribeToEventCallback(1, "", InputTrigger.cancel, [], handleUIEvent)
	
	#Set any and all properties we want
	window.SetProperty("bg.caption.lblTitle:Text", title).SetProperty("Prompt.lblPrompt:Text", text)
	
	window.SetProperty("btnOk:visible", "True").SetProperty("btnOk:Text", "btnOk")
	window.SetProperty("btnRevert:visible", "False").SetProperty("btnRevert:Text", "btnRevert")
	window.SetProperty("btnCancel:visible", "True").SetProperty("btnCancel:Text", "btnCancel")
	
	#Send it to the owner
	sui_service.OpenSUIWindow(window)
	
#sui_test.sendMessageBox(kernel, 8589934603, "Test", "Woot")