def getRenderLayer(s):
    # createNode renderLayer -n "defaultRenderLayer";
    return s[27:-3]

filePath = "J:/vd2/work/prod/lig/s036/030/scenes/vd2_s036_030_lig_v004.ma"
f = open(filePath,"r")
lines = f.readlines()

onRenderLayers = []
offRenderLayers = []
lastRenderLayer = ""
for l in lines:
    if "renderLayer -n" in l:
        if len(lastRenderLayer) > 0:
            onRenderLayers.append(lastRenderLayer)
        lastRenderLayer = getRenderLayer(l)
    if ".rndr" in l:
        offRenderLayers.append(lastRenderLayer)
        lastRenderLayer = ""
print(onRenderLayers)
print(offRenderLayers)