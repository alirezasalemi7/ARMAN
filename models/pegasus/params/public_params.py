# Copyright 2020 The PEGASUS Authors..
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Summarization params of baseline models for downstream datasets."""
import functools

from pegasus.data import parsers
from pegasus.eval import estimator_metrics
from pegasus.eval import text_eval
from pegasus.models import transformer
from pegasus.ops import public_parsing_ops
from pegasus.params import pegasus_params
from pegasus.params import registry
from tensorflow.contrib import training as contrib_training


def transformer_params(patterns, param_overrides):
  """Params for TransformerEncoderDecoderMLModel.

  Args:
    patterns: a dict include train_pattern, dev_pattern, test_pattern
    param_overrides: a string, comma separated list of name=value

  Returns:
    A instance of HParams
  """

  hparams = contrib_training.HParams(
      train_pattern=patterns["train_pattern"],
      dev_pattern=patterns["dev_pattern"],
      test_pattern=patterns["test_pattern"],
      vocab_filename="pegasus/ops/testdata/sp_test.model",
      encoder_type="sentencepiece_newline",
      length_bucket_size=0,
      add_task_id=False,
      batch_size=patterns["batch_size"],
      max_input_len=patterns["max_input_len"],
      max_target_len=patterns["max_output_len"],
      max_decode_len=patterns["max_output_len"],
      hidden_size=768,
      filter_size=3072,
      num_heads=12,
      num_encoder_layers=12,
      num_decoder_layers=12,
      beam_size=1,
      beam_start=5,
      beam_alpha=0.6,
      beam_min=0,
      beam_max=-1,
      temperature=0.0,
      top_k=0,
      top_p=0.0,
      optimizer_name="adafactor",
      train_steps=patterns["train_steps"],
      learning_rate=patterns["learning_rate"],
      label_smoothing=0.0,
      dropout=0.1,
      eval_max_predictions=patterns.get("eval_steps", 1000),
      use_bfloat16=False,
      model=None,
      parser=None,
      encoder=None,
      estimator_prediction_fn=None,
      eval=None,
      estimator_eval_metrics_fn=estimator_metrics.gen_eval_metrics_fn,
  )

  if param_overrides:
    hparams.parse(param_overrides)

  hparams.parser = functools.partial(
      parsers.supervised_strings_parser,
      hparams.vocab_filename,
      hparams.encoder_type,
      hparams.max_input_len,
      hparams.max_target_len,
      length_bucket_size=hparams.length_bucket_size,
      length_bucket_start_id=pegasus_params.LENGTH_BUCKET_START_ID,
      length_bucket_max_id=pegasus_params.TASK_START_ID - 1,
      add_task_id=hparams.add_task_id,
      task_start_id=pegasus_params.TASK_START_ID)

  hparams.encoder = public_parsing_ops.create_text_encoder(
      hparams.encoder_type, hparams.vocab_filename)

  hparams.model = functools.partial(
      transformer.TransformerEncoderDecoderModel, hparams.encoder.vocab_size,
      hparams.hidden_size, hparams.filter_size, hparams.num_heads,
      hparams.num_encoder_layers, hparams.num_decoder_layers,
      hparams.label_smoothing, hparams.dropout)

  beam_keys = ("beam_start", "beam_alpha", "beam_min", "beam_max",
               "temperature", "top_k", "top_p")
  beam_kwargs = {k: hparams.get(k) for k in beam_keys if k in hparams.values()}

  def decode_fn(features):
    return hparams.model().predict(features, hparams.max_decode_len,
                                   hparams.beam_size, **beam_kwargs)

  hparams.estimator_prediction_fn = decode_fn
  hparams.eval = functools.partial(
      text_eval.text_eval,
      hparams.encoder,
      num_reserved=pegasus_params.NUM_RESERVED_TOKENS)

  return hparams

@registry.register("MSR")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:MSR-train",
          "dev_pattern": "tfds:MSR-train",
          "test_pattern": "tfds:MSR-train",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 1000000,
          "learning_rate": 0.01,
          "batch_size": 128,
      }, param_overrides)

@registry.register("PN_Summary")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:PN_Summary-train",
          "dev_pattern": "tfds:PN_Summary-validation",
          "test_pattern": "tfds:PN_Summary-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 50000,
          "learning_rate": 5e-4,
          "batch_size": 128,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)

@registry.register("tebyan")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:Tebyan-train",
          "dev_pattern": "tfds:Tebyan-validation",
          "test_pattern": "tfds:Tebyan-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 50000,
          "learning_rate": 5e-4,
          "batch_size": 128,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)

@registry.register("perkey_summary")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:perkey_summary-train",
          "dev_pattern": "tfds:perkey_summary-validation",
          "test_pattern": "tfds:perkey_summary-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 50000,
          "learning_rate": 5e-4,
          "batch_size": 128,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)

@registry.register("perkey_title")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:perkey_title-train",
          "dev_pattern": "tfds:perkey_title-validation",
          "test_pattern": "tfds:perkey_title-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 50000,
          "learning_rate": 5e-4,
          "batch_size": 128,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)


@registry.register("parsi_nlu_entailment")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:ParsiNLU_Entailment-train",
          "dev_pattern": "tfds:ParsiNLU_Entailment-validation",
          "test_pattern": "tfds:ParsiNLU_Entailment-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 20000,
          "learning_rate": 5e-4,
          "batch_size": 48,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)

@registry.register("parsi_nlu_mch")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:ParsiNLU_MCH-train",
          "dev_pattern": "tfds:ParsiNLU_MCH-validation",
          "test_pattern": "tfds:ParsiNLU_MCH-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 20000,
          "learning_rate": 5e-4,
          "batch_size": 48,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)

@registry.register("parsi_nlu_qqp")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:ParsiNLU_QQP-train",
          "dev_pattern": "tfds:ParsiNLU_QQP-validation",
          "test_pattern": "tfds:ParsiNLU_QQP-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 20000,
          "learning_rate": 5e-4,
          "batch_size": 48,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)

@registry.register("parsi_nlu_sentence_sentiment_movie")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:ParsiNLU_sentence_sentiment_Movie-train",
          "dev_pattern": "tfds:ParsiNLU_sentence_sentiment_Movie-validation",
          "test_pattern": "tfds:ParsiNLU_sentence_sentiment_Movie-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 20000,
          "learning_rate": 5e-4,
          "batch_size": 48,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)

@registry.register("parsi_nlu_sentence_sentiment_food")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:ParsiNLU_sentence_sentiment_Food-train",
          "dev_pattern": "tfds:ParsiNLU_sentence_sentiment_Food-validation",
          "test_pattern": "tfds:ParsiNLU_sentence_sentiment_Food-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 20000,
          "learning_rate": 5e-4,
          "batch_size": 48,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)

@registry.register("wiki_summary_v1")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:wiki_summary_v1-train",
          "dev_pattern": "tfds:wiki_summary_v1-validation",
          "test_pattern": "tfds:wiki_summary_v1-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 50000,
          "learning_rate": 5e-4,
          "batch_size": 64,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)

@registry.register("voa_headlines")
def billsum_transformer(param_overrides):
  return transformer_params(
      {
          "train_pattern": "tfds:VOA_headlines-train",
          "dev_pattern": "tfds:VOA_headlines-validation",
          "test_pattern": "tfds:VOA_headlines-test",
          "max_input_len": 512,
          "max_output_len": 256,
          "train_steps": 20000,
          "learning_rate": 5e-4,
          "batch_size": 64,
          "label_smoothing" : 0.1,
          "beam_alpha" : 0.8
      }, param_overrides)