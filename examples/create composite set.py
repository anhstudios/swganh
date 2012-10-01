bicep_l = simulation.createObject('object/tangible/wearables/armor/composite/shared_armor_composite_bicep_l.iff', ContainerPermission.Default, False, False)
bicep_r = simulation.createObject('object/tangible/wearables/armor/composite/shared_armor_composite_bicep_r.iff', ContainerPermission.Default, False, False)
boots = simulation.createObject('object/tangible/wearables/armor/composite/shared_armor_composite_boots.iff', ContainerPermission.Default, False, False)
bicep_l = simulation.createObject('object/tangible/wearables/armor/composite/shared_armor_composite_bracer_l.iff', ContainerPermission.Default, False, False)
bracer_r = simulation.createObject('object/tangible/wearables/armor/composite/shared_armor_composite_bracer_r.iff', ContainerPermission.Default, False, False)
bracer_l = simulation.createObject('object/tangible/wearables/armor/composite/shared_armor_composite_bicep_l.iff', ContainerPermission.Default, False, False)
chest = simulation.createObject('object/tangible/wearables/armor/composite/shared_armor_composite_chest_plate.iff', ContainerPermission.Default, False, False)
gloves = simulation.createObject('object/tangible/wearables/armor/composite/shared_armor_composite_gloves.iff', ContainerPermission.Default, False, False)
leggings = simulation.createObject('object/tangible/wearables/armor/composite/shared_armor_composite_leggings.iff', ContainerPermission.Default, False, False)

bicep_l.stf_name('wearables_name', 'armor_composite_bicep_l')
bicep_l.condition = 0
bicep_l.max_condition = 1000
inventory.Add(actor, bicep_l)

bicep_r.stf_name('wearables_name', 'armor_composite_bicep_r')
bicep_r.condition = 0
bicep_r.max_condition = 1000
inventory.Add(actor, bicep_r)

boots.stf_name('wearables_name', 'armor_composite_boots')
boots.condition = 0
boots.max_condition = 1000
inventory.Add(actor, boots)

bracer_l.stf_name('wearables_name', 'armor_composite_bracer_l')
bracer_l.condition = 0
bracer_l.max_condition = 1000
inventory.Add(actor, bracer_l)

bracer_r.stf_name('wearables_name', 'armor_composite_bracer_r')
bracer_r.condition = 0
bracer_r.max_condition = 1000
inventory.Add(actor, bracer_r)

chest.stf_name('wearables_name', 'armor_composite_chest_plate')
chest.condition = 0
chest.max_condition = 1000
inventory.Add(actor, chest)

gloves.stf_name('wearables_name', 'armor_composite_gloves')
gloves.condition = 0
gloves.max_condition = 1000
inventory.Add(actor, gloves)

leggings.stf_name('wearables_name', 'armor_composite_pants')
leggings.condition = 0
leggings.max_condition = 1000
inventory.Add(actor, leggings)
