bicep_l = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bicep_l.iff', ContainerPermission.Default, False, False)
bicep_r = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bicep_r.iff', ContainerPermission.Default, False, False)
belt = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_belt.iff', ContainerPermission.Default, False, False)
boots = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_shoes.iff', ContainerPermission.Default, False, False)
bicep_l = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bracer_l.iff', ContainerPermission.Default, False, False)
bracer_r = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bracer_r.iff', ContainerPermission.Default, False, False)
bracer_l = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bicep_l.iff', ContainerPermission.Default, False, False)
chest = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_chest_plate.iff', ContainerPermission.Default, False, False)
gloves = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_gloves.iff', ContainerPermission.Default, False, False)
leggings = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_leggings.iff', ContainerPermission.Default, False, False)
helmet = simulation.createObject('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_helmet.iff', ContainerPermission.Default, False, False)

helmet.stf_name('wearables_name', 'armor_mandalorian_helmet')
helmet.condition = 0
helmet.max_condition = 1000
inventory.Add(actor, helmet)


belt.stf_name('wearables_name', 'armor_mandalorian_helmet')
belt.condition = 0
belt.max_condition = 1000
inventory.Add(actor, belt)

bicep_l.stf_name('wearables_name', 'armor_mandalorian_bicep_l')
bicep_l.condition = 0
bicep_l.max_condition = 1000
inventory.Add(actor, bicep_l)

bicep_r.stf_name('wearables_name', 'armor_mandalorian_bicep_r')
bicep_r.condition = 0
bicep_r.max_condition = 1000
inventory.Add(actor, bicep_r)

boots.stf_name('wearables_name', 'armor_mandalorian_shoes')
boots.condition = 0
boots.max_condition = 1000
inventory.Add(actor, boots)

bracer_l.stf_name('wearables_name', 'armor_mandalorian_bracer_l')
bracer_l.condition = 0
bracer_l.max_condition = 1000
inventory.Add(actor, bracer_l)

bracer_r.stf_name('wearables_name', 'armor_mandalorian_bracer_r')
bracer_r.condition = 0
bracer_r.max_condition = 1000
inventory.Add(actor, bracer_r)

chest.stf_name('wearables_name', 'armor_mandalorian_chest_plate')
chest.condition = 0
chest.max_condition = 1000
inventory.Add(actor, chest)

gloves.stf_name('wearables_name', 'armor_mandalorian_gloves')
gloves.condition = 0
gloves.max_condition = 1000
inventory.Add(actor, gloves)

leggings.stf_name('wearables_name', 'armor_mandalorian_pants')
leggings.condition = 0
leggings.max_condition = 1000
inventory.Add(actor, leggings)
