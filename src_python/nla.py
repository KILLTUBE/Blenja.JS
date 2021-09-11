# exec(bpy.data.texts[0].as_string());

for obj in bpy.context.scene.object:
    if obj.animation_data is not None:
        action = obj.animation_data.action
        if action is not None:
            track = obj.animation_data.nla_tracks.new()
            track.strips.new(action.name, action.frame_range[0], action)
            obj.animation_data.action = None
			
			
			
for obj in bpy.context.view_layer.objects:
    if obj.animation_data is not None:
	    print(obj.animation_data)
		
<bpy_struct, AnimData at 0x0000019DC9E2D2C8>
<bpy_struct, AnimData at 0x0000019DC9E2D648>
<bpy_struct, AnimData at 0x0000019DC9E2D148>
<bpy_struct, AnimData at 0x0000019DC9E2DD48>


for obj in bpy.context.view_layer.objects:
    if obj.animation_data is not None:
	    print(obj.animation_data.action)

<bpy_struct, Action("Action.002") at 0x0000019DCC457428>
<bpy_struct, Action("Action.005") at 0x0000019DCC456E88>
<bpy_struct, Action("Action.008") at 0x0000019DCC459DC8>
None


for obj in bpy.context.view_layer.objects:
    if obj.animation_data is not None:
	    print(obj.animation_data.nla_tracks)
		
<bpy_collection[1], NlaTracks>
<bpy_collection[1], NlaTracks>
<bpy_collection[2], NlaTracks>
<bpy_collection[1], NlaTracks>


# arr = Array([1,10,2,20,3,30])
# arr.push(4, 40)
# print(arr.map(lambda x: x*2)) # [2, 20, 4, 40, 6, 60, 8, 80]
# print(arr.filter(lambda x: x>=10)) # [10, 20, 30, 40]
class Array(list):
    def map(self, cb):
        return Array(map(cb, self));
    def filter(self, cb):
        return Array(filter(cb, self));
    def filtermap(self, cb):
        filtered = self.filter(cb);
        mapped = filtered.map(cb);
        return mapped;
    def push(self, *args):
        for arg in args:
            self.append(arg);


def getAnimationDatas():
    animationDatas = Array();
    for obj in bpy.context.view_layer.objects:
        animation_data = obj.animation_data
        if obj.animation_data is None:
            continue;
        animationDatas.push(animation_data);
    return animationDatas;

def getActions():
    getAnimationDatas().filtermap(lambda x: x.action)

    actions = Array();
    for obj in bpy.context.view_layer.objects:
        animation_data = obj.animation_data
        if obj.animation_data is None:
            continue;
        action = animation_data.action
        if action is None:
            continue;
        actions.push(action);
    return actions;

def info(_):
    if (type(_) == bpy.types.Action):
        infoAction(_);
    else:
        print('unknown thing', type(_), _);

def infoAction(action):
    print("action %s = {users: %d" % (action.name, action.users));
  

getAnimationDatas().filtermap(lambda x: x.action)

actions = getActions();
a, b, c, d = getActions();
