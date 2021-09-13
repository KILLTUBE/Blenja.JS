# exec(bpy.data.texts[0].as_string());

for obj in bpy.context.scene.object:
    if obj.animation_data is not None:
        action = obj.animation_data.action
        if action is not None:
            track = obj.animation_data.nla_tracks.new()
            track.strips.new(action.name, action.frame_range[0], action)
            obj.animation_data.action = None
			

# arr = Array([1,10,2,20,3,30])
# arr.push(4, 40)
# print(arr.map(lambda x: x*2)) # [2, 20, 4, 40, 6, 60, 8, 80]
# print(arr.filter(lambda x: x>=10)) # [10, 20, 30, 40]
class Array(list):
    def map(self, cb):
        return Array(map(cb, self));
    def forEach(self, cb):
        for _ in self:
            cb(_);
    def filter(self, cb):
        return Array(filter(cb, self));
    def filtermap(self, cb):
        filtered = self.filter(cb);
        mapped = filtered.map(cb);
        return mapped;
    def push(self, *args):
        for arg in args:
            self.append(arg);

#  getObjects().map(getName)                          # == ['Light', 'Camera', 'Bip001', 'Maila_EyeL', ...
def getObjects():
    return Array(bpy.context.view_layer.objects);

# getAnimationDatas().map(getInfo)
def getAnimationDatas():
    return getObjects().filtermap(lambda object: object.animation_data);

def getAction(_):
    if type(_) == bpy.types.Object:
        if _.animation_data is not None:
            return _.animation_data.action;
    elif type(_) == bpy.types.AnimData:
        return _.action;

# getMarkers().map(getName)                           # == ['F_42', 'A', 'C', 'B']
def getMarkers():
    return Array(bpy.context.scene.timeline_markers)

# b = getMarker('B')
def getMarker(name):
    return bpy.context.scene.timeline_markers.get(name);

def createMarker(name, frame=None):
    if frame is None:
        frame = bpy.context.scene.frame_current;
    bpy.context.scene.timeline_markers.new(name, frame=frame)

def select(_):
    _.select = True;

def deselect(_):
    _.select = False;

# getActions().map(getName)                           # == ['Action.002', 'Action.005', 'Action.008']
# getActions().map(getInfo)
def getActions():
    return getAnimationDatas().filtermap(lambda x: x.action)

# flat([[1, 2], [3, 4]])                              # == [1, 2, 3, 4]
def flat(_):
    return Array([x for __ in _ for x in __]);

# getNlaTracksArray()[2][0].name                      # == '[Action Stash]'
# getNlaTracksArray()[2][1].name                      # == 'LeftEyeTrack'
# getNlaTracksArray().map(lambda tracks: len(tracks)) # == [1, 1, 2, 1]
# getNlaTracksArray().map(len)                        # == [1, 1, 2, 1]
# getNlaTracksArray().map(getNames)                   # == [['NlaTrack'], ['RightEyeTrack'], ['[Action Stash]', 'LeftEyeTrack'], ['EyeTargetTrack']]
def getNlaTracksArray():
    return getAnimationDatas().filtermap(lambda x: Array(x.nla_tracks))

# getNlaTracks().map(getName)                         # == ['NlaTrack', 'RightEyeTrack', '[Action Stash]', 'LeftEyeTrack', 'EyeTargetTrack']
def getNlaTracks(o=None):
    if o is not None:
        return o.animation_data.nla_tracks;
    else:
        return flat(getNlaTracksArray());

def getName(_):
    if _ is None:
        return None;
    if not hasattr(_, 'name'):
        return None;
    return _.name

def getNames(_):
    return _.map(getName);

# getActions().map(getInfo);
def getInfo(_):
    t = type(_)
    if t == bpy.types.Action:
        print("Action {name='%s' users: %d, fcurves: #%d, frame_range=%s, pose_markers=#%d}" % (
            _.name, _.users, len(_.fcurves), _.frame_range, len(_.pose_markers)
        ));
    elif t == bpy.types.TimelineMarker:
        print("TimelineMarker {name='%s' frame=%d, select=%s, camera=%s}" % (
            _.name, _.frame, _.select, _.camera
        ));
    elif t == bpy.types.NlaTrack:
        print("NlaTrack {name='%s' active=%s, select=%s, strips=#%d, mute=%s}" % (
            _.name, _.active, _.select, len(_.strips), _.mute
        ));
    else:
        print('unknown thing', t, _);
        return False;
    return True;

def getFirst(array):
    return array[0];

def getLast(array):
    return array[len(array) - 1];

def newTrack(obj, name=None):
    _ = obj.animation_data.nla_tracks;
    prev = getLast(_); # will be shown at top in NLA Editor
    #prev = getFirst(_);
    track = _.new(prev=prev)
    if name is not None:
        track.name = name;
    return track;

getAnimationDatas().filtermap(lambda x: x.action)

actions = getActions();
a, b, c, d = getActions();

o = getObjects().filter(lambda o: o.name == 'EyeLeftParent')[0]
newTrackA = o.animation_data.nla_tracks.new()

# >>> bpy.types.NlaStrips()
# Traceback (most recent call last):
#   File "<blender_console>", line 1, in <module>
# TypeError: bpy_struct.__new__(type): expected a single argument

# >>> prev=o.animation_data.nla_tracks[0]
# >>> o.animation_data.nla_tracks.new(prev=prev)
# bpy.data.objects['EyeLeftParent']...NlaTrack

