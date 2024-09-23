Event.on("init",function()
    entity = Entity()

    entity.position = {
        x = 1,
        y = 2
    }

    entity.name = {
        name = "Player"
    }

    Debug.Log.pass("Entity:", entity.name.name, "created at", entity.position.x, entity.position.y)
end)

Event.on("update",function()
    ECS.foreach("position", function(position)
        position.x = position.x + 1
        position.y = position.y + 1
        Debug.Log.pass("Position:", position.x, position.y)
    end)
end)

Event.call("init")