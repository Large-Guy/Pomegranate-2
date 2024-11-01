Debug.Log.info("main.lua loaded!")

entity = Entity.new()
print("Entity created! ", entity, entity.id)
entity:add("Transform",{x=2,y=5})
entity:add("Name",{name=1})

transform = entity:get("Transform")
print("Transform component: ", transform.x, transform.y)
Debug.Log.info("main.lua finished!")