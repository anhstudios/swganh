from swgpy.app import SWGKernel
from swgpy.sui import SUIService, SUIWindow, InputTrigger

def handleUIEvent(owner, event_type, result_text_list):
	print("WOOHOO!")

def sendMessageBox(kernel, player_id, title, text):
	sui_service = kernel.service_manager().sui_service()
	owner = kernel.service_manager().simulation_service().findObjectById(player_id)
	
	window = sui_service.GetNewSUIWindow("Script.messageBox", owner)
	
	#These are the values we want to return
	returned_props = []
	
	#Add in a couple event listeners for ok and cancel
	window.SubscribeToEventCallback(0, "", InputTrigger.ok, returned_props, handleUIEvent)
	window.SubscribeToEventCallback(0, "", InputTrigger.canel, returned_props, handleUIEvent)
	
	#Set any and all properties we want
	window.SetProperty("bg.caption.lblTitle:Text", title).SetProperty("Prompt.lblPrompt:Text", text)
	
	window.SetProperty("btnOk:visible", "True").SetProperty("btnOk:Text", "")
	window.SetProperty("btnRevert:visible", "True").SetProperty("btnRevert:Text", "")
	window.SetProperty("btnCancel:visible", "True").SetProperty("btnCancel:Text", "")
	
	#Send it to the owner
	sui_service.CreateSUIWindow(window)
	
#sendMessageBox(8589934603, "Test", "Woot")