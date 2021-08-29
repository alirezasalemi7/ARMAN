# Evaluating ARMAN

ARMAN got SOTA results on all of the summarization downstream tasks/datasets that were available for us. We have used ROUGE and BERTScore metrics for evaluation. We used [this](https://github.com/hooshvare/pn-summary/tree/main/scripts) scripts to evaluate our model in the summarization task; also, we add BERTScore to this script which is now available in the `evaluation.py` file. We used output files from our model to evaluate outputs, and targets correspond to each output. These files are outputs of `pegasus/bin/evaluate.py` script in `models` directory.

*NOTE: install BERTScore with `pip install bert-score`*