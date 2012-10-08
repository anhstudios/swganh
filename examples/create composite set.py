bicep_l = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bicep_l.iff', ContainerPermission.DEFAULT, False, False)
bicep_r = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bicep_r.iff', ContainerPermission.DEFAULT, False, False)
belt = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_belt.iff', ContainerPermission.DEFAULT, False, False)
boots = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_shoes.iff', ContainerPermission.DEFAULT, False, False)
bicep_l = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bracer_l.iff', ContainerPermission.DEFAULT, False, False)
bracer_r = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bracer_r.iff', ContainerPermission.DEFAULT, False, False)
bracer_l = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bicep_l.iff', ContainerPermission.DEFAULT, False, False)
chest = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_chest_plate.iff', ContainerPermission.DEFAULT, False, False)
gloves = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_gloves.iff', ContainerPermission.DEFAULT, False, False)
leggings = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_leggings.iff', ContainerPermission.DEFAULT, False, False)
helmet = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_helmet.iff', ContainerPermission.DEFAULT, False, False)

helmet.stfName('wearables_name', 'armor_mandalorian_helmet')
helmet.condition = 0
helmet.max_condition = 1000
inventory.add(actor, helmet)


belt.stfName('wearables_name', 'armor_mandalorian_helmet')
belt.condition = 0
belt.max_condition = 1000
inventory.add(actor, belt)

bicep_l.stfName('wearables_name', 'armor_mandalorian_bicep_l')
bicep_l.condition = 0
bicep_l.max_condition = 1000
inventory.add(actor, bicep_l)

bicep_r.stfName('wearables_name', 'armor_mandalorian_bicep_r')
bicep_r.condition = 0
bicep_r.max_condition = 1000
inventory.add(actor, bicep_r)

boots.stfName('wearables_name', 'armor_mandalorian_shoes')
boots.condition = 0
boots.max_condition = 1000
inventory.add(actor, boots)

bracer_l.stfName('wearables_name', 'armor_mandalorian_bracer_l')
bracer_l.condition = 0
bracer_l.max_condition = 1000
inventory.add(actor, bracer_l)

bracer_r.stfName('wearables_name', 'armor_mandalorian_bracer_r')
bracer_r.condition = 0
bracer_r.max_condition = 1000
inventory.add(actor, bracer_r)

chest.stfName('wearables_name', 'armor_mandalorian_chest_plate')
chest.condition = 0
chest.max_condition = 1000
inventory.add(actor, chest)

gloves.stfName('wearables_name', 'armor_mandalorian_gloves')
gloves.condition = 0
gloves.max_condition = 1000
inventory.add(actor, gloves)

leggings.stfName('wearables_name', 'armor_mandalorian_leggings')
leggings.condition = 0
leggings.max_condition = 1000
inventory.add(actor, leggings)
