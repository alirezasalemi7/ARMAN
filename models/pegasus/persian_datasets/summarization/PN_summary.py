
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os

import tensorflow.compat.v2 as tf
import tensorflow_datasets.public_api as tfds
import re

_DOCUMENT = "_DOCUMENT"
_SUMMARY = "_SUMMARY"



DIR = '/*path to data*/'

class PN_Summary(tfds.core.GeneratorBasedBuilder):

    VERSION = tfds.core.Version('0.1.0')
    MANUAL_DOWNLOAD_INSTRUCTIONS = 'Here to be description of the dataset'
    SKIP_REGISTERING = True
    def _info(self):
        return tfds.core.DatasetInfo(
            builder=self,
            # This is the description that will appear on the datasets page.
            description=("persian dataset."),
        # tfds.features.FeatureConnectors
        features=tfds.features.FeaturesDict({
            _DOCUMENT: tfds.features.Text(),
            _SUMMARY: tfds.features.Text(),
        }),
        # If there's a common (input, target) tuple from the features,
        # specify them here. They'll be used if as_supervised=True in
        # builder.as_dataset.
        supervised_keys=(_DOCUMENT, _SUMMARY),
        )

    def _split_generators(self, dl_manager):
        # Download source data
        
        extracted_path = DIR
        # Specify the splits
        return [
            tfds.core.SplitGenerator(
                name=tfds.Split.TRAIN,
                gen_kwargs={
                    "raw_texts": os.path.join(extracted_path, "train/inputs.txt"),
                    "target_texts": os.path.join(extracted_path, "train/targets.txt")
                },
            ),
            tfds.core.SplitGenerator(
                name=tfds.Split.TEST,
                gen_kwargs={
                    "raw_texts": os.path.join(extracted_path, "test/inputs.txt"),
                    "target_texts": os.path.join(extracted_path, "test/targets.txt")
                },
            ),
            tfds.core.SplitGenerator(
                name=tfds.Split.VALIDATION,
                gen_kwargs={
                    "raw_texts": os.path.join(extracted_path, "validation/inputs.txt"),
                    "target_texts": os.path.join(extracted_path, "validation/targets.txt")
                },
            )
        ]
            
    def _generate_examples(self, raw_texts,target_texts):
        with tf.io.gfile.GFile(raw_texts,"rb") as input_file,tf.io.gfile.GFile(target_texts,"rb") as target_file:
            for i, (text, target) in enumerate(zip(input_file, target_file)):
                yield i, {_DOCUMENT: b"<SUMMARIZATION> " + text, _SUMMARY: target}
        
