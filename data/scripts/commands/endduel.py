import swgpy.app
import swgpy.combat

service_mgr = kernel.service_manager()
combat_svc = service_mgr.combat_service()
if (actor and creature_target):
	combat_svc.end_duel(actor, creature_target)
