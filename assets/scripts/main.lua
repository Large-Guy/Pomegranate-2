Event.on("init", function()
    Debug.Log.info("Game initialized")

    entity = Entity(1) -- Create a new entity

    entity.position = {
        x=1,
        y=2
    }
    entity.name = {
        name="Player"
    }
    entity.player = {
        health=5,
        mana=10,
        equipment = {
            weapon = {
                name="Sword",
                damage=5
            },
            alt_weapon = {
                name="Bow",
                damage=3
            },
            armor = {
                name="Iron Armor",
                defense=2
            }
        }
    }
end)

Event.on("update", function()
    ECS.foreach("position", function(position)
        position.x = position.x + 1
    end)
    Debug.Log.info("Player position: (", entity.position.x, entity.position.y,")")
end) -- Called by the engine every frame

Event.call("init")