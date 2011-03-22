from test_policies import *
print(GameObject.z)
#GameObject.z = 15.0
#print (GameObject.z)
print (CGameObject.x, CGameObject.y, CGameObject.z)
CGameObject.x = 5
print(CGameObject.x)
print(CGameObject1.x,CGameObject1.y,CGameObject1.z)
CGameObject1.x = 8
print(CGameObject1.x)

GameObject = CGameObject
print(GameObject.z)