import numpy as np
from matplotlib import pyplot as plt

from vis.utils import utils
from vis.utils.vggnet import VGG16
from vis.visualization import visualize_activation, get_num_filters

# Build the VGG16 network with ImageNet weights
model = VGG16(weights='imagenet', include_top=True)
print('Model loaded.')

# The name of the layer we want to visualize
# (see model definition in vggnet.py)
layer_name = 'block1_conv2'
layer_idx = [idx for idx, layer in enumerate(model.layers) if layer.name == layer_name][0]

# Visualize all filters in this layer.
filters = np.arange(get_num_filters(model.layers[layer_idx]))
print("1")
# Generate input image for each filter. Here `text` field is used to overlay `filter_value` on top of the image.
vis_images = []
for idx in filters:
    img = visualize_activation(model, layer_idx, filter_indices=idx) 
    img = utils.draw_text(img, str(idx))
    vis_images.append(img)
print("2")
# Generate stitched image palette with 8 cols.
stitched = utils.stitch_images(vis_images, cols=8)    
plt.axis('off')
plt.imshow(stitched)
plt.title(layer_name)
plt.show()
