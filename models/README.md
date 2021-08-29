# PEGASUS library Could be used for ARMAN

We decided not to develop any code for training ARMAN models, but we used codes published by the PEGASUS team. The main reason was that their code has high quality, and also our model architecture was the same, so we used theirs.

If you use this code or these models, please cite the following paper:
```
@misc{zhang2019pegasus,
    title={PEGASUS: Pre-training with Extracted Gap-sentences for Abstractive Summarization},
    author={Jingqing Zhang and Yao Zhao and Mohammad Saleh and Peter J. Liu},
    year={2019},
    eprint={1912.08777},
    archivePrefix={arXiv},
    primaryClass={cs.CL}
}
```

# Setup

## create an instance on google cloud with GPU (optional)

Please create a project first and create an instance

```
gcloud compute instances create \
  ${VM_NAME} \
  --zone=${ZONE} \
  --machine-type=n1-highmem-8 \
  --accelerator type=nvidia-tesla-v100,count=1 \
  --boot-disk-size=500GB \
  --image-project=ml-images \
  --image-family=tf-1-15 \
  --maintenance-policy TERMINATE --restart-on-failure
```

## install library and dependencies

Clone library on github and install requirements.

```
git clone https://github.com/google-research/pegasus
cd pegasus
export PYTHONPATH=.
pip3 install -r requirements.txt
```

Download vocab, pretrained and fine-tuned checkpoints of all experiments from [Google Cloud](https://console.cloud.google.com/storage/browser/pegasus_ckpt).

Alternatively in terminal, follow the instruction and install [gsutil](https://cloud.google.com/storage/docs/gsutil_install). Then

```
mkdir ckpt
gsutil cp -r gs://pegasus_ckpt/ ckpt/

```

# Finetuning on downstream datasets

Two types of dataset format are supported: [TensorFlow Datasets (TFDS)](https://www.tensorflow.org/datasets) or TFRecords.

[This tutorial](https://www.tensorflow.org/datasets/add_dataset) shows how to add a new dataset in TFDS.
(The fine-tuning dataset is expected to be supervised, please provide
`supervised_keys` in dataset info).

Tfrecords format requires each record to be a tf example of `{"inputs":tf.string, "targets":tf.string}`.

For example, if you registered a TFDS dataset called `new_tfds_dataset` for training and evaluation, and have some files in tfrecord format called `new_dataset_files.tfrecord*` for test, they can be registered in `/pegasus/params/public_params.py`.

```
@registry.register("new_params")
def my_param(param_overrides):
  return public_params.transformer_params(
      {
          "train_pattern": "tfds:new_tfds_dataset,train",
          "dev_pattern": "tfds:new_tfds_dataset,validation",
          "test_pattern": "tfrecord:new_dataset_files.tfrecord*",
          "max_input_len": 512,
          "max_output_len": 128,
          "train_steps": 10000,
          "learning_rate": 0.0001,
          "batch_size": 8,
      }, param_overrides)
```
Finetune on a dataset `new_params`.

```
python3 pegasus/bin/train.py --params=new_params \
--param_overrides=vocab_filename=/*address to vocab file*/ \
--train_init_checkpoint=/*address to initial model checkpoint*/ \
--model_dir=/*model output directory*/
```
Generate tests on the finetuned dataset.

```
python3 pegasus/bin/evaluate.py --params=new_params \
--param_overrides=vocab_filename=/*address to vocab file*/,batch_size=1,beam_size=5,beam_alpha=0.6 \
--model_dir=/*address to model_dir*/
```

Note that the above example is using a single GPU so the batch_size is much smaller
than the results reported in the paper.

# Pre-training

For pretraining a new model, use a pretraining data generator code available in our repository, then make a new dataset with its outputs and train the new model the same as fine-tuning procedure. Remember, in pretraining, `train_init_checkpoint` should not be given.
