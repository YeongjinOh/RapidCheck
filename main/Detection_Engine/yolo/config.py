# Global config variables
import os
# import yolo.conf.RCNet_2class_enduser_config as c
import yolo.conf.RCNet_2class_base_config as c
# import yolo.conf.vgg16_2class_config as c
# import yolo.conf.yolo_2class_base_config as c
# import yolo.conf.yolo_2class_config as c
# import yolo.conf.yolo_tiny_config as c
# import yolo.conf.cifar100_based_config as c
pretrained_model = c.pretrained_model

model_folder = c.model_folder
model_name = c.model_name
classes_name = c.classes_name


datacenter_root = c.datacenter_root
dataset_enduser_root = c.dataset_enduser_root

dataset_abs_location = c.dataset_abs_location
ann_location = c.ann_location
imageset_location = c.imageset_location

test_dataset_abs_location = c.test_dataset_abs_location
test_ann_location = c.test_ann_location
test_imageset_location = c.test_imageset_location

cell_size = c.cell_size
num_classes = c.num_classes
boxes_per_cell = c.boxes_per_cell

class_scale = c.class_scale
object_scale = c.object_scale
noobject_scale = c.noobject_scale
coord_scale = c.coord_scale

inp_size = c.inp_size
batch_size = c.batch_size
epochs= c.epochs

lr= c.lr
trainer= c.trainer

image_dim_order = c.image_dim_order
norm_type = c.norm_type

descriptions = c.descriptions