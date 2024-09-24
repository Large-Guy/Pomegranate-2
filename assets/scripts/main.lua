Debug.Log.info("Hello, World!")

Vec2 = {}

function Vec2:new()
    local vec2 = {
        x = 0,
        y = 0
    }

    self.__index = self

    return setmetatable(vec2,self)
end

function Vec2:new(x, y)
    local vec2 = {
        x = x,
        y = y
    }

    self.__index = self

    return setmetatable(vec2,self)
end

function Vec2:__add(other)
    return Vec2:new(self.x + other.x, self.y + other.y)
end

function Vec2:__sub(other)
    return Vec2:new(self.x - other.x, self.y - other.y)
end

function Vec2:__mul(other)
    return Vec2:new(self.x * other.x, self.y * other.y)
end

function Vec2:__div(other)
    return Vec2:new(self.x / other.x, self.y / other.y)
end

function Vec2:__tostring()
    return "Vec2: (" .. self.x .. ", " .. self.y .. ")"
end

time = 0

Event.on("init",function()
    entity = Entity()

    entity.position = Vec2:new(0, 0)

    entity.name = {
        name = "Player"
    }
end)

Event.on("update",function()
    time = time + 1.0/30.0
end)

Event.on("update",function()
    ECS.foreach("position", function(position)
        position.x = math.cos(time)*10
        position.y = math.sin(time)*10

        Debug.Log.info("Position:", position.x, position.y)
    end)
end)

Event.call("init")