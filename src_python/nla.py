# exec(bpy.data.texts[0].as_string());

"""
for obj in bpy.context.scene.object:
    if obj.animation_data is not None:
        action = obj.animation_data.action
        if action is not None:
            track = obj.animation_data.nla_tracks.new()
            track.strips.new(action.name, action.frame_range[0], action)
            obj.animation_data.action = None
"""			

# arr = Array([1,10,2,20,3,30])
# arr.push(4, 40)
# print(arr.map(lambda x: x*2)) # [2, 20, 4, 40, 6, 60, 8, 80]
# print(arr.filter(lambda x: x>=10)) # [10, 20, 30, 40]
# print(Array([1,10,2,20,3,30]).join('\n'))
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
    def findFirst(self, cb):
        for _ in self:
            if cb(_):
                return _;
    def get_length(self):
        return len(self);
    def join(self, separator=','):
        ret = '';
        i = 0;
        till = self.length - 1;
        for _ in self:
            ret += str(_);
            if i < till:
                ret += separator;
            i += 1;
        return ret;
    length = property(get_length);

# ensureAnimationData(getSelectedObject());
def ensureAnimationData(o):
    if not o:
        return None;
    if not o.animation_data:
        o.animation_data_create();

def firstObject():
    return getObjects()[0];

# getObject('Bip001')
def getObject(name):
    return getObjects().findFirst(lambda obj: obj.name == name);

# Example 1:
#   >>> getObjects().filter(lambda obj: obj.animation_data)
#   [bpy.data.objects['Bip001'], bpy.data.objects['EyeLeftParent'], bpy.data.objects['EyeRightParent'], bpy.data.objects['EyeTargetParent']]
#    >>> getObjects().filtermap(lambda obj: obj.animation_data).map(getParent)
#   [bpy.data.objects['Bip001'], bpy.data.objects['EyeLeftParent'], bpy.data.objects['EyeRightParent'], bpy.data.objects['EyeTargetParent']]
# Example 2:
#   getFcurves(getActions()[3]).map(getParent)
# Summary: Gets the "parent" object of e.g. object.animation_data
def getParent(_):
    return _.id_data;

#  getObjects().map(getName)                          # == ['Light', 'Camera', 'Bip001', 'Maila_EyeL', ...
def getObjects():
    return Array(bpy.context.view_layer.objects);

# getAnimationDatas().map(getInfo)
def getAnimationDatas():
    return getObjects().filtermap(lambda object: object.animation_data);

# getAction(getObject('Bip001'))
# getAction('Action.002')
# getSelectedObjects().map(getAction)

def getAction(_):
    t = type(_)
    if t == bpy.types.Object:
        if _.animation_data is not None:
            return _.animation_data.action;
    elif t == bpy.types.AnimData:
        return _.action;
    elif t == str:
        return getActions().findFirst(lambda action: action.name == _);

# getMarkers().map(getName)                           # == ['F_42', 'A', 'C', 'B']
def getMarkers():
    return Array(bpy.context.scene.timeline_markers)

# b = getMarker('B')
def getMarker(name):
    return bpy.context.scene.timeline_markers.get(name);

def firstStrip():
    _ = getStrips();
    if _.length:
        return _[0];

def getStrips():
    ret = getNlaTracks().map(lambda track: track.strips);
    ret = flat(ret);
    ret = Array(ret);
    return ret;

# interestingAttributes(firstAction())                  # == ['name', 'users', 'fcurves', 'frame_range', 'pose_markers', 'groups']
def firstAction():
    _ = getActions();
    if _.length:
        return _[0];

# getFcurves(BodyAction).map(lambda fcurve: len(fcurve.keyframe_points))     # = [74, 262, 105, 105, 98, 104, 2, 2, 2, 10, 29, 18, 14, ...
def getFcurves(_=None):
    if _ is None:
        return None;
    t = type(_);
    if t == bpy.types.Action:
        return Array(_.fcurves)
    if t == bpy.types.NlaStrip:
        return Array(_.fcurves);

def createMarker(name, frame=None):
    if frame is None:
        frame = bpy.context.scene.frame_current;
    bpy.context.scene.timeline_markers.new(name, frame=frame)

def createNlaTrack(obj, name=None):
    ensureAnimationData(obj);
    track = obj.animation_data.nla_tracks.new()
    if name != None:
        track.name = name;
    return track;

def createAction(name='Action'):
    return bpy.data.actions.new(name)

def select(_):
    t = type(_)
    if t == bpy.types.Object:
        return _.select_set(True);
    _.select = True;

def deselect(_):
    t = type(_)
    if t == bpy.types.Object:
        return _.select_set(False);
    _.select = False;

# getObjects().filter(isSelected).map(deselect)
def isSelected(_):
    t = type(_)
    if t == bpy.types.Object:
        return _.select_get();
    return _.select;

# getActions().map(getName)                           # == ['Action.002', 'Action.005', 'Action.008']
# getActions().map(getInfo)
# getActions().map(lambda action: action.users)       # == [1, 1, 1, 0, 1, 1, 12, 1, 7, 12, 1, 1, 1, 1, 12]
def getActions():
    #return getAnimationDatas().filtermap(lambda x: x.action)
    return Array(bpy.data.actions);

# flat([[1, 2], [3, 4]])                              # == [1, 2, 3, 4]
def flat(_):
    return Array([x for __ in _ for x in __]);

# getNlaTracksArray()[2][0].name                      # == '[Action Stash]'
# getNlaTracksArray()[2][1].name                      # == 'LeftEyeTrack'
# getNlaTracksArray().map(lambda tracks: len(tracks)) # == [1, 1, 2, 1]
# getNlaTracksArray().map(len)                        # == [1, 1, 2, 1]
# getNlaTracksArray().map(getNames)                   # == [['NlaTrack'], ['RightEyeTrack'], ['[Action Stash]', 'LeftEyeTrack'], ['EyeTargetTrack']]
def getNlaTracksArray():
    return getAnimationDatas().filtermap(lambda x: Array(x.nla_tracks));

# getNlaTracks().map(getName)                         # == ['NlaTrack', 'RightEyeTrack', '[Action Stash]', 'LeftEyeTrack', 'EyeTargetTrack']
def getNlaTracks(_=None):
    t = type(_)
    if t == bpy.types.Object:
        if _.animation_data:
            return Array(o.animation_data.nla_tracks);
    else:
        return flat(getNlaTracksArray());
    return Array();

def getName(_):
    if _ is None:
        return None;
    if not hasattr(_, 'name'):
        return None;
    return _.name

def getNames(_):
    return _.map(getName);

def interestingAttributes(_):
    t = type(_)
    if t == bpy.types.Action:
        return Array(['name', 'users', 'fcurves', 'frame_range', 'pose_markers', 'groups']);
    elif t == bpy.types.TimelineMarker:
        return Array(['name', 'frame', 'select', 'camera']);
    elif t == bpy.types.NlaTrack:
        return Array(['name', 'active', 'select', 'strips', 'mute']);
    elif t == bpy.types.NlaStrip:
        return Array([
            'name', 'strip_time', 'active', 'select', 'scale',
            'strips', 'mute', 'frame_start', 'frame_end', 'action_frame_start',
            'action_frame_end', 'fcurves'
        ]);
    elif t == bpy.types.FCurve:
        return Array([
            'array_index',        'auto_smoothing'     , 'color'                , 'color_mode', 'data_path',
            'hide',               'is_valid'           , 'keyframe_points'      , 'modifiers' , 'mute'     ,
            'range',              'sampled_points'     , 'select'
        ]);
    return [];

# getActions().map(getInfo);
# getMarkers().map(getInfo);
# getNlaTracks().map(getInfo)
# getFcurves(BodyAction).map(getInfo)
def getInfo(_):
    t = type(_)
    if t == bpy.types.Action:
        print("Action {name='%s' users=%d fcurves=#%d frame_range=%s pose_markers=#%d groups=#%d}" % (
            _.name, _.users, len(_.fcurves), _.frame_range, len(_.pose_markers), len(_.groups)
        ));
    elif t == bpy.types.TimelineMarker:
        print("TimelineMarker {name='%s' frame=%d select=%s camera=%s}" % (
            _.name, _.frame, _.select, _.camera
        ));
    elif t == bpy.types.NlaTrack:
        print("NlaTrack {name='%s' active=%s select=%s strips=#%d mute=%s}" % (
            _.name, _.active, _.select, len(_.strips), _.mute
        ));
    elif t == bpy.types.NlaStrip:
        print("NlaStrip {name='%s' strip_time=%s active=%s select=%s scale=%s strips=#%d mute=%s frame_start=%s frame_end=%s action_frame_start=%s action_frame_end=%s fcurves=#%s}" % (
            _.name            , _.strip_time  , _.active     , _.select   , _.scale             ,
            len(_.strips)     , _.mute        , _.frame_start, _.frame_end, _.action_frame_start,
            _.action_frame_end, len(_.fcurves)
        ));
    elif t == bpy.types.FCurve:
        print("FCurve {array_index=%s auto_smoothing=%s color=%s color_mode=%s data_path=%s hide=%s is_valid=%s keyframe_points=#%s modifiers=#%s mute=%s range=%s sampled_points=#%s select=%s}" % (
            _.array_index        , _.auto_smoothing     , _.color                , _.color_mode, _.data_path     ,
            _.hide               , _.is_valid           , len(_.keyframe_points) , len(_.modifiers), _.mute      ,
            _.range()            , len(_.sampled_points), _.select
        ));
    elif t == bpy.types.Object:
        print("Object {name='%s' location=%s rotation_axis_angle=%s rotation_euler=%s rotation_mode=%s rotation_quaternion=%s scale=%s}" % (
            _.name, _.location, _.rotation_axis_angle, _.rotation_euler, _.rotation_mode, _.rotation_quaternion, _.scale,
        ));
    else:
        print('unknown thing', t, _);
        return False;
    return True;

def getSelectedObject():
  _ = getSelectedObjects();
  if _.length:
    return _[0];

def getSelectedObjects():
  return Array(bpy.context.selected_objects);

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

def identity(_):
    return _;

"""
getAnimationDatas().filtermap(lambda x: x.action)

actions = getActions();
a, b, c, d = getActions();

o = getObjects().filter(lambda o: o.name == 'EyeLeftParent')[0]
newTrackA = o.animation_data.nla_tracks.new()
"""

# >>> bpy.types.NlaStrips()
# Traceback (most recent call last):
#   File "<blender_console>", line 1, in <module>
# TypeError: bpy_struct.__new__(type): expected a single argument

# >>> prev=o.animation_data.nla_tracks[0]
# >>> o.animation_data.nla_tracks.new(prev=prev)
# bpy.data.objects['EyeLeftParent']...NlaTrack

# BodyAction = getAction('BodyAction');
# BodyActionCopy = BodyAction.copy();


"""
getSelectedObjects().map(getAction)
"""
