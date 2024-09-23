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

ECS.foreach("Entity", function(entity)
    print(entity)
end)